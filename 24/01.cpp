#include <advent.hpp>

#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

using namespace advent;

enum class hex_direction
{
	e,
	w,
	se,
	sw,
	ne,
	nw
};

std::istream& operator>>(std::istream& in, hex_direction& d)
{
	char c;
	if(!(in>>c))
		return in;
	
	if(c=='e')
	{
		d=hex_direction::e;
		return in;
	}
	if(c=='w')
	{
		d=hex_direction::w;
		return in;
	}
	
	
	char sc;
	if(!(in>>sc))
		return in;
		
	if(c=='s' && sc=='e')
		d=hex_direction::se;
	if(c=='s' && sc=='w')
		d=hex_direction::sw;
	if(c=='n' && sc=='e')
		d=hex_direction::ne;
	if(c=='n' && sc=='w')
		d=hex_direction::nw;
		
	return in;
}

std::ostream& operator<<(std::ostream& out, hex_direction d)
{
	switch(d)
	{
		case hex_direction::e: return out<<"e";
		case hex_direction::w: return out<<"w";
		case hex_direction::se: return out<<"se";
		case hex_direction::sw: return out<<"sw";
		case hex_direction::ne: return out<<"ne";
		case hex_direction::nw: return out<<"nw";
	}
	return out;
}

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

enum class color
{
	white = 0,
	black
};

color flip(color c)
{
	return c==color::white?color::black:color::white;
}

int main(int argc, char* argv[])
{
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
	
	std::cout<<std::count_if(std::begin(colors),std::end(colors),[](auto p){ return p.second==color::black; })<<'\n';
	
	return 0;
}
