#include <algorithm>
#include <array>
#include <iostream>
#include <string>

#include <cstddef>

int main(int argc, char* argv[])
{
	std::size_t total = 0;
	std::string line;
	std::array<std::size_t,256> counts{};
	const auto begin = &counts['a'], end = &counts['z'+1];
	
	const auto finish_group = [&]()
	{		
		total+=std::count_if(begin,end,[&](auto v){ return v!=0; });
		std::fill(begin,end,0);
	};
	
	while(std::getline(std::cin,line))
	{
		if(line.empty())
			finish_group();
		else
		{
			for(auto c: line)
				++counts[c];
		}
	}
	finish_group();
	
	std::cout<<total<<'\n';
	return 0;
}
