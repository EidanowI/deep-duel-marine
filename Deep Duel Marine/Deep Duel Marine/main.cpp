#include "DeepDuelMarine.h"
#include <Windows.h>

int WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
	DeepDuelMarine itsSoDeep = DeepDuelMarine();
	itsSoDeep.Run();
	return 0;
}