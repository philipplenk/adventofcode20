#include <bitset>
#include <iostream>
#include <iterator>
#include <vector>

#include <cstddef>

int main(int argc, char* argv[])
{
	std::vector<int> vals(std::istream_iterator<int>{std::cin},{});
	std::bitset<2021> seen{};
		
	for(std::size_t i=0;i<vals.size();++i)
	{
		if(vals[i]>2020) continue;
		seen.set(vals[i]);
		
		for(std::size_t j=i+1;j<vals.size();++j)
		{
			if(vals[j]>2020) continue;
			seen.set(vals[j]);
			
			const auto target = 2020-vals[i]-vals[j];
			
			if(target<2021 && target>=0 && seen.test(target))
			{
				std::cout<<vals[i]*vals[j]*target<<'\n';
				return 0;
			}
		}
	}
	return 0;
}
