#include <algorithm>
#include <charconv>
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>

#include <cstddef>

struct passport_data
{
	std::unordered_map<std::string,std::string> data;
};

std::istream& operator>>(std::istream& in, passport_data& pwd)
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

constexpr auto all = [](auto... checkers) { return [=](const auto& str) { return (checkers(str) && ...); }; };
constexpr auto one_of = [](auto... checkers) { return [=](const auto& str) { return (checkers(str) || ...); }; };

constexpr auto chain = [](auto fun, auto... funs)
{
	const auto chain_impl = [](auto recurse, auto fun, auto... funs)
	{
		if constexpr(sizeof...(funs)==0)
			return fun;
		else
			return [=](const auto& arg) { return recurse(recurse, funs...)(fun(arg)); };
	};
	
	return chain_impl(chain_impl,fun,funs...);
};

constexpr auto is_length = [](std::size_t length) { return [=](const auto& str) { return str.size()==length; }; };

constexpr auto is_number=[](const auto& str)
{
	return std::all_of(std::begin(str),std::end(str),[](auto c){ return std::isdigit(c); });
};

constexpr auto is_hexnumber = [](const auto& str)
{
	return std::all_of(std::begin(str),std::end(str),[](auto c){ return std::isxdigit(c); });
};

constexpr auto is_one_of = [](auto... options)
{
	return [=](const auto& str)
	{
		return ((str==options) || ...);
	};
};

constexpr auto as_number= [](const auto& str)
{
	int n = 0;
	std::from_chars(str.data(),str.data()+str.size(),n);
	return n;
};

constexpr auto is_between= [](auto lower, auto higher)
{
	return [=](const auto& n)
	{
		return n>=lower && n<=higher;
	};
};

constexpr auto ends_with = [](std::string_view suffix)
{
	return [=](const auto& str)
	{
		return str.size()>=suffix.size() && str.compare(str.size()-suffix.size(),str.npos,suffix)==0;
	};
};

constexpr auto starts_with = [](std::string_view prefix)
{
	return [=](std::string_view str)
	{
		return str.substr(0, prefix.size())==prefix;
	};
};

constexpr auto remove_suffix = [](std::size_t suffix_length)
{
	return [=](std::string_view str)
	{
		str.remove_suffix(suffix_length);
		return str;
	};
};

constexpr auto remove_prefix = [](std::size_t prefix_length)
{
	return [=](std::string_view str)
	{
		str.remove_prefix(prefix_length);
		return str;
	};
};

const std::vector<std::pair<std::string_view,std::function<bool(std::string_view)>>> field_validator
{
	{"byr", all(is_length(4),is_number,chain(as_number,is_between(1920,2002)))},
		
	{"iyr", all(is_length(4),is_number,chain(as_number,is_between(2010,2020)))},
		
	{"eyr", all(is_length(4),is_number,chain(as_number,is_between(2020,2030)))},
		
	{"hgt", one_of
		(
			all(ends_with("cm"),chain(remove_suffix(2),is_number),chain(remove_suffix(2),as_number,is_between(150,193))),
			all(ends_with("in"),chain(remove_suffix(2),is_number),chain(remove_suffix(2),as_number,is_between(59,76)))
		)
	},
	
	{"ecl", is_one_of("amb","blu","brn","gry","grn","hzl","oth")},
	
	{"pid", all(is_length(9),is_number)},
	
	{"hcl", all(starts_with("#"), chain(remove_prefix(1),is_hexnumber))},
};

bool is_present_and_valid(const passport_data& pwd)
{
	for(const auto& [part,validate]: field_validator)
		if(auto it=pwd.data.find(std::string{part});it==std::end(pwd.data) || !validate(it->second))
			return false;
		
	return true;
}

int main(int argc, char* argv[])
{
	std::cout<<std::count_if(std::istream_iterator<passport_data>{std::cin},{},is_present_and_valid)<<'\n';
	return 0;
}
