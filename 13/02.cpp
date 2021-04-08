#include <algorithm>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

auto multiplicative_inverse(long long a, long long b)
{
	auto old_r = a;
	auto r = b;
	long long old_s = 1;
	long long s = 0;
	
	while(r!=0)
	{
		auto quotient = old_r/r;
		
		auto new_r = old_r-quotient*r;
		auto new_s = old_s-quotient*s;
		
		std::tie(old_r,r) = std::tie(r,new_r);
		std::tie(old_s,s) = std::tie(s,new_s);
	}
	
	while(old_s<0)
		old_s+=b;
		
	return old_s;
}


int main(int argc, char* argv[])
{
	struct offset_id_pair
	{
		long long offset, mod;
	};
	std::vector<offset_id_pair> ids;
	
	long long next, offset = 0;
	char ignore;
	std::string tmp;
	std::getline(std::cin,tmp);
	
	long long mod_product = 1;
	for(;std::cin;++offset)
	{
		if(std::cin.peek()=='x')
			std::cin>>ignore>>ignore;
		else
		{
			std::cin>>next>>ignore;
			ids.push_back({offset,next});
			mod_product*=next;
		}
	}
			
	long long result = 0;
	for(auto id:ids)
	{
		const auto mod = mod_product/id.mod;
		const auto inverse = multiplicative_inverse(mod,id.mod);
		
		auto off = -id.offset;
		while(off<0) off+=id.mod;

		result+=off*mod*inverse;
		result%=mod_product;
	}
		
	std::cout<<result<<'\n';

	return 0;
}
