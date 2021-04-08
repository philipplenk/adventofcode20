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
		bool found = false;
		for(std::size_t i=0;i<data.size();++i)
		{
			for(std::size_t j=i+1;j<data.size();++j)
			{
				if(data[i]+data[j]==next)
				{
					found = true;
					break;
				}
			}
			if(found) break;
		}
		if(!found) break;
		
		data.push_back(next);
	}
	
	std::cout<<next;
	
	return 0;
}
