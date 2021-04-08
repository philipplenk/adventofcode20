## Part 1

This one was a bit more interesting and certainly educational. We were given a lovely description of how a certain airliner numbers their seats by repeatedly subdividing a range, throwing away either the lower or the upper half with each incoming letter. Whilst reminiscent of binary search, it is basically even simpler. The description mirrors exactly the classical definition of what a bit actually is: something that halves your prior uncertainty. As such, the strings proved to be nothing more than a fancy encoding for a 10-digit binary number, MSB first. With this realization, completing the task - that of finding the highest id - was trivial:

	const auto seat_id=[](const std::string& str)
	{
		std::uint16_t id = 0;
		for(auto c: str)
		{
			id<<=1;
			if(c=='B' || c=='R')
				id|=1;
		}
		return id;
	};
	
	std::uint16_t max_id = std::numeric_limits<std::uint16_t>::min();
	for(std::string str;std::cin>>str;)
	{
		max_id = std::max(max_id,seat_id(str));
	}

Note how we cannot use [max_element](https://en.cppreference.com/w/cpp/algorithm/max_element), as that sadly requires a [forward iterator](https://en.cppreference.com/w/cpp/named_req/ForwardIterator).

## Part 2

Given the same list, we were now tasked with finding the one and only missing element. The list does not necessarily start or end at the lowest and highest possible id, but our missing element was declared to be somewhere in between.
This could - again - be solved in more than one way, with up and downsides of runtime, memory usage or readability. First, the [more readable solution](02_adjacent_find.cpp), at an additional O(n) memory (for storing all ids) and O(n log n) runtime (for sorting them):

	std::vector<std::uint16_t> ids;
	std::transform(std::istream_iterator<std::string>{std::cin},{},std::back_inserter(ids),seat_id);
	
	std::sort(std::begin(ids),std::end(ids));
	std::cout<<(*std::adjacent_find(std::begin(ids),std::end(ids),[](auto lhs, auto rhs) { return rhs-lhs>1; })+1)<<'\n';

Already pretty decent, but we can do better by knowing and abusing a tiny bit of bit magic, specifically the following lovely properties of the XOR operation:

	A ^ A == 0

I.e. a number xor'ed with itself disappears. Further:

	A^B == B^A

That is, the operation is commutative, the order in which it is performed does not matter. And finally:

	A^0 == A 

A number xor'ed with 0 does not change.

All of this can be combined to imply this:

	(A^B^C^E...) ^ (A^B^C^D^E...)

will be equivalent to

	(A^A) ^ (B^B) ^ (C^C) ^ (E^E) ... ^ D

meaning the only one left over that is not zeroed will be our missing element! This yields [this beautifully efficient solution](02_xor.cpp) that no one without knowledge of the trick has any realistic hope to grasp:

	std::uint16_t min=std::numeric_limits<std::uint16_t>::max(), max=std::numeric_limits<std::uint16_t>::min();
	std::uint16_t xored = 0, all = 0;
	std::for_each(std::istream_iterator<std::string>{std::cin},{},[&](const auto& str)
	{
		const auto id = seat_id(str);
		min=std::min(min,id);
		max=std::max(max,id);
		xored^=id;
	});
	
	for(std::uint16_t i=min;i<=max;++i)
		all^=i;
	
	std::cout<<(xored^all)<<'\n';

Knowing the trick, it does seem rather straightforward so and has only constant memory footprint and linear runtime, which is just fabulous ;-)

Accessibility can, however, be somewhat regained whilst not only keeping but even improving ever so slightly on this runtime by recognizing there is a simpler pair of operations with very similar properties: + and -. 

	A+0 = A

	A-A = 0

	A+B = B+A

Meaning, of course, 

	(A+B+C+D+E...) - (A+B+C+E...) = (A-A) + (B-B) + (C-C) + D + (E-E)... = D

Simply summing all elements and subtracting that sum from what it would be if none were missing yields exactly the one missing element! Knowing [a bit of history](https://www.youtube.com/watch?v=Dd81F6-Ar_0) and Gauss's famous formula for computing the sum of all integers up to a given bound((n*n+n)/2), we can skip the second loop and directly compute the desired sum in closed form:

	std::uint16_t min=std::numeric_limits<std::uint16_t>::max(), max=std::numeric_limits<std::uint16_t>::min();
	int sum = 0;
	std::for_each(std::istream_iterator<std::string>{std::cin},{},[&](const auto& str)
	{
		const auto id = seat_id(str);
		min=std::min(min,id);
		max=std::max(max,id);
		sum+=id;
	});
	
	--min;
	std::cout<<((max*max+max)/2-(min*min+min)/2-sum)<<'\n';

