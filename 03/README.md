## Part 1

Yeahy, may the ascii art begin! This was the first of a classical kind of 2d grid based puzzles this year! It was also the first problem this year where what little preparation I had done came in handy, having predefined point and vector types.

Given a map consisting of open spaces (represented as '.') and trees(represented as '#'), all we had to do was check how often one would hit a tree when going down a certain slope. Beginning in {0,0}, just adding the vector {3,1} until you hit the bottom, count the elements with value #. To ensure you always reach the bottom, the grid extends infinitely to the right, simply looping around in classic pacman fashion - which implemented via a simple % on the x value:

```cpp
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
```

## Part 2

Part 2 was simply doing the same thing with multiple slopes and multiplying the results. Nothing fancy, just moving the counting to a lambda and a small range-for does the trick:

```cpp
for(auto d: {vec2d{1,1},vec2d{3,1},vec2d{5,1},vec2d{7,1},vec2d{1,2}})
	result*=count_trees(d);
```