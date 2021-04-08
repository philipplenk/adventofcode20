
## Part 1

Given a number of "yes-or-no-questions" - each identified by a single character - we were provided with a list of "answers", i.e. lines containing a character if the corresponding question was answered in the affirmative and not containing it otherwise.

Those answers were separated into groups by empty lines and our first task was to count in how many groups each "question"(i.e. character) was present. We then should return the sum of those counts.

Again - as seems to be a theme in those early days - the most difficult or rather time consuming part here is parsing question and input correctly, with the actual task being simple counting. Here is how I did it:

```cpp
std::array<std::size_t,256> counts{};
const auto begin = &counts['a'], end = &counts['z'+1];
	
const auto finish_group = [&]()
{		
	total+=std::count_if(begin,end,[&](auto v){ return v!=0; });
	std::fill(begin,end,0);
};
	
while(std::getline(std::cin,line))
{
	if(line.empty())
		finish_group();
	else
	{
		for(auto c: line)
			++counts[c];
	}
}
finish_group();
```

Nothing much to explain here. I simply read the input, line by line, and count the frequency of all characters in an array indexed by the characters value - just like one would do when implementing [counting sort](https://en.wikipedia.org/wiki/Counting_sort). After an empty line, I finalize the group by counting how many entries were not 0 - that means they appeared at least once - and reset the counts.

Using an array to count is, of course, a bit of a waste of space, but I deemed it acceptable as the resulting code is succinct, readable and efficient. It also turned out to be useful for part 2.

## Part 2

The second part did not, in fact, significantly ramp up the difficulty. Instead of counting how often each answer was given in total, we should only count those answers in a group which were given by everyone in it. As I already counted the frequency, all that was left was determining the group size:

```cpp
	++group_size;
	for(auto c: line)
		++counts[c];
```

Followed by simply changing a "!=0" to a "==group_size":

```cpp
const auto finish_group = [&]()
{		
	total+=std::count(begin,end,group_size);
	group_size=0;
	std::fill(begin,end,0);
};
```

It might have broken if anyone had enthusiastically answered yes more than once on the same question, which, to my great relief, was not the case.