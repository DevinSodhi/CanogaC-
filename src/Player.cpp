#include "Player.h"


/****************************************************************************************************
Player::Player()
{

Purpose:	
Parameters: int & a_humanTotalScore, int & a_computerTotalScore



Return Value: none

Local Variables:


Algorithm: None

*****************************************************************************************************/
Player::Player()
{

}
/****************************************************************************************************
int Player::PerformDieRoll(bool diceFileExists)
{

Purpose:	roll dice 
Parameters bool diceFileExists
if the file exists, look up the data


Return Value: int theREsult that comes out of the die roll,

Local Variables:
int theREsult.

Algorithm: None

*****************************************************************************************************/
int Player::PerformDieRoll(bool diceFileExists)
{
	int theResult;
	cout << " A dice was rolled...";
	Sleep(250);
	deque<int> * diceList = m_boardPointer->GetDiceFilePtr();
	if (diceFileExists)
	{
		
		if (!diceList->empty())
		{
			cout << "from file" << endl;
			theResult = diceList->front();
			diceList->pop_front();
		}
		else
		{
			cout << " your die list is empty. switchng to manual mode" << endl;
			m_boardPointer->SetUsingDiceFile(false);
			theResult = 1 + rand() % 6;
		}
	}
	else
	{ 
	theResult =  1 + rand() % 6;
	}

	cout << "The result was: " << theResult << endl;
	return theResult;
}
/****************************************************************************************************
int Player::ResultOfDiceRoll()
{

Purpose:	grab both die and add them up
Parameters: 



Return Value: none

firstNum, secondNum, diceFileExists

Algorithm: None

*****************************************************************************************************/
int Player::ResultOfDiceRoll()
{
	bool diceFileExists = m_boardPointer->GetUsingDiceFile();
	int firstNum = PerformDieRoll(diceFileExists);
	int secondNum = PerformDieRoll(diceFileExists);

	int sum = firstNum + secondNum;
	
	cout << "The sum of the two dice was: " << sum << endl;
	return sum;
}
/****************************************************************************************************
void Player::AddToTotalScore(int a_winningScore)
{

Purpose:	add to total score
Parameters: int a_winningScore -> add this now



Return Value: none

Local Variables:

Algorithm: None

*****************************************************************************************************/
//going to assume you will know who to add this to by the time this Function is computed.
void Player::AddToTotalScore(int a_winningScore)
{
	m_totalScore = m_totalScore + a_winningScore;
}
/****************************************************************************************************
bool Player::AreBigNumsCovered(vector<PlayerSquare> a_inputTileSet)


Purpose:	check for big numbers...
Parameters: vector<PlayerSquare> a_inputTileSet



Return Value: bool => tell them whether the are covered or not

Local Variables:

Algorithm: None

*****************************************************************************************************/
bool Player::AreBigNumsCovered(vector<PlayerSquare> a_inputTileSet)
{
	bool bigNumsAreCovered = true;
	for (int i = 6; i < (int)a_inputTileSet.size(); i++)
	{
		if (!(a_inputTileSet.at(i).GetCoveredStatus()))
		{
			bigNumsAreCovered = false;
		}
	}

	return bigNumsAreCovered;
}




/****************************************************************************************************
void Game::GetBothScores(int & a_humanTotalScore, int & a_computerTotalScore)
{

Purpose:	 //the strategy will take into account the Valid Moves that one can get by either cover Or Uncover
//then the strategy will try to see if a win can be got
//if not, the strategy will choose the highest score maximizing opportunity.
//the strategy result will be:
/*
Cover Or UnCover
The Vector that results in the maximized score.
Each possible Choice will be preceded by a Reason Announcement.
reason announcemnt for cover/uncover -> no win, maximize score
reason announcement for vector -> maximizes the score

Parameters bool a_canMakeCoverMoves, bool a_canMakeUncoverMoves, bool & a_coverOrUncove

Return Value: none


Algorithm: defend first, choose the largest number of choices of squares, choose teh highest numbers if there is a tie

*****************************************************************************************************/
int Player::Strategy(bool a_canMakeCoverMoves, bool a_canMakeUncoverMoves, bool & a_coverOrUncover)
{
	int targetVector;

	if (a_canMakeCoverMoves)
	{
		cout << "I believe in a defensive Strategy." << endl;
		cout << "Since its possible to Cover your own numbers, Lets go with that" << endl;
		a_coverOrUncover = true;

		cout << " you want the largest number of choices" << endl;
		cout << "and if all options are the same choice, then you want the largest among them." << endl;
		int largestVectorSize = 0;
		for (int i = m_validCoverMoves.size()- 1; i >= 0; i--)
		{
			if ((int)m_validCoverMoves.at(i).size() > largestVectorSize)
			{
				largestVectorSize = (int)m_validCoverMoves.at(i).size();
				targetVector = i;
			}
		}

		cout << "So you want to use choice number" << targetVector + 1 << endl;
		cout << "this means you COVER the tiles: ";
		for (int j = 0; j < (int)m_validCoverMoves.at(targetVector).size(); j++)
		{
			cout << (int)m_validCoverMoves.at(targetVector).at(j) << " ";
		}

		return targetVector;	
	}
	else
	{
		cout << "There is no defensive option thats feasable. Will uncover opponent Tiles while we can." << endl;
		a_coverOrUncover = false;

		cout << " you want the largest number of choices- and if all options are the same choice, then you want the largest among them." << endl;

		int largestVectorSize = 0;
		for (int i = m_validUncoverMoves.size() - 1; i >= 0; i--)
		{
			if ((int)m_validUncoverMoves.at(i).size() > largestVectorSize)
			{
				largestVectorSize = (int)m_validUncoverMoves.at(i).size();
				targetVector = i;
			}
		}

		cout << "So you want to use choice number" << targetVector + 1 << endl;
		cout << "this means you UNCOVER the tiles: ";
		for (int j = 0; j < (int)m_validUncoverMoves.at(targetVector).size(); j++)
		{
			cout << (int)m_validUncoverMoves.at(targetVector).at(j) << " ";
		}

		return targetVector;
	
	}

}
/****************************************************************************************************
bool Player::ValidMovesExist(int a_diceRoll, bool & a_validCoverMoves, bool & a_validUnCoverMoves)
{

Purpose:	
Parameters:int a_diceRoll, bool & a_validCoverMoves, bool & a_validUnCoverMoves


Return Value: none

Local Variables:
valifcovermoves exist

Algorithm: None

*****************************************************************************************************/

//sets the appropriate vectors with valid Cover and Uncover moves.
bool Player::ValidMovesExist(int a_diceRoll, bool & a_validCoverMoves, bool & a_validUnCoverMoves)
{
	bool validMovesExist = false;
	a_validCoverMoves = false;
	a_validUnCoverMoves = false;

	m_validCoverMoves = m_boardPointer->GetValidMoves(a_diceRoll, true);
	m_validUncoverMoves = m_boardPointer->GetValidMoves(a_diceRoll, false);

	if (!(m_validCoverMoves.empty()))
	{
		validMovesExist = true;
		a_validCoverMoves = true;
	}

	if (!(m_validUncoverMoves.empty()))
	{
		validMovesExist = true;
		a_validUnCoverMoves = true;
	}

	return validMovesExist;
}

/****************************************************************************************************
Player() destructors

Purpose:	grab both scorews
Parameters: int & a_humanTotalScore, int & a_computerTotalScore
Return Value: none

Local Variables:

Algorithm: None

*****************************************************************************************************/
Player::~Player()
{
}
