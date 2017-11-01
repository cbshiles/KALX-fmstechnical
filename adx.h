// adx.h - Average Directional Index
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#pragma once
#include "../fmstechnical/ewma.h"

namespace technical {

	enum {
		ADX_HI,    // today's high
		ADX_LO,    // today's low
		ADX_PC,    // yesterday's close
		ADX_EWMAP, // ewma of plus moves
		ADX_EWMAN, // ewma of negative moves
		ADX_EWMA,  // ewma of difference of positive an negative indicator
		ADX_EWMATR // ewma of true range
	};

	template<class T = double>
	class adx : public indicator<T, std::tuple<double,double,double,ewma<T>,ewma<T>,ewma<T>,ewma<T>>> {
		T adx_;
	public:
		adx(T w)
			: indicator(std::make_tuple(ewma<T>(w0), ewma<T>(w1)))
		{ }
		size_t isize() const
		{
			return 3;
		}
		size_t osize() const
		{
			return 1;
		}
		// high, low, previus
		T* next(T* px)
		{
			/*
			UpMove = Today's High - Yesterday's High 
			DownMove = Yesterday's Low - Today's Low 
			if UpMove > DownMove and UpMove > 0, then +DM = UpMove, else +DM = 0 
			if DownMove > UpMove and DownMove > 0, then -DM = DownMove, else -DM = 0
			+DI = 100 times exponential moving average of +DM divided by Average True Range
			-DI = 100 times exponential moving average of -DM divided by Average True Range
			ADX = 100 times the exponential moving average of the Absolute value of (+DI - -DI) divided by (+DI + -DI)
			*/
			// up/down move
			T um = px[ADX_HI] - std::get<ADX_HI>(s_);
			T dm = std::get<ADX_LO>(s_) - px[ADX_LO];

			// positive/negative
			T pm = um > dm && um > 0 ? um : 0;
			T nm = dm > um && dm > 0 ? dm : 0;

			// average true range
			T tr = (std::max)(px[ADX_HI], px[ADX_PC]) - (std::min)(px[ADX_LO], px[ADX_PC]);
			T atr = std::get<ADX_EWMATR>(s_)(tr);

			// postive/negative moving average
			T dip = std::get<ADX_EWMAP>(s_)(pm)/atr;
			T din = std::get<ADX_EWMAN>(s_)(nm)/atr;

			adx_ = std::get<ADX_EWMA>(s_)(dip - din)/(dip + din);
			std::get<ADX_HI>(s_) = px[ADX_HI];
			std::get<ADX_LO>(s_) = px[ADX_LO];
			std::get<ADX_PC>(s_) = px[ADX_PC];

			return &adx_;
		}
		size_t size() const
		{
			return 6;
		}
		adx& state(const T* s)
		{
			std::get<ADX_HI>(s_) = s[ADX_HI];
			std::get<ADX_LO>(s_) = s[ADX_LO];
			std::get<ADX_PC>(s_) = s[ADX_PC];
			std::get<ADX_EWMAP> (s_).state(&s[ADX_EWMAP]);
			std::get<ADX_EWMAN> (s_).state(&s[ADX_EWMAN]);
			std::get<ADX_EWMA>  (s_).state(&s[ADX_EWMA]);
			std::get<ADX_EWMATR>(s_).state(&s[ADX_EWMATR]);

			return *this;
		}
	};
	
} // namespace technical
