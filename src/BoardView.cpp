#include "BoardView.h"


/****************************************************************************************************
Function Name: BoardView::BoardView(Board * a_GameBoardReference)
Purpose:	just a reference to the board
Parameters: Board * a_GameBoardReference

Return Value: none

Local Variables: 
Algorithm:
Assistance Received:

*****************************************************************************************************/
BoardView::BoardView(Board * a_GameBoardReference)
{
	m_boardReference = a_GameBoardReference;
}


BoardView::~BoardView()
{
}


/****************************************************************************************************
Function Name:void BoardView::getCurrentBoardView()
Purpose:	read and show the law
Parameters: 

Return Value: none

Local Variables:

Algorithm:

Assistance Received: Amanda Steidl gave me an idea to use instead of having a number associated with player square, but just use the bool position .

*****************************************************************************************************/
void BoardView::getCurrentBoardView()
{ 
	for (int i = 0; i < 25; i++)
	{
		cout << "*" ;
	}
	cout << "Current Board Status";
	for (int i = 0; i < 25; i++)
	{
		cout << "*";
	}
	cout << endl;
	
	vector<PlayerSquare> computerVector = m_boardReference->GetComputerSquares();
	vector<PlayerSquare> humanVector = m_boardReference->GetHumanSquares();

	cout << " THE COMPUTER HAS THE FOLLOWING SQUARES" << endl;
	/**********************************************************Top Line************************************8*/
	int lengthExtension = (computerVector.size() - 9) * 4;
	int totalLength = computerVector.size()*4 +lengthExtension;

	drawALine(totalLength);

	/******************************LineContent********************************************/

	cout << "|";
	for (int i = 0; i < (int)computerVector.size(); i++)
	{
		

		if (computerVector.at(i).GetCoveredStatus())
		{
			cout << " * ";
		}

		if (!computerVector.at(i).GetCoveredStatus())
		{
			cout << " " << i+1 << " ";
		}

		cout << "|";
	}
	cout << endl;
	
	drawALine(totalLength);
	cout << "\n\n\n\n" << endl;
	/**********************************************************END Line**************************************************/


	/************************************************************HUMAN STUFF***********************************************/

	

	cout << " THE HOOMAN HAS THE FOLLOWING SQUARES" << endl;
	/**********************************************************Top Line************************************************/
	drawALine(totalLength);

	/******************************LineContent********************************************/

	cout << "|";
	for (int i = 0; i < (int)humanVector.size(); i++)
	{


		if (humanVector.at(i).GetCoveredStatus())
		{
			cout << " * ";
		}

		if (!humanVector.at(i).GetCoveredStatus())
		{
			cout << " " << i + 1 << " ";
		}

		cout << "|";
	}
	cout << endl;
	/**********************************************************END Line**************************************************/
	drawALine(totalLength);
	cout << "\n\n\n\n" << endl;

}

/****************************************************************************************************
Function Name:void drawALine(ints a_sizeOfIt)
Purpose:	int a_sizeOfIt -> how big the lin should be
Parameters:

Return Value: none

Local Variables:

Algorithm:

Assistance Received: 

*****************************************************************************************************/
void BoardView::drawALine(int a_sizeOfIt)
{
	int totalLength = a_sizeOfIt;

	while (totalLength > 0)
	{
		cout << "_";
		totalLength = totalLength - 1;
	}
	cout << endl;

}
