#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

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

int main(int argc, char* argv[])
{
	std::vector<instruction> instructions(std::istream_iterator<instruction>{std::cin},std::istream_iterator<instruction>{});
	
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
	
	return 0;
}
