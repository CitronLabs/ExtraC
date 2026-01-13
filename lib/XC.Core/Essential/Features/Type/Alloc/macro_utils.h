#define CONCAT(a, b) a##b
#define EXPAND(a, b) CONCAT(a, b)

#define CALL_OVERLOAD(name, count, ...) \
    EXPAND(name, count)(__VA_ARGS__)

#define __CONSTRUCT_ARGS(type_name, ...) \
    CALL_OVERLOAD(__CONSTRUCT_ARGS_SET, __VA_OPT__(_TO), type_name __VA_OPT__(, __VA_ARGS__))

#define __CONSTRUCT_ARGS_SET(type_name) \
    generic type_name##_Type->construct 

#define __CONSTRUCT_ARGS_SET_TO(type_name, ...) \
    &(type_name##_ConstructArgs){__VA_ARGS__} \

