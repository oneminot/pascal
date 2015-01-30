

#include <iostream>

using namespace std;
const int size=11;

char tokenarray[]="BiV=VtiV=VtVCIeVCIeVCIE.";
int tokenloc=0;
int numberoftokens=1;
void factor();
void expr();
void term();
void mstat();
void stat();

void factorprime()
{
	if (tokenarray[tokenloc]=='*' || tokenarray[tokenloc]=='/)')
	{
		tokenloc++;
		factor();
		factorprime();
	}

}

void factor()
{

	if ( tokenarray[tokenloc]=='(')
	{
		tokenloc++;
		expr();
		if (tokenarray[tokenloc]==')')
		{
			tokenloc++;
			factorprime();
		}
		else 
		{
			cout << "Error" << endl;
			system("pause");
			exit(0);
		}
	}
	else if(tokenarray[tokenloc]=='R' || tokenarray[tokenloc]=='V' || tokenarray[tokenloc]=='I')
	{
		tokenloc++;
		factorprime();
	}
	else
	{
		cout << "Error" << endl;
		system("pause");
		exit(0);
	}


}


void termprime()
{
	if (tokenarray[tokenloc]=='+' || tokenarray[tokenloc]=='-')
	{
		tokenloc++;
		term();
		termprime();
	}
	
}

void term()
{  
	factor();
	termprime();
	
}

void expr()
{
	term();
}

void bexprprime()
{
	if (tokenarray[tokenloc]=='=')
	{
		tokenloc++;
		expr();
	}
	else if (tokenarray[tokenloc]=='<')	
	{
		tokenloc++;
		expr();
	}
	else
	{
		cout << "Error" << endl;
		system("pause");
		exit(0);
	}
}

void bexpr()
{
	expr();
	bexprprime();
}

void statprime()
{
	if (tokenarray[tokenloc]=='e')
	{
		tokenloc++;
		stat();
	}
}

void stat()
{
	if (tokenarray[tokenloc]=='V')
	{
		tokenloc++;
		if (tokenarray[tokenloc]=='C')
		{
			tokenloc++;
			expr();
		}
		else
		{
			cout << "Error" << endl;
			system("pause");
			exit(0);
		}
	}
	else if (tokenarray[tokenloc]=='B')
	{
		tokenloc++;
		mstat();
		if (tokenarray[tokenloc]=='E')
		{
			tokenloc++;
		}
		else
		{
			cout << "Error" << endl;
			system("pause");
			exit(0);
		}
	}
	else if (tokenarray[tokenloc]=='i')
	{
		tokenloc++;
		bexpr();
		if (tokenarray[tokenloc]=='t')
		{
			tokenloc++;
			stat();
			statprime();
		}
		else
		{
			cout << "Error" << endl;
			system("pause");
			exit(0);
		}
	}

}

void mstatprime()
{
	if (tokenarray[tokenloc]==';')
	{
		tokenloc++;
		mstat();
	}

}

void mstat()
{
	stat();
	mstatprime();
}

void block()
{
	if (tokenarray[tokenloc]=='B')
	{
		tokenloc++;
		mstat();
		if (tokenarray[tokenloc]=='E')
		{
			tokenloc++;
		}
		else
		{
			cout << "Error" << endl;
			system("pause");
			exit(0);
		}
	}
	else
	{
		cout << "Error" << endl;
		system("pause");
		exit(0);
	}

}

void program()
{
	block();
	if (tokenarray[tokenloc]=='.')
	{
		tokenloc++;
	}
	else
	{
		cout << "Error" << endl;
		system("pause");
		exit(0);
	}

}

void main()
{
	program();
	cout << "Gooooooooood code" << endl;
	system("pause");
		

}