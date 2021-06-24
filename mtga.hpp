/*
	MTGA (Make Tuples Great Again) - C++ std::tuple "utility" library
	
	Copyright (C) 2021 Łukasz "MasFlam" Drukała
	
	This library is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License version 3.0 as
	published by the Free Software Foundation.
	
	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	version 3.0 along with this library.
	If not, see https://www.gnu.org/licenses/gpl-3.0.html
*/

#include <tuple>
#include <type_traits>
#include <utility>

/**
	Make Tuples Great Again!
	
	This class contains different "utility" functions for working with 'std::tuple's.
	For each translation unit, define one global or namespace-scope variable, like this:
		$$ $;
	It makes for funny syntax.
*/
class $$ {
	
	/**** iota ****/
	
	template<typename Itype, Itype S, typename IntegralConstantN, Itype... Is>
	struct _iota_seq;
	
	template<typename Itype, Itype S, Itype N, Itype... Is>
	struct _iota_seq<Itype, S, std::integral_constant<Itype, N>, Is...>
		: _iota_seq<Itype, S + 1, std::integral_constant<Itype, N - 1>, Is..., S> {};
	
	template<typename Itype, Itype S, Itype... Is>
	struct _iota_seq<Itype, S, std::integral_constant<Itype, 0>, Is...> {
		typedef std::integer_sequence<Itype, Is...> iseq;
	};
	
	template<typename Itype, Itype S, typename IntegralConstantN, Itype... Is>
	struct _iota_rseq;
	
	template<typename Itype, Itype S, Itype N, Itype... Is>
	struct _iota_rseq<Itype, S, std::integral_constant<Itype, N>, Is...>
		: _iota_rseq<Itype, S - 1, std::integral_constant<Itype, N - 1>, Is..., S> {};
	
	template<typename Itype, Itype S, Itype... Is>
	struct _iota_rseq<Itype, S, std::integral_constant<Itype, 0>, Is...> {
		typedef std::integer_sequence<Itype, Is...> iseq;
	};
	
	template<typename Itype, bool asc, Itype S, Itype N>
	struct _iota;
	
	template<typename Itype, Itype S, Itype N>
	struct _iota<Itype, true, S, N> {
		typedef typename _iota_seq<Itype, S, std::integral_constant<Itype, N>>::iseq seq;
	};
	
	template<typename Itype, Itype S, Itype N>
	struct _iota<Itype, false, S, N> {
		typedef typename _iota_rseq<Itype, S, std::integral_constant<Itype, -N>>::iseq seq;
	};
	
	template<typename Itype, Itype... Is>
	inline constexpr auto _iota_internal(std::integer_sequence<Itype, Is...>) const {
		return (*this)(Is...);
	}
	
public:
	
	/**
		Returns a tuple containing 'abs(N)' consecutive values of type 'Itype'
		(works well with integers; use non-integral types at your peril),
		starting with 'S'. (Ascending if 'N >= 0', descending otherwise)
	*/
	template<typename Itype, Itype S, Itype N>
	inline constexpr auto iota() const {
		return _iota_internal(typename _iota<Itype, N >= 0, S, N>::seq());
	}
	
	/**
		Same as 'iota<int, S, N>()'.
	*/
	template<int S, int N>
	inline constexpr auto iota() const {
		return iota<int, S, N>();
	}
	
	/**** for_each ****/
	
private:
	
	template<typename Func, std::size_t I, std::size_t N, typename... Elements>
	struct _for_each_call {
		inline constexpr auto _call(
			const std::tuple<Elements...>& tuple,
			const Func& func
		) const {
			func(std::get<I>(tuple));
			_for_each_call<Func, I + 1, N - 1, Elements...>()._call(tuple, func);
		}
	};
	
	template<typename Func, std::size_t I, typename... Elements>
	struct _for_each_call<Func, I, 0, Elements...> {
		inline constexpr auto _call(
			const std::tuple<Elements...>& tuple,
			const Func& func
		) const {}
	};
	
public:
	
	/**
		Call 'func' on each element of 'tuple', in order.
	*/
	template<typename Func, typename... Elements>
	inline constexpr auto for_each(
		const std::tuple<Elements...>& tuple,
		const Func& func
	) const {
		_for_each_call<
			Func, 0,
			std::tuple_size<std::tuple<Elements...>>::value,
			Elements...
		>()._call(tuple, func);
	}
	
	/**** map ****/
	
private:
	
	template<typename Func, std::size_t I, typename Tuple, typename... NewElements>
	struct _map_internal {
		inline constexpr auto _do(
			const Tuple& tuple,
			const Func& func,
			const NewElements&... new_elements
		) const {
			return _map_internal<
				Func, I - 1, Tuple,
				decltype(func(std::get<I>(tuple))),
				NewElements...
			>()._do(tuple, func, func(std::get<I>(tuple)), new_elements...);
		}
	};
	
	template<typename Func, typename Tuple, typename... NewElements>
	struct _map_internal<Func, 0, Tuple, NewElements...> {
		inline constexpr auto _do(
			const Tuple& tuple,
			const Func& func,
			const NewElements&... new_elements
		) const {
			return std::make_tuple(func(std::get<0>(tuple)), new_elements...);
		}
	};
	
public:
	
	/**
		Call 'func' on each element in 'tuple', in order, saving the results
		in another tuple, and returning it.
	*/
	template<typename Func, typename... Elements>
	inline constexpr auto map(
		const std::tuple<Elements...>& tuple,
		const Func& func
	) const {
		return _map_internal<
			Func,
			std::tuple_size<std::tuple<Elements...>>::value - 1,
			std::tuple<Elements...>
		>()._do(tuple, func);
	}
	
	/**** operator() ****/
	
	/**
		Construct a tuple from 'elements'. Equivalent of 'std::make_tuple'.
	*/
	template<typename... Elements>
	inline constexpr auto operator()(Elements&&... elements) const {
		return std::make_tuple(std::forward<Elements>(elements)...);
	}
	
	/**** operator[] ****/
	
	/**
		Return the number of elements in 'tuple'.
	*/
	template<typename... Elements>
	inline constexpr std::size_t operator[](
		const std::tuple<Elements...>& tuple
	) const {
		return std::tuple_size<std::tuple<Elements...>>::value;
	}
};
