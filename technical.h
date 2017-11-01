// technical.h - platform independent technical indicators
// Copyright (c) 2007-2011 KALX, LLC. All rights reserved. No warranty is made.
#pragma once
#include <array>

namespace technical {

	// For use with Excel.
	template<class T = double>
	class indicator_base {
	public:
		virtual ~indicator_base()
		{ }
		virtual size_t isize() const = 0; // input size to next
		virtual size_t osize() const = 0; // output size from next
		virtual T* next(T*) = 0;
		virtual T operator()(T x) { return *next(&x); } // for scalar indicators

		// size of state
		virtual size_t size() const = 0;
		// set the current state from array data
		virtual indicator_base& state(const T*) = 0;
	};

	// Holds the current state of the indicator.
	// Subclasses hold the static parameters of the indicator.
	template<class T, class S>
	class indicator : public indicator_base<T> {
	protected:
		S s_; // dynamic state
	public:
		indicator(const S& s = S())
			: s_(s)
		{ }
	};

	template<class T = double>
	struct ind {
	};
	template<class T = double>
	struct tai_ewma {
		static const size_t isize = 1;
		static const size_t osize = 1;
		static const size_t psize = 1;
		static const size_t ssize = 1;
		T param[psize];
		T state[ssize];
		tai_ewma(double w)
		{
			param[0] = w;
		}
		void next(const T* i, T* o)
		{
			T w = param[0];

			state[0] = w*i[0] + (1 - w)*state[0];

			o[0] = state[0];
		}
	};
	
} // namespace technical