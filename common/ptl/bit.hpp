#ifndef PHIL_TEMPLATE_LIBRARY_BIT_H
#define PHIL_TEMPLATE_LIBRARY_BIT_H

#include <limits>

#include <cstring>

#ifndef __has_builtin
#define __has_builtin(x) false
#define redefined__has_builtin__
#endif

namespace ptl
{
	template <typename To, typename From>
	auto bit_cast(const From& src) noexcept
	{
		To dest;
		std::memcpy(&dest,&src, sizeof(dest));
		return dest;
	}

	template <typename T>
	constexpr auto popcount(T num) noexcept
	{
		if constexpr(sizeof(T)<=sizeof(unsigned) && __has_builtin(__builtin_popcount))
		{
			return __builtin_popcount(num);
		}
		else if constexpr(sizeof(T)<=sizeof(unsigned long) && __has_builtin(__builtin_popcountl))
		{
			return __builtin_popcountl(num);
		}
		else if constexpr(sizeof(T)<=sizeof(unsigned long long) && __has_builtin(__builtin_popcountll))
		{
			return __builtin_popcountll(num);
		}
		else
		{
			unsigned count=0;
			for(;num;++count)
				num&=(num-1);
			return count;
		}
	}
	
	template <typename T>
	constexpr unsigned countr_zero(T n) noexcept
	{
		if constexpr(sizeof(T)<=sizeof(unsigned) && __has_builtin(__builtin_ctz))
		{
			return n==0?(sizeof(T)*8):__builtin_ctz(n);
		}
		else if constexpr(sizeof(T)<=sizeof(unsigned long) && __has_builtin(__builtin_ctzl))
		{
			return n==0?(sizeof(T)*8):__builtin_ctzl(n);
		}
		else if constexpr(sizeof(T)<=sizeof(unsigned long long) && __has_builtin(__builtin_ctzll))
		{
			return n==0?(sizeof(T)*8):__builtin_ctzll(n);
		}
		else
		{
			unsigned i = 0;
			while(n)
			{
				n <<= 1;
				i++;
			}
			return sizeof(n)*8-i;
		}
	}
	
	template <typename T>
	constexpr unsigned countl_zero(T n) noexcept
	{
		if constexpr(sizeof(T)<=sizeof(unsigned) && __has_builtin(__builtin_clz))
		{
			return n==0?(sizeof(T)*8):__builtin_clz(n);
		}
		else if constexpr(sizeof(T)<=sizeof(unsigned long) && __has_builtin(__builtin_clzl))
		{
			return n==0?(sizeof(T)*8):__builtin_clzl(n);
		}
		else if constexpr(sizeof(T)<=sizeof(unsigned long long) && __has_builtin(__builtin_clzll))
		{
			return n==0?(sizeof(T)*8):__builtin_clzll(n);
		}
		else
		{
			unsigned i = 0;
			while(n)
			{
				n >>= 1;
				i++;
			}
			return sizeof(n)*8-i;
		}
	}
	
	template <typename T>
	constexpr T bit_width(T n) noexcept
	{
		return std::numeric_limits<T>::digits - ptl::countl_zero(n);
	}
	
	template <typename T>
	constexpr T bit_floor(T n) noexcept
	{
		if(n==0) return 0;
		return T{1} << (ptl::bit_width(n)-1);
	}
	
} //end namespace ptl

#ifdef redefined__has_builtin__
#undef redefined__has_builtin__
#undef has_builtin
#endif

#endif
