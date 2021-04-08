#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

#include <cstddef>

int main(int argc, char* argv[])
{
	std::vector<int> vals(std::istream_iterator<int>{std::cin},{});
	std::sort(std::begin(vals),std::end(vals));
	
	std::size_t lower = 0, higher = vals.size()-1;
	
	while(lower<higher)
	{
		const auto target = 2020-vals[lower]-vals[higher];
		if(std::binary_search(std::begin(vals),std::end(vals),target))
		{
			std::cout<<vals[lower]*vals[higher]*target<<'\n';
			return 0;
		}
		else if(vals[lower+1]-vals[lower]<vals[higher]-vals[higher-1])
			++lower;
		else
			--higher;
	}
	return 0;
}
