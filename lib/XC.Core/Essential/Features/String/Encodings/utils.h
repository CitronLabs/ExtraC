
#define UTF8_1BYTE_MASK 0b10000000
#define UTF8_2BYTE_MASK 0b11100000
#define UTF8_3BYTE_MASK 0b11110000
#define UTF8_4BYTE_MASK 0b11111000

#define UTF8_1BYTE_HEADER 0b00000000
#define UTF8_2BYTE_HEADER 0b11000000
#define UTF8_3BYTE_HEADER 0b11100000
#define UTF8_4BYTE_HEADER 0b11110000


// UTF-8 encoding boundaries
#define UTF8_1BYTE_MAX          0x7F
#define UTF8_2BYTE_MAX          0x7FF
#define UTF8_3BYTE_MAX          0xFFFF
#define UTF8_4BYTE_MAX          0x10FFFF

// UTF-8 byte patterns and masks
#define UTF8_CONT_BYTE_MASK     0x80 // 10xxxxxx
#define UTF8_CONT_BYTE_BITS     0x3F // 00111111
#define UTF8_2BYTE_PREFIX       0xC0 // 110xxxxx
#define UTF8_3BYTE_PREFIX       0xE0 // 1110xxxx
#define UTF8_4BYTE_PREFIX       0xF0 // 11110xxx

// UTF-8 character sizes
#define UTF8_1BYTE_SIZE         1
#define UTF8_2BYTE_SIZE         2
#define UTF8_3BYTE_SIZE         3
#define UTF8_4BYTE_SIZE         4

// UTF-16 surrogate pair boundaries
#define UTF16_HIGH_SURROGATE_START  0xD800
#define UTF16_HIGH_SURROGATE_END    0xDBFF
#define UTF16_LOW_SURROGATE_START   0xDC00
#define UTF16_LOW_SURROGATE_END     0xDFFF

// UTF-16 surrogate pair calculation
#define UTF16_SURROGATE_OFFSET_BASE 0x10000
#define UTF16_HIGH_SURROGATE_SHIFT  10
#define UTF16_LOW_SURROGATE_BITS    0x3FF
#define UTF16_HIGH_SURROGATE_OFFSET 0xD800
#define UTF16_LOW_SURROGATE_OFFSET  0xDC00

// Conversion size factors
#define UTF8_TO_UTF16_MAX_FACTOR  2 // UTF-8 1-byte char can become UTF-16 2-byte surrogate pair
#define UTF16_TO_UTF8_MAX_FACTOR  3 // UTF-16 high surrogate can become a 3-byte UTF-8 char
#define UTF32_TO_UTF8_MAX_FACTOR  4 // UTF-32 can become 4-byte UTF-8 char

// Null terminator size
#define null_TERMINATOR_SIZE      1

// ASCII boundary
#define ASCII_MAX                 0x7F
#define ASCII_REPLACEMENT_CHAR    '?'

#define UTF8_CHAR_SIZE(c) \
    ((c) <= UTF8_1BYTE_MAX ? UTF8_1BYTE_SIZE : \
     ((c) <= UTF8_2BYTE_MAX ? UTF8_2BYTE_SIZE : \
      ((c) <= UTF8_3BYTE_MAX ? UTF8_3BYTE_SIZE : UTF8_4BYTE_SIZE)))
