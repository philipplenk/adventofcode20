#include <advent.hpp>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include <cstdint>

using namespace advent;

int main(int argc, char* argv[])
{
	std::uint64_t mask_and = 0, mask_or = 0;
	
	const auto update_mask = [&](const std::string& new_mask)
	{
		mask_and = mask_or = 0;
		std::size_t shift = 0;
		for(auto it = new_mask.rbegin();it!=new_mask.rend();++it, ++shift)
		{
			mask_and |= (*it=='X'?1ll:0ll)<<shift;
			mask_or |= (*it=='1'?1ll:0ll)<<shift;
		}
	};
	
	
	std::unordered_map<std::uint64_t, std::uint64_t> memory;
	
	std::string tmp;	
	while(std::getline(std::cin,tmp))
	{
		if(tmp.substr(0,3)=="mem")
		{
			std::uint64_t addr = 0, value = 0;
			std::stringstream strm;
			strm<<tmp;
			
			parsers::parse(strm,"mem[",addr,"] = ",value);
			
			memory[addr] = (value&mask_and) | mask_or;						
		}
		else
		{
			std::stringstream strm;
			strm<<tmp;
			strm>>tmp>>tmp>>tmp;
			
			update_mask(tmp);
		}
	}
	
	std::cout<<std::accumulate(std::begin(memory),std::end(memory),std::uint64_t{0},[](auto total, auto cell){ return total+cell.second; })<<'\n';

	return 0;
}
