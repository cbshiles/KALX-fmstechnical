// range.h - range class
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
// A range is a wrapper around a sequence container.
#pragma once
#include <functional>
#include <map>

// range row???

template<class I, std::size_t S = 1>
class range {
protected:
	I b_, e_, i_;
public:
	range(I b, I e)
		: b_(b), e_(e), i_(b)
	{ }
	virtual ~range()
	{ }

	operator I()
	{
		return i_;
	}

	I begin() 
	{
		return b_; 
	}
	const I begin() const
	{
		return b_; 
	}
	I end() 
	{ 
		return e_; 
	}
	const I end() const
	{ 
		return e_; 
	}
	I cursor()
	{
		return i_;
	}
	const I cursor() const
	{
		return i_;
	}

	typename std::iterator_traits<I>::value_type operator*()
	{
		return *i_;
	}
	virtual range& operator++()
	{
		i_ += S;

		return *this;
	}
};

template<class I, std::size_t S = 1>
class range_filter : public range<I,S> {
	std::function<bool(I)> f_;
public:
	range_filter(const range<I>& r, std::function<bool(I)> f)
		: range<I>(r), f_(f)
	{
		// advance to first true value
		while (i_ != end() && !f(i_))
			operator++();
	}
	range_filter& operator++()
	{
		for (range<I>::operator++(); i_ != end() && !f_(i_); range<I>::operator++())
			;

		return *this;
	}
};

template<class I>
class range_join : public range<I> {
protected:
	bool first_;
	range<I> r_;
public:
	range_join(const range<I>& r0, const range<I>& r1)
		: first_(true), range<I>(r0), r_(r1)
	{ }
	range_join& operator++()
	{
		range<I>::operator++();
		if (first_ && i_ == e_) {
			first_ = false;
			range<I>::operator=(r_);
		}

		return *this;
	}
};

// use range row class
template<class T, std::size_t N>
inline
std::array<T, N> 
index(const T* a, const std::array<std::size_t, N>& i)
{
	std::array<T, N> ai;
	
	for (size_t j = 0; j < N; ++i)
		ai[j] = a[i[j]];

	return ai;
}

// group values by key using binop
template<class I, std::size_t S, std::size_t K, std::size_t V> 
inline
std::map<std::array<size_t, K>,
		 std::array<typename std::iterator_traits<I>::value_type, V>
>
group(std::array<size_t, K> key,
      std::array<size_t, V> value,
	  range<I,S> r,
	  std::array<std::function<
		typename std::iterator_traits<I>::value_type(
			typename std::iterator_traits<I>::value_type,
			typename std::iterator_traits<I>::value_type
		)
	>,V> binop,
	std::array<typename std::iterator_traits<I>::value_type,V> x0)
{
std::map<std::array<typename std::iterator_traits<I>::value_type, K>,
		 std::array<typename std::iterator_traits<I>::value_type, V>

	for (I i = r.begin(); i != r.end(); i = ++r) {
		auto k = index(i, key);
		auto v = kv.find(k);
		for (std::size_t j = 0; j < V; ++j) {
			if (v != kv.end())
				v->second[j] = binop[j](v->second[j], i[value[j]]);
			else
				kv[k]->second[j] = binop(x0[j], i[value[j]]);
		}
	}

	return kv;
}