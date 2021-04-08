#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

#include <cstdint>

int main(int argc, char* argv[])
{
	const auto seat_id=[](const std::string& str)
	{
		std::uint16_t id = 0;
		for(auto c: str)
		{
			id<<=1;
			if(c=='B' || c=='R')
				id|=1;
		}
		return id;
	};
	
	std::uint16_t min=std::numeric_limits<std::uint16_t>::max(), max=std::numeric_limits<std::uint16_t>::min();
	int sum = 0;
	std::for_each(std::istream_iterator<std::string>{std::cin},{},[&](const auto& str)
	{
		const auto id = seat_id(str);
		min=std::min(min,id);
		max=std::max(max,id);
		sum+=id;
	});
	
	--min;
	std::cout<<((max*max+max)/2-(min*min+min)/2-sum)<<'\n';
	
	return 0;
}
