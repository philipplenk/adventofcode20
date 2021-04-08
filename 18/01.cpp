#include <iostream>
#include <sstream>
#include <string>

enum class operation
{
	add,
	multiply
};

long long evaluate(std::istream& expression);

operation parse_operation(std::istream& expression)
{
	expression>>std::ws;
	
	operation op;
	if(expression.peek()=='*')
		op=operation::multiply;
	else
		op=operation::add;

	expression.get();
		
	return op;
}

long long parse_operand(std::istream& expression)
{
	expression>>std::ws;
	
	long long value = 0;
	if(expression.peek()=='(')
	{
		expression.get();
		value = evaluate(expression);
	}
	else
		expression>>value;

	return value;
}

long long evaluate(std::istream& expression)
{
	long long result = parse_operand(expression);
	
	do
	{
		operation op = parse_operation(expression);
		
		long long rhs = parse_operand(expression);
			
		result = op==operation::add?(result+rhs):(result*rhs);

		expression>>std::ws;
		if(expression.peek()==')')
		{
			expression.get();
			break;
		}
	}
	while(expression);
	
	return result;
}

int main(int argc, char* argv[])
{
	long long result = 0;
	std::string line;
	while(std::getline(std::cin,line))
	{
		std::stringstream strm;
		strm<<line;
		result+=evaluate(strm);
	}
	
	std::cout<<result<<'\n';
	return 0;
}
