#pragma once

/********************************************
Name: Devinder Sodhi
Project: Canoga
Class: OPL cmps366
Date:
*********************************************/

#include "Player.h"
#include "Board.h"
class Computer :
	public Player
{
public:
	Computer();
	Computer(Board* board, BoardView* a_boardView);
	void MakeMove();
	~Computer();

private:
	
};

