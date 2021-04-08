#include <advent.hpp>

#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include <cstddef>

using namespace advent;

int main(int argc, char* argv[])
{
	const std::vector<std::string> grid(std::istream_iterator<std::string>{std::cin},{});
		
	const auto count_trees=[&](auto direction)
	{
		point2d pos{0,0};
		
		int count = 0;
		while(pos.y<static_cast<int>(grid.size()))
		{
			pos.x%=grid[0].size();
			if(grid[pos.y][pos.x]=='#')
				++count;
			
			pos+=direction;
		};
		return count;
	};
	
	std::size_t result = 1;
	
	for(auto d: {vec2d{1,1},vec2d{3,1},vec2d{5,1},vec2d{7,1},vec2d{1,2}})
		result*=count_trees(d);
	
	std::cout<<result<<'\n';
	
	return 0;
}
