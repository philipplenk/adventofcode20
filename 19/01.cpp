#include <algorithm>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

enum class type
{
	single,
	sequence
};

struct rule
{
	type t;
	char c;
	std::vector<std::vector<long>> sequences;
};

std::istream& operator>>(std::istream& in, rule& r)
{
	r.sequences.clear();
	in>>std::ws;
	if(in.peek()=='"')
	{
		r.t = type::single;
		in.get();
		in>>r.c;
		in.get();
	}
	else
	{
		r.t = type::sequence;
		r.sequences.push_back({});
		do
		{
			if(in.peek()=='|')
			{
				r.sequences.push_back({});
				in.get();
				in>>std::ws;
			}
			else
			{
				long next;
				if(in>>next>>std::ws)
					r.sequences.back().push_back(next);
			}
		}
		while(in);
	}
	return in;
}


int main(int argc, char* argv[])
{
	std::string line;
	
	std::unordered_map<long,rule> rules;
	while(std::getline(std::cin,line) && !line.empty())
	{
		std::stringstream strm;
		strm<<line;
		
		char skip_c;
		long id;
		rule r;
		strm>>id>>skip_c>>r;
		rules[id]=r;
	}
	
	const auto matches = [&](std::string_view str, long rule_id, std::deque<long> follow, auto recurse) -> bool
	{
		const auto& r = rules[rule_id];
		if(r.t==type::single)
		{
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
		}
		
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
	};
	
	std::size_t count = 0;
	while(std::getline(std::cin,line) && !line.empty())
	{
		std::cout<<"Line is "<<line<<'\n';
		if(matches(line,0, {}, matches))
			++count;
	}
	std::cout<<count<<'\n';
	
	return 0;
}
