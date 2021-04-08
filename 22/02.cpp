#include <algorithm>
#include <array>
#include <iostream>
#include <deque>
#include <map>
#include <set>
#include <string>
#include <vector>

#include <cstddef>

int main(int argc, char* argv[])
{
	std::array<std::deque<int>,2> decks;
	
	std::string line;
	std::getline(std::cin,line);
	
	while(std::getline(std::cin,line) && !line.empty())
		decks[0].push_front(std::stoi(line));
		
	std::getline(std::cin,line);
	
	while(std::getline(std::cin,line) && !line.empty())
		decks[1].push_front(std::stoi(line));
		
	const auto compute_score = [](const auto& deck)
	{
		std::size_t score = 0;
		std::size_t factor = 1;
		for(auto c: deck)
			score+=(factor++)*c;
		
		return score;
	};
	
	const auto compute_id = [](const auto& deck)
	{
		std::size_t id = 0;
		std::size_t factor = 1;
		for(auto c: deck)
		{
			id+=factor*c;
			factor*=100;
		}
		
		return id;
	};
	
	struct ret_val_t
	{
		int winner_id;
		std::size_t winner_score;
	};
	
	const auto recursive_combat = [&](auto decks, auto recurse) -> ret_val_t
	{
		std::set<std::pair<std::size_t,std::size_t>> cache;
		
		const auto next_card_winner = [&]()
		{		
			if(decks[0].back()<static_cast<int>(decks[0].size()) && decks[1].back()<static_cast<int>(decks[1].size()))
			{
				auto decks_new = decks;
				decks_new[0].pop_back();
				decks_new[1].pop_back();
				
				decks_new[0].erase(std::begin(decks_new[0]),std::begin(decks_new[0])+decks_new[0].size()-decks[0].back());
				decks_new[1].erase(std::begin(decks_new[1]),std::begin(decks_new[1])+decks_new[1].size()-decks[1].back());
					
				auto result = recurse(std::move(decks_new),recurse);
				return result.winner_id;
			}	
			
			return decks[0].back()>decks[1].back()?0:1;
		};
		
	
		while(!decks[0].empty() && !decks[1].empty())
		{
			const std::pair cache_id = {compute_id(decks[0]),compute_id(decks[1])};
			if(cache.find(cache_id)!=cache.end())
				return ret_val_t{0,compute_score(decks[0])};
			
			cache.insert(cache_id);
					
			const auto winner = next_card_winner();
			const auto loser = winner==0?1:0;
			
			const auto other = decks[loser].back();
			decks[winner].push_front(decks[winner].back());
			decks[winner].push_front(other);
			decks[winner].pop_back();
			decks[loser].pop_back();			
		}
		
		return (decks[0].empty()?ret_val_t{1,compute_score(decks[1])}:ret_val_t{0,compute_score(decks[0])});
	};
	
	
	auto result = recursive_combat(decks,recursive_combat);
	std::cout<<result.winner_score<<'\n';
	
	return 0;
}
