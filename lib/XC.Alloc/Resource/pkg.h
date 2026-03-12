
Class(Resource,
INIT(inst(Collection) collection; len_t size),
FIELD(),
      	void* method(Resource, getData);
      	errvt method(Resource, setDestructor, errvt(*destructor)(void*));
      	bool method(Resource,  isNearRemoval);
	errvt method(Resource, moveTo, inst(Collection) new_collection);
	errvt method(Resource, resize, len_t new_size);

)
