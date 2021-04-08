#include <advent.hpp>

#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

#include <cstddef>

using namespace advent;

int main(int argc, char* argv[])
{
	auto grid = rowvector_from_grid(std::cin);
	const int grid_width = grid[0].size();
	const int grid_height = grid.size();
	
	const auto update = [&]()
	{
		constexpr std::array<vec2d,8> directions{{{1,0},{0,1},{1,1},{-1,0},{0,-1},{1,-1},{-1,1},{-1,-1}}};
		
		auto next = grid;
		bool changed = false;
		foreach_in_square({0,0},{grid_width-1,grid_height-1},[&](auto p)
		{
			if(grid[p.y][p.x]=='.') return;
			
			std::size_t adjacent_count = 0;
			for(auto d: directions)
			{
				const auto target = p+d;
				if(target.x>=0 && target.x<grid_width && target.y>=0 && target.y<grid_height)
					adjacent_count+=(grid[target.y][target.x]=='#');
			}
			
			if(grid[p.y][p.x]=='L' && adjacent_count==0)
			{
				next[p.y][p.x] = '#';
				changed = true;
			}
			else if(grid[p.y][p.x]=='#' && adjacent_count>=4)
			{
				next[p.y][p.x] = 'L';
				changed = true;
			}
		});
		
		grid = next;
		return changed;
	};
	
	while(update());
	
	const auto grid_range = rowwise_2d_range{grid};
	std::cout<<std::count(std::begin(grid_range),std::end(grid_range),'#')<<'\n';
	
	return 0;
}
