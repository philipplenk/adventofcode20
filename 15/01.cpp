#include <iostream>
#include <unordered_map>
#include <vector>

#include <cstddef>

int main(int argc, char* argv[])
{
	std::vector<int> numbers;
	char skip_c;
	int n;
	while(std::cin>>n)
	{
		numbers.push_back(n);
		std::cin>>skip_c;
	}
	
	struct diff
	{
		std::size_t prev = 0, last = 0;
	};
	std::unordered_map<int,diff> last_spoken;
	
	int last = 0;
	for(std::size_t i=0;i<2020;++i)
	{
		if(i<numbers.size())
		{
			last = numbers[i];
			last_spoken[numbers[i]] = {i,i};
		}
		else
		{
			const auto& data = last_spoken[last];
			const auto dist = data.last-data.prev;
			last = dist;
			
			if(auto it = last_spoken.find(dist); it==std::end(last_spoken))
			{
				auto& to_change_data = last_spoken[dist];
				
				to_change_data.prev = i;
				to_change_data.last = i;
			}
			else
			{
				auto& to_change_data = last_spoken[dist];
				
				to_change_data.prev = to_change_data.last;
				to_change_data.last = i;
			}
		}
	}
	
	std::cout<<last<<'\n';
	
	return 0;
}
