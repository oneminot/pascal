#include <map>
#include <string>
#include <utility>
#include <iostream>
int main() {
	std::map<std::string, std::string> production;
	production.insert(std::make_pair("070000", "08"));
	production.insert(std::make_pair("050907", "05"));
	production.insert(std::make_pair("051007", "05"));
	std::cout << production["050907"] << std::endl;
}