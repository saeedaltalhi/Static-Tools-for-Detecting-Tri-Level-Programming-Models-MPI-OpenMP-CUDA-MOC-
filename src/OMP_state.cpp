#include "OMP_state.h"

// Function for checking whether omp for loop is being called correctly
bool checkOmpFor(string codeLines[], int ompForLineNumber)
{
	string word = "for";
	string nextLine = codeLines[ompForLineNumber + 1];

	// Remove any preceding spaces
	size_t startPos = nextLine.find_first_not_of(" \t");
	
	bool startsWithFor = (nextLine.substr(startPos, word.length()) == word);
	return startsWithFor;
}
// Function for checking whether omp parallel is being called correctly
bool checkOmpParallel(string codeLines[], int ompParallelLineNumber)
{
	string word = "{";
	string nextLine = codeLines[ompParallelLineNumber + 1];

	// Remove any preceding spaces
	size_t startPos = nextLine.find_first_not_of(" \t");

	bool startsWithBracket = (nextLine.substr(startPos, word.length()) == word);
	return startsWithBracket;
}
// Function for identifying the OpenMP thread Id variable
string Identify_Loop_Var(string codeLines[], int ompForLineNumber)
{
	string ompForLine = codeLines[ompForLineNumber + 1];
	// Find the position of '='
	size_t equalPos = ompForLine.find('=');
	if (ompForLine[equalPos - 1] == ' ')
		equalPos -= 1;

	// Find the position of the last space before '='
	size_t lastSpacePos = ompForLine.find_last_of(" \t", equalPos - 1);

	// Extract the word before '='
	string loopVariable = ompForLine.substr(lastSpacePos + 1, equalPos - lastSpacePos - 1);

	// return tid variable
	return loopVariable;
}
// Function for checkting a race condition in omp parallel for loop
returnMessage detectRaceCondOpenMP(string loopVariable, string codeLines[], int forLineNumber)
{
	returnMessage message;
	bool isRace = false;
	for (int i = forLineNumber + 1; i < forLineNumber + 5; i++)
	{
		string line = codeLines[i];
		// Remove whitespace characters from the line
		line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

		// Check if the modified string contains "[loopVariable+" or "[loopVariable-"
		string searchString1 = "[" + loopVariable + "+";
		string searchString2 = "[" + loopVariable + "-";
		size_t found1 = line.find(searchString1);
		size_t found2 = line.find(searchString2);

		if (found1 != std::string::npos || found2 != std::string::npos)
		{
			message.errorLine = i;
			message.Error = "\t(OpenMP) DATA RACE: Using values that are possibly updated by other threads";
			message.Check = false;
			message.Potential = false;
			isRace = true;
			break;
		}
	}
	if (isRace == false)
	{
		message.errorLine = forLineNumber;
		message.Error = "\t(OpenMP) No data race";
		message.Check = true;
		message.Potential = false;
	}
	return message;
}