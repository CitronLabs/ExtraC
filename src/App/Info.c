#include <XC.pkg.c>
#include <___/App/pkg.c>
#define module app, Info




noFail moduleFn(printHelp)(std_String* command){
  switchs(command){
  cases("install") {
      println(
	"Install Command: \n\n",

	"Usage: xc install <flags> package_names...\n",
	"Flags: \n\t"
      );
  break;}
  cases("run") {
      println(
	"Run Command: \n\n",

	"Usage: xc run <flags> app_names...\n",
	"Flags: \n\t"
      );
  break;}
  cases("stop") {
      println(
	"Stop Command: \n\n",

	"Usage: xc stop <flags> app_names...\n",
	"Flags: \n\t"
      );
  break;}
  cases("info") {
      println(
	"Info Command: \n\n",

	"Usage: xc info <flags> app_names...\n",
	"Flags: \n\t"
      );
  break;}
  cases("version") {
      println(
	"Version Command: \n\n",

	"Usage: xc version <flags>\n",
	"Flags: \n\t"
      );
  break;}
  defaults {
      println(
	"Help Information: \n\n",

	"Usage: xc <command> <arguments>\n",
	"Commands: \n\t",
		"install\t 	- Installs an app package from a remote server\n\t",
		"run\t 		- Runs an application that has already been installed\n\t",
		"stop\t 	- Stops a running application\n\t",
		"info\t 	- Gets information about an application or module\n\t",
		"help\t 	- Prints out help information about a specific command\n\t",
		"version\t 	- Prints out the version of this app\n\n",

	"For more information about a specific command please use \n\t",
		"xc help <command>"
      );
  }
  }
}

noFail moduleFn(printVersion)(){
  println(
	"ExtraC - Version ", 
		$(app.Version.MAJOR), ".", 
		$(app.Version.MINOR), ".",
		$(app.Version.PATCH), " "
	);
}
