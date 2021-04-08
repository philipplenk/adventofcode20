#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

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

std::ostream& operator<<(std::ostream& out, const rule& r)
{
	return out<<r.name<<": "<<r.lower.start<<"-"<<r.lower.end<<" or "<<r.higher.start<<'-'<<r.higher.end<<'\n';
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
	std::getline(std::cin,line);
	std::getline(std::cin,line);
	std::getline(std::cin,line);
	
	std::vector<ticket_data> tickets(std::istream_iterator<ticket_data>{std::cin},{});
	
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
	
	std::cout<<error_rate<<'\n';
	
	return 0;
}
