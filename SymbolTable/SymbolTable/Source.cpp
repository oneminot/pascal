
#include <iostream>
#include <map>
#include <string>

using namespace std;

struct ParamData
{
	string type;
	int size;
	int offset;
	bool PassbyRef;
};

struct LocalVar
{
	string type;
	int size;
	int offset;
};

struct VarData
{
	string type;
	int size;
	int offset;
	map<string,ParamData> parameters;
	map<string,LocalVar> localvariables;

};


void main()
{
	map<string,VarData> symbolTable;
	
	symbolTable["var1"].type="Integer";
	symbolTable["var1"].size=4;
	symbolTable["var1"].offset=0;

	symbolTable["var3"].type="Boolean";
	symbolTable["var3"].size=1;
	symbolTable["var3"].offset=4;

	symbolTable["Doit"].parameters["p1"].size=4;
	symbolTable["Doit"].parameters["p1"].offset=0;
	symbolTable["Doit"].parameters["p1"].PassbyRef=true;
	symbolTable["Doit"].parameters["p1"].type="Integer";

	symbolTable["Doit"].parameters["p2"].size=4;
	symbolTable["Doit"].parameters["p2"].offset=4;
	symbolTable["Doit"].parameters["p2"].PassbyRef=false;
	symbolTable["Doit"].parameters["p2"].type="Integer";

	symbolTable["Doit"].parameters["p3"].size=4;
	symbolTable["Doit"].parameters["p3"].offset=8;
	symbolTable["Doit"].parameters["p3"].PassbyRef=false;
	symbolTable["Doit"].parameters["p3"].type="Integer";

	symbolTable["Doit"].localvariables["var1"].offset=12;
	symbolTable["Doit"].localvariables["var1"].size=4;
	symbolTable["Doit"].localvariables["var1"].type="Boolean";

	map<string,ParamData>::iterator p;

	for (p= symbolTable["Doit"].parameters.begin(); p != symbolTable["Doit"].parameters.end(); p++)
	{
		cout << p->first << endl;
		cout << symbolTable["Doit"].parameters[ p->first].offset << endl;
	}

	map<string,VarData>::iterator p2;
	if (symbolTable.find("Doit2") != symbolTable.end())
	{
		cout << "duplicate identifier" << endl;
	}
	else
	{
		cout << "You can add this procedure" << endl;
	}

	if (symbolTable.find("var2") == symbolTable.end())
	{
		cout << "identifier not found" << endl;
	}

	
	

}