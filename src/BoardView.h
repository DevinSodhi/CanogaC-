#pragma once
/********************************************
Name: Devinder Sodhi
Project: Canoga
Class: OPL cmps366
Date:
*********************************************/

#include "Board.h"
#include "HeaderList.h"

class BoardView
{
public:
	BoardView(Board * a_GameBoardReference); // make null?

	void getCurrentBoardView();

	void drawALine(int a_sizeOfIt);


	~BoardView();
private:
	Board * m_boardReference;
};

