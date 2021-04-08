## Part 1

We already had one [Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) type puzzle this year, so seeing this one did not excite me too much at first. But in this tragic year it seemed needed and - reiterating the idea of [spiraling](https://en.wikipedia.org/wiki/Spiral_approach) - this one did take the previous one up to 11. Or rather 3 and eventually 4.

We start of with an infinite 3 dimensional "Conway Cube" with only one slight slice initialized and are supposed to extrapolate its growth.

For this first task, I chose the probably worst possible representation of an expanding, very sparsely packed three dimensional grid possible: a vector of vector of string.

```cpp
using layer = std::vector<std::string>;
std::vector<layer> grid3d;
```

Doing it like this made the update step incredibly unwieldy and tedious, as I had to take care to add two layers and resize every layer by 2, whilst keeping track of shifted indices and different bounds for the updated copy: 

```cpp
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
```

Certainly not the best code I have ever written, if you care for your eyes you should avert them.

## Part 2

You thought 3 dimensions was fun? Let's add another one and consider 4D hypercubes. My first surprisingly working solution simply extended the approach above logically. Instead of a vector of vector of string, I kept a vector of vector of vector of string and proceeded similarly to above, just one more nesting deeper. Quite the abomination, too horrendous to ever share with any living thing. I did, however, eventually see the error of my ways and recognized the problem description clearly screams for generalization of n dimensions, so generalize is what I did.

First of all, I needed arbitrarily dimensional vector and point structs

```cpp
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
```

with appropriate operator overloads like this:

```cpp
template <std::size_t dim>
constexpr point<dim>& operator+=(point<dim>& lhs, const vec<dim>& rhs) noexcept
{
	for(std::size_t i=0;i<dim;++i)
		lhs.coordinate[i]+=rhs.coordinate[i];
	
	return lhs;
}
```

Computing vectors to all neighbors can be done with some simple recursion:

```cpp
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
```

Given all that, I needed a better, less wasteful representation of the grid/cube/hypercube/whatever, i.e. of infinite space in arbitrary dimensions. The one I chose was a simple set of active/occupied cells:

```cpp
constexpr auto dim = 4;
std::set<point<dim>> cells;
```

An unordered_set, i.e. a hashset, might have been more efficient than this tree based one, but I really did not know how to define a good hash function for an arbitrarily large collection of integers, whilst a operator< is very natural.

The update function becomes much better than in part 1, even bearable to look at:

```cpp
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
```

I first iterate over all active/set cells and add one to the neighbor count of each of their neighbors. I then iterate over those counts to create the new active cells according to the rules given. Almost beautiful ;-)