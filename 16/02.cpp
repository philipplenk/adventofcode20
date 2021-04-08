#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include <cstddef>

struct inclusive_range
{
	int start, end;
};

struct rule
{
	std::string name;
	inclusive_range lower, higher;
};

bool is_valid(int n, const rule& r)
{
	return (n>=r.lower.start && n<=r.lower.end) || (n>=r.higher.start && n<=r.higher.end);
}

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

struct ticket_data
{
	std::vector<int> entries;
};

std::istream& operator>>(std::istream& in, ticket_data& t)
{
	t.entries.clear();
	std::string line;
	std::getline(in,line);
	
	std::stringstream strm;
	strm<<line;
	
	int entry;
	while(strm>>entry)
	{
		char skip_c;
		strm>>skip_c;
		
		t.entries.push_back(entry);
	}
	
	return in;
}

int main(int argc, char* argv[])
{
	std::vector<rule> rules;
	
	std::string line;
	while(std::getline(std::cin,line) && !line.empty())
	{
		std::stringstream strm;
		strm<<line;
		rule r;
		strm>>r;
		rules.push_back(r);
	}
	
	std::getline(std::cin,line);
	ticket_data own_ticket;
	std::cin>>own_ticket;
	
	std::getline(std::cin,line);
	std::getline(std::cin,line);
	
	const auto is_invalid = [&](const auto& t)
	{
		const auto valid_for_some = [&](auto n)
		{
			return std::any_of(std::begin(rules),std::end(rules),[&](const auto& r)
			{
				return is_valid(n,r);
			});
		};
		
		return !std::all_of(std::begin(t.entries),std::end(t.entries),valid_for_some);
	};
	
	std::vector<ticket_data> tickets;
	std::for_each(std::istream_iterator<ticket_data>{std::cin},{},[&](auto t)
	{
		if(!is_invalid(t))
			tickets.push_back(t);
	});
	
	std::vector<std::optional<std::size_t>> found_target_entry;
	found_target_entry.resize(rules.size()); 
	
	std::vector<std::optional<std::size_t>> found_target_rule;
	found_target_rule.resize(rules.size()); 
	
	std::size_t rules_found_count = 0;
	
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
	
	std::size_t product = 1;
	for(std::size_t rn = 0; rn<rules.size();++rn)
	{
		if(rules[rn].name.substr(0,9)=="departure")
		{
			product*=own_ticket.entries[*found_target_entry[rn]];
			
		}
	}
	std::cout<<product<<'\n';
	
	return 0;
}
