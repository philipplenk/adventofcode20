#include <iostream>
#include <iterator>
#include <vector>

#include <cstddef>

int main(int argc, char* argv[])
{
	std::vector<int> vals(std::istream_iterator<int>{std::cin},{});
		
	for(std::size_t i=0;i<vals.size();++i)
	{
		for(std::size_t j=i+1;j<vals.size();++j)
		{
			if(vals[i]+vals[j]==2020)
			{
				std::cout<<vals[i]*vals[j]<<'\n';
				return 0;
			}
		}
	}
	return 0;
}
