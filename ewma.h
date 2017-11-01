// ewma.h - Exponentially Weighted Moving Average
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#pragma once
#include <tuple>
#include "technical.h"

namespace technical {

	enum { EWMA_PREV };

	template<class T = double>
	struct ewma : public indicator<T,std::array<T,1>> {
		T w_;
	public:
		ewma(T w = 0)
			: w_(w > 1 ? (w - 1)/(w + 1) : w)
		{
			s_[EWMA_PREV] = 0;
		}
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
			s_[EWMA_PREV] = (*px)*(1 - w_) + s_[EWMA_PREV]*w_;

			return &s_[EWMA_PREV];
		}
		size_t size() const
		{
			return 1;
		}
		ewma& state(const T* s)
		{
			s_[EWMA_PREV] = s[EWMA_PREV];

			return *this;
		}
	};

} // namespace technical
