#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include <cstddef>

struct dish
{
	std::vector<std::string> ingredients;
	std::vector<std::string> allergens;
};

std::istream& operator>>(std::istream& in, dish& d)
{
	d.ingredients.clear();
	d.allergens.clear();
	
	std::string line;
	if(!std::getline(in,line))
		return in;
	
	std::stringstream strm;
	strm<<line;
	
	strm>>std::ws;
	
	while(strm && strm.peek()!='(')
	{
		std::string next;
		strm>>next>>std::ws;
		d.ingredients.push_back(next);
	}
	
	strm>>line; //throw away contains
	
	while(strm>>line)
	{
		auto last = line.back();
		line.pop_back();
		d.allergens.push_back(line);
		if(last==')')
			break;
	}
	
	std::sort(std::begin(d.ingredients),std::end(d.ingredients));
	std::sort(std::begin(d.allergens),std::end(d.allergens));
	
	return in;
}

int main(int argc, char* argv[])
{
	std::vector<dish> dishes(std::istream_iterator<dish>{std::cin},{});
	
	std::vector<std::string> all_ingredients;
	
	for(const auto& d: dishes)
		all_ingredients.insert(std::end(all_ingredients),std::begin(d.ingredients),std::end(d.ingredients));
		
	std::sort(std::begin(all_ingredients),std::end(all_ingredients));
	auto last = std::unique(all_ingredients.begin(),all_ingredients.end());
	all_ingredients.erase(last, all_ingredients.end());
	
	
	std::unordered_map<std::string,std::vector<std::string>> allergen_to_ingredient;
	for(const auto& d: dishes)
		for(const auto& a: d.allergens)
			allergen_to_ingredient[a] = all_ingredients;
	
	for(const auto& d: dishes)
	{
		for(const auto& a: d.allergens)
		{
			auto& old = allergen_to_ingredient[a];
			std::vector<std::string> corrected;
			std::set_intersection(std::begin(old),std::end(old),std::begin(d.ingredients),std::end(d.ingredients),std::back_inserter(corrected));
			old = std::move(corrected);
		}
	}
	
	std::vector<std::string> harmless = all_ingredients;
	for(const auto& [a,ing] :allergen_to_ingredient)
	{
		std::vector<std::string> corrected;
		std::set_difference(std::begin(harmless),std::end(harmless),std::begin(ing),std::end(ing),std::back_inserter(corrected));
		harmless = std::move(corrected);
	}
	
	for(auto i: harmless)
		std::cout<<i<<' ';
	std::cout<<'\n';
	
	std::size_t count = 0;
	for(const auto& d: dishes)
	{
		for(const auto& i: d.ingredients)
			if(std::find(std::begin(harmless),std::end(harmless),i)!=std::end(harmless))
				++count;
	}
	
	std::cout<<count<<'\n';
	
	return 0;
}
