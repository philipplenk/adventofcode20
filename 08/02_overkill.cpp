#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include <cstddef>

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

void execute(const instruction& ins, state& s)
{
	switch(ins.op)
	{
		case opcode::acc: s.accumulator+=ins.arg; break;
		case opcode::jmp: s.pc+=ins.arg-1; break;
		case opcode::nop: break;
	}
}

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

int main(int argc, char* argv[])
{
	std::vector<instruction> instructions(std::istream_iterator<instruction>{std::cin},std::istream_iterator<instruction>{});
	
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
	
	return 0;
}
