#include <algorithm>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>

#include <cstddef>

int main(int argc, char* argv[])
{
	std::vector<int> cups;

	char c;
	while(std::cin>>c)
		cups.push_back(c-'0');
	
	cups.resize(1000000);
	
	std::iota(std::begin(cups)+9, std::end(cups),10);
	
	std::unordered_map<long,long> next, prev;
	
	for(std::size_t i=0;i<cups.size();++i)
	{
		next[cups[i]]=cups[(i+1)%cups.size()];
		prev[cups[i]]=cups[i==0?(cups.size()-1):(i-1)];
	}
	
	const auto move = [&](int original_value)
	{
		auto target_value = original_value-1;
		if(target_value==0) target_value = cups.size();
		 
		std::array<long,3> taken;
		taken[0]=next[original_value];
		taken[1]=next[taken[0]];
		taken[2]=next[taken[1]];
		
		prev[next[taken[2]]] = original_value;
		next[original_value] = next[taken[2]];
		
		while(std::find(std::begin(taken),std::end(taken),target_value)!=std::end(taken))
			target_value = target_value==1?cups.size():(target_value-1);
		
		next[taken[2]] = next[target_value];
		next[target_value] = taken[0];
		prev[taken[0]] = target_value;
		
		return next[original_value];
	};
	
	int current = cups[0];
	for(std::size_t i=0;i<10000000;++i)
		current = move(current);
	
	std::cout<<next[1]*next[next[1]]<<'\n';

	return 0;
}
