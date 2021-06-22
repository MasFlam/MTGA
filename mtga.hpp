/*
	MTGA (Make Tuples Great Again) - C++ std::tuple utility library
	
	Copyright (C) 2021 Łukasz "MasFlam" Drukała
	
	This library is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License version 3.0 as
	published by the Free Software Foundation.
	
	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License version 3.0
	along with this library. If not, see https://www.gnu.org/licenses/gpl-3.0.html
*/

#include <tuple>
#include <type_traits>
#include <utility>

// TODO: organize
// TODO: foreach and map on tuples
// TODO: reverse iota on negative N (presumably use std::enable_if)

class $$ {
	template<typename Itype, Itype S, typename IntegralConstantN, Itype... Is>
	struct _iota_seq;
	
	template<typename Itype, Itype S, Itype N, Itype... Is>
	struct _iota_seq<Itype, S, std::integral_constant<Itype, N>, Is...> : _iota_seq<Itype, S + 1, std::integral_constant<Itype, N - 1>, Is..., S> {};
	
	template<typename Itype, Itype S, Itype... Is>
	struct _iota_seq<Itype, S, std::integral_constant<Itype, 0>, Is...> { typedef std::integer_sequence<Itype, Is...> iseq; };
	
	template<typename Itype, Itype... Is>
	inline constexpr auto _iota_internal(std::integer_sequence<Itype, Is...>)
	{
		return (*this)(Is...);
	}
	
public:
	
	template<typename Itype, Itype S, Itype N>
	inline constexpr auto iota()
	{
		static_assert(N >= 0, "$.iota: N must be non-negative");
		return _iota_internal(typename _iota_seq<Itype, S, std::integral_constant<Itype, N>>::iseq());
	}
	
	template<std::size_t S, std::size_t N>
	inline constexpr auto iota()
	{
		return iota<std::size_t, S, N>();
	}
	
	template<typename... Elements>
	inline constexpr auto operator()(Elements&&... elements)
	{
		return std::make_tuple(std::forward<Elements>(elements)...);
	}
	
	template<typename... Elements>
	inline constexpr std::size_t operator[](const std::tuple<Elements...>& tuple)
	{
		return std::tuple_size<std::tuple<Elements...>>::value;
	}
};
