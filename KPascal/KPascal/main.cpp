//#include <iostream>
//#include "Lexer.h"
//#include "Token.h"
//#include "Keyword.h"
//#include "Production.h"
//#include "Parser.h"
//int main()
//{
//	KPascal::Parser parser;
//	KPascal::Keyword keyword;
//	std::string x = keyword.getKeywordValue("integer");
//	KPascal::Production p;
//	x = p.getProductionValue("050907");
//	KPascal::Lexer lexer;
//	KPascal::Token token;
//	while (lexer.getToken(token))
//	{
//		std::string s = keyword.getKeywordValue(token.sType);
//		std::cout << token.type << std::endl;
//		std::cout << token.value << std::endl;
//		std::cout << token.sType << std::endl;
//		if (!parser.parse(token))
//		{
//			std::cout << "Error!!" << std::endl;
//		};
//	}
//	token.sType = "special";
//	token.value = "$";
//	token.type = 15;
//	parser.parse(token);
//	std::vector<std::string> y = parser.getmystack();
//	std::cout << "Parse successful" << std::endl;
//	system("pause");
//}

//#include "ReservedKeyword.h"
//
//int main()
//{
//	KPascal::ReservedKeyword r;
//	if (r.IsKeywordReserved("Kushal"))
//	{
//		std::cout << "found!" << std::endl;
//	}
//}