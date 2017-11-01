// lag.h - lag from previous value
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#pragma once
#include <cmath>
#include <valarray>
#include "technical.h"

namespace technical {

	template<class T = double>
	class lag : public indicator<T, std::valarray<T>> {
		size_t n_;
	public:
		lag(size_t n)
			: n_(n)
		{
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
			static T empty = std::numeric_limits<T>::quiet_NaN();

			if (s_.size() < n_) {
				std::valarray<T> s(s_.size() + 1);
				s[std::slice(1, s_.size(), 1)] = s_;
				s_.swap(s);
			}
			else {
				s_.shift(-1);
			}

			if (s_.size() > 0)
				s_[0] = *px;

			return n_ == s_.size() ? &s_[s_.size() - 1] : &empty;
		}
		size_t size() const
		{
			return s_.size();
		}
		lag& state(const T* px)
		{
			s_ = std::valarray<T>(px, n_);

			return *this;
		}
	};


} // namespace technical