#include <iostream>
#include "mtga.hpp"

$$ $;

int main()
{
	auto asc = $.iota<5, 5>();
	auto desc = $.iota<5, -10>();
	std::cout << "asc: " <<
		std::get<0>(asc) << ' ' <<
		std::get<1>(asc) << ' ' <<
		std::get<2>(asc) << ' ' <<
		std::get<3>(asc) << ' ' <<
		std::get<4>(asc) << std::endl;
	std::cout << "desc: " <<
		std::get<0>(desc) << ' ' <<
		std::get<1>(desc) << ' ' <<
		std::get<2>(desc) << ' ' <<
		std::get<3>(desc) << ' ' <<
		std::get<4>(desc) << ' ' <<
		std::get<5>(desc) << ' ' <<
		std::get<6>(desc) << ' ' <<
		std::get<7>(desc) << ' ' <<
		std::get<8>(desc) << ' ' <<
		std::get<9>(desc) << std::endl;
	auto tup = $(1, 2, 3, 4, 5);
	$.for_each(tup, [](auto x) { std::cout << 2*x << std::endl; });
	return 0;
}
