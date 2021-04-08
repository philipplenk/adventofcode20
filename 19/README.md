## Part 1

More parsing fun, but this time the difficulty is not so much in parsing but in writing a proper parser. We are given a bunch of simple replacement rules and are supposed to tell how many of the input strings match the rules. At first, they are non-recursive and provided in a simple form: 

0: 1 2
1: "a"
2: 1 3 | 3 1
3: "b"

This is basically a very simple [grammar](https://en.wikipedia.org/wiki/Formal_grammar), with numbers as nonterminals and characters as terminals. I chose to represent the production rules as a simple struct:

	enum class type
	{
		terminal,
		nonterminal
	};
	
	struct rule
	{
		type t;
		char c;
		std::vector<std::vector<long>> sequences;
	};

If type is terminal, the char matters, otherwise the vector of alternative sequences is relevant. Given that exclusivity, I would have done better to use a [std::variant](https://en.cppreference.com/w/cpp/utility/variant) instead, but I guess I was feeling particularly lazy that day. 

The parser itself is, as so frequently, a simple recursive function with this signature:

	[&](std::string_view str, long rule_id, std::deque<long> follow, auto recurse) -> bool

"str" is what remains of the input, "rule_id" identifies the first rule we are trying to match, whilst "follow" is a list of rules we have to match after that first one.

Matching differentiates based on the next rule type. If it is a terminal, i.e. a single character, we simply compare the next input character:

	if(!str.empty() && str[0]==r.c)
	{
		str.remove_prefix(1);
		if(follow.empty())
			return str.empty();
				
		auto next = follow.front();
		follow.pop_front();
		return recurse(str,next,follow,recurse);
	}
	else
		return false;

If it does not match, we failed. If it succeeded and we have no more rules to match, we succeed exactly if we consumed all of our input. Otherwise, we recursively try to match the next rule in our sequence.

If, on the contrary, we are dealing with a nonterminal, matching is a little more complicated, as we have to try all options:
		
	for(const auto& seq: r.sequences)
	{
		auto follow_cpy = follow;
		follow_cpy.insert(std::begin(follow_cpy),std::begin(seq),std::end(seq));
		auto next = follow_cpy.front();
		follow_cpy.pop_front();
		if(recurse(str,next,follow_cpy,recurse))
			return true;
	}
		
	return false;

For each of our possible sequences, we first create a copy of what has to follow so far and prepend that with the sequence we are currently testing. Then we proceed as we did in the terminal case above, by taking off the first of that sequence and recursing. If any one of the recursive calls succeeded, we propagate success. Otherwise, we propagate failure.

## Part 2

For part 2, we were supposed to replace two rules, thereby introducing right recursion. We were warned - in not too weak terms - that we should only try to handle our special case and a general solution would be significantly more difficult. I was stumped for several minutes trying to figure out what such a special cased solution would look like, until I decided to try and see what my solution would do with the modified input. Turns out it seems to have been perfectly general already and spewed out the correct answer within the fraction of a second. Wasted minutes and leaderboard opportunities... Oh well, next year is another chance xD 
