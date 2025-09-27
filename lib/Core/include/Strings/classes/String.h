#pragma once
#define __XC_STRINGS__
#include "../pkg.h"
#include "../types.h"

#undef s
#undef S

Class(String,
INIT(str_t txt; u64 bytes_len; bool inline_alloc),
FIELD(str_t txt),
	
	interfaceAs(StringUtils) Utils;


	/**
	 * @brief Matches a regular expression against the string.
	 * @details This function performs a regular expression search on the string.
	 * It populates a buffer with the results of the match.
	 *
	 * @param self A pointer to the `String` instance to search within.
	 * @param regex A pointer to the `String` instance containing the regular expression.
	 * @param result_buffer A pointer to a buffer to store the results.
	 * @param buffer_max The maximum number of results the buffer can hold.
	 * @return A signed 64-bit integer representing the number of matches found, or a negative value on error.
	 */
	i64 		method(String, Regex,, inst(String) regex, str_regex_result* result_buffer, size_t buffer_max);
	
	
	/**
	 * @brief Scans a formatted string into a `String` instance.
	 * @details This function reads formatted data from the string and
	 * populates variables based on the format specifiers.
	 *
	 * @param self A pointer to the `String` instance to be scanned.
	 * @param ... A variable number of arguments corresponding to the format specifiers.
	 * @return The number of characters successfully scanned.
	 */
	u64 		method(String, Scan,, ...);
	
	/**
	 * @brief Compares two strings for equality based on their codepoints.
	 * @details This method compares the contents of two String objects. It first
	 * checks for mismatches in length and encoding type. If they match, it iterates
	 * through the strings, decoding codepoints from the UTF-8 representation and
	 * comparing them.
	 *
	 * @param self A pointer to the `String` instance on which the method is called.
	 * @param cmp_string A pointer to the `String` instance to compare against.
	 * @return Returns `true` if the strings are equal in content and encoding, `false` otherwise.
	 */
	bool 		method(String, Compare,, inst(String) string2);
	
	/**
	 * @brief Creates a heap copy of the String object.
	 * @details This function allocates new memory and copies the string data,
	 * including its encoding and private information, to the new location. The
	 * new string can be an inline or dynamically allocated string, depending
	 * on the source.
	 *
	 * @param self A pointer to the `String` instance to be copied.
	 * @return A new `String` instance containing a copy of the original.
	 */
	inst(String) 	method(String, Copy);


	/**
	 * @brief Concatenates one or more strings to the current string.
	 * @details This function uses a StringBuilder to efficiently append
	 * multiple strings or C-style strings (if `usingCstr` is true). It
	 * returns a new `String` instance containing the concatenated result.
	 *
	 * @param self A pointer to the `String` instance to which other strings will be concatenated.
	 * @param usingCstr A boolean flag; if true, subsequent `...` arguments are treated as C-style strings.
	 * @param ... A variable number of `inst(String)` or `char*` (if `usingCstr` is true) arguments to concatenate.
	 * @return A new `String` instance with the combined content.
	 */
	inst(String) 	method(String, Cat,, bool usingCstr, ...);


	/**
	 * @brief Cuts a specified number of characters from the front or back of a string.
	 * @details This function allocates new memory and copies the string data,
	 * including its encoding and private information, to the new location. The
	 * new string can be an inline or dynamically allocated string, depending
	 * on the source.
	 *
	 * @param self A pointer to the `String` instance to be cut.
	 * @param fromFront A boolean flag; if `true`, cuts from the front, otherwise from the back.
	 * @param by The number of characters to cut.
	 * @return The modified `String` instance, or `NULL` if an error occurs.
	 */
	inst(String) 	method(String, Cut,, bool fromFront, u64 by);


	/**
	 * @brief Converts the string to a new encoding type.
	 * @details This function allocates a new string and converts the content
	 * from its current encoding to the specified target encoding. It handles
	 * conversions between UTF-8, UTF-16, UTF-32, and ASCII.
	 *
	 * @param self A pointer to the `String` instance to be converted.
	 * @param type The target `chartype` (encoding) for the new string.
	 * @return A new `String` instance with the converted content, or `NULL` if conversion fails.
	 * @note The returned string must be freed by the caller.
	 */
	inst(String)	method(String, Convert,, chartype type);
)



#define pushString(string, max_len) initialize(String,			\
	alloca(								\
 		sizeof(String_Instance)  + 				\
 		sizeof_String_Private    + 				\
 		strnlenbytes(string, max_len) + 1			\
	), 								\
	{strnlen(string, max_len), getCharType(string), generic string},\
	strnlenbytes(string, max_len), 					\
	true 								\
)

#define newString(string, max_len) initialize(String,			\
	malloc(								\
 		sizeof(String_Instance)  + 				\
 		sizeof_String_Private    + 				\
 		strnlenbytes(string, max_len) + 1			\
	), 								\
	{strnlen(string, max_len), getCharType(string), generic string},\
	strnlenbytes(string, max_len), 					\
	true 								\
)


#define s(string) initialize(String,					\
	alloca(								\
 		sizeof(String_Instance)  + 				\
 		sizeof_String_Private    + 				\
 		sizeof(stirng)						\
	), 								\
	{sizeof(string) / getCharSize(string), 				\
	getCharType(string), generic string},				\
	(sizeof(string)) - 1,						\
	true 								\
)

#define S(string) initialize(String,					\
	malloc(								\
 		sizeof(String_Instance)  + 				\
 		sizeof_String_Private    + 				\
 		sizeof(stirng)						\
	), 								\
	{sizeof(string) / getCharSize(string), 				\
	getCharType(string), generic string},				\
	(sizeof(string)) - 1,						\
	true 								\
)

