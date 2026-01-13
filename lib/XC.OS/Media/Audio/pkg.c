#include <XC.OS/pkg.c>

from(os_Audio,
	Player as Speaker
)

void test(){

	var speaker = new(Speaker);

	os.Audio.Player.stream

}
