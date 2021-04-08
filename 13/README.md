
## Part 1

Given a starting time and a number of bus ids - which also represented their round trip time - we were tasked with finding the first bus to arrive at or after the starting time. For this one, all busses started out at 0, meaning we only had determine the first multiple of each id >= the starting time and choose the minimum of those. To do so, we compute the ceil(i.e. rounded up) result of dividing the starting time by id and subsequently multiply id by that result:

```cpp
long min_time = std::numeric_limits<long>::max();
long best_id = 0;
for(auto id:ids)
{
	auto next_factor = first_timestamp/id + (first_timestamp%id!=0);
	auto ts = next_factor * id;
	
	if(ts<min_time)
	{
		best_id = id;
		min_time = ts;
	}
}
```

## Part 2

It really pays off to know a tiny bit of linear algebra from time to time ;-) Without it, this one would be really, really hard. 

Given not only the periods, but a desired offset for each id, we had to determine the first time satisfying for which each bus arrived exactly its offset later. In other words, we had to solve a series of equations of the form

	x = -a mod id0
	x = -b mod id1
	x = -c mod id2...

That is a case for the [famous Chinese remainder theorem](https://en.wikipedia.org/wiki/Chinese_remainder_theorem)! 
  
As such, we first need a way to compute [modular multiplicative inverses](https://en.wikipedia.org/wiki/Modular_multiplicative_inverse), which can be done with [the extended euclidean algorithm](https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm):

```cpp
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
```

For each id, we compute the modular inverse of it with the product of all others and multiply it with that product and the desired offset. Our result is the sum of doing that for each id, modulo the product of all ids. Unfortunately, the offsets are negative in our case and modulo with negative numbers is - ummm - [let's say *special*](https://stackoverflow.com/questions/7594508/modulo-operator-with-negative-values) in C++, so I used an expensive loop instead:
	
```cpp	
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
```