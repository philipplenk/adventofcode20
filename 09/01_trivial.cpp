#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main(int argc, char* argv[])
{
	constexpr auto n = 25;
	std::vector<int> data(std::istream_iterator<int>{std::cin},{});
	
	for(std::size_t i=n;i<data.size();++i)
	{
		bool found = false;
		for(std::size_t j=i-n;j<i;++j)
		{
			for(std::size_t k=j+1;k<i;++k)
			{
				if(data[j]+data[k]==data[i])
				{
					found = true;
					break;
				}
			}
			if(found) break;
		}
		
		if(!found)
		{
			std::cout<<data[i]<<'\n';
			break;
		}
	}
	
	return 0;
}
