
#define loop(loopvar, num) for(int loopvar = 0; loopvar < num; loopvar++)

#define loopat(loopvar, init, num) for(int loopvar = init; loopvar < num; loopvar++)

#define iter(array, size, type, item) 	\
type* item; 				\
for(int __##item##_iterator = 0; 	\
    __##item##_iterator < size; 	\
    item = &((type*)array)[__##item##_iterator++])


/**
@def foreach(iterable, type, var)
@param iterable any object who's class has implemented the __ITER interface
@param type the type of a single member within the iterable object
@param var the name of the varible which stores the current member of the iteration
*/
#define foreach(iterableVar, _type, _var) 					\
	variableData* _var##_iterable = &iterableVar;				\
	u64 __##_var##_size = _var##_iterable->type				\
		.ops->Size(generic _var##_iterable, true); 			\
	_type* _var = _var##_iterable->type					\
		.ops->Iter(generic _var##_iterable, 0);				\
	if(_var) for(size_t _var##_iterator = 0; 				\
     	    _var##_iterator < __##_var##_size; 					\
	    _var = _var##_iterable->type.					\
		ops->Iter(generic _var##_iterable, ++_var##_iterator)		\
	)
