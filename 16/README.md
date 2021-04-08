## Part 1

More data validation, yeahy! *sigh*

We have a bunch of tickets with a number of fields, all identified in a language we do not understand. Luckily for us, we also know which fields have to follow which rules, so we might have a shot at ordering them. But first, let's make sure our data is valid by identifying fields that cannot be valid for any rule and count them.

Rules are given in the form

	class: 1-3 or 5-7

which implies all entries of the "class" type have to be between 1 and 3(inclusive) or 5 and 7. It's always exactly two non-overlapping ranges and the second always starts after the first. So here is how I represented and parsed the rulesets:

```cpp
struct inclusive_range
{
	int start, end;
};

struct rule
{
	std::string name;
	inclusive_range lower, higher;
};

std::istream& operator>>(std::istream& in, rule& r)
{
	r.name.clear();
	
	std::string part;
	while(in>>part && part.back()!=':')
	{
		r.name+=part+' ';
	}
	r.name+=part;
	r.name.pop_back();
	
	char skip_c;
	in>>r.lower.start>>skip_c>>r.lower.end;
	in>>part;
	in>>r.higher.start>>skip_c>>r.higher.end;
	
	return in;
}
```	

With the parsing out of the way, as not exactly infrequently, the real task is just a form of counting:

```cpp
const auto error_rate = std::accumulate(std::begin(tickets),std::end(tickets),0,[&](const auto total, const auto t)
	{
		int sum = 0;
		for(auto e:t.entries)
		{
			bool valid = false;
			for(const auto& r:rules)
			{
				if(e>=r.lower.start && e<=r.lower.end)
					valid = true;
					
				if(e>=r.higher.start && e<=r.higher.end)
					valid = true;
			}
			if(!valid)
				sum+=e;
		}
		
		return total + sum;
	});
```

## Part 2

Alright, now we get to the ever so slightly more interesting part: Identifying which column belongs to which field. The simplest way to do this is checking all options. The somewhat more clever one I could think of is to determine the first rule by finding one that can only be valid for one column. Afterwards, rinse and repeat:

First, we setup variables to keep track of which rules and fields we have already identified:

```cpp
std::vector<std::optional<std::size_t>> found_target_entry;
found_target_entry.resize(rules.size()); 

std::vector<std::optional<std::size_t>> found_target_rule;
found_target_rule.resize(rules.size()); 

std::size_t rules_found_count = 0;
```

Determining if one and only one previously unassigned entry fits a given rule is done via this lovely lambda:

```cpp
const auto determine_unique_entry_for = [&](int rule_id)
{
	std::optional<std::size_t> entry_n;
	for(std::size_t j=0;j<tickets[0].entries.size();++j)
	{
		if(found_target_rule[j])
			continue;
				
		const auto valid = std::all_of(std::begin(tickets),std::end(tickets),[&](const auto& t)
		{
			return is_valid(t.entries[j],rules[rule_id]);
		});
				
		if(valid)
		{
			if(entry_n)
			{
				entry_n = std::nullopt;
				break;
			}
			else
				entry_n = j;
		}
	}
	
	return entry_n;
};
```

It checks all, skipping over all assigned ones and checks validity. If no option or more than one valid option is found, we return nullopt, otherwise we return the index.

We then repeat it for all rules until everything becomes clear:

```cpp
while(rules_found_count<rules.size())
{
	for(std::size_t i=0;i<rules.size();++i)
	{
		if(!found_target_entry[i])
		{
			if(auto entry_n = determine_unique_entry_for(i); entry_n)
			{
				found_target_entry[i] = *entry_n;
				found_target_rule[*entry_n] = i;
				++rules_found_count;
				break;
			}
		}
	}
}
```

And that's it for the day. It would - of course - fail spectacularly if more than one valid solution exited ;-)

Whilst I am not generally too big a fan of the validation type puzzles - as they really mostly test if you are capable of correctly reading instructions and data - the second part today was amazing and a lot of fun!