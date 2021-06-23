#include <iostream>
#include "mtga.hpp"

$$ $;

int main()
{
	auto asc = $.iota<5, 5>();
	auto desc = $.iota<5, -10>();
	
	std::cout << "asc: ";
	$.for_each(asc, [](auto x) { std::cout << x << ' '; });
	std::cout << std::endl;
	
	std::cout << "desc: ";
	$.for_each(desc, [](auto x) { std::cout << x << ' '; });
	std::cout << std::endl;
	
	auto tup = $(1, 2, 3, 4, 5);
	$.for_each($.map(tup, [](auto x) { return 2*x; }),
	           [](auto x) { std::cout << x << std::endl; });
	
	return 0;
}
