#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
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
	
	int next;
	while(std::cin>>next)
	{
		std::array<int,n> buffer;
		std::partial_sort_copy(std::begin(data),std::end(data),std::begin(buffer),std::end(buffer));
		
		bool found = false;
		for(auto v: buffer)
		{
			found = std::binary_search(std::begin(buffer),std::end(buffer),next-v);
			if(found) break;
		}
		if(!found) break;
		
		data.push_back(next);
	}
	
	std::cout<<next;
	
	return 0;
}
