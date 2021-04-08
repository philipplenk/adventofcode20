#include <iostream>
#include <limits>
#include <vector>

int main(int argc, char* argv[])
{
	long first_timestamp;
	std::vector<long> ids;
	std::cin>>first_timestamp;
	
	long next;
	char ignore;
	for(;std::cin;)
	{
		if(std::cin.peek()=='x')
			std::cin>>ignore>>ignore;
		else
		{
			std::cin>>next>>ignore;
			ids.push_back(next);
		}
	}
	
	long min_time = std::numeric_limits<long>::max();
	long best_id = 0;
	for(auto id:ids)
	{
		auto next_factor = first_timestamp/id + (first_timestamp%id!=0);
		auto ts = next_factor * id;
		
		if(ts<min_time)
		{
			best_id = id;
			min_time = ts;
		}
	}
	
	std::cout<<(min_time-first_timestamp)*best_id<<'\n';
	return 0;
}
