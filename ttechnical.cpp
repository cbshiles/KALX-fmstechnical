// ttechnical.cpp - test technical analysis library
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#include "technical.h"

#include "change.h"
#include "ewma.h"
#include "macd.h"
#include "minmax.h"
#include "moments.h"
#include "relative_strength.h"
#include "tick.h"

#include "blotter.h"

#ifndef ensure
#include <cassert>
#define ensure(x) assert(x)
#endif

using namespace technical;

void
fms_test_change(void)
{
	std::array<double, 4> x = {1, 2, 3, 0}, y, y0 = {1, 1, 1, -3};

	std::transform(x.begin(), x.end(), y.begin(), change<>());
	ensure (y == y0);
}

void
fms_test_ewma(void)
{
	std::array<double,3> x = {1, 2, 3}, y;
	double w = .5;

	std::transform(x.begin(), x.end(), y.begin(), ewma<>(w).state(&x[0]));
	ensure (y[0] == x[0]);
	ensure (y[1] == w*x[1] + (1 - w)*y[0]);
	ensure (y[2] == w*x[2] + (1 - w)*y[1]); 

}

void
fms_test_macd(void)
{
	std::array<double,3> x = {1, 2, 3}, y, y0, y1;
	double w0 = 0.9, w1 = 0.5;

	ewma<> a0(w0); a0.state(&x[0]);
	ewma<> a1(w1); a1.state(&x[0]);

	std::transform(x.begin(), x.end(), y.begin(), [&a0, &a1](double x) { return a0(x) - a1(x); } );
	std::transform(x.begin(), x.end(), y0.begin(), ewma<>(w0).state(&x[0]));
	std::transform(x.begin(), x.end(), y1.begin(), ewma<>(w1).state(&x[0]));

	for (size_t i = 0; i < x.size(); ++i)
		ensure (y[i] == y0[i] - y1[i]);

	macd<> d(w0, w1);
	double s[2];
	s[0] = s[1] = x[0];
	d.state(s);
	std::transform(x.begin(), x.end(), y0.begin(), d);
	for (size_t i = 0; i < x.size(); ++i)
		ensure (y[i] == y0[i]);
}

void
fms_test_minmax(void)
{
	std::array<double,4> x = {1, 2, 4, -1}, y, ymin = {1, 1, 1, -1}, ymax = {1, 2, 4, 4};

	std::transform(x.begin(), x.end(), y.begin(), minimum<>());
	ensure (y == ymin);

	std::transform(x.begin(), x.end(), y.begin(), maximum<>());
	ensure (y == ymax);
}

void 
fms_test_moments(void)
{
	double x, *m, c[3];
	moments<> m3(3);

	x = 1;
	m = m3.next(&x);
	ensure (m[0] == 1);
	ensure (m[1] == 1);
	ensure (m[2] == 1);
	ensure (m[3] == 1);

	x = 2;
	m = m3.next(&x);
	ensure (m[0] == 2);
	ensure (m[1] == (1 + 2)/2.);
	ensure (m[2] == (1 + 4)/2.);
	ensure (m[3] == (1 + 8)/2.);
	
	central(3, ++m, c);
	ensure (c[0] == 0);
	ensure (c[1] == (m[0]*m[0] - 2*m[0]*m[0] + m[1]));
	ensure (c[2] == (-m[0]*m[0]*m[0] + 3*m[0]*m[0]*m[0] - 3*m[1]*m[0] + m[2]));
}

void
fms_test_tick(void)
{
	std::array<double,4> x = {0, 1, 1, 0}, y;

	std::transform(x.begin(), x.end(), y.begin(), tick_up<>());
	ensure (y[0] == 0);
	ensure (y[1] == 1);
	ensure (y[2] == 0);
	ensure (y[3] == 0);

	double x0 = 1.23;
	std::transform(x.begin(), x.end(), y.begin(), tick_down<>().state(&x0));
	ensure (y[0] == 1.23);
	ensure (y[1] == 0);
	ensure (y[2] == 0);
	ensure (y[3] == 1);
}

void
fms_test_range()
{
	double p[] =
	{ 
		0, 1,
		2, 3,
		4, 5
	};

	range<double*> r(p, p + sizeof(p)/sizeof(*p));

	double s(0);
	for (double* i = r.begin(); i != r.end(); ++i) {
		s += *i;
	}
	ensure (s == 1 + 2 + 3 + 4 + 5);

	range<double*,2> r2(p, p + sizeof(p)/sizeof(*p));
	ensure (*r2 == 0);
	++r2;
	ensure (*r2 == 2);
	++r2;
	ensure (*r2 == 4);

	range_filter<double*> r3(r, [](double* i) { return i[1] == 3; });
	ensure (*r3 == 2);
	++r3;
	ensure (r3.cursor() == r3.end());

	auto r4 = range_join<double*>(range<double*>(p, p + 2), range<double*>(p + 2, p + 4));
	ensure (*++r4 == 1);
	ensure (*++r4 == 2);
	ensure (*++r4 == 3);
	ensure ((++r4).cursor() == r4.end());
}

void
fms_test_blotter(void)
{/*
	std::array<double,28> b =
	{ 
	// (t;    a, i, c;  a', i', c')
		0,  100, 0, 0,  -1, 1, 1,
		1,  -99, 0, 0,  +1, 1, 1,
		2,  -98, 0, 0,  +1, 1, 2, 
		3,  101, 0, 0,  -1, 1, 2 
	};

	range<double*,7> p(b.data(), b.data() + b.size());
	std::array<size_t,1> k = {3};
	std::array<size_t,1> v = {1};
//	std::array<std::function<double(double)>,1> binop = { std::plus<double>() };
	std::array<double,1> x0 = {0};
//	auto kv = group(k, v, p, binop, x0);
//	ensure (kv[0] == p[1] + p[1 + 7] + p[1 + 2*7] + p[1 + 3*7]);

//	auto kv2 = group([](double*i) { return std::make_pair(i[3],i[6]); }, 1, p, std::plus<double>());
//	ensure (kv2[std::make_pair(0,1)] == p[1] + p[1 + 7]);
//	ensure (kv2[std::make_pair(0,2)] == p[1 + 2*7] + p[1 + 3*7]);
*/}

void 
fms_test_technical(void)
{
	fms_test_change();
	fms_test_ewma();
	fms_test_macd();
	fms_test_minmax();
	fms_test_moments();
	fms_test_tick();
	fms_test_range();
	fms_test_blotter();
}