#include <algorithm>
#include <iostream>

int main(int argc, char* argv[])
{
	long long card_key, door_key;
	std::cin>>card_key>>door_key;
	
	constexpr long long modulus = 20201227;
	constexpr long long subject_number = 7;
	
	const auto transform_with = [](long long subject_number, long long loop_size)
	{
		long long value = 1;
		
		while(loop_size--)
			value=value*subject_number%modulus;
		
		return value;
	};
	
	const auto find_factor = [&](long long key)
	{
		long long value = 1, loop_size = 0;
		while(value!=key)
		{
			value=value*subject_number%modulus;
			++loop_size;
		}
		return loop_size;
	};
		
	std::cout<<transform_with(card_key,find_factor(door_key))<<'\n';
	
	return 0;
}
