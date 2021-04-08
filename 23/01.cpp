#include <algorithm>
#include <iostream>
#include <vector>

#include <cstddef>

int main(int argc, char* argv[])
{
	std::vector<int> cups;

	char c;
	while(std::cin>>c)
		cups.push_back(c-'0');
	
	const auto move = [&](std::size_t current)
	{
		auto original_value = cups[current];
		auto target_value = cups[current]-1;
		if(target_value==0) target_value = 9;
		 
		std::array<int,3> taken;
		for(std::size_t i=0;i<3;++i)
		{
			taken[i] = cups[(current+1+i)%cups.size()];
			cups[(current+1+i)%cups.size()]=0;
		}
		cups.erase(std::remove(std::begin(cups),std::end(cups),0),cups.end());
		
		while(std::find(std::begin(taken),std::end(taken),target_value)!=std::end(taken))
			target_value = target_value==1?9:(target_value-1);
		
		auto target_pos = std::find(std::begin(cups),std::end(cups),target_value)+1;
			
		cups.insert(target_pos,std::begin(taken),std::end(taken));
		
		auto original_pos = std::find(std::begin(cups),std::end(cups),original_value) - std::begin(cups);
		
		if(static_cast<long>(current)<original_pos)
			std::rotate(std::begin(cups),std::begin(cups)+(original_pos-current), std::end(cups));
		else
			std::rotate(std::rbegin(cups),std::rbegin(cups)+(current-original_pos), std::rend(cups));
		
		return (current+1)%cups.size();
	};
	
	std::size_t current = 0;
	for(std::size_t i=0;i<100;++i)
		current = move(current);
	
	
	auto target_pos = (std::find(std::begin(cups),std::end(cups),1)-std::begin(cups))+1;
	for(std::size_t i=0;i<8;++i)
		std::cout<<cups[(target_pos+i)%cups.size()];
	std::cout<<'\n';

	return 0;
}
