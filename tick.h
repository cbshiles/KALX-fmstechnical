// tick.h - up and down ticks
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#pragma once
#include "technical.h"

namespace technical {

	template<class T = double>
	class tick_up : public indicator<T, std::array<T,1>> {
		T dx_;
	public:
		tick_up()
		{
			s_[0] = 0;
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
			dx_ = (std::max)(*px - s_[0], 0.);
			s_[0] = *px;

			return &dx_;
		}
		size_t size() const
		{
			return 1;
		}
		tick_up& state(const T* px)
		{
			s_[0] = *px;

			return *this;
		}
	};

	template<class T = double>
	class tick_down : public indicator<T, std::array<T,1>> {
		T dx_;
	public:
		tick_down()
		{
			s_[0] = 0;
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
			dx_ = (std::max)(s_[0] - *px, 0.);
			s_[0] = *px;

			return &dx_;
		}
		size_t size() const
		{
			return 1;
		}
		tick_down& state(const T* px)
		{
			s_[0] = *px;

			return *this;
		}
	};

} // namespace technical