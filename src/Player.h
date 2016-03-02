#pragma once
#include "Board.h"
#include "HeaderList.h"
#include "PlayerSquare.h"
#include "BoardView.h"
/********************************************
Name: Devinder Sodhi
Project: Canoga
Class: OPL cmps366
Date:
*********************************************/

class Player
{
public:
	Player();

	//Either the player or the computer will make a move depending on what they need to do
	//A return of True will Signal a WIN in the game
	virtual void MakeMove() = 0;

	bool AreBigNumsCovered(vector<PlayerSquare> a_inputTileSet);
	//the PLayer will Roll a Dice and return the value
	//void SingleDiceRoll(int & a_diceResult) = 0;

//	void TwoDiceRoll(int &a_firstResult, int &a_secondResult) = 0;

	//
	int PerformDieRoll(bool diceFileExists);
	int ResultOfDiceRoll();



	string GetName()
	{
		return m_whichPlayer;
	}

	//make this more resiliant down the line
	bool SetTotalScore(int a_theScore) 
	{
		m_totalScore = a_theScore;
		return true;
	}

	int GetTotalScore()
	{
		return m_totalScore;
	}

	void AddToTotalScore(int a_winningScore);


	int Strategy(bool a_canMakeCoverMoves, bool a_canMakeUncoverMoves, bool & a_coverOrUncover);

	bool ValidMovesExist(int a_diceRoll, bool & a_validCoverMoves, bool & a_validUnCoverMoves);

	//bool CheckForWinner();

	
	bool GetHasCovered()
	{
		return m_hasCovered;
	}

	bool GetHasUnCovered()
	{
		return m_hasUncovered;
	}

	void SetHasCovered(bool a_hasCovered)
	{
		m_hasCovered = a_hasCovered;
	}
	void SetHasUnCovered(bool a_hasUnCovered)
	{
		m_hasUncovered = a_hasUnCovered;
	}

	virtual ~Player();
protected:
	// covered choice
	bool m_hasCovered = false;
	//uncovered choice
	bool m_hasUncovered = false;
	//what player is covered
	string m_whichPlayer ="Player";
	//board needs to be got
	Board * m_boardPointer;
	//a pointer to the view
	BoardView * m_boardViewPointer;
	// total score
	int m_totalScore = 0;

	//Once You Check Valid Moves, you know ->
	vector<vector<int>> m_validCoverMoves;
	//valid move set
	vector<vector<int>> m_validUncoverMoves;
	//player square pointer...
	vector<PlayerSquare> * m_vectorIWantToPlayWith;

  };

