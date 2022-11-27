#include "Point/Point.h"
#include "Utils/Utils.h"

#include <limits> /* std::numeric_limits */
#include <stdint.h> /* uint16_t, ... */
#include <string> /* std::string */

#ifdef max
#undef max
#endif

int main()
{
	/* NOTE: This does NOT start the emulator, or load the Tetris ROM, those must be started by the user! */
	/* Emulator download link: https://www.emulator-zone.com/misc/bizhawk */
	/* Rom download link: https://www.emulatorgames.net/roms/nintendo/tetris/ */
	Utils::InitializeTetris();

	Sleep(1000);

	return 0;
}