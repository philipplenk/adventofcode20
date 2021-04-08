
## Part 1

This time, we were supposed to trace modified writes to a special memory. Whenever a value is written, it is first modified based on a simple bitmap by either setting the bits to 0 or 1 or alternatively, leaving them as they were in case of an X. For example, given the following input:

mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X
mem[8] = 11

memory at address 8 should obtain the value 1011 (i.e. binary 11), but adding a 1 bit at the 7th position and changing the 1 at the second one to a zero: 1001001 (which is 73 in decimal).

My implementation simply constructed two masks, one to &(representing the X's) and one to |(representing everything else), for those operations:

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

Which were then applied to modify assignments:

	memory[addr] = (value&mask_and) | mask_or;	

As the address range was 36 bits and as such incredibly huge, whilst only a small number of cells could actually be used, I chose to represent it as a hashmap:

	std::unordered_map<std::uint64_t, std::uint64_t> memory;

## Part 2

Same input, same mask, but very different interpretation. Now an X does not mean the given value, but any possible one, meaning we will have to iterate over all combinations. Furthermore, 0 means to leave the input untouched(the role previously occupied by X), whilst we modify the address written to, not the value. As a first step, we now create 3 masks(one to set to 1, one to leave untouched and one to try all combinations on):

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

Listing all options might seem complicated at first, but luckily for us, our languages and computers already have a beautiful system to iterate over all combinations of 0 and 1 for a given number of those strung together: counting! So we do just that and simply shift the relevant bits into their desired places afterwards:

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

To count bits and determine bit positions, I would have loved to use standard functions, but the [bit header](https://en.cppreference.com/w/cpp/header/bit) is only available from C++20 onwards and I was using C++17 only here. As such, I substituted my own inferior version created for [my chess engine, paulchen332](https://github.com/philipplenk/paulchen332).