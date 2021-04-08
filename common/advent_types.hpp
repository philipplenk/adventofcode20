#ifndef ADVENT_TYPES_H
#define ADVENT_TYPES_H

#include <iostream>
#include <tuple>

#include <cstddef>
#include <cstdint>

namespace advent
{
	struct point2d
	{
		int x,y;
	};
	
	constexpr bool operator<(const point2d& lhs, const point2d&rhs) noexcept
	{
		return std::tie(lhs.x,lhs.y)<std::tie(rhs.x,rhs.y);
	}
	
	constexpr bool operator==(const point2d& lhs, const point2d&rhs) noexcept
	{
		return lhs.x==rhs.x && lhs.y==rhs.y;
	}
	
	constexpr bool operator!=(const point2d& lhs, const point2d&rhs) noexcept
	{
		return !(lhs==rhs);
	}
	
	template <typename fun_t>
	void foreach_in_square(point2d start, point2d end, const fun_t& fun) noexcept
	{
		for(auto y=start.y;y<=end.y;++y)
			for(auto x=start.x;x<=end.x;++x)
				fun(point2d{x,y});
	}
	
	struct vec2d
	{
		int x,y;
	};
	
	constexpr bool operator==(const vec2d& lhs, const vec2d&rhs) noexcept
	{
		return lhs.x==rhs.x && lhs.y==rhs.y;
	}
	
	constexpr bool operator!=(const vec2d& lhs, const vec2d&rhs) noexcept
	{
		return !(lhs==rhs);
	}
	
	constexpr vec2d operator*(const vec2d& lhs, int rhs) noexcept
	{
		return {lhs.x*rhs,lhs.y*rhs};
	}
	
	constexpr vec2d& operator+=(vec2d& lhs, const vec2d& rhs) noexcept
	{
		lhs.x+=rhs.x;
		lhs.y+=rhs.y;
		return lhs;
	}
	
	constexpr point2d& operator+=(point2d& lhs, const vec2d& rhs) noexcept
	{
		lhs.x+=rhs.x;
		lhs.y+=rhs.y;
		return lhs;
	}
	
	constexpr point2d operator+(point2d lhs, const vec2d& rhs) noexcept
	{
		return lhs+=rhs;
	}
	
	struct point3d
	{
		int x,y,z;
	};
	
	constexpr bool operator<(const point3d& lhs, const point3d&rhs) noexcept
	{
		return std::tie(lhs.x,lhs.y,lhs.z)<std::tie(rhs.x,rhs.y,rhs.z);
	}
	
	constexpr bool operator==(const point3d& lhs, const point3d&rhs) noexcept
	{
		return lhs.x==rhs.x && lhs.y==rhs.y && lhs.z==rhs.z;
	}
	
	constexpr bool operator!=(const point3d& lhs, const point3d&rhs) noexcept
	{
		return !(lhs==rhs);
	}
	
	struct vec3d
	{
		int x,y,z;
	};
	
	
	constexpr bool operator==(const vec3d& lhs, const vec3d&rhs) noexcept
	{
		return lhs.x==rhs.x && lhs.y==rhs.y && lhs.z==rhs.z;
	}
	
	constexpr bool operator!=(const vec3d& lhs, const vec3d&rhs) noexcept
	{
		return !(lhs==rhs);
	}
	
	constexpr vec3d operator*(const vec3d& lhs, int rhs) noexcept
	{
		return {lhs.x*rhs,lhs.y*rhs, lhs.z*rhs};
	}
	
	constexpr vec3d& operator+=(vec3d& lhs, const vec3d& rhs) noexcept
	{
		lhs.x+=rhs.x;
		lhs.y+=rhs.y;
		lhs.z+=rhs.z;
		return lhs;
	}
	
	constexpr point3d& operator+=(point3d& lhs, const vec3d& rhs) noexcept
	{
		lhs.x+=rhs.x;
		lhs.y+=rhs.y;
		lhs.z+=rhs.z;
		return lhs;
	}
	
	constexpr point3d operator+(point3d lhs, const vec3d& rhs) noexcept
	{
		return lhs+=rhs;
	}
	
	enum class direction
	{
		up, down, left, right
	};
	
	std::istream& operator>>(std::istream& in, direction& d)
	{
		char c;
		in>>c;
		switch(c)
		{
			case '<': d=direction::left; break;
			case '>': d=direction::right; break;
			case '^': d=direction::up; break;
			case 'v': d=direction::down; break;
			default: in.setstate(std::ios_base::failbit);
		}
		
		return in;
	}
	
	constexpr vec2d to_vec(direction d) noexcept
	{
		switch(d)
		{
			case direction::up: return {0,-1};
			case direction::down: return {0,1};
			case direction::left: return {-1,0};
			case direction::right: return {1,0};
		}
		return {0,0};
	}
	
	enum class cardinal_direction
	{
		north, south, west, east
	};
}

namespace std
{
	template <>
	struct hash<advent::point2d>
	{
		std::size_t operator()(const advent::point2d& p) const noexcept
		{
			return p.x^p.y;
		}
	};
	
	template <>
	struct hash<advent::vec2d>
	{
		std::size_t operator()(const advent::vec2d& p) const noexcept
		{
			return p.x^p.y;
		}
	};
}

#endif
