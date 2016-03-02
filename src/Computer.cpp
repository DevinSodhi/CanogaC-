#include "Computer.h"

//if the computer can't make any valid moves to cover its own tiles, then it should make offensive moves.

Computer::Computer()
{
}
Computer::Computer(Board* a_board, BoardView* a_boardView) : Player()
{
	m_validCoverMoves.clear();
	m_validUncoverMoves.clear();

	m_whichPlayer = "Computer";
	m_boardPointer = a_board;
	m_boardViewPointer = a_boardView;
}

//Make the human Move


/****************************************************************************************************
Function Name:void Computer::MakeMove()

Purpose: make the move for a player specifically human here

Parameters: None

Return Value: none

Local Variables:

bool canMakeValidMoves = false;
bool canMakeCoverMoves = false;
bool canMakeUncoverMoves = false;
bool coverOrUncover;

Algorithm:

Assistance Received: None

*****************************************************************************************************/
void Computer::MakeMove()
{
	int rollResult = -1;

	bool canMakeValidMoves = false;
	bool canMakeCoverMoves = false;
	bool canMakeUncoverMoves = false;
	bool coverOrUncover;
	int makeThisMove;

	do 
	{
		vector <PlayerSquare> mySquares = m_boardPointer->GetComputerSquares();
		m_boardViewPointer->getCurrentBoardView();
		cout << "Its the Computers Turn....\n\n\n\n\n" << endl;
		Sleep(2000);

		cout << "Lets start with rolling 1 dice if I have the option, OtherWise 2 Dice\n\n\n" << endl;
		//computer chooses one die if number is less than 6 ->statistically proven strategy
		if (AreBigNumsCovered(mySquares))
		{
			rollResult = PerformDieRoll(m_boardPointer->GetUsingDiceFile());
		}
		else
		{
			rollResult = ResultOfDiceRoll();
		}

/*******************************************CHECK TO SEE IF YOU CAN MAKE VALID MOVES.********************************************************************/
		canMakeValidMoves = ValidMovesExist(rollResult, canMakeCoverMoves, canMakeUncoverMoves);

		if (canMakeValidMoves)
		{
			makeThisMove = Strategy(canMakeCoverMoves, canMakeUncoverMoves, coverOrUncover);
			//Copy OVer the computers version of the strategy here. 
			//we have the cover or uncover
			//we have the number of the strategy.
			//we just need to translate that to computer. SHOULD BE EASY
			if (coverOrUncover)
			{
				m_hasCovered = true;
				//which tileset do we want to change? we intend to cover, so thats the one we change.
				m_vectorIWantToPlayWith = m_boardPointer->GetVectorOfInterest(coverOrUncover);

				//get the correct vector that lists the tiles  you want to cover
				m_validCoverMoves.at(makeThisMove);

				for (int i = 0; i < (int)m_validCoverMoves.at(makeThisMove).size(); i++)
				{
					//  The Correct Vector   at  ( the correct vector, the correct number, adjusted for 0-N)
					m_vectorIWantToPlayWith->at(m_validCoverMoves.at(makeThisMove).at(i) - 1).SetCoveredStatus(true);
				}
				/**********************IF WIN BY COVER, END TURN***********************************/
				bool wonByCovering = true;
				for (int i = 0; i < (int)m_vectorIWantToPlayWith->size(); i++)
				{
					//if one of the tiles is uncovered, you shouldn't have a winner
					if (!(m_vectorIWantToPlayWith->at(i).GetCoveredStatus()))
					{
						wonByCovering = false;
					}
				}

				if (wonByCovering)
				{
					return;
				}
				/**********************IF WIN BY COVER, END TURN***********************************/

			}
			else
			{
				m_hasUncovered = true;
				//which tileset do we want to change? we intend to uncover, so thats the one we change.
				m_vectorIWantToPlayWith = m_boardPointer->GetVectorOfInterest(coverOrUncover);

				//get the correct vector that lists the tiles  you want to uncover
		//		m_validCoverMoves.at(makeThisMove);
				
				for (int i = 0; i < (int)m_validUncoverMoves.at(makeThisMove).size(); i++)
				{
					
					//  The Correct Vector   at  ( the correct vector, the correct number, adjusted for 0-N)
					m_vectorIWantToPlayWith->at(m_validUncoverMoves.at(makeThisMove).at(i) - 1).SetCoveredStatus(false);
				}

				/**********************IF WIN BY COVER, END TURN***********************************/

				bool wonByUncovering = true;
				for (int i = 0; i < (int)m_vectorIWantToPlayWith->size(); i++)
				{
					//if one of the tiles is covered you shouldn't have a winner
					if ((m_vectorIWantToPlayWith->at(i).GetCoveredStatus()))
					{
						wonByUncovering = false;
					}
				}
				if (wonByUncovering)
				{
					return;
				}
				/**********************IF WIN BY COVER, END TURN***********************************/

			}
		}

		system("PAUSE");
	} while (canMakeValidMoves);

	cout << "No more Valid moves can be Made and no winner, Switching turn back to Human" << endl;
	return ;
}


Computer::~Computer()
{
}
