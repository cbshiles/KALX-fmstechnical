// relative_strength.h - Relative Strength and index.
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#pragma once
#include "ewma.h"
#include "tick.h"

namespace technical {

	template<class T = double>
	class relative_strength : indicator<T, std::tuple<tick_up<T>,ewma<T>, tick_down<T>,ewma<T>>> {
		T rs_;
		enum { TICK_UP, EWMA_UP, TICK_DN, EWMA_DN };
	public:
		relative_strength(T w)
			: indicator(std::make_tuple(tick_up<T>(), ewma<T>(w), tick_down<T>(), ewma<T>(w)))
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
			T u, d;

			u = std::get<EWMA_UP>(s_)(std::get<TICK_UP>(s_)(*px));
			d = std::get<EWMA_DN>(s_)(std::get<TICK_DN>(s_)(*px));
			rs_ = d ? u/d : std::numeric_limits<T>::infinity();

			return &rs_;
		}
		size_t size() const
		{
			return 4;
		}
		relative_strength& state(const T* s)
		{
			std::get<0>(s_).state(&s[0]);
			std::get<1>(s_).state(&s[1]);
			std::get<2>(s_).state(&s[2]);
			std::get<3>(s_).state(&s[3]);

			return *this;
		}
		// relative strength to relative strength index between 0 and 1
		static T index(T rs)
		{
			return rs == std::numeric_limits<T>::infinity() ? 1 : 1 - 1/(1 + rs); // need test???
		}
	};
} // namespace technical
