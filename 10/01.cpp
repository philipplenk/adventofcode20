#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

#include <cstddef>

int main(int argc, char* argv[])
{
	std::vector<int> jolts(std::istream_iterator<int>{std::cin},std::istream_iterator<int>{});
	std::sort(std::begin(jolts),std::end(jolts));
	
	std::size_t diff1 = 0, diff3 = 0;
	int last = 0;
	for(std::size_t i = 0;i<jolts.size(); ++i)
	{
		auto diff = jolts[i]-last;
		if(diff==1)
			++diff1;
		if(diff==3)
			++diff3;
		last = jolts[i];
	}
	std::cout<<(diff1*(diff3+1))<<'\n';
	return 0;
}
