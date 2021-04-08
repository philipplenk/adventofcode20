## Part 1

This one was quite interesting. A puzzle that was literally... a puzzle ;-). Given a bunch of "tiles", i.e. small rectangles of either "." or "#", the task was to reassemble a complete image by placing, rotating and flipping the tiles such that all borders match up. Part 1, however, was a significantly easier first step to this grand endeavor: Determining which tiles are corner tiles. A tiny bit of thought reveals that all tiles appearing on the inside match at least 3 other tiles on at least 3 of their borders. As such, we first need to determine the 4 borders:

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

"grid" here is a vector of string, the unfortunately inefficient representation I chose at the time. For this part, it was entirely unnecessary to save more than the borders and those could have been a simple bitset/integer instead. Nonetheless, due to an irresistible bout of laziness and in anticipation of what was to come, for better or worse, I did as I did.

Given a way to get at the borders for any given tile, we need a way to check if any borders of two tiles could match, regardless of orientation:

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

We then can, in horrendously quadratic brute force fashion, determine all possible neighbors for all tiles:

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

The looked for tiles are those with exactly 2 possible neighbors and we were supposed to return the product of their ids:

	long product = 1;
	for(const auto& [id,neighbors]: neighbors)
	{
		if(neighbors.size()==2)
			product*=id;
	}
	std::cout<<product<<'\n';

All of this could, of course, have failed miserably, as there is no real reason why a corner tile could not match any additional tiles on at least one side, but it seems like we were supposed to get lucky.

## Part 2

Alright, now that we have the corners, solving the complete puzzle and reassembling our image should be trivial, right? Well, it should be...

I did, however, run into some troubles along the way, wrapping my head around it was the cause of some minor and major headaches and I spent far more time than I'd be willing to admit until I got all the details just right. My code is a bit of mess, so before we get to it, allow me to explain in general terms what I eventually came up with:

Assume we start out with an empty grid

	....
	....
	....
	....

Begin by setting any arbitrary corner tile:

	0...
	....
	....
	....

As corner has exactly 2 neighbors and we do not care if our assembled image is in the "correct" orientation - whatever that even means - we can randomly assign one of them to the right and the other to the bottom:

	01..
	2...
	....
	....

What now? Consider the position marked "X":

	01..
	2X..
	....
	....

What do we know about the tile which will eventually end up there? It has 4 neighbors, two of which will be 1 and 2. It is also the only tile adjacent to both of them! Two placed neighbors are always enough to uniquely identify any tiles position. As such, we can place all tiles with that property:

	01..
	23..
	....
	....

Aaaand, we are stuck again. Or are we? 1 and 2 have exactly one unplaced neighbor left and exactly one space to put it. As such, we can place those, leaving us with:

	014.
	23..
	5...
	....

Which provides us with two more positions clearly defined by two neighbors. Filling them gives us two more tiles on the edges. Rinse and repeat until the grid is filled.

Which brings us to the god awful implementation. We first do exactly what we did in part 1, just instead of outputting some value based on the corner ids, we select one of them as the starting point for whatever we will do next:

	long start = 0;
	for(const auto& [id,neighbors]: neighbors)
	{
		if(neighbors.size()==2)
		{
			start = id;
			break;
		}
	}

For simplicities sake, I once again inefficiently represented a grid as map and used a hashmap to keep track of which tiles were already set:

	std::unordered_map<long,bool> used;
	std::map<point2d,long> grid;

Filling the grid is done as described above:

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

First, we set the three initial tiles and then, until we have set all of them, we fill in those with two neighbors, advance the ones on the borders and repeat.

To find where to place one with two neighbors, we find the position of those neighbors and take the max of their x and y coordinates:

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

The direction for the other case is predefined, so we only need to find the unset neighbor and add the direction vector:

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

Afterwards, we still have to correct flips and rotations, remove the border, compose a complete grid and search for a specified pattern - an ascii art sea monster - within that image. As that is not particularly interesting, quite tedious and even repetitive, I refer you to [the complete code](02.cpp) instead of elaborating on it any further. It is a mess and you have been warned ;-)