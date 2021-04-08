# Part 1

Space Cards! Almost missed them from last year, when they proved [one of the most interesting challenges](https://adventofcode.com/2019/day/22), due to their sheer number alone. This year, however, we are far more relaxed. Hardly any shuffling involved, just a [simple game between two players](https://en.wikipedia.org/wiki/War_(card_game)).

Both are given a deck of numbered cards and repeatedly and simultaneously draw and reveal their top card. The owner of the higher valued one wins the lower one and puts both cards below their own deck. Once a deck is cleared, the other player is declared the winner. Rather straightforward so far:

```cpp
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
```

If we only wanted to know who wins, it would be enough to check who owns the highest card in the beginning. The required result, however, was a score computed based on the order in the final deck and I couldn't think of any better solution than a simple simulation.

# Part 2

Part 2, as usual, is where things get interesting. Whenever the two top numbers are smaller than what is left of the respective decks, we recurse on a copy of the top that many cards to determine the winner. If a position repeats, player 0 wins by default.

That rule was a bit ambiguous and hard to grasp. What player 0 wins is that single subgame. Not the current round and not the whole game. I lost about 10 minutes on misunderstanding that, as I thought it referred to the round only and got an error I failed to understand. The given example input does not trigger that specific case and erroneous behavior, yielding the correct result even with the misunderstanding. Well, the rest was fairly easy with the difficulty coming mainly from more or less efficiently implementing the game rules, which I notably did not do.

I am certain it could be optimized significantly, as my solution takes about half a second on my laptop, which is huge. I am, however, lazy, so I declared it adequate.

The main game logic is pretty much equivalent to the one from part 1. I just cleaned it up a little bit and used a call to a more complicated function to determine who wins the next card instead of simply comparing the top ones:

```cpp
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
```

I also kept a set of already appeared deck combinations, to implement the "player 1 wins on repetition" rule. *next\_card\_winner* is equally trivial:

```cpp
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
```

If there are enough cards in both decks, we recurse into a subgame and return the winner of that. Otherwise, the higher card wins.