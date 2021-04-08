#include <advent.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>

using namespace advent;

enum class type
{
	turn_left,
	turn_right,
	move_forward,
	move
};

struct instruction
{
	type t;
	int step;
	vec2d direction;
};

std::istream& operator>>(std::istream& in, instruction& ins)
{
	char c;
	in>>c;
	in>>ins.step;
	switch(c)
	{
		case 'N': ins.t=type::move; ins.direction={0,-1}; break;
		case 'E': ins.t=type::move; ins.direction={1,0}; break;
		case 'W': ins.t=type::move; ins.direction={-1,0}; break;
		case 'S': ins.t=type::move; ins.direction={0,1}; break;
		case 'F': ins.t=type::move_forward; break;
		case 'R': ins.t=type::turn_right; ins.step/=90; break;
		case 'L': ins.t=type::turn_left; ins.step/=90; break;
		default: break;
	}
	return in;
}

vec2d turn_left(vec2d old)
{
	std::swap(old.x,old.y);
	old.y=-old.y;
	return old;
}

vec2d turn_right(vec2d old)
{	
	std::swap(old.x,old.y);
	old.x=-old.x;
	return old;
}

int main(int argc, char* argv[])
{
	point2d pos{0,0};
	vec2d current_direction{1,0};
	
	std::for_each(std::istream_iterator<instruction>{std::cin},{},[&](auto ins)
	{
		switch(ins.t)
		{
			case type::move: pos+=ins.direction*ins.step; break;
			case type::turn_left: for(int i=0;i<ins.step;++i) current_direction = turn_left(current_direction); break;
			case type::turn_right: for(int i=0;i<ins.step;++i) current_direction = turn_right(current_direction); break;
			case type::move_forward: pos+=current_direction*ins.step; break;
		}
	});
	
	std::cout<<(std::abs(pos.x)+std::abs(pos.y))<<'\n';
	
	return 0;
}
