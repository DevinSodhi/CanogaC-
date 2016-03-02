/********************************************
Name: Devinder Sodhi
Project: Canoga
Class: OPL cmps366
Date:
*********************************************/
#pragma once
#include "HeaderList.h"
#include "Game.h"
/**
Tournament Class has the following functionality:
1. allow the start of a game.
2. allow the load of a game.
3. allow the continuation of a game
4. all of the above  require a game round to start from its current state, so a single function should Trigger the Game to start a round.
*/

class Tournament
{
public:
	Tournament();

	//start things off, ask the player if they want to start a new game or a Load a game.
	void GameOptions();

	//start a new game. resets all stats. Might indicate if its a new game.
	void NewRoundSetup(bool isNewGame, string a_whoWonLastRound);

	//Load a game. loads the stats necessary from the file or invalidates it
	void LoadGameFromFileSetup();
	~Tournament();

private:

	//main game engine
	Game  m_GameEngine;
	//human score
	int humanScore;
	//computer score
	int ComputerScore;
	//size of the tilesset
	int m_sizeOfTileset;
	//track for continues game.
	string m_whoWonLastRound;
};

