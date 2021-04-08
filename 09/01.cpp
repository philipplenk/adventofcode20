#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <unordered_map>
#include <vector>

#include <cstddef>

template <typename T,std::size_t N>
class trivial_fixed_ring
{
	public:
	using value_type = T;
	
	void push_back(const T& value)
	{
		data_[start_++] = value;
		start_%=N;
		
	}
	
	auto& operator[](std::size_t idx)
	{
		return data_[(start_+idx)%N];
	}
	
	auto& operator[](std::size_t idx) const
	{
		return data_[(start_+idx)%N];
	}
	
	auto size() const { return N; }
	auto begin() const { return std::begin(data_); }
	auto end() const { return std::end(data_); } 
	
	private:
	std::array<T,N> data_;
	std::size_t start_ = 0;
};

int main(int argc, char* argv[])
{
	constexpr auto n = 25;
	trivial_fixed_ring<int,n> data;
	std::copy_n(std::istream_iterator<int>{std::cin},n,std::back_inserter(data));
	
	std::unordered_map<int,int> seen;
	for(auto v: data) ++seen[v];
	
	int next;
	while(std::cin>>next)
	{
		bool found = false;
		for(auto v: data)
		{
			found = seen[next-v]>0;
			if(found) break;
		}
		if(!found) break;
		
		--seen[data[0]];
		++seen[next];
		data.push_back(next);
	}
	
	std::cout<<next;
	
	return 0;
}
