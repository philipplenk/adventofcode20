#include <advent.hpp>

#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include <cstddef>

using namespace advent;

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
	
	long start = 0;
	for(const auto& [id,neighbors]: neighbors)
	{
		if(neighbors.size()==2)
		{
			start = id;
			break;
		}
	}
	
	std::unordered_map<long,bool> used;
	std::map<point2d,long> grid;
	
	const auto place_tile = [&](long id)
	{
		std::size_t found = 0;
		std::array<point2d,2> n_coord;
		for(auto [p,n_id]: grid)
		{
			for(auto n: neighbors[id])
				if(n_id==n)
				{
					n_coord[found++]=p;
					break;
				}
			if(found==2)
				break;
		}
		
		const point2d place_at = {std::max(n_coord[0].x,n_coord[1].x),std::max(n_coord[0].y,n_coord[1].y)};
		used[id] = true;
		grid[place_at] = id;
	};
	
	const auto place_missing_at = [&](long set_neighbor_id, vec2d direction)
	{
		point2d n_coord{0,0};
		for(auto [p,n_id]: grid)
		{
			if(n_id == set_neighbor_id)
			{
				n_coord=p;
				break;
			}
		}
		
		for(auto n_id: neighbors[set_neighbor_id])
		{
			if(!used[n_id])
			{			
				used[n_id] = true;
				return grid[n_coord+direction] = n_id;
			}
		}
		
		return set_neighbor_id;
		
	};
	
	const auto fill = [&](point2d p, long id, long right_id, long down_id)
	{
		used[id] = true;
		used[right_id] = true;
		used[down_id] = true;
		
		grid[p] = id;
		grid[p+vec2d{1,0}] = right_id;
		grid[p+vec2d{0,1}] = down_id;
		
		while(grid.size()<neighbors.size())
		{
			for(const auto& [id, n]: neighbors)
			{
				std::size_t defined_count = 0;
				for(auto n_id: n)
					defined_count += used[n_id]?1:0;
					
				if(!used[id] && defined_count>=2)
					place_tile(id);
				
			}
			
			right_id = place_missing_at(right_id,vec2d{1,0});
			down_id = place_missing_at(down_id,vec2d{0,1});
		}
	};
	
	fill({0,0},start,neighbors[start][0],neighbors[start][1]);
	
	int max_x=0, max_y=0;
	for(const auto& [p,ignored]: grid)
	{
		max_x = std::max(p.x,max_x);
		max_y = std::max(p.y,max_y);
	}
	
	const auto rotate = [](const auto& tile)
	{
		std::vector<std::string> ret_val;
		ret_val.resize(tile[0].size());
		for(auto& row: ret_val)
			row.resize(tile.size());
		
		foreach_in_square(point2d{0,0},point2d{(int)(tile[0].size()-1),(int)(tile.size()-1)},[&](auto p)
		{
			ret_val[p.x][p.y]=tile[tile.size()-1-p.y][p.x];
		});
		
		return ret_val;
	};
	
	const auto flip = [](const auto& tile)
	{
		auto cpy = tile;
		for(auto& row: cpy)
			std::reverse(std::begin(row),std::end(row));
		return cpy;
	};
	
	std::cout<<"Fixing rotations\n";

	for(int y=0;y<=max_y-1;++y)
	{
		for(int x=0;x<=max_x;++x)
		{
			bool done = false;
			for(std::size_t outer = 0;outer<4;++outer)
			{
				tiles[grid[{x,y}]] = rotate(tiles[grid[{x,y}]]);
				for(std::size_t inner = 0;inner<4;++inner)
				{
					tiles[grid[{x,y+1}]] = rotate(tiles[grid[{x,y+1}]]);
					auto reversed_top = tiles[grid[{x,y+1}]][0];
					std::reverse(std::begin(reversed_top),std::end(reversed_top));
					if(tiles[grid[{x,y}]].back()==tiles[grid[{x,y+1}]][0] || tiles[grid[{x,y}]].back()==reversed_top)
					{
						done = true;
						break;
					}
				}
				if(done)
					break;
			}
		}
	}
	
	std::cout<<"Fixing flips\n";
	for(int y=0;y<=max_y;++y)
	{
		for(int x=0;x<=max_x-1;++x)
		{
			bool done = false;
			for(std::size_t outer = 0;outer<2;++outer)
			{
				tiles[grid[{x,y}]] = flip(tiles[grid[{x,y}]]);
				for(std::size_t inner = 0;inner<2;++inner)
				{
					tiles[grid[{x+1,y}]] = flip(tiles[grid[{x+1,y}]]);
					
					done = true;
					for(int cy = 0;cy<10;++cy)
						if(tiles[grid[{x,y}]][cy].back()!=tiles[grid[{x+1,y}]][cy][0])
							done=false;
					
					if(done)
						break;
				}
				if(done)
					break;
			}
		}
	}
	
	std::cout<<"Alright, building final search grid...\n";
	
	std::vector<std::string> final_grid;
	for(int y=0;y<=max_y;++y)
	{
		for(int l=1;l<9;++l)
		{
			std::string line;
			for(int x=0;x<=max_x;++x)
			{
				auto line_part = tiles[grid[{x,y}]][l];
				line_part = line_part.substr(1);
				line_part.pop_back();
				line+=line_part;
			}
			final_grid.push_back(line);
		}
	}
	
	for(const auto& line: final_grid)
		std::cout<<line<<'\n';
		
	std::vector<std::string> sea_monster = 
	{
		"                  # ",
		"#    ##    ##    ###",
		" #  #  #  #  #  #   ",
	};
	
	const auto find_monsters = [&](const auto& grid)
	{
		std::size_t count = 0;
		for(std::size_t y=0;y<grid.size()-3;++y)
		{
			for(std::size_t x=0;x<grid[0].size()-sea_monster[0].size();++x)
			{
				bool failed = false;
				for(std::size_t sy=0;sy<3;++sy)
				{
					for(std::size_t sx=0;sx<sea_monster[0].size();++sx)
					{
						if(sea_monster[sy][sx]=='#' && grid[y+sy][x+sx]!=sea_monster[sy][sx])
						{
							failed = true;
							break;
						}
					}
					if(failed)
						break;
				}
				if(!failed)
					++count;
			}
		}
		return count;
	};
	
	std::size_t monster_count = 0;
	for(std::size_t rotations = 0;rotations<4;++rotations)
	{
		final_grid = rotate(final_grid);
		for(std::size_t flips = 0;flips<2;++flips)
		{
			final_grid = flip(final_grid);
			
			auto cnt = find_monsters(final_grid);
			if(cnt!=0)
			{
				std::cout<<"Number of sea monsters: "<<cnt<<'\n';
				monster_count = cnt;
				break;
			}
			
		}
	}
	
	const auto grid_range = rowwise_2d_range{final_grid};
	const auto monster_range = rowwise_2d_range{sea_monster};
	std::cout<<std::count(std::begin(grid_range),std::end(grid_range),'#')-std::count(std::begin(monster_range),std::end(monster_range),'#')*monster_count<<'\n';
	
	return 0;
}
