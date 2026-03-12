#include <XC.pkg.c>

alias(std.Graphics.Window, Window)

errvt processEvents(std_Window* window){

    foreach(window, std_Window_Event, event){
	switchV(event->type){
	caseV(Window.Event.MOVE){
	    var position = event->data.move;


	}
	}
    }

return OK;
}


void test(){
	var window   = new(std_Window);
	var renderer = new(std_Renderer);

	Window.setRenderer(window, renderer);
	
	Window.run(window);

	while(Window.isRunning(window)){

	    // EVENT HANDLING HERE
	    if(Window.events(window)) 
		processEvents(window);

	    // APP/GAME LOGIC HERE
	    
	    // RENDER HERE

	    Window.update(window);
	}
}

