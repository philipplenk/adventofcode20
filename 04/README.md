## Part 1

Given a list of key:value pairs, separated by empty lines, we were first asked to simply check for the presence of various fields. Quite similar to the first day, this was mostly an exercise in getting parsing somewhat right, which, frankly, annoyed me a little at the time. As on that day, I first simply defined a type for the data: 

```cpp
struct password_data
{
	std::unordered_map<std::string,std::string> data;
};
```	

followed by an overloaded operator>>:

```cpp
std::istream& operator>>(std::istream& in, password_data& pwd)
{
	pwd.data.clear();
	
	std::string line;
	std::stringstream strm;
	while(std::getline(in,line) && !line.empty())
	{
		for(auto c: line)
			strm<<(c==':'?' ':c);
		strm<<' ';
	}
	
	std::string name, data;
	while(strm>>name>>data)
		pwd.data[name]=data;
		
	if(pwd.data.empty())
		in.setstate(std::ios::failbit);
	else
		in.clear();
	
	return in;
}	
```

We then define what it means to be valid: 

```cpp
bool is_valid(const password_data& pwd)
{
	std::array<std::string,7> required{"byr","iyr","eyr","hgt","hcl","ecl","pid"};
	for(const auto& part: required)
		if(pwd.data.find(part)==std::end(pwd.data))
			return false;
	return true;
}
```

and let count_if do all the work:

```cpp
std::cout<<std::count_if(std::istream_iterator<password_data>{std::cin},{},is_valid)<<'\n';
```

The different maps were separated by an empty line in between them, which made it rather tedious (and likely a bad idea) to parse as I did by overloading operator>> on std::istream. I also - as usual - committed another ridiculously stupid mistake that cost me a good 5 minutes: 

Note how I read lines until either std::getline fails or returns an empty line. That is because there are actually two ways a collection of related key-value pairs is terminated: With an empty line OR with the end of input. The problem occurs in the latter case. If std::getline fails, it sets, appropriately, the std::ios::failbit on the underlying stream. If I do not clear it, however, this indicates to the istream_iterator that extraction failed and the last entry will not be accepted.

## Part 2

Now, in addition to merely checking for their presence, we were tasked with checking each fields validity based on certain criteria. Again, not too difficult, just quite tedious and easy to get wrong, yielding somewhat unsightly code by necessity. Originally, I had a gigantic if-else block and repeated code, just to get it done as quickly as possible(faaar too slow for the leaderboard, of course), but looking at that code was the sort of torture I could not possibly subject my dear readers to, so I went kind of overboard and constructed a bunch of composable function objects that allowed me to define the criteria in an almost declarative style:

```cpp
const std::vector<std::pair<std::string_view,std::function<bool(std::string_view)>>> field_validator
{
	{"byr", all(is_length(4),is_number,chain(as_number,is_between(1920,2002)))},
		
	{"iyr", all(is_length(4),is_number,chain(as_number,is_between(2010,2020)))},
		
	{"eyr", all(is_length(4),is_number,chain(as_number,is_between(2020,2030)))},
		
	{"hgt", one_of
		(
		all(ends_with("cm"),chain(remove_suffix(2),is_number),chain(remove_suffix(2),as_number,is_between(150,193))),
			all(ends_with("in"),chain(remove_suffix(2),is_number),chain(remove_suffix(2),as_number,is_between(59,76)))
		)
	},
		
	{"ecl", is_one_of("amb","blu","brn","gry","grn","hzl","oth")},
	
	{"pid", all(is_length(9),is_number)},
	
	{"hcl", all(starts_with("#"), chain(remove_prefix(1),is_hexnumber))},
};
```

The business logic ones are rather trivial and uninteresting. The ones used for composition can, however, prove educational:

```cpp
constexpr auto all = [](auto... checkers) { return [=](const auto& str) { return (checkers(str) && ...); }; };
	
constexpr auto one_of = [](auto... checkers) { return [=](const auto& str) { return (checkers(str) || ...); }; };
	
constexpr auto chain = [](auto fun, auto... funs)
{
	const auto chain_impl = [](auto recurse, auto fun, auto... funs)
	{
		if constexpr(sizeof...(funs)==0)
			return fun;
		else
			return [=](const auto& arg) { return recurse(recurse, funs...)(fun(arg)); };
	};
	
	return chain_impl(chain_impl,fun,funs...);
};
```

*all* simply takes a bunch of callables and returns a new callable that utilizes [C++17's fold expressions](https://en.cppreference.com/w/cpp/language/fold) to call each one of the given checkers in a chain connected via &&, i.e. returning true exactly if all of the given checkers would return true.

*one_of* does pretty much the same thing, just with ||, i.e. or, instead of and.

*chain*, as the other two, takes a collection of callables and returns another callable. This one, however, is constructed in a little more complicated fashion. First, we call a helper *chain_impl*. This is necessary to get recursion, which is otherwise not easily available in lambda expressions. The helper does one of two things:
- If there is only one callable left, it is returned.
- Otherwise, we construct a new callable that calls the first one and passes the result on to whatever the recursion will construct for all other functions.

Effectively, given functions a, b, c and d, this will construct an expression equivalent to d(c(b(a(input)))), i.e. it chains the calls, using the result of the first as input to the second and so on(similar to the use of [pipes](https://en.wikipedia.org/wiki/Pipeline_(Unix)) in Unix, a | b | c | d).