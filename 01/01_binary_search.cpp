#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

#include <cstddef>

int main(int argc, char* argv[])
{
	std::vector<int> vals(std::istream_iterator<int>{std::cin},{});
	std::sort(std::begin(vals),std::end(vals));
		
	for(auto v:vals)
	{
		if(std::binary_search(std::begin(vals),std::end(vals),2020-v))
		{
			std::cout<<v*(2020-v)<<'\n';
			return 0;
		}
	}
	return 0;
}
