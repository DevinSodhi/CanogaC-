#include "Board.h"


/****************************************************************************************************
Function: Board()


Purpose:	Constructor. -> all private variables are taken care of.
Parameters:
none

Return Value: none

Local Variables: 


Algorithm: none

Assistance Received: none

*****************************************************************************************************/
Board::Board()
{
}
Board::~Board()
{
}

/****************************************************************************************************
Function Name: void Board::ClearVectors()


Purpose:	Clears human and computer vectors for a new round
Parameters:
none

Return Value: none

Local Variables: (self explanatory members.)

Algorithm: none

Assistance Received: none

*****************************************************************************************************/
void Board::ClearVectors()
{
	m_computerSquares.clear();
	m_humanSquares.clear();
}
/****************************************************************************************************
Function Name: SetBoardVariables(vector<PlayerSquare> a_computerSquares, vector<PlayerSquare> a_humanSquares, int a_tilesetSize, string a_firstTurn, string a_nextTurn)

Purpose:	records all the variables into the board to set its current state. this can come from New/Load/Continue and is completely usable
Parameters:
1. int a_tilesetSize
2. vector<PlayerSquare> a_computerSquares

4. vector<PlayerSquare> a_humanSquares

6. string a_firstTurn
7. string a_nextTurn

Return Value: none

Local Variables: (self explanatory)
1. int m_tilesetSize
2. vector<PlayerSquare> m_computerSquares

4. vector<PlayerSquare> m_humanSquares

6. string m_firstTurn
7. string m_nextTurn

Algorithm:

Assistance Received:

*****************************************************************************************************/

void Board::SetBoardVariables(vector<PlayerSquare> a_computerSquares, vector<PlayerSquare> a_humanSquares, int a_tilesetSize, string a_firstTurn, string a_nextTurn)
{
	m_computerSquares = a_computerSquares;
	m_humanSquares = a_humanSquares;

	m_tilesetSize = a_tilesetSize;
	m_firstTurn = a_firstTurn;
	m_nextTurn = a_nextTurn;
}

/****************************************************************************************************
Function Name: DeliverAdvantage(string a_whoGetsAdvantage, int a_winningScore)

Purpose: Find out from the Game who gets the game advantage at the beginning of a "Continued Round".
		Then, apply the advantage given the winning score from the Last round.
Parameters:
1. string whoGetsAdvantage => a string that tells you whom the advantage belongs to
2. int a_winningscore => gives you the winning score which is stripped to get the advantage amount.

Return Value: none //subject to change later.

Local Variables: 
1. int operateOnthis -> just because i want to emphasize what it is that I m working on.
2. sum-> constructor for the advantage score.

Algorithm: simple conditional -> no real algorithm

Assistance Received:

*****************************************************************************************************/
void Board::DeliverAdvantage(string a_whoGetsAdvantage, int a_winningScore)
{
	int operateOnThis = a_winningScore;
	int sum = 0;
	cout << "The winning score last game was :" << operateOnThis << " \n\n\n\n " << endl;
	Sleep(1000);
	//parse the0000222222222222
	while (operateOnThis > 0)
	{
		sum += operateOnThis % 10;
		operateOnThis /= 10;
	}
	
	if (sum > 9)
	{
		sum = sum - 9;
	}
	if (sum == 0)
	{
		sum = 1;
	}
	int advantageTileNumber = sum ;

	cout << " Because they had the advantage, the tile  " << advantageTileNumber << "    should be covered for them" << endl;
	Sleep(2000);
	m_extractThisNumber = advantageTileNumber;

	
	if (a_whoGetsAdvantage == "Human")
	{
		m_humanSquares.at(advantageTileNumber - 1).SetCoveredStatus(true);
		m_humanGetsAdvantage = true;
		return;
	}

	if (a_whoGetsAdvantage == "Computer")
	{
		m_computerSquares.at(advantageTileNumber - 1).SetCoveredStatus(true);
		m_computerGetsAdvantage = true;
		return;
	}
	//havent determined edge cases yet. might be necessary later.
}

 
/****************************************************************************************************
Function Name: vector<vector<int>> Board::GetValidMoves(int a_rollSum, bool a_coverOrUncover)


Purpose:	Given a Roll, Give back valid moves 
Parameters: int a_rollSum --> the roll counted sum as input
			bool a_coverOrUncover --> the strategy for which valid moves are asked for
Return Value: vector<vector<int>> a vector of vector of ints that holds all possible moves

Local Variables: list of nums that hold teh value

Algorithm: will lead to an algoritm that are called in a later function

Assistance Received: none

*****************************************************************************************************/
vector<vector<int>> Board::GetValidMoves(int a_rollSum, bool a_coverOrUncover)
{
	//figure out who's turn it is. this is the player calling you, but you have no way of knowing who that is.
	vector<PlayerSquare> * vectorOfInterest = GetVectorOfInterest(a_coverOrUncover);
	

	//at this point I have the correct vector that I want to modify. 
	/*
	Now Consider the following problem:
	If I chose to cover, then I want the list of Uncovered squares.
	if I chose to uncover, then I want the list of Covered Squares.
	*/
	vector<int> listOfNums;
	//So What I need to do is Generate a vector of vector of ints...
	if (a_coverOrUncover) 
	{
		for (int i = 0; i < (int)vectorOfInterest->size(); i++)
		{
			
			if (!vectorOfInterest->at(i).GetCoveredStatus())
			{
				listOfNums.push_back(i + 1);
			}
		}
	}
	else
	{
		// in the case that the player wants to uncover.
		//if human had advantage you can't touch human's number
		//if computer had advantage, you can't touch computer's number
		for (int i = 0; i < (int)vectorOfInterest->size(); i++)
		{
			if (vectorOfInterest->at(i).GetCoveredStatus())
			{ //have to plug it in somehow.. probably ehere
				if (i + 1 == m_extractThisNumber &&(m_gameCounter < 1 ))
				{
					m_computerGetsAdvantage = false;
					m_humanGetsAdvantage = false;
					continue;
				}
				listOfNums.push_back(i + 1);
			}
		}
	}


	
	//we have the information we need to determine what the valid moves are at this point. 
	//This is true for either player, Using Either Strategy.

	int sizeOfNumList = (int)listOfNums.size();
	//clear anything that might be in the list Before.
	m_masterList.clear();
	if (sizeOfNumList == 0)
	{
		return m_masterList;
	}
	SubsetsForCalculation(listOfNums, sizeOfNumList, a_rollSum);


	//if coverOrUncover is true, you are looking to Cover your Tiles
	//if it is false, then you are looking to uncover enemy Tiles
	return m_masterList;
}



/****************************************************************************************************
Function Name:vector<PlayerSquare> * Board::GetVectorOfInterest(bool a_coverOrUncover)


Purpose:	returns the vector you care about given the current turn and given whether you want to cover your own or uncover opponents
the reason to return a pointer it to simultaneously eliminate the need to modify the function 
when I need to "make a move" and affect the Board Pointers

Parameters:  bool a_coverOrUncover --> this give you the vector,

Return Value: vector<PlayerSquares> which represents the appropriate board for the correct player.

Local Variables: vectorInterest * just represents the pointer we'll be passing over.
Algorithm: Check Next player. Check cover or Uncover, Determine correct player

Assistance Received: none

*****************************************************************************************************/
vector<PlayerSquare> * Board::GetVectorOfInterest(bool a_coverOrUncover)
{
	vector<PlayerSquare> * vectorOfinterest;
	if (m_nextTurn == "Human")
	{
		//Current turn belongs to Computer

		if (a_coverOrUncover)
		{
			//then I want to cover - ie effect my own tileset
			vectorOfinterest = & m_computerSquares;
		}
		else vectorOfinterest = & m_humanSquares;
	}

	if (m_nextTurn == "Computer")
	{
		//Current turn belongs to Human
		
		if (a_coverOrUncover)
		{
			//then I want to cover - ie effect my own tileset
			vectorOfinterest = & m_humanSquares;
		}
		else vectorOfinterest = & m_computerSquares;
	}
	
	return vectorOfinterest;
}
/****************************************************************************************************
Function Name:void Board::SubsetsForCalculation(vector<int> & a_vectorWithFullSet, int a_size, int a_targetVal)

Purpose:	first part of a two part function that grabs the data for calculating valid numbers that are avaialble and add up to the number you care about

Parameters:  vector<int> & a_vectorWithFullSet, -> the vector built to give correct set of numbers 
			int a_size,  -> size of that vector
			int a_targetVal->and the target value youre trying to add up to

Return Value: none

Local Variables: vectorInterest * just represents the pointer we'll be passing over.
Algorithm: for a given vector. give it a dummy variable and a size consistent with size of the vector. get 

Assistance Received: GeeksForGeeks.com had the idea laid out and a similar implementation in C. http://www.geeksforgeeks.org/backttracking-set-4-subset-sum/
I adjusted to meet my needs.

*****************************************************************************************************/
void Board::SubsetsForCalculation(vector<int> & a_vectorWithFullSet, int a_size, int a_targetVal)
{
	vector<int>  tupletVector;
	//preallocate some space - this will keep things nice and clean as far as 
	for (int i = 0; i < a_size; i++)
	{
		tupletVector.push_back(1000);
	}
	int total = 0;

	sort(a_vectorWithFullSet.begin(), a_vectorWithFullSet.end());

	//will come in handy for neg nums i think...
	for (int i = 0; i < a_size; i++)
	{
		total += a_vectorWithFullSet.at(i);
	}
	//make sure that the first number in the list is smaller than the size of the target
	//also make sure that the sum of the numbers in the list is bigger than the target ( just in case for negs )
	if (a_vectorWithFullSet.at(0) <= a_targetVal && total >= a_targetVal)
	{
		sumOfSubset(a_vectorWithFullSet, tupletVector, a_size, 0, 0, 0, a_targetVal);
	}

	tupletVector.clear();

}

/****************************************************************************************************
Function Name:void Board::sumOfSubset(vector<int>  a_vectorWithFullSet, vector<int> &  a_tupletVector,
	int a_setSize, int a_currentTupletSize, int a_sumSoFar,
	int a_nodesSoFar, int const a_targetVal)

Purpose:	second  of a two part function iteratively recurses through
Parameters: vector<int>  a_vectorWithFullSet, ->all your options
			vector<int> &  a_tupletVector,-> vector for individual part
			int a_setSize, -> the size of the set
			int a_currentTupletSize, -> tuplet countera as itgrows
			int a_sumSoFar, -> total sum for the numbers to track
			int a_nodesSoFar, -> which nodes aer available
			int const a_targetVal -> the target number (dice roll)

Return Value: void

Local Variables: vector<int> pushThisIn -> contains 
Algorithm: recursively check each element (which you go through iteratively) and compare vs target size.

Assistance Received: see above references to geeksforgeek

*****************************************************************************************************/
void Board::sumOfSubset(vector<int>  a_vectorWithFullSet, vector<int> &  a_tupletVector,
	int a_setSize, int a_currentTupletSize, int a_sumSoFar,
	int a_nodesSoFar, int const a_targetVal)
{

	if (a_targetVal == a_sumSoFar)
	{
		vector<int> pushThisIn;

		for (int i = 0; i < a_currentTupletSize; i++)
		{
			if (!(a_tupletVector.at(i) == 0))
			{
				pushThisIn.push_back(a_tupletVector.at(i));
			}
		}

		m_masterList.push_back(pushThisIn);
		//	a_tupletVector.clear(); // just incase.....



		//first constraint check
		if (a_nodesSoFar + 1 < a_setSize && a_sumSoFar - a_vectorWithFullSet.at(a_nodesSoFar) + a_vectorWithFullSet.at(a_nodesSoFar + 1) <= a_targetVal)
		{
			//exclude previous added item and consider next candidate
			sumOfSubset(a_vectorWithFullSet, a_tupletVector, a_setSize, a_currentTupletSize - 1, a_sumSoFar - a_vectorWithFullSet.at(a_nodesSoFar), a_nodesSoFar + 1, a_targetVal);
		}
		return;
	}
	else
	{
		//again check constraints
		//have i covered all the nodes? AND the sum accumulated so far smaller than or equal to the target value?
		if (a_nodesSoFar < a_setSize && a_sumSoFar + a_vectorWithFullSet.at(a_nodesSoFar) <= a_targetVal)
		{
			//generate nodes on the same depth
			for (int i = a_nodesSoFar; i < a_setSize; i++)
			{
				//the current tuple is what you have at the current tuple size, 
				//vector at i.....
				a_tupletVector.at(a_currentTupletSize) = a_vectorWithFullSet.at(i);

				//same test for checking teh sum so far
				if (a_sumSoFar + a_vectorWithFullSet.at(i) <= a_targetVal)
				{
					//consider the next level of depth and try to get the sum to that level
					sumOfSubset(a_vectorWithFullSet, a_tupletVector, a_setSize, a_currentTupletSize + 1, a_sumSoFar + a_vectorWithFullSet.at(i), i + 1, a_targetVal);
				}
			}
		}
	}
}

