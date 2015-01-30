// Mitchel Steinman
// This is the Maroon Logbook
// AKA Parser part 1

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <map>
#include "C:/Users/Mitchel/Documents/Visual Studio 2013/Projects/Lexer/Lexer/Lexer.cpp"
using namespace std;
const int SQUARE = 15;
const char * TUNGSTEN = "Accidentally Tungsten.txt";
const char * KEYWORDS = "Helpless Bulldozer.txt";
const char * PRODUCTIONS = "Rebel Coffin.txt";

int WatchYoProfamnity(token youreRightImSorry)
{
	int type = youreRightImSorry.type;
	switch (type)
	{
	case 0:
	{
			  type = 15;
			  break;
	}
	case 2:
	{
			   type = 1;
			   break;
	}
	case 3:
	{
			   // have to figure out what this is.
			  if (youreRightImSorry.word == "+")
				  type = 9;
			  else if (youreRightImSorry.word == "-")
				  type = 10;
			  else if (youreRightImSorry.word == "*")
				  type = 11;
			  else if (youreRightImSorry.word == "/")
				  type = 12;
			  else if (youreRightImSorry.word == "(")
				  type = 13;
			  else if (youreRightImSorry.word == ")")
				  type = 14;
			  break;
	}
	case 10:
	{
			   type = 2;
			   break;
	}
	default:
	{
			   type = 0;
			   break;
	}
	}
	return type;
}
int main()
{

	map<string, string> productionMap;
	int a = atoi("b5");
	//cout << a << endl;
	char b = 'a';
	char ** states;
	states = new char*[15];
	ifstream infile, keyfile, profile;
	infile.open(TUNGSTEN, ios_base::in);
	if (!infile.is_open())
	{
		cout << "oops" << endl;
	}

	else
	{
		// need to build the multi-dimensional array.
		// 15 by 15 array
		states = new char*[SQUARE];
		for (int i = 0; i < SQUARE; i++)
		{
			states[i] = new char[SQUARE];
		}
		for (int i = 0; i < SQUARE; i++)
		{
			while (!infile.eof())
			{
				for (int a = 0; a < SQUARE; a++)
				{
					for (int b = 0; b < SQUARE; b++)
					{
						infile >> states[a][b];
					}
				}
			}
		}

	}

	// read in the keyword list text file
	keyfile.open(KEYWORDS, ios_base::in);
	if (!keyfile.is_open())
	{
		cout << "oops, keyfile is not open" << endl;

	}
	else
	{
		string s1;
		vector<string> stuff;
		map<string, string> keyMap;
		while (!keyfile.eof())
		{
			getline(keyfile, s1);
			stuff = split(s1, ' ');
			keyMap[stuff[1]] = stuff[0];
		}
	}
	profile.open(PRODUCTIONS, ios_base::in);
	if (!profile.is_open())
	{
		cout << "oops, productions file has a problem" << endl;
	}
	else
	{
		string s2;
		vector<string> productionLine;
		//map<string, string> productionMap;
		while (!profile.eof())
		{
			getline(profile, s2);
			productionLine = split(s2, ' ');
			productionMap[productionLine[1]] = productionLine[0];
		}
	}
	// The next step is the do the actual parsing work. Get a token, add it to the stack, follow the
	// steps/rules for adding that type of token to the stack. Use the production list if we need to reduce.
	// Try a vector
	lexer Lutherer;
	token idktest;
	vector<string> vStack;
	vStack.push_back("15");
	while (true)
	{
		// alright, so what do we have to do here...
		// we need to first put "$" or nothing onto the stack
		// let's put $ on the stack... which is............. 15
		idktest = Lutherer.getNextToken();
		//cout << "I got this token: " << idktest.word << " which is type " << idktest.type << endl;
		// have to convert the numbers you get for 
		int newType = WatchYoProfamnity(idktest);
		idktest.type = newType;
		// need to look at the multidimensional array to figure out if I
		// need to put anything on the stack beyond the token.
		//string what;
		//what = vStack[vStack.end];
		// problem here: atoi is going to give me garbage because there
		// could be a "<" on the stack, which returns a 0.
		int lastItem = atoi(vStack.back().c_str());
		//cout << lastItem << endl;
		char addStack = states[lastItem-1][idktest.type-1];
		//cout << addStack << endl;
		if (addStack != '>')
		{
			string addToStack;
			addToStack.push_back(addStack);
			vStack.push_back(addToStack);
			if (idktest.type < 10) // as in, has only 1 digit, have to pad zeros
			{
				string uwotm8;
				uwotm8.push_back('0');
				//uwotm8.push_back(atoi(idktest.type));
				uwotm8 += to_string(idktest.type);
				vStack.push_back(uwotm8);
			}
			else
				vStack.push_back(to_string(idktest.type));
		}
		if (addStack == '>')
		{
			//cout << " reduce yay!" << endl;
			// first, we want to figure what we're reducint, so we're going back until we run into the
			// "<" and make that a temporary string
			do
			{

				string reduce;
				int i = vStack.size() - 1;
				while (vStack[i] != "<")
				{
					i--;
				}
				int j = 0;
				for (i += 1; i <= vStack.size() - 1; i++)
				{
					if (vStack[i] != "=")
					{
						reduce += vStack[i];
					}
					j++;
				}
				// have to use keyMap to find the information
				if (productionMap.find(reduce) != productionMap.end())
				{
					string reduced = productionMap.find(reduce)->second;
					for (int o = 0; o < j+1; o++)
					{
						vStack.pop_back();
					}
					int currentLast = atoi(vStack.back().c_str());
					char currentAddStack = states[currentLast - 1][atoi(reduced.c_str()) - 1];
					string addtoStack;
					addtoStack.push_back(currentAddStack);
					vStack.push_back(addtoStack);
					vStack.push_back(reduced);
					lastItem = atoi(vStack.back().c_str());
					//cout << lastItem << endl;
					currentAddStack = states[lastItem - 1][idktest.type - 1];
					addStack = currentAddStack;
					//cout << currentAddStack << endl;
					// if currentAddstack is =, then what should we do, that means we're done reducing.
					if (addStack == '=')
					{
						addtoStack.clear();
						addtoStack.push_back(addStack);
						vStack.push_back(addtoStack);
						addtoStack.clear();


					}
					if (addStack == 'a')
					{
						//cout << "I got an accept" << endl;
						// there should be nothing else on the stack...
						if (vStack[1] == "<")
						{
							if (vStack[2] == "08")
							{
								cout << "this is a good line." << endl;
							}
							else
								cout << "error" << endl;
						}
						else
							cout << "error" << endl;
					}
					// so we reduced, but we still need to add the new thing onto the stack.
				}
				else
				{
					cout << "we have a reduction error " << endl;
					system("pause");
					return 1;
				}
				// remove the reduced stuff from the stack
				//for (int o = 0; o < j; o++)
				//{
				//	vStack.pop_back();
				//}
				//vStack.push_back(reduced);
			}
			while (addStack == '>');
			if (idktest.type < 10) // as in, has only 1 digit, have to pad zeros
			{
				string uwotm8;
				uwotm8.push_back('0');
				//uwotm8.push_back(atoi(idktest.type));
				uwotm8 += to_string(idktest.type);
				vStack.push_back(uwotm8);
			}
			else 
				vStack.push_back(to_string(idktest.type));
			if (vStack.back() == "15")
				break;
		}
		if (addStack == '.')
		{
			cout << "uh oh" << endl;
			cout << "this is a bad file" << endl;
			system("pause");
			return 1;
		}

	}
	cout << endl;
	system("pause");
	return 0;
}