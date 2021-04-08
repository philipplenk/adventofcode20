## Part 1

Travel power adapters, oh my. The cover story is a cute one, but the actual problem is rather straightforward. Given a number of "joltage" adapters output values, connectable if they are between 1 and 3 apart and the second is higher than the first, figure out how often they are apart by 1 and how often they are apart by 3 if all were connected together. Since we never can connect a higher to a lower adapter, we simply sort and count the difference between every two elements:

	std::vector<int> jolts(std::istream_iterator<int>{std::cin},std::istream_iterator<int>{});
	std::sort(std::begin(jolts),std::end(jolts));
	
	std::size_t diff1 = 0, diff3 = 0;
	int last = 0;
	for(std::size_t i = 0;i<jolts.size(); ++i)
	{
		auto diff = jolts[i]-last;
		if(diff==1)
			++diff1;
		if(diff==3)
			++diff3;
		last = jolts[i];
	}
	std::cout<<(diff1*(diff3+1))<<'\n';

## Part 2

That was ever so slightly more difficult. My first hunch was a simple recursive solution, counting the options:


	std::size_t count_options(const std::vector<long long>& jolts, long long last_jolt, std::size_t next_idx)
	{
		long long begin_jolt = last_jolt;
		
		std::size_t result = 0;
		for(std::size_t i=next_idx;i<jolts.size(); ++i)
		{
			const auto diff = jolts[i]-last_jolt;
			
			if(diff>3)
				return 0;
				
			if(diff<3 && i<jolts.size()-1)
				result+=count_options(jolts,last_jolt,i+1,cache);
			
			last_jolt = jolts[i];
		}
		
		return result + 1;
	}

Turns out that is not ideal for a number of reasons, most obvious among them is runtime and repeated work. If, for instance, we skip the first adapter and take the second and third on one path, whilst we take the first, skip the second and take the third, the total may be different, but what follows afterwards is exactly the same. Nonetheless, the code above will compute this twice. Every subpath can be reached in a multitude of ways and I recompute it every. single. time. Let's fix that:

	std::size_t count_options(const std::vector<long long>& jolts, long long last_jolt, std::size_t next_idx, std::map<std::pair<long long,std::size_t>,std::size_t>& cache)
	{
		if(auto it=cache.find({last_jolt,next_idx});it!=std::end(cache))
			return it->second;
			
		long long begin_jolt = last_jolt;
		
		std::size_t result = 0;
		for(std::size_t i=next_idx;i<jolts.size(); ++i)
		{
			const auto diff = jolts[i]-last_jolt;
			
			if(diff>3)
				return 0;
				
			if(diff<3 && i<jolts.size()-1)
				result+=count_options(jolts,last_jolt,i+1,cache);
			
			last_jolt = jolts[i];
		}
		
		return cache[{begin_jolt,next_idx}] = result + 1;
	}

I employed the time honored tradition of slapping a cache onto my recursion to avoid duplicating work. This turns out to be more than fast enough for our problem and a result is available within the fraction of a second again. Nonetheless, we could have done even better, even if I failed to figure that one out myself in the heat of the moment. It was pointed out to me on the [#include discord](https://www.includecpp.org/discord/) and is visible in [this solution](https://gitlab.com/stephanroslen/aoc2020/-/blob/master/day10/main.cpp). Instead of going forward and counting what could follow, we can - true to the spirit of [dynamic programming](https://en.wikipedia.org/wiki/Dynamic_programming) - go backward and count the pathes we have already seen. Basically the same idea, but we can save the hashmap and know ahead of time how many entries we will need.

Wonderful, but checking [the lovely reddit](https://www.reddit.com/r/adventofcode/comments/kabi91/2020_day_10_closedform_mathematical_solution/), you will figure out there is an even better, if far more mathy[^mathy] and slightly less intuitive way running in what is apparently linear time. Turns out there is a connection to the [tribonacci sequence](https://en.wikipedia.org/wiki/Generalizations_of_Fibonacci_numbers#Tribonacci_numbers).

[^mathy]: which is a good thing!

That was fun and I almost got near the global leaderboard for once, rank 508, my best so far this year.)
