#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>
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
	std::vector dishes(std::istream_iterator<dish>{std::cin},{});
	
	std::vector<std::string> all_ingredients;
	
	for(const auto& d: dishes)
		all_ingredients.insert(std::end(all_ingredients),std::begin(d.ingredients),std::end(d.ingredients));
		
	std::sort(std::begin(all_ingredients),std::end(all_ingredients));
	auto last = std::unique(std::begin(all_ingredients),std::end(all_ingredients));
	all_ingredients.erase(last, std::end(all_ingredients));
	
	
	std::map<std::string,std::vector<std::string>> allergen_to_ingredient;
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
	
	std::size_t done_cnt = 0;
	do
	{
		done_cnt = 0;
		for(const auto& [a,ing] :allergen_to_ingredient)
		{
			if(ing.size()==1)
			{
				++done_cnt;
				for(auto& [a_other,ing_other] :allergen_to_ingredient)
				{
					if(a_other==a || ing_other.size()==1) continue;
					std::vector<std::string> corrected;
					std::set_difference(std::begin(ing_other),std::end(ing_other),std::begin(ing),std::end(ing),std::back_inserter(corrected));
					ing_other = std::move(corrected);
				}
			}
		}
	}
	while(done_cnt<allergen_to_ingredient.size());
	

	for(const auto& [a,ing] :allergen_to_ingredient)
	{
		for(auto i:ing)
			std::cout<<i<<',';
	}
	std::cout<<'\n';
	
	return 0;
}
