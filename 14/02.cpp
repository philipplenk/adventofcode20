#include <advent.hpp>

#include <ptl/bit.hpp>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include <cstddef>
#include <cstdint>

using namespace advent;

int main(int argc, char* argv[])
{
	std::uint64_t mask_and = 0, mask_or = 0, mask_float = 0;
	
	const auto update_mask = [&](const std::string& new_mask)
	{
		mask_and = mask_or = mask_float = 0;
		std::size_t shift = 0;
		for(auto it = new_mask.rbegin();it!=new_mask.rend();++it, ++shift)
		{
			mask_and |= (*it=='X'?0ull:1ull)<<shift;
			mask_or |= (*it=='1'?1ull:0ull)<<shift;
		}
		
		mask_float = (~mask_and)&((1ull<<36)-1);
	};
	
	const auto foreach_address = [&](std::uint64_t addr, auto fun)
	{
		addr = (addr&mask_and) | mask_or;
		
		std::uint64_t set_bits = ptl::popcount(mask_float);
		for(std::uint64_t i=0;i<(1ull<<set_bits);++i)
		{
			auto addr_cpy = addr;
			auto mask_cpy = mask_float;
			for(std::size_t j=0;j<set_bits;++j)
			{
				const auto bitpos = ptl::countr_zero(mask_cpy);
				mask_cpy&=~(1ull<<bitpos);
				addr_cpy|=((i>>j)&1ull)<<bitpos;
			}
			
			fun(addr_cpy);
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
			
			foreach_address(addr,[&](auto addr) { memory[addr] = value; });		
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
