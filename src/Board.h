#pragma once
#include "HeaderList.h"
#include "PlayerSquare.h"
/********************************************
Name: Devinder Sodhi
Project: Canoga
Class: OPL cmps366
Date:
*********************************************/


/*Board Class holds all the  information about the rows of each player. Humand and Computer
*/

class Board
{
public:
	Board();
	~Board();

	//DiceThrow is used throughout the game. miht end up using Kumars thing later...so who the fuck knows.
	//void DiceThrow();


	
	//prepare he board for a gamestate given the input variables 
	void SetBoardVariables(vector<PlayerSquare> a_computerSquares, vector<PlayerSquare> a_humanSquares, int a_tilesetSize, string a_firstTurn, string a_nextTurn);
	
	//void resetSquareTiles();

//	void determineWhoGetsAdvantage();

	void ClearVectors();

	//get the Valid Moves
	vector<vector<int>> GetValidMoves(int a_rollSum, bool a_coverOrUncover);

	vector<PlayerSquare> * GetVectorOfInterest(bool a_coverOrUncover);
	
	void SubsetsForCalculation(vector<int> & a_vectorWithFullSet, int a_size, int a_targetVal);
	
	void sumOfSubset(vector<int>  a_vectorWithFullSet, vector<int> &  a_tupletVector,
		int a_setSize, int a_currentTupletSize, int a_sumSoFar,
		int a_nodesSoFar, int const a_targetVal);


	//set squares' numerical value
	void SetFirstTurn(string a_firstTurn)
	{
		m_firstTurn = a_firstTurn;
	}

	const string  GetFirstTurn()
	{
		return m_firstTurn;
	}

	const string GetNextTurn()
	{
		return m_nextTurn;
	}

	void SetNextTurn(string a_nextTurn)
	{

		m_nextTurn = a_nextTurn;
	}

	int  GetWinningScore()
	{
		return m_winningScore;
	}

	void DeliverAdvantage(string a_whoGetsAdvantage, int a_winningScore);

	vector<PlayerSquare> GetComputerSquares()
	{
		return m_computerSquares;
	}

	vector<PlayerSquare> GetHumanSquares()
	{
		return m_humanSquares;
	}
	void SetUsingDiceFile(bool a_usingDiceFile)
	{
		m_usingDiceFile = a_usingDiceFile;
	}

	const bool GetUsingDiceFile()
	{
		return m_usingDiceFile;
	}
	void SetStackOfDice(deque<int> a_stackOfDice)
	{
		m_diceFileContent = a_stackOfDice;
	}

	deque<int> * GetDiceFilePtr()
	{
		return &m_diceFileContent;
	}

	
	const bool GetisLoadedGame()
	{
		return m_isLoadedGame;
	}
	
	bool SetisLoadedGame(bool a_isLoadedGame)
	{
		m_isLoadedGame = a_isLoadedGame;
	}

	void SetGameCounter(int a_gameCounter)
	{
		m_gameCounter = a_gameCounter;
	}

	const int GetGameCounter()
	{
		return m_gameCounter;
	}
	


private:
	//master list of vectors to hold strategies
	vector <vector <int> > m_masterList;
	//size of the tileset
	int m_tilesetSize;
	//computer tileset
	vector<PlayerSquare> m_computerSquares;
	//human tileset
	vector<PlayerSquare> m_humanSquares;
	//first turn
	string m_firstTurn;
	//next turn
	string m_nextTurn;
	//track game progress up to 2 billion or so turns
	int m_gameCounter = -1;
	//used to determine blocked tile
	int m_extractThisNumber = 0;
	//determine if an advantage was had
	bool m_humanGetsAdvantage = false;
	bool m_computerGetsAdvantage = false;
	//score that was used to win
	int m_winningScore;
	//loaded game or new game
	bool m_isLoadedGame = false;
	//are we or awre we not
	bool m_usingDiceFile = false;
	//content of the dice file
	deque<int> m_diceFileContent;

};

