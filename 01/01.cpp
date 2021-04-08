#include <bitset>
#include <iostream>
#include <iterator>
#include <vector>

#include <cstddef>

int main(int argc, char* argv[])
{
	std::vector<int> vals(std::istream_iterator<int>{std::cin},{});
	std::bitset<2021> seen{};
		
	for(auto v: vals)
	{
		if(v>2020) continue;
		
		seen.set(v);
		if(seen.test(2020-v))
		{
			std::cout<<v*(2020-v)<<'\n';
			return 0;
		}
	}
	return 0;
}
