#include <iostream>
#include <deque>
#include <string>

int main(int argc, char* argv[])
{
	std::deque<int> player0, player1;
	
	std::string line;
	std::getline(std::cin,line);
	
	while(std::getline(std::cin,line) && !line.empty())
		player0.push_front(std::stoi(line));
		
	std::getline(std::cin,line);
	
	while(std::getline(std::cin,line) && !line.empty())
		player1.push_front(std::stoi(line));
		
	while(!player0.empty() && !player1.empty())
	{
		if(player0.back()>player1.back())
		{
			auto other = player1.back();
			player0.push_front(player0.back());
			player0.push_front(other);
			player0.pop_back();
			player1.pop_back();
		}
		else
		{
			auto other = player0.back();
			player1.push_front(player1.back());
			player1.push_front(other);
			player0.pop_back();
			player1.pop_back();
		}
	}
	
	const auto& winner = player0.empty()?player1:player0;
	
	std::size_t score = 0;
	std::size_t factor = 1;
	for(auto c: winner)
	{
		score+=(factor++)*c;
	}
	
	std::cout<<score<<'\n';
	
	return 0;
}
