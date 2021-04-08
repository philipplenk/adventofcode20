## Part 1

Given a number of instructions for moving a ship around on a simple 2 dimensional coordinate system (but with cardinal directions), we had to correctly determine which position we end up in. The minor difficulty arises from some directions not being absolute, but relative to the ships current orientation, which we also have to keep track of. Let's not get ahead of ourselves and first declare the possible instructions and define how they are parsed:

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

I already did some minimal preprocessing here. For one, I chose to save the directions as 2d vectors for easier addition.

Furthermore, I noticed all turns happened in multiples of 90 degree, so instead of saving the angles, I remembered how many 90 degree turns they correspond to. 90 degree turns are easy:

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


Whilst it could maybe have been solved more elegantly with complex multiplication, this works perfectly fine for now so and I really like the clarity of this code which is readable without much of a mathematical background (although it's always nice to have that nonetheless).

Given this scaffolding, all that is left is iterating over the instructions and modifying either position or direction accordingly:

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

## Part 2

Part 2 on this day was nearly identical, only the "move"-instructions were changed to not move the ship directly but modify its movement vector instead, which was applied on the "move-forward" command. The turn commands were also changed to turn the movement vector. Again, it could have been simplified with complex numbers, but worked out fine with my trivialized approach, the only change being one case of instruction handling:


	switch(ins.t)
	{
		case type::move: waypoint+=ins.direction*ins.step; break;
		case type::turn_left: for(int i=0;i<ins.step;++i) waypoint = turn_left(waypoint); break;
		case type::turn_right: for(int i=0;i<ins.step;++i) waypoint = turn_right(waypoint); break;
		case type::move_forward: pos+=waypoint*ins.step; break;
	}

