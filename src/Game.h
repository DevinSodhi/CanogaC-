#pragma once
/********************************************
Name: Devinder Sodhi
Project: Canoga
Class: OPL cmps366
Date:
*********************************************/
#include "PlayerSquare.h"
#include "HeaderList.h"
#include "Board.h"
#include "Player.h"
#include "Human.h"
#include "Computer.h"
#include "BoardView.h"

#include "HeaderList.h"
#include "Player.h"
#include "Human.h"
#include "Computer.h"

class Game
{
public:
	Game();


// if you play a new game. Should set this up.
	void IntializeNewRound(int a_tilesetSize, bool a_isNewGame, string a_lastRoundWinner = "");

	//initialize this round if you have a load file or are continuing a game
	void InitializeWithLoadedVariables();

	//take the load file variables and prepare game to start. Regardless of whether its a new game or an old
	bool CheckLoadFileAndPass();

	//record the variables in a Load file and if successful; pass them to "ReadyTheBoard"
	bool RecordVariablesAndPass(ifstream &datastream);

	//should generate a Round of Gameplay
	void PlayAGameRound();

	bool CheckForWinner(Player * a_PlayerBeingTested, bool & a_wonByCover, bool & a_wonByUncover);

	//For a continue game scenario. just figure out who won last round, and 
	string DetermineAdvantage(string a_whoWonLastRound, string a_whoWasFirstPlayer);

	void DeliverAdvantage(string a_ThisPlayerGetsAdvantage);

	//Build a vector for a game load
	void BuildVector(string a_inputString, vector<PlayerSquare> & a_dataVector);

	//determine who plays first at the beginning of a round. if loaded game, then just load variable. else roll a Dice
	string WhoGoesFirst(string & a_nextTurnPick);

	void GetBothScores(int & a_humanTotalScore, int & a_computerTotalScore);

	//string get opponent name
	string GetOpponentName(string a_entryString);

	//you only set this value from within the Game Class so it should NOT be accessible.
	string GetWhoWonTheRound()
	{
		if (m_whoWonTheRound == "")
		{
			cerr << "SomeHow The winner was not determined... Check your continue/playround sequence" << endl;
			exit(1);
		}
		else
		{
			return m_whoWonTheRound;
		}
	}

	void AnnounceWinner(Player * winnerReference, bool wonByCover);

	void SaveGame();
	void DetermineAndAddWinningScore();
	void AskIfDiceFile();

	int GetCounter()
	{
		return m_counter;
	}


	~Game();

private: 

	bool m_newOrContinue;
	int m_counter = 0;
	Board m_board;

	Human m_human;
	Computer m_computer;

	BoardView m_boardView;

	Player * m_PointerToHuman;
	Player * m_PointerToComputer;
	
	int m_winningScore = 0;

	//Set who won the round
	string m_whoWonTheRound= "";

	
};

