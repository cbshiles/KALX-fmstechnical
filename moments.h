// moments.h - moments of data
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#pragma once
#include <cmath>
#include <array>
#include <valarray>
#include "technical.h"

namespace technical {

	template<class T = double>
	class moments : public indicator<T, std::valarray<T>> {
	public:
		moments(size_t n)
			: indicator<T, std::valarray<T>>(std::valarray<T>(n + 1))
		{
			s_ = 0;
		}
		size_t isize() const
		{
			return 1;
		}
		size_t osize() const
		{
			return s_.size();
		}
		T* next(T* x)
		{
			T xk = 1;

			s_[0] += 1;
			for (size_t i = 1; i < s_.size(); ++i) {
				xk *= *x;
				s_[i] += (xk - s_[i])/s_[0];
			}

			return &s_[0];
		}
		size_t size() const
		{
			return s_.size();
		}
		moments& state(const T* px)
		{
			std::copy(px, px + size(), &s_[0]);

			return *this;
		}
	};

	// convert moments (m1, ..., mk) to central moments (c1, ..., ck)
	template<class T> inline
	T* central(int k, const T* m, T* c)
	{
		c[0] = 0; // the mean

		for (int j = 1; j < k; ++j) {
			double _mj_i = pow(-m[0], j + 1);
			c[j] = _mj_i;
			double Cij = 1; // C(i,j)
			for (int i = 0; i <= j; ++i) {
				_mj_i /= -m[0];
				Cij *= j + 1 - i;
				Cij /= i + 1;
				c[j] += Cij*m[i]*_mj_i;
			}
		}

		return c;
	}

} // namespace technical