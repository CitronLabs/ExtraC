
#define INIT(...) __VA_ARGS__;
#define FIELD(...) __VA_ARGS__;

#define priv self->__private
#define self(Class) inst(Class) self = class(Class) object;

typedef void* inst;
#define inst(Class) Class##_Instance*
#define intf(Class) const Class##_Interface*
#define data(Class) Class##_Instance
#define ifob(Class) ifob

#define generic (void*)
#define class(type) (inst(type))
#define like(var) (typeof(var))

#define useIntf(obj, ...) (ifob){generic &((*obj->__methods)__VA_ARGS__), generic obj}

#define InType(name, type)  typedef struct in_##name{const _Atomic(bool)* isReady; type* data;}in_##name;
#define OutType(name, type) typedef type out_##name;
