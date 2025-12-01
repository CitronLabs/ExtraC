#pragma once

#define __XC_STRINGS__
#include "../pkg.h"
#include "../types.h"

#define package std

Decl(List)

Class(StringBuilder,
INIT(std_char_type type; std_String* init_str; u64 limit;),
FMT(),

private(
	std_List* data;
	std_Stream* in_stream;
	std_char_type ctype;
	rune currentCharacter;
	u64 len;
)
){
      	std_StreamEncoder streamEncoder;
      	std_StreamDecoder streamDecoder;

	/**
	 * @brief Sets a maximum length for the StringBuilder.
	 * @details This function limits the total number of characters that the
	 * StringBuilder can hold. Any subsequent append, prepend, or insert
	 * operations will respect this limit.
	 *
	 * @param self A pointer to the `StringBuilder` instance.
	 * @param max_len The maximum length in characters.
	 * @return An `errvt` (error value) indicating success or failure.
	 */
	errvt method(StringBuilder, Max, u64 max_len);
	
	/**
	 * @brief Sets the content of the StringBuilder.
	 * @details This function replaces the existing content of the StringBuilder
	 * with the provided string. It can accept either a `String` instance or
	 * a variable number of formatted arguments.
	 *
	 * @param self A pointer to the `StringBuilder` instance.
	 * @param arg va_list of format args used to set, or `null` if using variable arguments.
	 * @param ... A variable number of arguments to be formatted and set as the content.
	 * @return The new length of the StringBuilder in characters.
	 */
	u64 method(StringBuilder, Set, __);
	
	/**
	 * @brief Appends a string or formatted content to the StringBuilder.
	 * @details This function adds new content to the end of the StringBuilder's
	 * current content. It can accept a `String` instance or variable formatted arguments.
	 *
	 * @param self A pointer to the `StringBuilder` instance.
	 * @param arg va_list of format args used to append, or `null` if using variable arguments.
	 * @param ... A variable number of arguments to be formatted and appended.
	 * @return The number of characters appended.
	 */
	u64 method(StringBuilder, Append, __);
	
	/**
	 * @brief Prepends a string or formatted content to the StringBuilder.
	 * @details This function adds new content to the beginning of the StringBuilder's
	 * current content. It can accept a `String` instance or variable formatted arguments.
	 *
	 * @param self A pointer to the `StringBuilder` instance.
	 * @param arg va_list of format args used to prepend, or `null` if using variable arguments.
	 * @param ... A variable number of arguments to be formatted and prepended.
	 * @return The number of characters prepended.
	 */
	u64 method(StringBuilder, Prepend, __);
	
	/**
	 * @brief Inserts a string or formatted content at a specified index.
	 * @details This function inserts new content into the StringBuilder at a specific
	 * character index. It can accept a `String` instance or variable formatted arguments.
	 *
	 * @param self A pointer to the `StringBuilder` instance.
	 * @param index The character index at which to insert the new content.
	 * @param arg va_list of format args used to insert, or `null` if using variable arguments.
	 * @param ... A variable number of arguments to be formatted and inserted.
	 * @return The number of characters inserted.
	 */
	u64 method(StringBuilder, Insert, u64 index, __);
	

	/**
	 * @brief Clears the content of the StringBuilder.
	 * @details This function empties the StringBuilder, resetting its length to zero
	 * and freeing any allocated data.
	 *
	 * @param self A pointer to the `StringBuilder` instance.
	 * @return An `errvt` (error value) indicating success or failure.
	 */
	errvt method(StringBuilder, Clear);
	
	/**
	 * @brief Retrieves the current content of the StringBuilder as a `String_Instance`.
	 * @details This function returns a copy of the StringBuilder's content in the
	 * form of a `String_Instance`. It does not create a new `String` object on the heap.
	 *
	 * @param self A pointer to the `StringBuilder` instance.
	 * @return A `String_Instance` containing the builder's current content.
	 */
	std_String* method(StringBuilder, GetStr);
	
	/**
	 * @brief Creates a new `String` instance from the StringBuilder's content.
	 * @details This function allocates a new `String` object on the heap and copies
	 * the StringBuilder's content into it. The returned `String` must be freed
	 * by the caller.
	 *
	 * @param self A pointer to the `StringBuilder` instance.
	 * @return A new `String` instance, or `null` on failure.
	 */
	std_String* method(StringBuilder, CreateStr);

};

#undef package
