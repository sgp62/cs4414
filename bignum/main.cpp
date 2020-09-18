/*
 * netID: sgp62
 * Name: Stefen Pegels
 * Main file
 */
#include <string>
#include <iostream>
#include "bignum.hpp"



int main(int argc, char** argv)
{;
	if(argc != 4){
		std::cout << "Usage: bignum op number1 number2.\n";
		return 0;
	}
	std::string args[4];
	for(int n = 0; n < argc; n++)
		args[n] = std::string(argv[n]);
	std::string nums = "0123456789";//Used for checking args contents
	int flag = 0;
	int flag2 = 0;
	for(int i = 0; i < (int) args[2].length(); i++){//Searches for nondigits in args[]
		if(nums.find(args[2][i]) == std::string::npos){
			flag = 1;
		}
	}
	for(int i = 0; i < (int) args[3].length(); i++){
		if(nums.find(args[3][i]) == std::string::npos){
			flag2 = 1;
		}
	}

	if(flag == 1){
	       	std::cout << "Error: \"" << args[2] << "\" is not an unsigned integer.\n";   
		if(flag2 == 1) std::cout << "Error: \"" << args[3] << "\" is not an unsigned integer.\n";	
		return 0;
	}
	if(flag2 == 1){
	       	std::cout << "Error: \"" << args[3] << "\" is not an unsigned integer.\n";   
		if(flag == 1) std::cout << "Error: \"" << args[2] << "\" is not an unsigned integer.\n";	
		return 0;
	}
	std::string ops = "+-*%/gcd";//Used for verifying operators
	for(int i = 0; i < (int) args[1].length(); i++){
		if(ops.find(args[1][i]) == std::string::npos){
			std::cout << "Error: " << args[1] << " is not a supported operator.\n";
			return 0;
		}
	}
	Bignum wow = Bignum(args[2]);
	Bignum cool = Bignum(args[3]);
	if (args[1] == "gcd") std::cout << (wow.gcd(wow,cool)).to_string() << "\n";
	else if (args[1] == "+") std::cout << (wow + cool).to_string() << "\n";
	else if (args[1] == "-") std::cout << (wow - cool).to_string() << "\n";
	else if (args[1] == "*") std::cout << (wow * cool).to_string() << "\n";
	else if (args[1] == "/") std::cout << (wow / cool).to_string() << "\n";
	else if (args[1] == "%") std::cout << (wow % cool).to_string() << "\n";
}
