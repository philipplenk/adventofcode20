#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

#include <cstddef>
#include <cstdint>

int main(int argc, char* argv[])
{	
	struct forward_list_element
	{
		std::uint32_t next;
	};
	std::vector<forward_list_element> list;
	list.resize(1000000);
	
	for(std::size_t i=0;i<list.size()-1;++i)
		list[i].next = i+1;
	
	char first_c, prev_c, c;
	std::cin>>first_c;
	list[list.size()-1].next = first_c-'1';

	std::size_t prefix_size = 1;
	prev_c = first_c;
	while(std::cin>>c)
	{
		list[prev_c-'1'].next = c-'1';
		prev_c = c;
		++prefix_size;
	}
	list[prev_c-'1'].next = prefix_size;
	
	const auto move = [&](int original_value)
	{
		auto target_value = original_value==0?(list.size()-1):(original_value-1);
		 
		std::array<std::size_t,3> taken;
		taken[0]=list[original_value].next;
		taken[1]=list[taken[0]].next;
		taken[2]=list[taken[1]].next;
		
		list[original_value].next = list[taken[2]].next;
		
		while(std::find(std::begin(taken),std::end(taken),target_value)!=std::end(taken))
			target_value = target_value==0?list.size()-1:(target_value-1);
		
		list[taken[2]].next = list[target_value].next;
		list[target_value].next = taken[0];
		
		return list[original_value].next;
	};
	
	int current = first_c-'1';
	for(std::size_t i=0;i<10000000;++i)
		current = move(current);
	
	const std::size_t next = list[0].next;
	const std::size_t next_next = list[next].next;
	std::cout<<(next+1)*(next_next+1)<<'\n';
	return 0;
}
