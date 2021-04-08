#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <cstddef>

struct rule
{
	std::string own_name;
	std::unordered_map<std::string,std::size_t> can_contain_n;
};

std::istream& operator>>(std::istream& in, rule& r)
{
	r.own_name.clear();
	r.can_contain_n.clear();
	
	std::string str, last;
	while(in>>str && str!="contain")
	{
		r.own_name+=last;
		last = str+" ";
	}
	r.own_name.pop_back();
	
	if(!in)
		return in;
	
	do
	{
		std::size_t n;
		std::string name;
		std::string n_str;
		in>>n_str;
		if(n_str=="no")
		{
			in>>str>>str>>str;
			break;
		}
		
		n=std::stoi(n_str);
		while(in>>str && str.back()!=',' && str.back()!='.')
		{
			name+=str+" ";
		}
		name.pop_back();
		r.can_contain_n[name]=n;
	}
	while(str.back()!='.');
	
	return in;
}

int main(int argc, char* argv[])
{
	std::unordered_map<std::string,std::vector<std::string>> reverse_graph;
	rule r;
	while(std::cin>>r)
	{
		for(auto [name,n]:r.can_contain_n)
			reverse_graph[name].push_back(r.own_name);
	}
	
	std::vector<std::string> left;
	std::unordered_set<std::string> seen;
	left.push_back("shiny gold");
	
	std::size_t count = 0;
	while(!left.empty())
	{
		const auto next = left.back();
		
		left.pop_back(); 
		for(const auto& target: reverse_graph[next])
		{
			if(seen.find(target)==std::end(seen))
			{
				seen.insert(target);
				left.push_back(target);
				++count;
			}
		}
	}
	std::cout<<count<<'\n';
	
	return 0;
}
