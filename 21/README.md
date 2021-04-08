## Part 1

Given a list of unknown ingredients and a list of allergens contained in them (each allergen appearing in exactly one ingredient, each ingredient containing at most one allergen and not all allergens listed every time), we are tasked with figuring out which ingredient contains which allergen. Well, first of all we are asked a simpler question: which ingredients do definitely not contain any allergen.
It took me a while to understand what exactly was asked here, but it all comes down to set intersections and differences. Whenever an allergen is contained in two lists, it must be contained in the intersection of those lists, not in the difference. Thanks to the beauty of the C++ standard library, this simple insight leads to a very straightforward solution using simple calls to std::set_intersection and std::set_difference:


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


## Part 2

Having solved this simple question, we now have to uniquely identify which ingredient contains which allergen. Given our previous solution and knowing it is uniquely solvable, this is rather trivial. First, identify one allergen that can only be in one ingredient. Remove it as a possibility from all other allergens. Rinse and repeat:


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
