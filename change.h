// change.h - change from previous value
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#pragma once
#include <cmath>
#include "technical.h"

namespace technical {

	enum { CHANGE_PREV };

	template<class T = double>
	class change : public indicator<T, std::array<T,1>> {
		T dx_;
	public:
		change()
		{
			s_[CHANGE_PREV] = 0;
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
			dx_ = *px - s_[CHANGE_PREV];
			s_[CHANGE_PREV] = *px;

			return &dx_;
		}
		size_t size() const
		{
			return 1;
		}
		change& state(const T* px)
		{
			s_[CHANGE_PREV] = *px;

			return *this;
		}
	};

	template<class T = double>
	class change_relative : public indicator<T, std::array<T,1>> {
		T dx_;
	public:
		change_relative()
		{
			s_[CHANGE_PREV] = 0;
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
			dx_ = s_[CHANGE_PREV] == *px ? 0 : s_[CHANGE_PREV] ? (*px - s_[CHANGE_PREV])/s_[CHANGE_PREV] : std::numeric_limits<T>::infinity();
			s_[CHANGE_PREV] = *px;

			return &dx_;
		}
		size_t size() const
		{
			return 1;
		}
		change_relative& state(const T* px)
		{
			s_[CHANGE_PREV] = px[CHANGE_PREV];

			return *this;
		}
	};

	template<class T = double>
	class change_logarithmic : public indicator<T, std::array<T,1>> {
		T dx_;
	public:
		change_logarithmic()
		{
			s_[CHANGE_PREV] = 0;
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
			dx_ = s_[CHANGE_PREV] == *px ? 0 : s_[CHANGE_PREV] ? log(*px/s_[CHANGE_PREV]) : std::numeric_limits<T>::infinity();
			s_[CHANGE_PREV] = *px; 

			return &dx_;
		}
		size_t size() const
		{
			return 1;
		}
		change_logarithmic& state(const T* px)
		{
			s_[CHANGE_PREV] = px[CHANGE_PREV];

			return *this;
		}
	};

} // namespace technical