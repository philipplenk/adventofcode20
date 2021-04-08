#include <advent.hpp>

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

#include <cstddef>

using namespace advent;

int main(int argc, char* argv[])
{
	const auto directions = []()
	{
		std::array<vec3d,26> ret_val;
			
		std::size_t count = 0;
		for(int z=-1;z<2;++z)
			for(int y=-1;y<2;++y)
				for(int x=-1;x<2;++x)
					if(!(x==y && y==z && z==0))
						ret_val[count++]=vec3d{x,y,z};
		return ret_val;
	}();
		
	const auto top_slice = rowvector_from_grid(std::cin);
	using layer = std::vector<std::string>;

	std::vector<layer> grid3d;
	grid3d.push_back(top_slice);
	
	int grid_width = top_slice[0].size();
	int grid_height = top_slice.size();
	
	const auto valid_in_original = [&](auto p)
	{
		return p.z>=0 && p.z<static_cast<int>(grid3d.size()) && 
			p.y>=0 && p.y<grid_height && 
			p.x>=0 && p.x<grid_width;
	};
	
	const auto update_grid = [&]()
	{
		std::vector<layer> ret_val;
		
		for(int z=-1;z<static_cast<int>(grid3d.size()+1);++z)
		{
			ret_val.push_back({});
			ret_val.back().resize(grid_height+2);
			for(auto& row: ret_val.back())
				row.resize(grid_width+2);
			
			foreach_in_square({-1,-1},{grid_width,grid_height},[&](auto p2)
			{
				auto p3 = point3d{p2.x,p2.y,z};
				
				std::size_t count = 0;
				for(auto v: directions)
				{
					if(auto target = p3+v; valid_in_original(target))					
						count += grid3d[target.z][target.y][target.x]=='#';
				}
				 
				if(valid_in_original(p3) && grid3d[p3.z][p3.y][p3.x]=='#')
					ret_val.back()[p3.y+1][p3.x+1] = (count==2 || count==3)?'#':'.';
				else
					ret_val.back()[p3.y+1][p3.x+1] = (count==3)?'#':'.';
			});
		}
		
		grid_width+=2;
		grid_height+=2;
		return ret_val;
	};
	
	for(std::size_t i=0;i<6;++i)
		grid3d = update_grid();
	
	std::size_t sum = 0;
	for(const auto& layer: grid3d)
		for(const auto& row: layer)
			sum+=std::count(std::begin(row),std::end(row),'#');
	
	std::cout<<sum<<'\n';
	
	return 0;
}
