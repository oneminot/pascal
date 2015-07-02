#include <iostream>
#include <string>

class Poop
{
public:
	//Poop()
	//{
	//	my_poop_array_p_ = nullptr;
	//	size = 0;
	//	is_empty_b_ = true;
	//};
	//int * my_poop_array_p_;
	bool is_empty_b_;
	bool is_empty_b_2;
	bool is_empty_b_3;
	long long size;
	bool is_empty_b_4;
	bool is_empty_b_5;
	bool is_empty_b_6;
	bool is_empty_b_7;
	bool is_empty_b_8;
};

int main() {
	Poop my_poop;
	//my_poop.my_poop_array_p_ = new int[10];
	int x = sizeof(Poop);
	std::cout << x;
	return x;
}