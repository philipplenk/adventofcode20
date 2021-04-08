## Part 1

The second day was algorithmically far easier(and imho less interesting) than the first one, as the only difficulty here was avoiding typos and parsing the input. You got a list of words and *policies*, which described how to determine if a given word was valid: 

1-3 p: word

Count the occurrences of the given letter(p) in the given word(word) and check it is present between the lower(1) and higher(3) bound times. We then had to count the number of valid words, i.e. words that satisfied their own policy.

Really, this was mostly just a typing speed challenge, with standard algorithms doing the bulk of the work. I simply defined a struct describing one such rule/word pair: 

```cpp
struct policy_word
{
	char letter;
	int min, max;
	std::string str;
};
```

Followed by overloading operator<<(lazily, without any error handling), so I could extract properly typed input from stdin:
	
```cpp
std::istream& operator>>(std::istream& in, policy_word& p)
{
	char dummy;
	in>>p.min;
	in>>dummy;
	in>>p.max;
	in>>p.letter;
	in>>dummy;
	in>>p.str;
	
	return in;
}
```

That allowed me to conveniently combine istream_iterator and std::count_if to solve the actual problem: 

```cpp
std::cout<<std::count_if(std::istream_iterator<policy_word>{std::cin},{},[](auto pw)
{
	const auto cnt=std::count(std::begin(pw.str),std::end(pw.str),pw.letter);
	return cnt>=pw.min && cnt<=pw.max;
})<<'\n';
```

## Part 2

Part 2 was even more trivial, as the interpretation of the policies was changed. Now a valid word has to have the given letter at exactly one of the given positions. Simply change the validity checking lambda and you get the following code:

```cpp
std::cout<<std::count_if(std::istream_iterator<policy_word>{std::cin},{},[](auto pw)
{
	return pw.str[pw.min-1]!=pw.str[pw.max-1] && (pw.str[pw.min-1]==pw.letter || pw.str[pw.max-1]==pw.letter);
})<<'\n';
```

The only potential issue, and obviously one I stumbled upon and which cost me one minute because of an incorrect answer is that position - of course - starts at 1, not 0 here.
