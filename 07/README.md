## Part 1

Finally a graph problem! Given a number of bags, all of which containing a number of different bags, first count all that will, in one of those interior ones, contain a "shiny gold" one. Before tackling the more interesting part here, we unfortunately do have to get some annoying parsing out of the way, which - as so frequently - took the bulk of my required time.

Which bags contain how many of which other bags was given as a string of this form:

	vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.

Or sometimes:

	faded blue bags contain no other bags.

It ought to be trivial, but I am embarrassed to say that this years problems really drove home the point that my skills in the parsing area are severely lacking and I have some serious brushing up to do. Some higher proficiency with regular expressions, for instance, might have tremendously simplified the following abomination I came up with at the time:

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
	

Don't look at it too closely. Great! Now that we have this, let's return to the root of our problem. Or rather the reverse. Interpreting the "contains" relation as edges in a graph, we can have one node for each bag type and an edge to all other bags in which it might be contained:

For instance, this description:

	green bags contain 2 red bags, 2 black bags.
	red bags contain 1 blue bag.
	blue bags contain 1 shiny gold bag, 3 black bags.
	black bags contain no other bags.
	shiny gold bags contain 7 gold bags.

would yield the following:

	"red" <--------- "blue" <----- "shiny gold" <----- "gold"
	  |                ^ 
	  v                |
	"green"<--"black"__|
	

Thanks to the eldritch horror above, such a reversed graph can be constructed easily, representing it as a map from *node-name* to *list-of-children-names*:

	std::unordered_map<std::string,std::vector<std::string>> reverse_graph;
	rule r;
	while(std::cin>>r)
	{
		for(auto [name,n]:r.can_contain_n)
			reverse_graph[name].push_back(r.own_name);
	}

Given that, all we need to do is walk each possible path away from our starting node ("shiny gold") and count the total number of nodes we pass by(remembering which we have already seen and counted on the way):

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

## Part 2

Wonderful! Now for part 2 we were tasked with doing basically the reverse. Instead of counting how many different kinds of bags can contain a shiny gold one, we are asked how many bags are contained within. This requires us to consider one more piece of information given in our data, the number telling us how often each kind of bag is contained within. Reversing the edges from part 1 and labeling them with a "cost" representing their quantity yields a graph like this one:

	"red" ----1----> "blue" --1--> "shiny gold" ---7--> "gold"
	  ^                  | 
	  2                  3
	  |                  |
	"green"-2->"black"<__|

Which we will, once again, simply walk recursively from our starting location, multiplying the results by the given quantities: 

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
