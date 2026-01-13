#include <OS/pkg.c>

from(os_Env,
	Input_Vec2D     	as Vec2D,
	Graphics_Handle 	as GraphicsHandle,
	Graphics_Display_Event  as DisplayEvent,
)

alias(OSEnv.Graphics.Display, Display)

void test(){
	std_Array_Queue* evntQueue = newArrayQueue(DisplayEvent, 10);

	GraphicsHandle window =
     		Display.init(
			push(Vec2D, 0,0),
			push(Vec2D, 1920, 1080),
			Display.getDefault()
		);

     	OSEnv.Graphics.handleEvents(window, evntQueue);

     	bool running = true;

     	while(running){
	  while(len(evntQueue)){
		DisplayEvent evnt;

     		read(evntQueue, &evnt);

     		if  (evnt.type == Display.Event.CLOSE){
			running = false;
     			break;
     		}
     		elif(evnt.type == Display.Event.RESIZE){

     		}

     	  }
     	}
}
