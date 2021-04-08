#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>

#include <cstddef>

struct password_data
{
	std::unordered_map<std::string,std::string> data;
};

std::istream& operator>>(std::istream& in, password_data& pwd)
{
	pwd.data.clear();
	
	std::string line;
	std::stringstream strm;
	while(std::getline(in,line) && !line.empty())
	{
		for(auto c: line)
			strm<<(c==':'?' ':c);
		strm<<' ';
	}
	
	std::string name, data;
	while(strm>>name>>data)
		pwd.data[name]=data;
		
	if(pwd.data.empty())
		in.setstate(std::ios::failbit);
	else
		in.clear();
	
	return in;
}

bool is_valid(const password_data& pwd)
{
	std::array<std::string,7> required{"byr","iyr","eyr","hgt","hcl","ecl","pid"/*,"cid"*/};
	for(const auto& part: required)
		if(pwd.data.find(part)==std::end(pwd.data))
			return false;
	return true;
}

int main(int argc, char* argv[])
{
	std::cout<<std::count_if(std::istream_iterator<password_data>{std::cin},{},is_valid)<<'\n';
	return 0;
}
