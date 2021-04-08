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
	long long lhs = parse_operand(expression);
	
	long long product = 1;
	do
	{
		operation op = parse_operation(expression);
		
		long long rhs = parse_operand(expression);
			
		if(op==operation::multiply)
		{
			product*=lhs;
			lhs = rhs;
		}
		else
			lhs+=rhs;
			
		expression>>std::ws;
		if(expression.peek()==')')
		{
			expression.get();
			break;
		}
	}
	while(expression);
	
	return product*lhs;
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
