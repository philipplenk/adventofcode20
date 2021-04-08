Well, that was an experience... My sleep deprivation at this point might have hit a new high, causing me to have somewhat less concentration and a much better excuse than usual, but it is just an excuse and I failed to see the obvious solution right in front of my eyes. But that's something for later, first, lets describe the problem:

## Part 1

Given a bunch of numbered cups, arranged in a circle, our opponent - a very intelligent crab, apparently the [pinnacle of evolution](https://en.wikipedia.org/wiki/Carcinisation) - rearranges them according to a number of simple rules. We have to determine where a specific cup ends up after a given number of moves. In each move, the crab picks up three cups clockwise after the "current" one and moves them right after the next lower numbered cup. The one after the newly inserted ones then becomes the next "current" cup.

Here is my hacky, god-awful looking and performing code simulating one such move:

	const auto move = [&](std::size_t current)
	{
		auto original_value = cups[current];
		auto target_value = cups[current]-1;
		if(target_value==0) target_value = 9;
		 
		std::array<int,3> taken;
		for(std::size_t i=0;i<3;++i)
		{
			taken[i] = cups[(current+1+i)%cups.size()];
			cups[(current+1+i)%cups.size()]=0;
		}
		cups.erase(std::remove(std::begin(cups),std::end(cups),0),cups.end());
		
		while(std::find(std::begin(taken),std::end(taken),target_value)!=std::end(taken))
			target_value = target_value==1?9:(target_value-1);
		
		auto target_pos = std::find(std::begin(cups),std::end(cups),target_value)+1;
			
		cups.insert(target_pos,std::begin(taken),std::end(taken));
		
		auto original_pos = std::find(std::begin(cups),std::end(cups),original_value) - std::begin(cups);
		
		if(static_cast<long>(current)<original_pos)
			std::rotate(std::begin(cups),std::begin(cups)+(original_pos-current), std::end(cups));
		else
			std::rotate(std::rbegin(cups),std::rbegin(cups)+(current-original_pos), std::rend(cups));
		
		return (current+1)%cups.size();
	};

As you can see, I chose to represent the cups as integers in a giant array and copied and shifted them around, which was likely the worst, most expensive representation possible and which came back to bite me in part 2.

## Part 2

Part 2 took the problem up to 11. Or rather up to 1000000 cups * 10000000 moves, causing this most trivial of brute force solutions to fail horrendously.

For the longest of times, I suspected there must be some sort of clever closed form solution. Turns out it was just an issue with implementing it in a little less brain dead way. A lot of cursing later, I realized that - of course - this is **NOT** a [rotate](https://www.youtube.com/watch?v=UZmeDQL4LaE), its simply a freaking linked list and that is one of the very few cases where using one actually proves more efficient and beneficial than a simple vector. Especially as we can still find any given list element in O(1), as they only contain one number. So [this simple solution](02_unordered_map.cpp) finally gave me my desired star, using two hashmaps to represent the lists:


	std::unordered_map<long,long> next, prev;
	
	for(std::size_t i=0;i<cups.size();++i)
	{
		next[cups[i]]=cups[(i+1)%cups.size()];
		prev[cups[i]]=cups[i==0?(cups.size()-1):(i-1)];
	}
	
	const auto move = [&](int original_value)
	{
		auto target_value = original_value-1;
		if(target_value==0) target_value = cups.size();
		 
		std::array<long,3> taken;
		taken[0]=next[original_value];
		taken[1]=next[taken[0]];
		taken[2]=next[taken[1]];
		
		prev[next[taken[2]]] = original_value;
		next[original_value] = next[taken[2]];
		
		while(std::find(std::begin(taken),std::end(taken),target_value)!=std::end(taken))
			target_value = target_value==1?cups.size():(target_value-1);
		
		next[taken[2]] = next[target_value];
		next[target_value] = taken[0];
		prev[taken[0]] = target_value;
		
		return next[original_value];
	};

That is - of course - an utter waste and neither hashmaps nor doubly linked lists are required here. Instead, we can simply store the indices of our followup elements as a sort of in place forward list

	struct forward_list_element
	{
		std::uint32_t next;
	};
	std::vector<forward_list_element> list;
	list.resize(1000000);

and implement a move by changing the followup indices of the current element and the last one taken:

	const auto move = [&](int original_value)
	{
		auto target_value = original_value==0?(list.size()-1):(original_value-1);
		 
		std::array<std::size_t,3> taken;
		taken[0]=list[original_value].next;
		taken[1]=list[taken[0]].next;
		taken[2]=list[taken[1]].next;
		
		list[original_value].next = list[taken[2]].next;
		
		while(std::find(std::begin(taken),std::end(taken),target_value)!=std::end(taken))
			target_value = target_value==0?list.size()-1:(target_value-1);
		
		list[taken[2]].next = list[target_value].next;
		list[target_value].next = taken[0];
		
		return list[original_value].next;
	};