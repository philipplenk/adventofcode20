#include <advent.hpp>

#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

#include <cstdint>

using namespace advent;

int main(int argc, char* argv[])
{
	auto grid = rowvector_from_grid(std::cin);
	const int grid_width = grid[0].size();
	const int grid_height = grid.size();
	
	auto update = [&, next = grid]() mutable
	{
		constexpr std::array<vec2d,8> directions{{{1,0},{0,1},{1,1},{-1,0},{0,-1},{1,-1},{-1,1},{-1,-1}}};
		
		bool changed = false;
		foreach_in_square({0,0},{grid_width-1,grid_height-1},[&](auto p)
		{
			next[p.y][p.x] = grid[p.y][p.x];
			
			if(grid[p.y][p.x]=='.') return;
			
			std::size_t adjacent_count = 0;
			for(auto d: directions)
			{
				auto target = p+d;
				while(target.x>=0 && target.x<grid_width && target.y>=0 && target.y<grid_height)
				{
					if(grid[target.y][target.x]=='#')
					{
						++adjacent_count;
						break;
					}
					if(grid[target.y][target.x]=='L')
						break;
					
					target+=d;
				}
			}
			
			if(grid[p.y][p.x]=='L' && adjacent_count==0)
			{
				next[p.y][p.x] = '#';
				changed = true;
			}
			else if(grid[p.y][p.x]=='#' && adjacent_count>=5)
			{
				next[p.y][p.x] = 'L';
				changed = true;
			}
		});
		
		std::swap(grid,next);
		return changed;
	};
	
	while(update());
	
	const auto grid_range = rowwise_2d_range{grid};
	std::cout<<std::count(std::begin(grid_range),std::end(grid_range),'#')<<'\n';
		
	return 0;
}
