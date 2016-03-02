#include "Game.h"


/**
The Game class should allow control of the board, and playing of a round.
The control will allow manipulation of the Score and the Game so that the game can be set to a New, a Load, or a "Play another Round" state.
*/

/****************************************************************************************************
Function Name: Game()

Purpose:	constructor for the game. Initialize the paraemters and seed the time
Parameters:  none, but a constructor list exists

Return Value: none

Local Variables: 

Algorithm: intializer list will always set the appropriate pointers for board/boardview

Assistance Received:

*****************************************************************************************************/
Game::Game() : m_board(), m_boardView(&m_board), m_human(&m_board, &m_boardView), m_computer(&m_board, &m_boardView)
{
	srand(static_cast<unsigned int>(time(NULL)));
	m_PointerToHuman = &m_human;
	m_PointerToComputer = &m_computer;

	AskIfDiceFile();
}

/****************************************************************************************************
Function Name: void Game::AskIfDiceFile()

Purpose:	prepare dicefile deque if the option is set to true.
Parameters:  none

Return Value: none

Local Variables:

Algorithm: none

Assistance Received:

*****************************************************************************************************/
void Game::AskIfDiceFile()
{
	int userChoice = 0;
	do
	{
		//Validate input
		do
		{
			cout << "Please enter 1 if you want to load a dicefile 2 if not." << endl;

		} while (!validateInput(userChoice));

	} while (userChoice != 1 && userChoice != 2);

	
	if (userChoice == 1)
	{
		m_board.SetUsingDiceFile(true);
		//Check to see if File name is Correct
		string loadFile;
		cout << "Enter the Dice File name (this should be the name of your txt file. Please Include .txt at the end) :" << endl;

		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, loadFile);
		ifstream dataStream(loadFile);

		//if datastream fails, exit
		if (!dataStream)
		{
			cerr << "Wrong file name. To keep you from getting in a loop, Ending program" << endl;
			exit(1);
		}

		string oneLineOfCodedMessage;
		int oneDieRoll;
		deque <int> myListOfDie;
		//grab a line of stringstream and read it piece by piece. do this until the end of file
		while (!dataStream.eof())
		{
			getline(dataStream, oneLineOfCodedMessage);
			istringstream lineStream(oneLineOfCodedMessage);
			

			while (lineStream >> oneDieRoll)
			{

				myListOfDie.push_back(oneDieRoll);
			}
		}
		// read in the data
		m_board.SetStackOfDice(myListOfDie);

	}

	if (userChoice == 2) {
		m_board.SetUsingDiceFile(false);
	}
	return;
	//PUT it in here
}


/****************************************************************************************************
Function Name: void Game::InitializeWithLoadedVariables()

Purpose:	intialize with load variables. Decided to chain the functions to separate tasks but keep the function simple
Parameters:  none, 

Return Value: none

Local Variables:

Algorithm: 

Assistance Received:

*****************************************************************************************************/
void Game::InitializeWithLoadedVariables()
{
	CheckLoadFileAndPass();
}

/****************************************************************************************************
Function Name: void Game::IntializeNewRound(int a_inputTileset, bool a_isNewGame, string a_whoWonLastRound)
Purpose:	initialize a new round (continue or new)
Parameters: int a_inputTileset=>  size of tileset
			bool a_isNewGame=> new or continue game
			string a_whoWonLastRound => check to see who won the previous round

Return Value: none

Local Variables: string a_nextTurn
				string a_firstTurn
				vector<PlayerSquare> a_computerSquares;
				vector<PlayerSquare> a_humanSquares; temporary VARIABLES for loading and filling information


Algorithm:

Assistance Received:

*****************************************************************************************************/
void Game::IntializeNewRound(int a_inputTileset, bool a_isNewGame, string a_whoWonLastRound)
{
	/*****************************************************************************************************/
	//1. Get the tileset from the input
	int a_tilesetSize = a_inputTileset;
	string a_nextTurn;
	string a_firstTurn;
	m_newOrContinue = true;
	m_counter = 0;
	m_board.SetGameCounter(0);

	//1.a clear out the old vectors if there are any.
	m_board.ClearVectors();
	m_human.SetHasCovered(false);
	m_human.SetHasCovered(false);

	//built out a new vector of teh appropriate size with the proper size of the filled position at the end....
	vector<PlayerSquare> a_computerSquares;
	vector<PlayerSquare> a_humanSquares;
	for (int i = 0; i < a_inputTileset; i++)
	{
		PlayerSquare playersquare;
		//playersquare.SetNumericalValue(i + 1);
		playersquare.SetCoveredStatus(false);

		a_computerSquares.push_back(playersquare);

		PlayerSquare playersquare2;
		//playersquare2.SetNumericalValue(i + 1);
		playersquare2.SetCoveredStatus(false);

		a_humanSquares.push_back(playersquare2);
	}
	if (!a_isNewGame)
	{
		//Before we set the board, we must grab the information we need from the last round. Specifically, this is "who had first turn"
		//WE need this, and who won last round, to find out who gets the advantage next turn.
		string  whoHadFirstTurn = m_board.GetFirstTurn();

		//2. Now we setup for the next round.
		a_firstTurn = WhoGoesFirst(a_nextTurn);

		//at this point, we're in a position to Set the Board to its New Variables for the new round.
		m_board.SetBoardVariables(a_computerSquares, a_humanSquares, a_tilesetSize, a_firstTurn, a_nextTurn);

		//after the board is Loaded, we can apply the advantage. The requirements are: Must be a ContinueGame.
		//What if SOMEHOW the last game was a draw? don't need to worry about it because such a situation cannot exist.

		//anyways, we now need to Determine and apply the advantage.
		
		//get this information an hold on to it until we have a Board thats setup.
		string whoGetsAdvantage = DetermineAdvantage(a_whoWonLastRound, whoHadFirstTurn);

		//Now apply the advantage to the Board Directly. It needs two variables. 
		//The winning Score - which must exist because a round was played previously and this variable was not discarded.
		//the string determining who gets the advantage.

		m_board.DeliverAdvantage(whoGetsAdvantage, m_winningScore);
		m_board.SetGameCounter(0);
	}
	else
	{
		//2. Now we setup for the next round.
		a_firstTurn = WhoGoesFirst(a_nextTurn);

		//This sets the BoardVariables to the ones in the file.
		m_board.SetBoardVariables(a_computerSquares, a_humanSquares, a_tilesetSize, a_firstTurn, a_nextTurn);

		m_winningScore = 0;
		m_human.SetTotalScore(0);
		m_computer.SetTotalScore(0);
	}
}

/****************************************************************************************************
Function Name: string Game::DetermineAdvantage(string a_whoWonLastRound, string a_whoWasFirstPlayer)
Purpose:	calculates who gets the adantage
Parameters: string a_whoWonLastRound, 
string a_whoWasFirstPlayer self explanatory

Return Value: none

Local Variables: string advantageGoesToThisGuy; tracks whot hte advantage goes to.


Algorithm: check first player. Check who won.

Assistance Received:

*****************************************************************************************************/
string Game::DetermineAdvantage(string a_whoWonLastRound, string a_whoWasFirstPlayer)
{
	string advantageGoesToThisGuy;
	if (a_whoWonLastRound == a_whoWasFirstPlayer)
	{
		 advantageGoesToThisGuy = GetOpponentName(a_whoWonLastRound);
	}
	else
	{
		advantageGoesToThisGuy = a_whoWonLastRound;
	}

	cout << advantageGoesToThisGuy << " gets the Advantage...";
	return advantageGoesToThisGuy;

	//Now the Board should deliver the advantage to the one who gained. it . 
}

/****************************************************************************************************
Function Name: string Game::GetOpponentName(string a_entryString)
Purpose:	just gets teh opponent of the input name
Parameters: string a_entryString
string a_whoWasFirstPlayer self explanatory

Return Value: none

Local Variables: string advantageGoesToThisGuy; tracks whot hte advantage goes to.


Algorithm: check first player. Check who won.

Assistance Received:

*****************************************************************************************************/
string Game::GetOpponentName(string a_entryString)
{
	if (a_entryString == "Human")
	{
		return "Computer";
	}
	else return "Human";
}

/****************************************************************************************************
Function Name: WhoGoesFirst(string & a_nextTurnPick)

Purpose:	this function sets the groundwork to picking "FirstPlayer" and "Next Turn" for a new game
Parameters: a_nextTurnPick Easy to just send this back to the board.

Return Value: string => this is the FirstPlayer that is chosen at the end of this selection.

Local Variables: 1. thisWillBeFirstPlayer => the returned value
				 2. yourNumber => the dice value for player one
				 3. computerNumber => the dice value for Computer

Algorithm:

Assistance Received:

*****************************************************************************************************/
string Game::WhoGoesFirst(string & a_nextTurnPick) {
	string thisWillBeFirstPlayer;
	int yourNumber = 0;
	int computerNumber = 0;

	while (yourNumber == computerNumber) {
		cout << "Need to select who goes first" << endl;

		cout << "Rolling  a Dice On Behalf of Player....." << endl;
		yourNumber = m_PointerToHuman->ResultOfDiceRoll();
		Sleep(500);

		cout << "Rolling  a Dice on Behalf of the Computer....." << endl;
		computerNumber = m_PointerToComputer->ResultOfDiceRoll();
		Sleep(500);
		if (yourNumber > computerNumber)
		{
			cout << "You won, and Get first turn" << endl;
			system("PAUSE");
			a_nextTurnPick = "Human";//bigChange
			return (thisWillBeFirstPlayer = "Human");
		}

		if (yourNumber < computerNumber)
		{
			cout << "Computer won, and Gets first turn" << endl;
			system("PAUSE");
			a_nextTurnPick = "Computer";
			return (thisWillBeFirstPlayer = "Computer");
		}
	}
	return "badStuffHappened";
}
/****************************************************************************************************
Function Name: CheckLoadFileAndPass()

Purpose:	test the name of the load file and pass the variables to the Board Class
Parameters: None

Return Value: none

Local Variables: 1. string loadFile => file name to test
				 2. ifstream dataStream => data stream containing the relevant file information
				 3. boolean successfulRead => the boolean testing the read success of the RecordVariablesAndPass

Algorithm:

Assistance Received:

*****************************************************************************************************/
bool Game::CheckLoadFileAndPass()
{
	//Check to see if File name is Correct
	string loadFile;
	cout << "Enter the File name (this should be the name of your txt file. Please Include .txt at the end) :" << endl;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, loadFile);
	ifstream dataStream(loadFile);

	//if datastream fails, exit
	if (!dataStream)
	{
		cerr << "Wrong file name. To keep you from getting in a loop, Ending program" << endl;
		exit(1);
	}

	bool successfulRead = false;
	successfulRead = RecordVariablesAndPass(dataStream);

	//if can't read the file properly - exit
	if (!successfulRead)
	{
		cerr << " Error reading file" << loadFile << endl;
		exit(1);
	}
	else
	{
		return true;
	}

}

/****************************************************************************************************
Function Name: RecordVariablesAndPass()

Purpose:	records all the variables from the load file.
Parameters: 1. ifstream &a_dataStream => a reference to the stream which will read us the now (partially) validate file.

Return Value: none

Local Variables: (self explanatory)
	1. int a_tilesetSize 
	2. vector<PlayerSquare> a_computerSquares
	3. int a_computerScore
	4. vector<PlayerSquare> a_humanSquares
	5. int a_humanScore
	6. string a_firstTurn
	7. string a_nextTurn
	8. string oneLineOfCodedMessage
	9. string oneStreamNugget

Algorithm:

Assistance Received: http://stackoverflow.com/questions/2323929/istringstream-how-to-do-this

*****************************************************************************************************/
bool Game::RecordVariablesAndPass(ifstream &a_dataStream) 
{
	int a_tilesetSize;
	vector<PlayerSquare> a_computerSquares;
	int a_computerScore;
	vector<PlayerSquare> a_humanSquares;
	int a_humanScore;
	string a_firstTurn;
	string a_nextTurn;

	//Snce its a load game, wins are Fair Game
	m_human.SetHasCovered(true);
	m_human.SetHasCovered(true);

	string oneLineOfCodedMessage;
	string oneStreamNugget;
	//grab a line of stringstream and read it piece by piece. do this until the end of file
	while (!a_dataStream.eof())
	{
		getline(a_dataStream, oneLineOfCodedMessage);
		istringstream datastream(oneLineOfCodedMessage);

		datastream >> oneStreamNugget;

		// validated the first computer batch data while also picking up the number of tiles/squares
		if (oneStreamNugget == "Computer:")
		{
			getline(a_dataStream, oneLineOfCodedMessage);
			BuildVector(oneLineOfCodedMessage, a_computerSquares);
			a_tilesetSize = a_computerSquares.size();

			getline(a_dataStream, oneLineOfCodedMessage);
			istringstream scoreDataStream(oneLineOfCodedMessage);
			scoreDataStream >> oneStreamNugget;
			if (oneStreamNugget == "Score:")
			{
				scoreDataStream >> oneStreamNugget;
				a_computerScore = atoi(oneStreamNugget.c_str());
				continue;
			}
		}
		// validated the human batch data while also picking up the number of tiles/squares
		if (oneStreamNugget == "Human:")
		{
			getline(a_dataStream, oneLineOfCodedMessage);
			BuildVector(oneLineOfCodedMessage, a_humanSquares);

			getline(a_dataStream, oneLineOfCodedMessage);
			istringstream scoreDataStream(oneLineOfCodedMessage);
			scoreDataStream >> oneStreamNugget;
			if (oneStreamNugget == "Score:")
			{
				scoreDataStream >> oneStreamNugget;
				a_humanScore = atoi(oneStreamNugget.c_str());
				continue;
			}
		}

		if (oneStreamNugget == "First")
		{
			datastream >> oneStreamNugget;
			datastream >> oneStreamNugget;
			a_firstTurn = oneStreamNugget;
		}

		if (oneStreamNugget == "Next")
		{
			datastream >> oneStreamNugget;
			datastream >> oneStreamNugget;
			a_nextTurn = oneStreamNugget;
		}
//should have downloaded the load file properlby now.

	}

	//Set the player scores
	m_computer.SetTotalScore(a_computerScore);
	m_human.SetTotalScore(a_humanScore);
	//This sets the BoardVariables to the ones in the file.
	m_board.SetBoardVariables(a_computerSquares, a_humanSquares, a_tilesetSize, a_firstTurn, a_nextTurn);

	return true;
}

/****************************************************************************************************
Function Name: BuildVector()

Purpose:	Build out the vectors that will be used as tilesets for each player. This is only necessary for load giles
Parameters:
	1. a_vectorString => this string has all the data that will populate the vector
	2. vector<PlayerSquare> & a_dataVector => A reference to the vector that needs to be populated

Return Value: none

Local Variables: 
	1. string aSingleElement => used for reading each component of the stream
	2. istringstream turnThisIntoAVector => data stream containing the relevant line information from the a_vectorString parameter
	3. Playersquare playerSquare => the copyable player square that gets properly instantiated and assigned, and then copied into the vector

Algorithm:

Assistance Received: http://stackoverflow.com/questions/7382090/c-vector-push-back
*****************************************************************************************************/
 void Game::BuildVector(string a_vectorString, vector<PlayerSquare>& a_dataVector)
{

	string aSingleElement;
	istringstream turnThisIntoAVector(a_vectorString);
	turnThisIntoAVector >> aSingleElement;

//	cout << aSingleElement;

	if (!(aSingleElement == "Squares:"))
	{
		cout << "dude.. its fine" << endl;
		exit(1);
	}

	int counter = 1;

	while (turnThisIntoAVector >> aSingleElement) 
	{
		PlayerSquare playerSquare;
//		playerSquare.SetNumericalValue(counter);

		if (aSingleElement == "*")
		{
			playerSquare.SetCoveredStatus(true);
		}
		else
		{
			playerSquare.SetCoveredStatus(false);
		}
		a_dataVector.push_back(playerSquare);
		counter++;
	}

	

}

 /****************************************************************************************************
 Function Name:  Game::CheckForWinner(Player * a_PlayerBeingTested, bool & a_wonByCover, bool & a_wonByUncover)

 Purpose:	determine winner.
 Parameters:
1. Player * a_PlayerBeingTested -> Pointer to "current Player"
2; bool & a_wonByCover, bool & a_wonByUncover -> see if player won by cover or Uncover

 Return Value: none

 Local Variables:
 1. string aSingleElement => used for reading each component of the stream
 2. istringstream turnThisIntoAVector => data stream containing the relevant line information from the a_vectorString parameter
 3. Playersquare playerSquare => the copyable player square that gets properly instantiated and assigned, and then copied into the vector

 Algorithm:

 Assistance Received: http://stackoverflow.com/questions/7382090/c-vector-push-back
 *****************************************************************************************************/
 bool Game::CheckForWinner(Player * a_PlayerBeingTested, bool & a_wonByCover, bool & a_wonByUncover)
 {
	 
	 vector<PlayerSquare> * vectorIWantToPlayWith;
	 bool wonInSomeWay = false;
	 bool wonByCovering = true;
	 bool wonByUncovering = true;
	 // Check to see if I covered all my own...
	 bool a_coveredOrNot = true;
	 
	 if (a_PlayerBeingTested->GetHasCovered())
	 {
		 
		 vectorIWantToPlayWith = m_board.GetVectorOfInterest(a_coveredOrNot);

		 for (int i = 0; i < (int)vectorIWantToPlayWith->size(); i++)
		 {
			 //if one of the tiles is uncovered, you shouldn't have a winner
			 if (!(vectorIWantToPlayWith->at(i).GetCoveredStatus()))
			 {
				 wonByCovering = false;
			 }
		 }
		 //requires that two conditions be met. 1. you have covered. 2. you won by covering
		 if (wonByCovering)
		 {
			 wonInSomeWay = true;
		 }
	 }
	 a_coveredOrNot = false;

	 if (a_PlayerBeingTested->GetHasUnCovered())
	 {
		 vectorIWantToPlayWith = m_board.GetVectorOfInterest(a_coveredOrNot);

		 for (int i = 0; i < (int)vectorIWantToPlayWith->size(); i++)
		 {
			 //if one of the tiles is covered you shouldn't have a winner
			 if ((vectorIWantToPlayWith->at(i).GetCoveredStatus()))
			 {
				 wonByUncovering = false;
			 }
		 }
		 if (wonByUncovering)
		 {
			 wonInSomeWay = true;
		 }
	 }

	 //the game is at round 0, No winner should be possible.
	 if (m_newOrContinue && m_counter < 1)
	 {
		 wonInSomeWay = false;
	 }
	 a_wonByCover = wonByCovering;
	 a_wonByUncover = wonByUncovering;

	 return wonInSomeWay;

 }

 /****************************************************************************************************
 Function Name:  me::PlayAGameRound() 
 {

 Purpose:	PLAY A  ROUND OF THE GAME..
 Parameters:


 Return Value: none

 Local Variables:
 Player *currentPlayer; ->represents the current player.
 string thisGuyHasNextTurn
 string playerForThisTurn
 coveroruncover, wonByCover, wonByUncover

 Algorithm: Check win/PickTurn player/ play turn/ offer save.

 *****************************************************************************************************/
 void Game::PlayAGameRound() 
 {

	 cout << "This is the beginning State of your Board \n\n\n" << endl;
	// m_boardView.getCurrentBoardView();
	 Player *currentPlayer;

	 //we need the previous player. which should be the Opposite of the "Next Player".
	 //we need to know who won.
	 /*********************************************************************************/
	 string thisGuyHasNextTurn = m_board.GetNextTurn();
	 string playerForThisTurn = GetOpponentName(thisGuyHasNextTurn);

	 if (playerForThisTurn == "Human")
	 {
		 currentPlayer = &m_human;
	 }
	 else if (playerForThisTurn == "Computer")
	 {
		 currentPlayer = &m_computer;
	 }

	 /*******************************We should have the Previous Player now.**********************************************/

	 bool aWinnerExists = false;
	 bool wonByCover;
	 bool wonByUncover;
	 do 
	 {
		 /* Initialization happens first.*/


		 /*A turn Starts.*
		 
		 Step 1. Check for winners. Make sure if its the first 2 turns, Deny a win.
			Reason 1.if game checks this before anyone plays, no winner should be declared
			Reason 2.if I play round one and cover my squares. Game, checks for winners and gives win by uncover.
		 */        
		 aWinnerExists = CheckForWinner(currentPlayer, wonByCover, wonByUncover);
		 /**********************************DO WHAT NEEDS TO BE DONE***************************************************************/

		 if (aWinnerExists)
		 {
			 bool coverOrUncover;
			 if (wonByCover)
			 {
				 coverOrUncover = true;
			 }
			 else if (wonByUncover)
			 {
				 coverOrUncover = false;
			 }

			 //Send in the Pointer to the Current Player and whetherTheyWon by Covering or Uncovering
			 AnnounceWinner(currentPlayer, coverOrUncover);
			 return;
		 }


		 /**************************************************************************************/
		 // we need to make the switch to get the next player.
		 // the next player will become the effective CURRENT player for the rest of this round.

		 //First Swap who the Next player will be to the new Next Player
		 m_board.SetNextTurn(GetOpponentName(m_board.GetNextTurn()));
		
		 //Get the name of the player who has next turn
		 string thisGuyHasNextTurn = m_board.GetNextTurn();

		 //The Player for this turn will be the Guy who is the opposite of the next turn guy.
		 string playerForThisTurn = GetOpponentName(thisGuyHasNextTurn);

		 //bigChange

		 //get a pointer to the current player
		 if (playerForThisTurn == "Human")
		 {
			 currentPlayer = &m_human;
		 }
		 else if (playerForThisTurn == "Computer")
		 {
			 currentPlayer = &m_computer;
		 }
		 //find out if there is a winner while letting the Current player make a move
		 currentPlayer->MakeMove();

		 //validate input, and check if user wants to save game..
		 int userChoice;
		 if (m_newOrContinue)
		 {
			 m_counter++;
			 
			 m_board.SetGameCounter(m_counter);
		 }
		 else
		 {
			 m_counter = 3;
		 }
		 if (m_counter > 2)
		 {
			 do
			 {
				 do
				 {
					 cout << "Please enter 1 to save your game progress (and exit) OR enter 2 to continue..." << endl;

				 } while (!validateInput(userChoice));

			 } while (userChoice != 1 && userChoice != 2);

			 if (userChoice == 1)
			 {
				 SaveGame();
			 }
		 }
		// m_board.SetNextTurn(playerForThisTurn);
		//even if the other check fails somehow. This should not.
	 } while (!aWinnerExists);
	 //This my friends, is where the Magic will happen.

	 //Announce Winner. Set Winning Score. Give Winning Score to 
 }

 /****************************************************************************************************
 void Game::AnnounceWinner(Player * a_winningPlayer, bool wonByCover)
 {

 Purpose:	Announce Winner. You know who won. and how....
 Parameters: Player * a_winningPlayer,
 bool wonByCove


 Return Value: none

 Local Variables:
 vectorOfInterest winner player's 
 didWinByCover

 Algorithm: None

 *****************************************************************************************************/
 void Game::AnnounceWinner(Player * a_winningPlayer, bool wonByCover)
 {

	 /*
	 I know a Player Won and I know how they won...
	 */
	 
	 /*Start with a score of 0. Also Grab the vector you care about.
			This will work because the Round is still set to "Previous Player"
	 */

	 vector<PlayerSquare> * vectorOfInterest;
	 int score = 0;
	 
	 //1. Determine if the player won by Covering or Uncovering
	 bool didWinByCover = wonByCover;
	 
	 //If the Player won by Cover. They grab the opponents Vector, Then for every uncovered tile, add to Score
	 if (didWinByCover)
	 {
		 vectorOfInterest = m_board.GetVectorOfInterest(false);

		 for (int i = 0; i < (int)vectorOfInterest->size(); i++)
		 {
			 if (!vectorOfInterest->at(i).GetCoveredStatus())
			 {
				 score += i + 1;
			 }
		 }
	 }
	 else
		 //If the Player won by Uncover, they grab their own tileset, and add up all the Covered to get the score
	 {
		 vectorOfInterest = m_board.GetVectorOfInterest(true);

		 for (int i = 0; i < (int)vectorOfInterest->size(); i++)
		 {
			 if (vectorOfInterest->at(i).GetCoveredStatus())
			 {
				 score += i + 1;
			 }
		 }
	 }

	 //We have the score. We need to do a few things with it.

	 //1. set the winning score and winner name in Game incase the player wants to continue Game
	 m_winningScore = score;
	 m_whoWonTheRound = a_winningPlayer->GetName();

	 //Add to the winning players score, the Score they have earned.
	 int addIt = a_winningPlayer->GetTotalScore() + m_winningScore;
	 a_winningPlayer->SetTotalScore(addIt);
	
	 //The Last thing to do is to Announce the Winner.
	 cout << "THIS ROUND IS OVER!" << endl;
	 cout << "Congratulations to the " << m_whoWonTheRound << " FOR WINNING THIS ROUND!" << endl;
	 cout << " THE WINNING SCORE OF " << m_winningScore << " WILL BE ADDED TO THE " << m_whoWonTheRound << "'s TOTAL SCORE!" << endl;
 }


 /****************************************************************************************************
  void Game::GetBothScores(int & a_humanTotalScore, int & a_computerTotalScore)
 {

 Purpose:	grab both scorews
 Parameters: int & a_humanTotalScore, int & a_computerTotalScore



 Return Value: none

 Local Variables:
string nameOfFile
ofstream writeWithThis
just names of people 

 Algorithm: None

 *****************************************************************************************************/
 void Game::GetBothScores(int & a_humanTotalScore, int & a_computerTotalScore)
 {
	 a_humanTotalScore = m_PointerToHuman->GetTotalScore();
	 a_computerTotalScore = m_PointerToComputer->GetTotalScore();
 }

 /****************************************************************************************************
 void Game::SaveGame()
 {

 Purpose:	save the game.
 Parameters: 


 Return Value: none

 Local Variables:
 string nameOfFile
 ofstream writeWithThis
 just names of people

 Algorithm: None

 *****************************************************************************************************/
 void Game::SaveGame()
 {
	 string nameOfFile;
	 cout << "My friend. For you, file I save." << endl;
	 cout << "just give me valid file name: (make sure you put .txt at end:  ";
	 cin.ignore(numeric_limits<streamsize>::max(), '\n');


	 getline(cin, nameOfFile);
	 ofstream writeWithThis(nameOfFile);

	 while (!writeWithThis)
	 {
		 cout << "There was an error in saving the file. Try one more time.. with a valid file name " << endl;
		 cin.ignore(numeric_limits<streamsize>::max(), '\n');
		 getline(cin, nameOfFile);
		 writeWithThis.open(nameOfFile);
	 }

	 writeWithThis << "Computer:\n";
	 writeWithThis << "\t Squares:";

	 vector<PlayerSquare> computerData = m_board.GetComputerSquares();

	 for (int i = 0; i < (int)computerData.size(); i++)
	 {
		 writeWithThis << " ";
		 if (computerData.at(i).GetCoveredStatus() == true)
		 {
	
			 writeWithThis << "*";
		 }
		 else
		 {
			 writeWithThis << i + 1;
		 }
	 }
	 writeWithThis << "\n";
	 writeWithThis << "\t Score: ";
	 writeWithThis << m_computer.GetTotalScore();
	 writeWithThis << "\n\n";

	 writeWithThis << "Human: \n";
	 writeWithThis << "\t Squares:";

	 vector<PlayerSquare> humanData = m_board.GetHumanSquares();

	 for (int i = 0; i < (int)humanData.size(); i++)
	 {
		 writeWithThis << " ";
		 if (humanData.at(i).GetCoveredStatus() == true)
		 {

			 writeWithThis << "*";
		 }
		 else
		 {
			 writeWithThis << i + 1;
		 }
	 }

	 writeWithThis << "\n";
	 writeWithThis << "\t Score: ";
	 writeWithThis << m_human.GetTotalScore();
	 writeWithThis << "\n\n";

	 writeWithThis.close();

	 cout << " Your file has been saved. GoodBye! " << endl;
	 exit(0);


 }

Game::~Game()
{
}
