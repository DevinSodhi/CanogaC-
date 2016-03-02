#pragma once

/*
A Player Square has the following Properties:
1. it is either Covered or UnCovered. These are selectors and mutators.

*/

class PlayerSquare
{
public:
	PlayerSquare();
	PlayerSquare(bool a_isCovered, int a_squareValue);
	~PlayerSquare();

	//sets the current status of the Square
	void SetCoveredStatus(bool a_coverStatus)
	{
		m_isCovered = a_coverStatus;
	}

	//gets the current status of the Square
	bool GetCoveredStatus()
	{
		return m_isCovered;
	}

private:
	bool m_isCovered = true;
	int m_squareValue;
};

