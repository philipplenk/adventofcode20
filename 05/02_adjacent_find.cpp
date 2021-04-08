#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

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
	
	std::vector<std::uint16_t> ids;
	std::transform(std::istream_iterator<std::string>{std::cin},{},std::back_inserter(ids),seat_id);
	
	std::sort(std::begin(ids),std::end(ids));
	std::cout<<(*std::adjacent_find(std::begin(ids),std::end(ids),[](auto lhs, auto rhs) { return rhs-lhs>1; })+1)<<'\n';
	
	return 0;
}
