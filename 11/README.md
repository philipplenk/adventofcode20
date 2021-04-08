## Part 1

[Cellular automata](https://en.wikipedia.org/wiki/Cellular_automaton)! Quite fitting, to honor [John Conway's](https://en.wikipedia.org/wiki/John_Horton_Conway) legacy - in the sad year of his passing - with another one of those. The cover story here is about seats and people admirably practicing social distancing. If they have too many neighbors, they move away. Given an initial state and a simple set of rules, we had to simulate the seating constellations until they reach a stable, unchanging state.
Rather straightforward, so edges, corners and the fact that all cells update simultaneously always complicates issues slightly. Here is one update step:

```cpp
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
```

We create a copy of all cells, and iterate over all coordinates. For each of those we iterate over all valid neighbors and count how many of them are occupied, setting the new value in our copy according to the given rules. We can't set it in our current grid, as all cells should act as if they updated simultaneously and modifying the grid step by step would change results for later processed cells which contain already updated ones as neighbors.

## Part 2

The difficulty was increased ever so slightly by not only considering direct neighbors but everything within ones line of sight. Nothing a simple loop can't solve, thankfully:

```cpp
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
```
