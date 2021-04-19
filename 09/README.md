## Part 1

Surprisingly easy given the previous two assignments, our quest on this day was simply adding up numbers. We were given a list of them and should find the first one that cannot be represented as a sum of two of the 25 preceding ones. The more perceptive of my readers probably realized by now, that in the grand tradition of instructional [spiraling](https://en.wikipedia.org/wiki/Spiral_approach), this is but a slightly more complicated variation of what we did on day 1! 

As such, a trivial - yet horrific in time complexity - [solution](01_trivial.cpp) is to brute force our way through the problem with 3 ugly nested loops:

```cpp
for(std::size_t i=n;i<data.size();++i)
{
	bool found = false;
	for(std::size_t j=i-n;j<i;++j)
	{
		for(std::size_t k=j+1;k<i;++k)
		{
			if(data[j]+data[k]==data[i])
			{
				found = true;
				break;
			}
		}
		if(found) break;
	}
	
	if(!found)
	{
		std::cout<<data[i]<<'\n';
		break;
	}
}
```

Funnily enough, that is still fast enough and if we analyzed its complexity at university level, we would call it O(n), as 25 is a constant and so is 25^2. 375 * n is still O(n), so we are golden xD. Well, as you might have guessed from my somewhat snappy tone here, I don't really like that much. We also keep all of our range in memory right now, which could be avoided.

We only need the last 25 numbers, so lets start with that:

```cpp
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
```

Yeah, as there is none in the standard library<a title="Unfortunately, we did not get a ring in C++20 either. Can anyone tell me what happend to [p0059?](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0059r4.pdf)"><sup>1</sup></a>, implementing a small, bad and severely broken ring buffer might be ever so slight overkill, considering the input list is a mere 1000 numbers and the approach above runs in the fraction of a fraction of a fraction of a second, but who am I if not one to always entertain the notion of severely overengineering the most trivial of tasks?

Utilizing this data structure, our solution changes to look like this:

```cpp
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
```

Our theoretical runtime can be reduced by using the exact same methods employed on day 1: either via sorting or via wasting a potentially huge amount of memory. 

Let's do [sorting first](01_sort.cpp). We can't sort in place, as the original order dictates which elements will be replaced in succeeding steps, which is why we have to copy them into an additional buffer first:

```cpp
std::array<int,n> buffer;
std::partial_sort_copy(std::begin(data),std::end(data),std::begin(buffer),std::end(buffer));
```

We then walk this buffer once, binary-searching for the missing element:

```cpp
bool found = false;
for(auto v: buffer)
{
	found = std::binary_search(std::begin(buffer),std::end(buffer),next-v);
	if(found) break;
}
if(!found) break;
```

Lovely, isn't it? We reduced our complexity down to O(n) * O(25 log 25), which is, asymptotically speaking, completely irrelevant. If the constant were ever increased, however, it could prove useful to reduce it even further.

Last time, we had a convenient constraint on how big our numbers could get, which was completely thrown out for this task. Numbers here can - and apparently do - get arbitrarily big, so its kind of impossible to allocate a big enough array in advance. Whilst we could determine this number in linear time(by simply scanning the input once and saving max and min values), a simple bitset won't do, as our sliding window might contain more than one copy of the same value and we have to keep track of when all of those fall out of the window, i.e. we have to count. Considering my solution was >20 million, keeping one integer for each *possible* one in the input constitutes unfathomable abuse of memory even for the likes of me, so I bailed out and decided on using a hashmap instead, which will - at least on average - still provide me with O(1)-ish access time:

```cpp
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
```

## Part 2

Surprisingly even easier than part 1 this time. Take the number we just obtained and find a contiguous set of size>2 in our data that sums to it. We can do this easily by keeping a running set, always adding the next number and - if we ever run over our target - erase from the start until we are <= to it again:

```cpp
std::vector<int> data(std::istream_iterator<int>{std::cin},{});
std::size_t sum = data[0], min = 0;
for(std::size_t i=1;i<data.size();++i)
{
	sum+=data[i];
	while(sum>target && min<i)
	{
		sum-=data[min];
		++min;
	}
	if(sum==target)
	{
		auto [min_it,max_it] = std::minmax_element(&data[min],&data[i]+1);
		std::cout<<*min_it+*max_it<<'\n';
		break;
	}
}
```
