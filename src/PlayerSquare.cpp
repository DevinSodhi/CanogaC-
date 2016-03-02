#include "PlayerSquare.h"



/****************************************************************************************************
Function Name: Domino (Constructor)

Purpose: Constructor. Sets "cover" status of Square. Holds numerical value indicating position of Square (might remove this if vector position works out alright)

Parameters: None

Return Value: none

Local Variables: 1. bool a_isCovered => status of coverage.
				 2. int  a_squareValue => the numerical value of the square.

Algorithm:

Assistance Received: None

*****************************************************************************************************/
PlayerSquare::PlayerSquare(bool a_isCovered, int a_squareValue)
{
	m_isCovered = a_isCovered;

	m_squareValue = a_squareValue;
}
PlayerSquare::PlayerSquare() {}

PlayerSquare::~PlayerSquare()
{
}
