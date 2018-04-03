#include <ctime>
#include "2048_game.h"

/*
	Use buttons 'R', 'L', 'U', 'D' for moving right, left, up or down.
	Use keyword "Undo" to undo the step.
	Use keyword "Redo" to redo the step.
	Use keyword "END" to exit the game.
*/

int main()
{
	srand(time(0));
	Game g;
	g.start();
}