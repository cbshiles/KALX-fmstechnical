// true_range.h - High/low range incorporating previous last.
// Copyright (c) 2012 KALX, LLC. All rights reserved. No warranty is made.
#pragma once
#include <cmath>
#include <limits>
#include "technical.h"

namespace technical {

	template<class T = double>
	class true_range : public indicator<T, std::array<T,1>> {
		T tr_;
	public:
		// previous last price
		true_range(double s0 = std::numeric_limits<double>::quiet_NaN())
		{
			s_[0] = s0;
		}
		// high, low, last
		size_t isize() const
		{
			return 3;
		}
		size_t osize() const
		{
			return 1;
		}
		T* next(T* px)
		{
			if (_isnan(s_[0]))
				tr_ = px[0] - px[1]; // high - low
			else
				tr_ = (std::max)(px[0], s_[0]) - (std::min)(px[1], s_[0]);

			s_[0] = px[2];

			ensure (tr_ >= 0);

			return &tr_;
		}
		size_t size() const
		{
			return 1;
		}
		true_range& state(const T* px)
		{
			s_[0] = *px;

			return *this;
		}
	};

} // namespace technical