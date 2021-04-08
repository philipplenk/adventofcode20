#include <algorithm>
#include <iostream>
#include <iterator>

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
		return pw.str[pw.min-1]!=pw.str[pw.max-1] && (pw.str[pw.min-1]==pw.letter || pw.str[pw.max-1]==pw.letter);
	})<<'\n';
	return 0;
}
