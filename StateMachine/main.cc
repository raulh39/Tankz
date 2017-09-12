#include "States.h"
#include <iostream>

class GameMode;

int main()
{
	GameMode game;
	std::cout << "Initiating\n";
	game.initiate();

	std::cout << "Pressing cycle\n";
	game.process_event( EvCycle() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );
	std::cout << "Move WASD\n";
	game.process_event( EvMove() );
	std::cout << "Pan with mouse\n";
	game.process_event( EvPan() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );
	std::cout << "Move WASD\n";
	game.process_event( EvMove() );
	std::cout << "Pan with mouse\n";
	game.process_event( EvPan() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );
	std::cout << "Pressing Escape\n";
	game.process_event( EvEsc() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );
	std::cout << "Pressing Escape\n";
	game.process_event( EvEsc() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );
	std::cout << "Pressing Escape\n";
	game.process_event( EvEsc() );


	std::cout << "\n-------Now it should be in FIRINGState-----\n";
	std::cout << "Pressing cycle\n";
	game.process_event( EvCycle() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );
	std::cout << "Pressing cycle\n";
	game.process_event( EvCycle() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );
	std::cout << "Pressing Escape\n";
	game.process_event( EvEsc() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );




	std::cout << "\n-------Now it should be in FIRINGState-----\n";
	std::cout << "Pressing cycle\n";
	game.process_event( EvCycle() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );
	std::cout << "Pressing cycle\n";
	game.process_event( EvCycle() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );
	std::cout << "Pressing select\n";
	game.process_event( EvSelect() );

	
	std::cout << "\n-------Terminate\n";
	game.terminate();
	std::cout << "Ending\n";
}
