#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
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

struct edge
{
	std::string target;
	std::size_t weight;
};

std::size_t number_of_interior_bags(const std::string& outer_name, const std::unordered_map<std::string,std::vector<edge>>& graph)
{
	std::size_t count = 1;
	auto it = graph.find(outer_name);
	if(it==std::end(graph))
		return count;
		
	for(const auto& [name,n]: it->second)
		count+=n*number_of_interior_bags(name,graph);
	
	return count;
}

int main(int argc, char* argv[])
{
	std::unordered_map<std::string,std::vector<edge>> graph;
	rule r;
	while(std::cin>>r)
	{
		for(auto [name,n]:r.can_contain_n)
			graph[r.own_name].push_back({name,n});
	}
	
	std::cout<<number_of_interior_bags("shiny gold",graph)-1<<'\n';
	
	return 0;
}
