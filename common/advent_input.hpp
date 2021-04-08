#ifndef ADVENT_INPUT_H
#define ADVENT_INPUT_H

#include <iostream>
#include <iterator>

namespace advent
{
	namespace parsers
	{
		inline auto string_constant(std::string_view target)
		{
			return [=](std::istream& in) -> std::istream&
			{
				for(auto c: target)
				{
					if(in.peek()==c)
						in.get();
					else
					{
						in.setstate(std::ios::failbit);
						return in;
					}
				}		
				return in;
			};
		}
		
		template <typename integral_type>
		inline auto integer(integral_type& target)
		{
			return [&](std::istream& in) -> std::istream&
			{
				target = integral_type{0};
				
				while(std::isdigit(in.peek()))
				{
					target*= integral_type{10};
					target+=integral_type(in.get()-'0');
				}
				
				return in;
			};
		}
		
		inline auto character_constant(char target)
		{
			return [=](std::istream& in) -> std::istream&
			{
				if(in.peek()==target)
					in.get();
				else
					in.setstate(std::ios::failbit);
				
				return in;
			};
		}
		
		inline auto not_character_constant(char target)
		{
			return [=](std::istream& in) -> std::istream&
			{
				if(in.peek()==target)
				{
					in.setstate(std::ios::failbit);
					return in;
				}
				
				while(in.peek()!=target)
					in.get();
				
				return in;
			};
		}
		
		constexpr auto whitespace = [](std::istream& in) -> std::istream&
		{
			if(!std::isspace(in.peek()))
			{
				in.setstate(std::ios::failbit);
				return in;
			}
			
			while(std::isspace(in.peek()))
				in.get();
			
			return in;
		};
		
		template <typename T>
		inline std::istream& parse(std::istream& in, T&& part)
		{
			constexpr bool is_lvalue_ref = std::is_lvalue_reference_v<T>;
			constexpr bool is_const = std::is_const_v<T>;
			using type = std::decay_t<decltype(part)>;
			
			if constexpr (std::is_invocable_v<T,std::istream&>)
			{
				return part(in);
			}
			else if constexpr (std::is_same_v<type,char>)
			{
				if constexpr (!is_lvalue_ref || is_const)
					return character_constant(part)(in);
			}
			else if constexpr (std::is_integral_v<type>)
			{
				if constexpr (is_lvalue_ref && !is_const)
					return integer(std::forward<T>(part))(in);
			}
			else if constexpr (std::is_convertible_v<type,std::string_view>)
			{
				if constexpr (!is_const)
					return string_constant(std::forward<T>(part))(in);
			}

			return in;
		}
		
		template <typename... T>
		inline std::istream& parse(std::istream& in, T&&... parts)
		{
			(parse(in,std::forward<T>(parts)) && ...);
			return in;
		}
	}
}

#endif
		
