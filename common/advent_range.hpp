#ifndef ADVENT_RANGE_H
#define ADVENT_RANGE_H

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <type_traits>

namespace advent
{
	template <typename container_t>
	struct rowwise_2d_range
	{	
		struct end_tag{};
		
		struct iterator
		{
			using iterator_category = std::forward_iterator_tag;
			using value_type = std::decay_t<decltype(*std::begin(*std::begin(std::declval<container_t>())))>;
			using difference_type = std::ptrdiff_t;
			using pointer = value_type*;
			using reference = value_type&;
			
			constexpr explicit iterator(container_t& container):
				container_{container},
				outer_{std::begin(container_)},
				inner_{std::begin(*outer_)}
			{}
			
			constexpr explicit iterator(container_t& container, end_tag):
				container_{container},
				outer_{std::end(container_)}
			{}
			
			constexpr iterator& operator++()
			{
				++inner_;
				if(inner_==std::end(*outer_))
				{
					++outer_;
					if(outer_!=std::end(container_))
						inner_=std::begin(*outer_);
				}
				return *this;
			}
			
			constexpr auto operator*() const
			{
				return *inner_;
			}
			
			constexpr auto operator==(const iterator& other) const
			{
				return outer_==other.outer_ && (outer_==std::end(container_) || inner_==other.inner_);
			} 
			
			constexpr auto operator!=(const iterator& other) const
			{
				return !operator==(other);
			} 
			
			container_t& container_;
			std::decay_t<decltype(std::begin(container_))> outer_;
			std::decay_t<decltype(std::begin(*outer_))> inner_;
		};
		
		constexpr explicit rowwise_2d_range(container_t& container):
			container_{container}
		{}
		
		constexpr auto begin() const { return iterator{container_}; }
		constexpr auto end() const { return iterator{container_,end_tag{}}; }
		
		private:
		container_t& container_;
	};
	
	inline auto rowvector_from_grid(std::istream& in)
	{
		std::string line;
		std::vector<std::string> grid;
		while(std::getline(in,line))
			grid.push_back(line);
			
		return grid;
	}
}

#endif
