#pragma once
/********************************************
Name: Devinder Sodhi
Project: Canoga
Class: OPL cmps366
Date:
*********************************************/
#include "PlayerSquare.h"
#include "Player.h"
#include "Board.h"
#include "HeaderList.h"

class Human :
	public Player
{
public:
	Human();
	Human(Board* board, BoardView* a_boardView);
	//Human overload of make move
	void MakeMove();

	bool AskIfOneDice();

	bool AskIfCover(int a_rollResult, bool a_canMakeCoverMoves, bool a_canMakeUnCoverMoves);

	int WhichMoveDoYouWant(int a_MaxPossible);

	

	~Human();


private: 
	
};

