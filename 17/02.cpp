#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <set>
#include <string>

#include <cstddef>

template <std::size_t dim>
struct vec
{
	std::array<int,dim> coordinate;
};

template <std::size_t dim>
struct point
{
	std::array<int,dim> coordinate;
};

template <std::size_t dim>
constexpr point<dim>& operator+=(point<dim>& lhs, const vec<dim>& rhs) noexcept
{
	for(std::size_t i=0;i<dim;++i)
		lhs.coordinate[i]+=rhs.coordinate[i];
	
	return lhs;
}

template <std::size_t dim>
constexpr point<dim> operator+(point<dim> lhs, const vec<dim>& rhs) noexcept
{
	return lhs+=rhs;
}

template <std::size_t dim>
constexpr bool operator<(const point<dim>& lhs, const point<dim>& rhs) noexcept
{
	return lhs.coordinate<rhs.coordinate;
}

constexpr auto pow(std::size_t base, std::size_t exponent) noexcept
{
	std::size_t ret_val = 1;
	while(exponent--)
		ret_val*=base;
	return ret_val;
}

template <std::size_t dim>
constexpr auto neighbor_vectors() noexcept
{
	std::array<vec<dim>,pow(3,dim)-1> ret_val;
	
	std::size_t count = 0;
	
	const auto recursive_fill = [&](vec<dim> current, std::size_t coord, auto recurse)
	{
		if(coord==dim)
		{
			if(!std::all_of(std::begin(current.coordinate),std::end(current.coordinate),[](auto v){ return v==0; }))
				ret_val[count++]=current;
			return;
		}
		
		for(int v=-1;v<2;++v)
		{
			current.coordinate[coord]=v;
			recurse(current, coord+1, recurse);
		}
	};
	
	recursive_fill({},0,recursive_fill);
	
	return ret_val;
}

int main(int argc, char* argv[])
{
	constexpr auto dim = 4;
	std::set<point<dim>> cells;
	
	std::string line;
	int y = 0;
	while(std::cin>>line)
	{
		for(int x=0;x<static_cast<int>(line.size());++x)
			if(line[x]=='#') cells.insert({x,y});
		++y;
	}
	
	const auto update_cells = [&]()
	{
		std::map<point<dim>,std::size_t> counts;
		for(auto p: cells)
			for(auto n: neighbor_vectors<dim>())
				++counts[p+n];
		
		std::set<point<dim>> new_cells;
		
		for(auto [p,count]: counts)
		{
			if(count==3 || (count==2 && cells.find(p)!=std::end(cells)))
				new_cells.insert(p);
		}

		return new_cells;
	};
	
	
	for(std::size_t i=0;i<6;++i)
		cells = update_cells();
	
	std::cout<<cells.size()<<'\n';
	
	return 0;
}
