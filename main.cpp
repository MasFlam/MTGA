#include <iostream>
#include <string>
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
	           [](auto x) { std::cout << x << ' '; });
	std::cout << std::endl;
	
	auto tup2 = $('b', 'c', 'd', 'e');
	std::cout <<
		'"' <<
		$.fold_left(
			tup2,
			'a',
			[](const auto& x, const auto& y) { return x + std::string({ y }); }
		) <<
		'"' << std::endl;
	std::cout <<
		'"' <<
		$.fold_right(
			tup2,
			'f',
			[](const auto& x, const auto& y) { return std::string({ x }) + y; }
		) <<
		'"' << std::endl;
	
	$.for_each(
		$.subtuple<1, 4>($(1, 2, 3, 4, 5, 6, 7)),
		[](auto x) { std::cout << x << ' '; }
	);
	std::cout << std::endl;
	
	return 0;
}
