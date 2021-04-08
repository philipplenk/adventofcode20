#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

#include <cstddef>

int main(int argc, char* argv[])
{
	constexpr auto target = 23278925;
	
	std::vector<int> data(std::istream_iterator<int>{std::cin},{});
	std::size_t sum = data[0], min = 0;
	for(std::size_t i=1;i<data.size();++i)
	{
		sum+=data[i];
		while(sum>target && min<i)
		{
			sum-=data[min];
			++min;
		}
		if(sum==target)
		{
			auto [min_it,max_it] = std::minmax_element(&data[min],&data[i]+1);
			std::cout<<*min_it+*max_it<<'\n';
			break;
		}
	}
	return 0;
}
