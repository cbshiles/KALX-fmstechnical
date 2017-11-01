// minmax.h - minimum or maximum value
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#pragma once
#include <cmath>
#include "technical.h"

namespace technical {

	template<class T = double>
	class minimum : public indicator<T, std::array<T,1>> {
		T x_;
	public:
		minimum()
		{
			s_[0] = std::numeric_limits<T>::max();
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
			x_ = (std::min)(*px, s_[0]);
			s_[0] = x_;

			return &x_;
		}
		size_t size() const
		{
			return 1;
		}
		minimum& state(const T* px)
		{
			s_[0] = *px;

			return *this;
		}
	};

	template<class T = double>
	class maximum : public indicator<T, std::array<T,1>> {
		T x_;
	public:
		maximum()
		{
			s_[0] = -std::numeric_limits<T>::max();
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
			x_ = (std::max)(*px, s_[0]);
			s_[0] = x_;

			return &x_;
		}
		size_t size() const
		{
			return 1;
		}
		maximum& state(const T* px)
		{
			s_[0] = *px;

			return *this;
		}
	};

} // namespace technical