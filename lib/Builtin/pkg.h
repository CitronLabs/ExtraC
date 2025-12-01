#include <Core/pkg.h>


#include "Rendering/lib/pkg.h"
#include "Testing/lib/pkg.h"

Module(builtin){
	interface(rk_render)    render;
	interface(testing_test) test;

};

#undef package


import(builtin)

use(rk, 
	Terminal,
    	Screen
)

void it(){
	Terminal* term = builtin.render.Terminal.getPrimary();

	Screen* t_scrn = builtin.render.Terminal.getScreen(term);

	
}
