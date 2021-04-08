#include <iostream>
#include <iterator>
#include <string>
#include <unordered_set>
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

int main(int argc, char* argv[])
{
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
	return 0;
}
