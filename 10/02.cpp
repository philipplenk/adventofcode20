#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>

#include <cstddef>

std::size_t count_options(const std::vector<long long>& jolts, long long last_jolt, std::size_t next_idx, std::map<std::pair<long long,std::size_t>,std::size_t>& cache)
{
	if(auto it=cache.find({last_jolt,next_idx});it!=std::end(cache))
		return it->second;
		
	long long begin_jolt = last_jolt;
	
	std::size_t result = 0;
	for(std::size_t i=next_idx;i<jolts.size(); ++i)
	{
		const auto diff = jolts[i]-last_jolt;
		
		if(diff>3)
			return 0;
			
		if(diff<3 && i<jolts.size()-1)
			result+=count_options(jolts,last_jolt,i+1,cache);
		
		last_jolt = jolts[i];
	}
	
	return cache[{begin_jolt,next_idx}] = result + 1;
}

int main(int argc, char* argv[])
{
	std::vector<long long> jolts(std::istream_iterator<long long>{std::cin},std::istream_iterator<long long>{});
	std::sort(std::begin(jolts),std::end(jolts));
	
	std::map<std::pair<long long,std::size_t>, std::size_t> cache;
	std::cout<<count_options(jolts,0,0,cache)<<'\n';

	return 0;
}
