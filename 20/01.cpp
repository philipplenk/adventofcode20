#include <algorithm>
#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
	std::unordered_map<long,std::vector<std::string>> tiles;
	
	std::string line;
	while(std::getline(std::cin,line))
	{
		std::stringstream strm;
		strm<<line;
		
		long id;
		strm>>line>>id;
		
		std::vector<std::string> grid;
		while(std::getline(std::cin,line) && !line.empty())
			grid.push_back(line);
		
		tiles[id] = grid;
	}
	
	std::unordered_map<long,std::vector<long>> neighbors;
	
	const auto get_borders = [&](const auto& grid)
	{
		std::array<std::string,4> borders;
		borders[0]=grid[0];
		borders[1]=grid.back();
		for(const auto& line: grid)
		{
			borders[2].push_back(line[0]);
			borders[3].push_back(line.back());
		}
		return borders;
	};
	
	const auto match_sides = [&](const auto& grid_lhs, const auto& grid_rhs)
	{
		auto lhs_borders = get_borders(grid_lhs);
		auto rhs_borders = get_borders(grid_rhs);
		
		for(const auto& lhs: lhs_borders)
			for(const auto& rhs: rhs_borders)
				if(lhs==rhs)
					return true;
					
		for(auto& rhs: rhs_borders)
			std::reverse(std::begin(rhs),std::end(rhs));
		
		for(const auto& lhs: lhs_borders)
			for(const auto& rhs: rhs_borders)
				if(lhs==rhs)
					return true;
					
		return false;
	};
	
	for(auto it = tiles.begin(); it!=tiles.end();++it)
	{
		for(auto other_it = std::next(it); other_it!=tiles.end();++other_it)
		{
			if(match_sides(it->second,other_it->second))
			{
				neighbors[it->first].push_back(other_it->first);
				neighbors[other_it->first].push_back(it->first);
			}
		}
	}
	
	long product = 1;
	for(const auto& [id,neighbors]: neighbors)
	{
		if(neighbors.size()==2)
			product*=id;
	}
	std::cout<<product<<'\n';
	
	return 0;
}
