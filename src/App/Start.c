#include <XC.pkg.c>
#include <___/App/pkg.c>
#define module app


main(
.NAME   	= "ExtraC",
.DOMAIN 	= "thecitronlabs.com",
.DESCRIPTION 	=
	"Unified application interface for the "
	"management of XC apps and development."
){
	var cli = push(std_CLI);

	foreach(cli, std_String, cmd){
	    switchs(cmd){
	    cases("install"){
		// Install application / module
	    break;}

	    cases("run"){
		// Run application
	    break;}

	    cases("stop"){
		// Stops a running application
	    break;}

	    cases("info"){
		// Get info about an app / module
	    break;}

	    cases("version"){ 
		app.Info.printVersion(); 
	    break;}

	    cases("help"){ 
		var cmdName = len(cli) > 1 ? index(cli, cmd_iterator++) : nil; 
		app.Info.printHelp(cmdName); 
	    break;}

	    defaults{
		println("Unknown Command: ", $(cmd), "refer to our help information");
		app.Info.printHelp(nil);			
	    }
	    }
	}

return OK;
}
