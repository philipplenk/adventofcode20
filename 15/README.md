## Part 1

Let's play a number game. We start of by reading a bunch of given numbers and remembering the previous two times they were said. Now, after they were read once, the next number depends on the previous one. If it was spoken for the first time, we speak 0. Otherwise, we speak the difference between the last two times it was spoken. That is all. Our task was to determine the 2020th number spoken. Rather cryptic instructions to read and it took me a few minutes to comprehend what was asked of us, but once that fog had cleared, a solution is rather trivial. First define what we have to remember for each spoken number and a hashmap storing this piece of information:

	struct diff
	{
		std::size_t prev = 0, last = 0;
	};
	std::unordered_map<int,diff> last_spoken;

And then simply simulate the process exactly as given: 

	int last = 0;
	for(std::size_t i=0;i<2020;++i)
	{
		if(i<numbers.size())
		{
			last = numbers[i];
			last_spoken[numbers[i]] = {i,i};
		}
		else
		{
			const auto& data = last_spoken[last];
			const auto dist = data.last-data.prev;
			last = dist;
			
			if(auto it = last_spoken.find(dist); it==std::end(last_spoken))
			{
				auto& to_change_data = last_spoken[dist];
				
				to_change_data.prev = i;
				to_change_data.last = i;
			}
			else
			{
				auto& to_change_data = last_spoken[dist];
				
				to_change_data.prev = to_change_data.last;
				to_change_data.last = i;
			}
		}
	}

## Part 2

Nothing changed, we are just asked for a significantly higher number of turns. Seems difficult at first, but I took the lucky shot and tested if my previous solution worked fast enough and it turns out it completed within 8 seconds with a correct result, jumping in leaderboard position from 1700-ish to 500-ish(far from global again, of course). Nonetheless, I was certain we can do better here.
 
The first trick I could think of, as it appeared occasionally in problems of this kind, is detecting cycles, so I tried this and it turns out, at least for my numbers and my bound, there was no cycle ;_; So we had to get smarter than that... aaand after far too much time spent on my own attempts I began to google and figured out that:

1. Given the right starting conditions it is actually a named sequence with an entry in the [OEOIS](https://oeis.org/A181391)
2. There is a [numberphile video about it](https://www.youtube.com/watch?v=etMJxB-igrc), which I highly recommend watching. And - of course:
3. There is no known way to get at the entries faster than brute force. So anything that is asymptotically better than what I wrote for part 1 would be a minor mathematical breakthrough ;-)

As such, all I could do was optimize what I was already doing. The first and most obvious choice is cutting down on unnecessary allocations, by preallocating enough elements into my hashmap:

	last_spoken.reserve(30000000);

This cut the runtime down to 5 seconds, which is quite a decent improvement for one line. Switching from a hashmap to a simple vector of 30000000 elements brought another similar improvement, getting the time down to just 2 seconds, switching the type saved from a wasteful std::size_t to int reduced it to 1.5 seconds. In the process, I also made my code look significantly more compact and beautiful than the horrid mess I showed you for part 1:

	for(;i<30000000;++i)
	{
		const auto& data = last_spoken[last];
		const auto dist = data.last-data.prev;
		last = dist;
		
		auto& to_change_data = last_spoken[dist];
			
		to_change_data.prev = to_change_data.seen?to_change_data.last:i;
		to_change_data.last = i;
		to_change_data.seen = true;
	}

Hacking around a tiny bit more to get a more compact memory layout by stealing a bit from "last" instead of having a separate variable for "seen" got the time down to about 1 second, at which point I stopped bothering.