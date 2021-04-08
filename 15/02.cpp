#include <iostream>
#include <vector>

#include <cstddef>

int main(int argc, char* argv[])
{	
	struct diff
	{
		int prev = 0, last_ = 0;
		
		int last() const { return last_&(~(1<<30)); }
		void see() { last_|=(1<<30); }
		bool seen() const { return last_&(1<<30); }
		
	};
	std::vector<diff> last_spoken(30000000);
	
	int last, i = 0;
	char skip_c;
	for(;std::cin>>last;++i)
	{
		last_spoken[last] = {i,i};
		last_spoken[last].see();
		std::cin>>skip_c;
	}
	
	for(;i<30000000;++i)
	{
		const auto& data = last_spoken[last];
		const auto dist = data.last()-data.prev;
		last = dist;
		
		auto& to_change_data = last_spoken[dist];
			
		to_change_data.prev = to_change_data.seen()?to_change_data.last():i;
		to_change_data.last_ = i;
		to_change_data.see();
	}
	
	std::cout<<last<<'\n';
	
	return 0;
}
