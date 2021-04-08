#include <advent.hpp>

#include <iostream>
#include <string>
#include <vector>

using namespace advent;

int main(int argc, char* argv[])
{
	std::string line;
	std::vector<std::string> grid;
	while(std::getline(std::cin,line))
		grid.push_back(line);
		
	vec2d direction{3,1};
	point2d pos{0,0};
	
	int count = 0;
	while(pos.y<static_cast<int>(grid.size()))
	{
		pos.x%=grid[0].size();
		if(grid[pos.y][pos.x]=='#')
			++count;
		
		pos+=direction;
	};
	
	std::cout<<count<<'\n';
	
	return 0;
}
