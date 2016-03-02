#include "Human.h"


Human::Human()
{

}
Human::Human(Board* a_board, BoardView* a_boardView) : Player()
{
	m_validCoverMoves.clear();
	m_validUncoverMoves.clear();

	m_whichPlayer = "Human";
	m_boardPointer = a_board;
	m_boardViewPointer = a_boardView;
}

/****************************************************************************************************
Function Name:void BoardView::MakeMove()
Purpose:	make the full move of a Human
Parameters:

Return Value: none

Local Variables:

bool canMakeValidMoves = false;
bool canMakeCoverMoves = false;
bool canMakeUncoverMoves = false;
bool coverOrUncover;
Algorithm:

Assistance Received: 

*****************************************************************************************************/
void Human::MakeMove()
{
	int rollResult = -1;
	//find out whats covered for you.... (both players need to know this.)

	bool canMakeValidMoves = false;
	bool canMakeCoverMoves = false;
	bool canMakeUncoverMoves = false;
	bool coverOrUncover;
	/****************************************THE LOOP WHERE YOU KEEP ROLLING DICE TILL YOU DON'T HAVE VALID MOVES*************************************************************/
	do
	{
		vector <PlayerSquare> mySquares = m_boardPointer->GetHumanSquares();
		m_boardViewPointer->getCurrentBoardView();
		//Check to see if player won??

		/*******************************************CHECK IF THE BIG NUMBERS ARE COVERED 7-N************************************************/
		if (AreBigNumsCovered(mySquares))
		{
			cout << "Looks like your big numbers are ALL covered. Do you want an Option to Roll just one Dice?" << endl;

			//ask player if they want to roll one die or two Dice and save the result
			bool oneDiceForLife = AskIfOneDice();

			if (oneDiceForLife)
			{
				rollResult = PerformDieRoll(m_boardPointer->GetUsingDiceFile());
			}
			else
			{
				rollResult = ResultOfDiceRoll();
			}

		}
		else
		{
			cout << "Lets roll Two Dice for you" << endl;
			rollResult = ResultOfDiceRoll();
		}
/*******************************************CHECK TO SEE IF YOU CAN MAKE VALID MOVES.********************************************************************/
		canMakeValidMoves = ValidMovesExist(rollResult, canMakeCoverMoves, canMakeUncoverMoves);
/***************************************************IF A MOVE IS POSSIBLE, TRY TO FIND OUT WHAT THE HUMAN WANTS TO DO************************************************************/
		if (canMakeValidMoves)
		{
			//THe computer just makes use of the results from the Strategy Function => Human gets options.
			coverOrUncover = AskIfCover(rollResult, canMakeCoverMoves, canMakeUncoverMoves);
/******************************************NOW WE KNOW WHAT IT IS THAT WE WANT TO DO...*************************************************************************/
			int whichMoveYouWantNum;
			if (coverOrUncover)
			{
				m_hasCovered = true;
				int whichMoveYouWantNum = 0;
				for (int i = 0; i < (int)m_validCoverMoves.size(); i++)
				{
					whichMoveYouWantNum++;
					cout << "Move number " << whichMoveYouWantNum << ":  You can cover these Tiles" << endl;
					for (int j = 0; j < (int)m_validCoverMoves.at(i).size(); j++)
					{
						cout << (int)m_validCoverMoves.at(i).at(j) << " ";
					}
					
					cout << endl;
				}
				//figure out which move you want to make?
				int makeThisMove = WhichMoveDoYouWant(whichMoveYouWantNum) - 1;

				//which tileset do we want to change?
				m_vectorIWantToPlayWith = m_boardPointer->GetVectorOfInterest(coverOrUncover);

				//This is what we use to get how we want to change the tileset we care about
				//m_validCoverMoves.at(makeThisMove);

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
				whichMoveYouWantNum = 0;
				//if they want to uncover. List the possible cover moves that they can make.
				for (int i = 0; i < (int)m_validUncoverMoves.size(); i++)
				{
					whichMoveYouWantNum++;
					cout << "Move number " << whichMoveYouWantNum << ":  You can uncover these Tiles: ";
					for (int j = 0; j < (int)m_validUncoverMoves.at(i).size(); j++)
					{
						cout << (int)m_validUncoverMoves.at(i).at(j) << " ";
					}
					
					cout << endl;
				}
				//figure out which move you want to make?
				int makeThisMove = WhichMoveDoYouWant(whichMoveYouWantNum) - 1;

				//which tileset do we want to change?
				m_vectorIWantToPlayWith = m_boardPointer->GetVectorOfInterest(coverOrUncover);

				//This is what we use to get how we want to change the tileset we care about
			//	m_validCoverMoves.at(makeThisMove);

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
	//if you can't make valid moves, you can't do anything anyways so might as well step out of the loop gracefully
	} while (canMakeValidMoves);

	cout << "No more Valid moves can be Made and no winner, Switching turn back to Computer" << endl;
	//note the computer will just choose to roll one dice if anything less than 7 is possible.
	//Tell the player if they can Cover, Uncover or Both with their current set of choices.
	//then ask them to choose an option.

	//if the return vector for a choice is empty, it is not a valid choice and should not be allowed.
	//if the player has NO valid choices, the turn should end. It should be determined whether the current player
	// has won or not......
	//I have roll dice.... Now i need to ask if the player wants to Cover or Uncover
	//GetValidMoves if i want to cover my Tiles or if they want to cover their opponents tiles
	// if player has no valid moves- ie the valid moves for them
	return ;
}


/****************************************************************************************************
Function Name:bool Human::AskIfOneDice()

Purpose:ask for one Dice

Parameters: None

Return Value: none

Local Variables:

Algorithm:

Assistance Received: None

*****************************************************************************************************/
bool Human::AskIfOneDice()
{
	int userChoice = 0;
	do
	{
		//Validate input
		do
		{
			cout << "Please Enter 1 if you would like the option to play with a Single Die, else 2  for both Dice to be rolled" << endl;
			cout << "Generally, if you have the option, you only want to roll one Dice, unless you're only missing a 5 or 6" << endl;

		} while (!validateInput(userChoice));

	} while (userChoice != 1 && userChoice != 2);

	if (userChoice == 1)
		return true;
	else
		return false;
}

/****************************************************************************************************
Function Name:int Human::WhichMoveDoYouWant(int a_MaxPossible)

Purpose:ask user the number they want to make

Parameters: int max_possible

Return Value: none

Local Variables:

Algorithm:

Assistance Received: None

*****************************************************************************************************/
int Human::WhichMoveDoYouWant(int a_MaxPossible)
{
	int userChoice = 0;

	do
	{
		//Validate input
		do
		{
			cout << "Please enter which number you want. Make sure you pick a number from the list above  " << endl;

		} while (!validateInput(userChoice));


	} while (userChoice < 1 || userChoice > a_MaxPossible);

	return userChoice;
}


/****************************************************************************************************
Function Name:bool Human::AskIfCover(int a_rollResult, bool a_canMakeCoverMoves, bool a_canMakeUnCoverMoves)

Purpose:give the human a choice.

Parameters: int a_rollResult, bool a_canMakeCoverMoves, bool a_canMakeUnCoverMoves

Return Value: none

Local Variables:

Algorithm:

Assistance Received: None

*****************************************************************************************************/
bool Human::AskIfCover(int a_rollResult, bool a_canMakeCoverMoves, bool a_canMakeUnCoverMoves)
{
	int userChoice = 0;
	do
	{
		//Validate input
		do
		{
			cout << "You have the following Choices" << endl;
			if (a_canMakeCoverMoves)
			{
				cout << "Enter 1 if you want to Cover Your tiles" << endl;
			}
			if (a_canMakeUnCoverMoves)
			{
				cout << "Enter 2 if you want to Uncover your Opponent's Tiles" << endl;
			}

			if (userChoice != 3)
			{
				cout << "You can also ask the Computer for help by Entering 3" << endl;
			}

		} while (!validateInput(userChoice));

		//cout the Strategy Idea and then let the opponent Choose.
		if (userChoice == 3)
		{
			bool dummyResult;
			int targetVector;

			// no vector to return to? i guess thats ok....
			targetVector = Strategy(a_canMakeCoverMoves, a_canMakeUnCoverMoves, dummyResult);
			cout << "The rest is up to you" << endl;
		}

		//if the user tries to make an invalid move. Stop them and put them back in the loop......
		if (userChoice == 1 && !a_canMakeCoverMoves)
		{
				cout << "Thats not a valid choice right now, Lets try this again...." << endl;
				userChoice = 0;
		}
		if (userChoice == 2 && !a_canMakeUnCoverMoves)
		{
			cout << "Thats not a valid choice right now, Lets try this again...." << endl;
			userChoice = 0;
		}
		

	} while (userChoice != 1 && userChoice != 2);

	if (userChoice == 1)
		return true;
	else 
		return false;
}

Human::~Human()
{
}
