#include <iostream>
#include <limits>
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
	
	std::uint16_t max_id = std::numeric_limits<std::uint16_t>::min();
	for(std::string str;std::cin>>str;)
	{
		max_id = std::max(max_id,seat_id(str));
	}
	std::cout<<max_id<<'\n';
	return 0;
}
