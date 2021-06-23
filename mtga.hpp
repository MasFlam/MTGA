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

class $$ {
	
	/**** iota ****/
	
	template<typename Itype, Itype S, typename IntegralConstantN, Itype... Is>
	struct _iota_seq;
	
	template<typename Itype, Itype S, Itype N, Itype... Is>
	struct _iota_seq<Itype, S, std::integral_constant<Itype, N>, Is...>
		: _iota_seq<Itype, S + 1, std::integral_constant<Itype, N - 1>, Is..., S>
	{};
	
	template<typename Itype, Itype S, Itype... Is>
	struct _iota_seq<Itype, S, std::integral_constant<Itype, 0>, Is...>
	{
		typedef std::integer_sequence<Itype, Is...> iseq;
	};
	
	template<typename Itype, Itype S, typename IntegralConstantN, Itype... Is>
	struct _iota_rseq;
	
	template<typename Itype, Itype S, Itype N, Itype... Is>
	struct _iota_rseq<Itype, S, std::integral_constant<Itype, N>, Is...>
		: _iota_rseq<Itype, S - 1, std::integral_constant<Itype, N - 1>, Is..., S>
	{};
	
	template<typename Itype, Itype S, Itype... Is>
	struct _iota_rseq<Itype, S, std::integral_constant<Itype, 0>, Is...>
	{
		typedef std::integer_sequence<Itype, Is...> iseq;
	};
	
	template<typename Itype, bool asc, Itype S, Itype N>
	struct _iota;
	
	template<typename Itype, Itype S, Itype N>
	struct _iota<Itype, true, S, N>
	{
		typedef typename _iota_seq<Itype, S, std::integral_constant<Itype, N>>::iseq seq;
	};
	
	template<typename Itype, Itype S, Itype N>
	struct _iota<Itype, false, S, N>
	{
		typedef typename _iota_rseq<Itype, S, std::integral_constant<Itype, -N>>::iseq seq;
	};
	
	template<typename Itype, Itype... Is>
	inline constexpr auto _iota_internal(std::integer_sequence<Itype, Is...>) const noexcept
	{
		return (*this)(Is...);
	}
	
public:
	
	template<typename Itype, Itype S, Itype N>
	inline constexpr auto iota() const noexcept
	{
		return _iota_internal(typename _iota<Itype, N >= 0, S, N>::seq());
	}
	
	template<int S, int N>
	inline constexpr auto iota() const noexcept
	{
		return iota<int, S, N>();
	}
	
	/**** for_each ****/
	
private:
	
	template<typename Func, std::size_t I, std::size_t N, typename... Elements>
	struct _for_each_call
	{
		inline constexpr auto _call(const std::tuple<Elements...>& tuple, const Func& func) const
		{
			func(std::get<I>(tuple));
			_for_each_call<Func, I + 1, N - 1, Elements...>()._call(tuple, func);
		}
	};
	
	template<typename Func, std::size_t I, typename... Elements>
	struct _for_each_call<Func, I, 0, Elements...>
	{
		inline constexpr auto _call(const std::tuple<Elements...>& tuple, const Func& func) const noexcept
		{}
	};
	
public:
	
	template<typename Func, typename... Elements>
	inline constexpr auto for_each(const std::tuple<Elements...>& tuple, const Func& func) const
	{
		_for_each_call<Func, 0, std::tuple_size<std::tuple<Elements...>>::value, Elements...>()
			._call(tuple, func);
	}
	
	/**** operator() ****/
	
	template<typename... Elements>
	inline constexpr auto operator()(Elements&&... elements) const noexcept
	{
		return std::make_tuple(std::forward<Elements>(elements)...);
	}
	
	/**** operator[] ****/
	
	template<typename... Elements>
	inline constexpr std::size_t operator[](const std::tuple<Elements...>& tuple) const noexcept
	{
		return std::tuple_size<std::tuple<Elements...>>::value;
	}
};
