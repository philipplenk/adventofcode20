#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

#include <cstddef>

int main(int argc, char* argv[])
{
	std::vector<int> vals(std::istream_iterator<int>{std::cin},{});
	std::sort(std::begin(vals),std::end(vals));
		
	for(std::size_t i=0;i<vals.size();++i)
	{
		for(std::size_t j=i+1;j<vals.size();++j)
		{
			const auto target = 2020-vals[i]-vals[j];
			if(std::binary_search(std::begin(vals),std::end(vals),target))
			{
				std::cout<<vals[i]*vals[j]*target<<'\n';
				return 0;
			}
		}
	}
	return 0;
}
