## Part 1

Expression parsing and evaluation! Great thing I have written some very simple toy (and still simple, but not so toy) compilers before :). First thing is simply evaluating with no precedence at all, just considering parenthesis and left to right, which can easily be done in one pass.

First, we define how to parse a single operation:

```cpp
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
```

Skip all whitespace and return operation::add or operation::multiply depending on the character seen(We should only support addition and multiplication). Nothing fancy so far. Parsing an operand, however, already contains most of the logic:

```cpp
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
```

If the operand is a simple number, we read and return it. Otherwise, we first recursively evaluate the subexpression describing it and then return its result. Evaluation itself can now be described in terms of the two functions above:
	
```cpp
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
```	

We first parse the left operand, followed by the operation and its right operand. Our result gets computed as the result of applying the operator to it and the right operand. While there are still more operations to process, we repeat the procedure with result as the new left operand.  

Not the most pretty, not the most ugly, just simple enough and it gave the correct result on first try and me place 185th on the damn global leaderboard. So close, yet so far...

## Part 2

Now we do have a precedence, just the opposite of what is commonly used. In hindsight, it might have been worth it to just swap the symbols, let some automatic parenthesizing program run over it, swap them again and use bc or something like it. Or copy the expressions to code and use custom types with overloaded operators. I believe many solutions worked exactly like that, taking advantage of already properly implemented precedence rules in languages and calculators. I, as I tend to do in life, went the long and tedious route:

It wasn't really that long and tedious so. All that had to be adjusted was the evaluate function and the change was rather minimal and intuitive. Ignore subexpressions for a moment - as those are handled recursively by parse_operand anyway - and simply consider what is happening here:

	1+2*3+4+5*6+7*9+8+7

That is interpreted as:

	(1+2)*(3+4+5)*(6+7)*(9+8+7)

As you can see, we have a product of sums. What we can do, as such, is keep a running product, starting with 1 and a running sum, starting with the first operand. We then add to the sum until we encounter a multiply, at which point we multiply our running product by the sum and reset sum to zero.

With our example:

	product = 1, sum = 1;
	sum = 3
	product = 1*3, sum = 0
	sum = 3
	sum = 7
	sum = 12
	product = 3*12, sum = 0

and so on. At the very end, we return the running product multiplied by the last sum. Here it is in code:

```cpp
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
```
