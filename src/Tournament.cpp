#include "Tournament.h"
#include "Helpers.h"

/**
This Class should basically just ask the player if they want to play a game (start or load)

Ask: Should i start a new 


NEW PLAN : New Game -> get new game stats, load one constructor of game
		Load gaame -> get load game stats, load the same constructor with those rules.
*/



/****************************************************************************************************
Function Name: Tournament::Tournament()

Purpose: constructor. starts the options

Parameters: None

Return Value: none

Local Variables: 

Algorithm:

Assistance Received:

*****************************************************************************************************/
Tournament::Tournament()
{
	
	GameOptions();
}
//start things off, ask the player if they want to start a new game or a Load a game.

/****************************************************************************************************
Function Name: Game Options

Purpose: At constructor time, Ask the player if they want to start a New Game, or Load a game.
		Validate input received, and depending on Condition Satisfied, Move on to start a New game or Load a game.

Parameters: None

Return Value: none

Local Variables: 1. int gameChoice => takes input data from user.

Algorithm:

Assistance Received: http://en.cppreference.com/w/cpp/io/basic_istream/ignore

*****************************************************************************************************/
void Tournament::GameOptions() 
{
	int userChoice = 0;
	cout << "****************************************************************************\n\n\n\n\n" << endl;
	//Take Input
	cout << "Welcome to Canoga!\n\n\n" << endl;
	do 
	{
		//Validate input
		do
		{
			cout << "Please enter 1 for New Game OR enter 2 to Load a file:  " << endl;

		} while (!validateInput(userChoice));

	} while (userChoice != 1 && userChoice != 2);

	//use validated input to trigger a New or Load Game action.
	if (userChoice == 1)
	{
		bool isNewGame = true;
		m_whoWonLastRound = "";
		NewRoundSetup(isNewGame, m_whoWonLastRound);
	}
	else 
	{
		LoadGameFromFileSetup();
	}

	//Play Game round. Offer continued GamePlay, set WhoWonLastRound

	//Game will trigger the round winner, so thats fine. But Tournament winner declared out here.. Else Play another round.

	//MAKE SURE TO DECLARE WHO WON LAST ROUND IF THE PLAYER CHOOSES TO PLAY ANOTHER GAME.

	bool continuePlaying = true;
	int continueGameChoice = 0;
	do
	{
		//1. First you let the player play through a Round of the Game
		m_GameEngine.PlayAGameRound();
		/************************************JUST GETTING INPUT AND VALIDATING IT***********************************************************/
		//2. Get confirmation on whether they'd like to keep playing or not
		do
		{
			do
			{
				cout << "Thats the end of that round, Would you like to Continue playing?" << endl;
				cout << " Type 1 for yes" << endl;
				cout << "Type 2 for no " << endl;
			} while (!validateInput(continueGameChoice));
		} while (continueGameChoice != 1 && continueGameChoice != 2);

		if (continueGameChoice == 1)
		{
			string whoWonLastRound;
			whoWonLastRound = m_GameEngine.GetWhoWonTheRound();
			NewRoundSetup(false, whoWonLastRound);
		}

		else
		{
			continuePlaying = false;
		}

	} while (continuePlaying);

	cout << "Well done Young One. Lets figure out your scores." << endl;

	int totalHumanScore;
	int totalComputerScore;
	m_GameEngine.GetBothScores(totalHumanScore, totalComputerScore);

	if (totalHumanScore == totalComputerScore)
	{
		cout << "The Human Score was: " << totalHumanScore  << endl;
		cout << "The Computer Score was: " << totalComputerScore << endl;
		cout << "THe Tournament was a draw. did you cheat???" << endl;
	}
	
	if (totalHumanScore < totalComputerScore)
	{
		cout << "The Human Score was: " << totalHumanScore << endl;
		cout << "The Computer Score was: " << totalComputerScore << endl;
		cout << "The Computer Won this round!, HA! Lost to a machine..... Shame on you" << endl;
	}

	if (totalHumanScore > totalComputerScore)
	{
		cout << "The Human Score was: " << totalHumanScore << endl;
		cout << "The Computer Score was: " << totalComputerScore << endl;
		cout << "Well, Well... Luck was on your side it seems! YOU WON! Congrats, Now see if you can get that time back...." << endl;
	}

	cout << "Thats all Folks. Have a good night and Thank you for Playing Canoga!!";
	system("PAUSE");
	exit(0);

	//get human and computer scores and compare them.
	// declare winner give the response
}

/****************************************************************************************************
Function Name: NewRoundSetup()

Purpose:	Start a New Game. This means prepare the beginning of the round, which requires the setting up of all the variables.
			COntinue game can also use this
Parameters: bool a_isNewGame is it a new game?
, string a_whoWonLastRound just check on winner of the last round for a continue game

Return Value: none

Local Variables: 1. int numberOfSquare => input that determines the size of the Tileset

Algorithm:

Assistance Received:

*****************************************************************************************************/
void Tournament::NewRoundSetup(bool a_isNewGame, string a_whoWonLastRound)
{
	int numberOfSquares = 0;
	do
	{
		do
		{
			cout << "Please Enter whether you'd like play with 9,10 or 11 squares." << endl;

		} while (!validateInput(numberOfSquares));


	} while (numberOfSquares !=9 && numberOfSquares !=10 && numberOfSquares !=11);

	//this is more of general idea solidification than it is necessary to do.
	m_sizeOfTileset = numberOfSquares;


	//Load a new game of specific size
	m_GameEngine.IntializeNewRound( m_sizeOfTileset, a_isNewGame, a_whoWonLastRound);

}



/****************************************************************************************************
Function Name: NewRoundSetup()

Purpose:	//Set game to Load and initialize. then Start round from current position
Parameters: 

Return Value: none

Local Variables: 
Algorithm:

Assistance Received:

*****************************************************************************************************/
void Tournament::LoadGameFromFileSetup()
{
	m_GameEngine.InitializeWithLoadedVariables();
	//load game variables as required. 
	//ply the round
}











Tournament::~Tournament()
{

}
