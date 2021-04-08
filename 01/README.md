## Part 1

The first puzzle was simply finding two elements in a list of numbers which sum to another number(in our case the then current year, 2020). 

This can, of course, be solved trivially in quadratic time, as can be seen in [01_nested_loop.cpp](01_nested_loop.cpp):
	
	```cpp
	for(std::size_t i=0;i<vals.size();++i)
	{
		for(std::size_t j=i+1;j<vals.size();++j)
		{
			if(vals[i]+vals[j]==2020)
			{
				//found it!
			}
		}
	}
	```

If we want to [get fancy](01_binary_search.cpp), we can sort our input in O(n log n) time, iterate over it and find the potentially corresponding number in O(log n) time, yielding a total running time of O(n log n) again:

	std::sort(std::begin(vals),std::end(vals));
		
	for(auto v:vals)
	{
		if(std::binary_search(std::begin(vals),std::end(vals),2020-v))
		{
			//found it!
		}
	}

Yes, [binary_search in the standard library returns a boolean](https://en.cppreference.com/w/cpp/algorithm/binary_search). This is one of the rare occasions where that is actually exactly what we want. For all the others, there is [lower_bound](https://en.cppreference.com/w/cpp/algorithm/lower_bound) and [upper_bound](https://en.cppreference.com/w/cpp/algorithm/upper_bound).

That is rather nice, but if we are willing to sacrifice just a tiny little bit of additional memory(O(m) with m being the searched for sum) and assume all numbers are non-negative we could [even go linear](01.cpp):

	std::bitset<2021> seen{};
		
	for(auto v: vals)
	{
		if(v>2020) continue;
		
		seen.set(v);
		if(seen.test(2020-v))
		{
			//found it!
		}
	}

Without the assumption of non-negativity - which held for my input, but might not for everyones -  we can still do a variation of this, but would have to use significantly more memory and/or rely on some sort of hashset.

## Part 2

For a tiny bit more difficulty (and were the numbers bigger also a significant spike in runtime for the trivial solutions), the second part asks us to find three numbers that sum to 2020. Luckily for us, the list is reasonably short and even the [now cubic nested for loops](02_nested_loop.cpp) produce the correct result in a very short time(a very small fraction of a second on even my old machine):
	
	for(std::size_t i=0;i<vals.size();++i)
	{
		for(std::size_t j=i+1;j<vals.size();++j)
		{
			for(std::size_t k=j+1;k<vals.size();++k)
			{
				if(vals[i]+vals[j]+vals[k]==2020)
				{
					//found it!
				}
			}
		}
	}

Both of our slightly more sophisticated methods for part 1 can relatively easily be adapted to this new constraint. For the one applying sorting, simply add yet another for loop, yielding a O(n^2 log(n)) runtime. [Nothing to write home about](02_binary_search.cpp), but a step up from cubic...
	
	std::sort(std::begin(vals),std::end(vals));
		
	for(std::size_t i=0;i<vals.size();++i)
	{
		for(std::size_t j=i+1;j<vals.size();++j)
		{
			const auto target = 2020-vals[i]-vals[j];
			if(std::binary_search(std::begin(vals),std::end(vals),target))
			{
				//found it!
			}
		}
	}

Again, we can get rid of that tiny annoying log(n) factor, given the same assumptions and using the same technique as above. Adding another for loop results in [the following code](02.cpp), for "only" quadratic time.

	std::bitset<2021> seen{};
		
	for(std::size_t i=0;i<vals.size();++i)
	{
		if(vals[i]>2020) continue;
		seen.set(vals[i]);
		
		for(std::size_t j=i+1;j<vals.size();++j)
		{
			if(vals[j]>2020) continue;
			seen.set(vals[j]);
			
			const auto target = 2020-vals[i]-vals[j];
			
			if(target<2021 && target>=0 && seen.test(target))
			{
				std::cout<<vals[i]*vals[j]*target<<'\n';
				return 0;
			}
		}
	}

More than sufficient for the small input set, yet I really did not like stopping here. 

Whilst it remains an open research problem whether or not the general [3SUM problem](https://en.wikipedia.org/wiki/3SUM) is solvable in subquadratic time, we do have some additional constraints. Our input consists of only non-negative integers smaller or equal to our target value. Wikipedia claims that for integers in range -n to n there exists a solution in O(n log n) utilizing a Fast Fourier Transform on the input set represented as a bit vector, but unfortunately I failed to understand how exactly such a solution could be constructed.

So I tried to find a different, better solution, gave it a tiny bit more thought and my contemplations yielded [the following code](02_wrong.cpp):

	std::sort(std::begin(vals),std::end(vals));	
	std::size_t lower = 0, higher = vals.size()-1;
	
	while(lower<higher)
	{
		const auto target = 2020-vals[lower]-vals[higher];
		if(std::binary_search(std::begin(vals),std::end(vals),target))
		{
			//found it!
		}
		else if(vals[lower+1]-vals[lower]<vals[higher]-vals[higher-1])
			++lower;
		else
			--higher;
	}

The runtime is bounded fairly simple: lower and higher start n values apart from each other and in each iteration either one is moved towards the other or a solution is found. As such, the while loop runs at most n times. The most expensive operation within is the binary search, running in log n time. As such, the overall complexity is O(n*log(n)) (for sorting) + O(n*log(n)) (for the loop), yielding a glorious O(n*log(n)) in total!

That is all fine and dandy, it even worked for my input and the few others I could find and try it on at the time, but that alone is far from sufficient to prove it could work for any input. As such, I tried to sketch a proof and in so doing quickly had to come to a shattering realization: **this is completely wrong and ridiculously stupid!**

For instance, it fails with the following simple input: 

	1 9 1009 1010 2000

Nonetheless, I considered this failure educational and working out why it works for some inputs and how exactly it fails so spectacularly on many others can be quite instructive, which is why I still showcase it here.
