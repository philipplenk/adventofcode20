## Part 1

The interesting emulation problems have finally started! I really loved all the intcode problems in 2019, so this was getting my hopes up!

We started out slowly so and should just fix a simple program which executes one of only three types of instructions:

- acc, adding to the accumulator
- jmp, a relative jump
- nop, my favorite thing, doing nothing

As given, the program contained an infinite loop and we were tasked with determining its state once we enter the loop, i.e. once we execute an instruction for the second time. Without further ado, lets jump right into the code.

First, we define structs for our state and instructions:

```cpp
struct state
{
	int accumulator = 0;
	int pc = 0;
};

enum class opcode
{
	acc,
	jmp,
	nop
};

struct instruction
{
	opcode op;
	int arg;
};
```

Parsing them is thankfully easy:

```cpp
std::istream& operator>>(std::istream& in, instruction& ins)
{
	std::string name;
	in>>name;
	if(name=="acc")
	{
		ins.op=opcode::acc;
		return in>>ins.arg;
	}
	else if(name=="jmp")
	{
		ins.op=opcode::jmp;
		return in>>ins.arg;
	}
	else if(name=="nop")
	{
		ins.op=opcode::nop;
		return in>>ins.arg;
	}
	return in;
}
```

As is executing them, either adding to the accumulator or adding to the program counter or alternatively doing nothing at all:

```cpp
void execute(const instruction& ins, state& s)
{
	switch(ins.op)
	{
		case opcode::acc: s.accumulator+=ins.arg; break;
		case opcode::jmp: s.pc+=ins.arg-1; break;
		case opcode::nop: break;
	}
}
```

To solve our problem, we first read all instructions into a vector, setup a pristine state with accumulator = pc = 0 and begin executing. 
After each instruction we increment the program counter to determine the next one, whilst keeping track - in a simple hashset - of all positions we have encountered so far. Upon reaching one that was previously seen, we declare the cycle detected and break our loop:

```cpp
std::vector<instruction> instructions(std::istream_iterator<instruction>{std::cin},{});

state s{};
std::unordered_set<int> seen;

while(seen.find(s.pc)==seen.end())
{
	seen.insert(s.pc);
	execute(instructions[s.pc],s);
	++s.pc;
}

std::cout<<s.accumulator<<'\n';
```

Lovely, lets move on to...

## Part 2

Now that we are able to emulate our loop, we are supposed to break it. There is one single nop or jmp instruction that if flipped to the opposite will cause our program to terminate by causing the program counter to reach the end of memory.

The most trivial and perfectly sufficient idea is to simply brute force it. Iterate over every instruction and if it is a jmp or nop, toggle it and try running our program from part 1. If a loop was detected, we try the next one. If not, we have found the instruction to change:

```cpp
for(auto& ins: instructions)
{
	if(ins.op!=opcode::acc)
	{
		ins.op=ins.op==opcode::jmp?opcode::nop:opcode::jmp;
		auto [loop_detected, result] = detect_loop(instructions);
		if(!loop_detected)
		{
			std::cout<<result.accumulator<<'\n';
			break;
		}
		ins.op=ins.op==opcode::jmp?opcode::nop:opcode::jmp;
	}
}
```

To keep it readable, I outsourced loop detection and gave it a nice, expressive return type:

```cpp
struct execution_result
{
	bool loop_detected = false;
	state final_state;
};

execution_result detect_loop(const std::vector<instruction>& instructions)
{
	state s{};
	std::vector<bool> seen(instructions.size(),false);
	
	while(s.pc>=0 && s.pc<static_cast<int>(instructions.size()) && !seen[s.pc])
	{
		seen[s.pc] = true;
		execute(instructions[s.pc],s);
		++s.pc;
	}
	
	return {s.pc>=0 && s.pc<static_cast<int>(instructions.size()), s};
}
```

Whilst this is perfectly sufficient in our case and terminates within a small fraction of a second even on my old machines, it is not exactly ideal. Worst case, its asymptotic complexity seems quadratic. Consider this example:

	nop 1
	nop 1
	nop 1
	....
	jmp -program_size

It would run over each nop, run the complete program to the very end, detect the loop and try the next one. That is, each attempt before the last executes n-1 nop and 2 jmp instructions and we need to do that n-1 times. Quite terrifying. So lets do better:

```cpp
auto reaches_end = compute_end_reachability(instructions);

state s{};
bool changed_one = false;

while(s.pc>=0 && s.pc<static_cast<int>(instructions.size()))
{
	if(!changed_one)
	{
		const auto target_pc = s.pc+instructions[s.pc].arg;
		if(instructions[s.pc].op==opcode::jmp && !reaches_end[s.pc] && reaches_end[s.pc+1])
		{
			instructions[s.pc].op=opcode::nop;
			changed_one = true;
		}
		else if(instructions[s.pc].op==opcode::nop && !reaches_end[s.pc] && reaches_end[target_pc])
		{
			instructions[s.pc].op=opcode::jmp;
			changed_one = true;
		}
	}
	execute(instructions[s.pc],s);
	++s.pc;
}

std::cout<<s.accumulator<<'\n';
```

Looks a bit more complex than our previous attempt, but the running time turns out to be linear. Allow me to first explain my reasoning:

Assuming we knew for each instruction if beginning execution from it will reach the end, we could execute our code once and simply change the first instruction we encounter that could not reach the end but could if toggled. Doing such a thing would run in linear time, as no loops are entered and each instruction is executed at most once. That is exactly what you see happening in the code above.

This, however, delegates the hard problem to figuring out which instructions can and cannot reach the end:

```cpp
std::vector<std::vector<int>> compute_jump_sources(const std::vector<instruction>& instructions)
{
	std::vector<std::vector<int>> jump_sources(instructions.size());
	for(int pc = 0; pc<static_cast<int>(instructions.size());++pc)
	{
		const auto target_pc = pc+instructions[pc].arg;
		if(instructions[pc].op==opcode::jmp && target_pc>=0 && target_pc<static_cast<int>(instructions.size()))
			jump_sources[target_pc].push_back(pc);
	}
	
	return jump_sources;
}
	
std::vector<bool> compute_end_reachability(const std::vector<instruction>& instructions)
{
	std::vector<bool> reaches_end(instructions.size(),false);
	std::vector<bool> seen(instructions.size(),false);
	
	std::vector<std::vector<int>> jump_sources = compute_jump_sources(instructions);
	
	const auto mark_upward = [&](int start_pc, bool value, auto recurse) -> void
	{		
		for(int pc = start_pc; pc>=0 && !seen[pc]; --pc)
		{
			const auto target_pc = pc+instructions[pc].arg;
			if(instructions[pc].op==opcode::jmp)
			{
				if(target_pc<0 || target_pc>=static_cast<int>(instructions.size()))
					value = true;
				else if(seen[target_pc])
					value = reaches_end[target_pc];
				else
					break;
			}
			seen[pc] = true;
			reaches_end[pc] = value;
			
			for(auto source: jump_sources[pc])
				recurse(source,value,recurse);
		}
	};
	
	mark_upward(instructions.size()-1,true,mark_upward);
	
	return reaches_end;
}
```

I am iterating over all instructions once to determine from where each instruction can be reached(compute\_jump\_sources). Then I iterate again and mark which instructions can reach the end. Turns out that is not quite so hard either. We have to realize the following simple facts:

- A non-jmp instruction can reach the end if it is at the end or if the one following it could reach the end
- A jmp instruction can reach the end if its target is the end or can reach the end 

As such, we start with the only one we know about - the end - and work our way upwards. If its not a jump, we simply mark it with the previous result. If its a jump and we know about its destination, we continue with the destinations value. If its a jump and we don't know about its destination, we can't proceed and break. Furthermore, if the current instruction can be reached by a jump, we recurse and work our way upward from its source, too.

Runtime analysis of this might seem like a mess, but our seen-vector rushes in to save the day: 
There are at most n jump sources in total (as there are at most n jmp instructions), as such recurse can be called at most n times in total), so we have at most n loops.
Each iteration of the loop either breaks or sets a value in the seen vector to true which was not set previously. As each loop can of course only break once, the total number the first can happen is n.
As seen has exactly n elements and we only enter an iteration and set a value if it was not set, the total number the second can happen - over all loops - is also n.

Just lovely, isn't it?
