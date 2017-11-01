// blotter.h - Functions related to trade blotter.
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
/*
A blotter consists of entries of the form (t; a, i, c; a', i', c') where
t is the time of the trade, a is the amount the buyer c received of
instrument i and a' is the amount of instrument i' given to the seller c'.
*/
#pragma once
#include <functional>
#include <map>
#include "range.h"

namespace technical {

	template<class T, class A, class I, class C>
	std::map<std::pair<I,C>,A> accumulate(const range<T>& r, const std::function<C*(T)>& buyer, const std::function<C*(T)>& seller, 
		const std::function<A(C*)>& amount, const std::function<I(C*)>& instrument)
	{
		std::map<std::pair<I,C>,A> a;

		for (r = r.begin(); r != r.end(); ++r) {
			a[std::make_pair(instrument(buyer(*r)), buyer(*r))] += amount(buyer(*r));
			a[std::make_pair(instrument(seller(*r)), seller(*r))] += amount(seller(*r));
		}

		return a;
	}

} // namespace techncial