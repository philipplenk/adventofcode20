#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

struct policy_word
{
	char letter;
	int min, max;
	std::string str;
};


std::istream& operator>>(std::istream& in, policy_word& p)
{
	char dummy;
	in>>p.min;
	in>>dummy;
	in>>p.max;
	in>>p.letter;
	in>>dummy;
	in>>p.str;
	
	return in;
}

int main(int argc, char* argv[])
{
	std::cout<<std::count_if(std::istream_iterator<policy_word>{std::cin},{},[](auto pw)
	{
		const auto cnt=std::count(std::begin(pw.str),std::end(pw.str),pw.letter);
		return cnt>=pw.min && cnt<=pw.max;
	})<<'\n';
	return 0;
}
