#include "HeaderList.h"
#include "Helpers.h"



/****************************************************************************************************
Function Name:bool validateInput(int & a_numericalInput)


Purpose: clean effortless check of int data

Parameters:int & a_numericalInput

Return Value: none

Local Variables:


Algorithm:

Assistance Received: None

*****************************************************************************************************/
bool validateInput(int & a_numericalInput)
{

	if (!(std::cin >> a_numericalInput)) {
		cerr << "Please Enter a valid Integer" << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		return false;
	}
	

	
	return true;
}