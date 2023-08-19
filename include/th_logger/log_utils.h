#pragma once

#include <concepts>
#include <string>
#include <ostream>

namespace Th::Log
{

template <typename T, typename U>
concept IsConvertibleFrom = requires { std::is_convertible_v<U, T>; };

/// @brief Checks whether a type behaves like a stream - contains `Stream & operator<<(T)`.
/// @tparam Stream stream
/// @tparam T type
template <typename OStream, typename T>
concept BehavesOStreamLike = requires(OStream s) {
	{
		s << (T())
	} -> IsConvertibleFrom<OStream>;
};

/// @brief StreamType concept. A type which behaves like (BehavesOStreamLike<OStream, T>) a
/// std::ostream is considered a StreamType - contains `Stream & operator<<(T)` and can be flushed
/// @see https://en.cppreference.com/w/cpp/io/basic_ostream/operator_ltlt
/// @see https://en.cppreference.com/w/cpp/io/basic_ostream/operator_ltlt2
/// @tparam Stream stream
template <typename Stream>
concept IsOStreamLike = requires(Stream s) {
	// Concepts aren't recursive. Using recursive traits won't tell you, where the error
	// specifically is. So we're stuck with copy-pasting
	requires BehavesOStreamLike<Stream, bool>;
	requires BehavesOStreamLike<Stream, long>;
	requires BehavesOStreamLike<Stream, unsigned long>;
	requires BehavesOStreamLike<Stream, long long>;
	requires BehavesOStreamLike<Stream, unsigned long long>;
	requires BehavesOStreamLike<Stream, double>;
	requires BehavesOStreamLike<Stream, long double>;
	requires BehavesOStreamLike<Stream, const void *>;
	requires BehavesOStreamLike<Stream, std::nullptr_t>;
	requires BehavesOStreamLike<Stream, short>;
	requires BehavesOStreamLike<Stream, int>;
	requires BehavesOStreamLike<Stream, unsigned short>;
	requires BehavesOStreamLike<Stream, unsigned int>;
	requires BehavesOStreamLike<Stream, float>;
	requires BehavesOStreamLike<Stream, std::string>;
	requires BehavesOStreamLike<Stream, char>;
	requires BehavesOStreamLike<Stream, const char *>;
	{
		std::flush(s)
	} -> IsConvertibleFrom<Stream>;
};

} // namespace Th::Log
