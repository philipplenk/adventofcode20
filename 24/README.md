
## Part 1

Flipping tiles in a hex grid. Fun, if only I remembered to get my stupid hex coordinates right. Took me a while to get back to it and in my learning I discovered [a beautiful resource going into great detail](https://www.redblobgames.com/grids/hexagons/), which I highly recommend reading. 

Especially the little graphic under axial coordinates listing neighbors really saved my skin this time. Everything but this was, of course, rather trivial.

Starting out with an infinitely extending hexagonal grid, all of which white, we were given a number of pathes from a starting point and were supposed to flip the tiles on the destinations to black(or back to white if they were reached a second time).

As such, we need to represent binary colors and offer a way to flip them:

```cpp
enum class color
{
	white = 0,
	black
};

color flip(color c)
{
	return c==color::white?color::black:color::white;
}
```

As well as the 6 possible hexadecimal directions and a way to compute coordinates based on them:

```cpp
enum class hex_direction
{
	e,
	w,
	se,
	sw,
	ne,
	nw
};

vec2d to_vec2d(hex_direction dir)
{
	switch(dir)
	{
		case hex_direction::e: return {1,0};
		case hex_direction::w: return {-1,0};
		case hex_direction::se: return {1,-1};
		case hex_direction::sw: return {0,-1};
		case hex_direction::ne: return {0,1};
		case hex_direction::nw: return {-1,1};
	}
	
	return{};
}
```

As you can see, I chose axial coordinates. To understand why exactly that works, I would like to once again refer you to [this wonderful explanation](https://www.redblobgames.com/grids/hexagons/), which is far better than anything I could offer on that topic.

With that groundwork laid(and an overloaded operator>> for parsing directions), the input based flipping is rather straightforward:

```cpp
std::map<point2d,color> colors;

std::string line;
while(std::getline(std::cin,line))
{
	std::stringstream strm;
	strm<<line;
	
	point2d pos{0,0};
	hex_direction d;
	while(strm>>d)
		pos+=to_vec2d(d);
	
	colors[pos]=flip(colors[pos]);
}
```

## Part 2

And here we come to the reason for all those shenanigans. We had a seating area cellular automaton, we had a 4 dimensional cellular automaton, why not have one on a hex grid? 

The code is almost identical to the one used for arbitrarily dimensional hypercubes before, so in case anything is unclear, I suggest looking at my solution for Day 17. If anything remains unclear, feel free to leave a comment on my blog or simply send me an email ;-)

```cpp
const auto update=[&]()
{
	std::map<point2d,std::size_t> counts;
	for(auto [p,c]: colors)
	{
		if(c==color::white)
			continue;
		
		for(auto n: {hex_direction::e,hex_direction::w,hex_direction::ne, hex_direction::nw, hex_direction::sw, hex_direction::se})
			++counts[p+to_vec2d(n)];
	}
	
	for(auto [p,c]: colors)
		if(c==color::black)
			counts[p]=counts[p];
	
	for(auto [p,cnt]: counts)
	{
		auto& c = colors[p];
		if(c==color::black && (cnt==0 || cnt>2))
			c=color::white;
		else if(c==color::white && cnt==2)
			c=color::black;
	}
};
```