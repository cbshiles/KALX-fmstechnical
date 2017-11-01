// macd.h - Moving Average Convergence Divergence
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#pragma once
#include "ewma.h"

namespace technical {

	enum { EWMA0, EWMA1 };

	template<class T = double>
	class macd : public indicator<T, std::tuple<ewma<T>,ewma<T>>> {
		T dx_;
	public:
		macd(T w0, T w1)
			: indicator(std::make_tuple(ewma<T>(w0), ewma<T>(w1)))
		{ }
		size_t isize() const
		{
			return 1;
		}
		size_t osize() const
		{
			return 1;
		}
		T* next(T* px)
		{
			dx_ = std::get<EWMA0>(s_)(*px) - std::get<EWMA1>(s_)(*px);

			return &dx_;
		}
		size_t size() const
		{
			return 2;
		}
		macd& state(const T* s)
		{
			std::get<EWMA0>(s_).state(&s[EWMA0]);
			std::get<EWMA1>(s_).state(&s[EWMA1]);

			return *this;
		}
	};
	
} // namespace technical
