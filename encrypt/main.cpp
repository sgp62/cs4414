/*
 * Ken's bignum program
 */
#include <iostream>
#include <algorithm>
#include <vector>
#include "bignum.hpp"

std::string ops("+-*/%<>=^");
std::string gcd("gcd");
std::string rsa_n = "9616540267013058477253762977293425063379243458473593816900454019721117570003248808113992652836857529658675570356835067184715201230519907361653795328462699";
std::string rsa_e = "65537";
std::string rsa_d = "4802033916387221748426181350914821072434641827090144975386182740274856853318276518446521844642275539818092186650425384826827514552122318308590929813048801";

bool Bignum::OPT1 = false;
bool Bignum::OPT2 = false;
bool Bignum::OPT3 = false;
bool Bignum::OPT4 = false;

bool is_digit(const char value) { return std::isdigit(value); }
bool is_numeric(const std::string& value) { return std::all_of(value.begin(), value.end(), is_digit); }

std::string to_numeric(std::string next_line)
{
	std::string nlad("");
	for(auto c: next_line)
	{
		int d = ((int)c & 0xFF);
		nlad += (d/100)%10 + '0';
		nlad += (d/10)%10 + '0';
		nlad += d%10 + '0';
	}
	return nlad;
}

std::string to_chars(Bignum to_numeric)
{
	std::string nlas("");
	int d_to_go = to_numeric.as_vec().size()%3;
	if(d_to_go == 0)
		d_to_go = 3;
	int index = 0;
	int next_d = 0;
	int size = (to_numeric.as_vec().size()+2)/3;
	for(int n = 0; n < size; n++)
	{
		do
		{
			next_d = next_d*10 + to_numeric.as_vec()[index++];
		}
		while(--d_to_go);
		nlas += (char)next_d;
		next_d = 0;
		d_to_go = 3;
	}
	return nlas;
}

int main(int argc, char **argv)
{
	std::string op;
	std::string opt;
	std::string num1;
	std::string num2;
	std::string num3;
	int j=0;
	if(argc >= 2){
		if(std::string(argv[1]).find("-o") != std::string::npos) j = 1;
	}
	if(argc != (4+j) && (argc != (5+j) || *argv[1+j] != '^') && (argc != (2+j) || (*argv[1+j] != 'e' && *argv[1+j] != 'd')))
	{
		std::cout << "Run as bignum -op number1 number2, or bignum -help, where:\n	op is one of + - * / %% gcd encrypt decrypt\n	number1 and number2 are positive integers of arbitrary length\n" << std::endl;
		return 0;
	}

	if(j){
		opt = std::string(argv[1]);
		if(opt.find("1") != std::string::npos) Bignum::OPT1 = true;
		if(opt.find("2") != std::string::npos) Bignum::OPT2 = true;
		if(opt.find("3") != std::string::npos) Bignum::OPT3 = true;
		if(opt.find("4") != std::string::npos) Bignum::OPT4 = true;
	}

	op = std::string(argv[1+j]);
	if(op[0] != 'd' && op[0] != 'e')
	{
		num1 = std::string(argv[2+j]);
		if(!is_numeric(num1))
		{
			std::cout << "Error: " << num1 << " is not an unsigned integer.\n" << std::endl;
			return 1;
		}
		num2 = std::string(argv[3+j]);
		if(!is_numeric(num2))
		{
			std::cout << "Error: " << num2 << " is not an unsigned integer.\n" << std::endl;
			return 1;
		}
		if(argc == 5+j)
		{
			num3 = std::string(argv[4+j]);
			if(!is_numeric(num3))
			{
				std::cout << "Error: " << num3 << " is not an unsigned integer.\n" << std::endl;
				return 1;
			}
		}
		else if(op[0] == '^')
		{
			std::cout << "Error: Modular exponentiation requires an exponent and a modulus!\n" << std::endl;
			return 1;
		}
	}
	int counter = 3;
	switch(op[0])
	{
		case '+':
			std::cout << (Bignum(num1)+Bignum(num2)).to_string() << std::endl;
			return 0;
		case '-':
			std::cout << (Bignum(num1)-Bignum(num2)).to_string() << std::endl;
			return 0;
		case '*':
			std::cout << (Bignum(num1)*Bignum(num2)).to_string() << std::endl;
			return 0;
		case '/':
			std::cout << (Bignum(num1)/Bignum(num2)).to_string() << std::endl;
			return 0;
		case '%':
			std::cout << (Bignum(num1)%Bignum(num2)).to_string() << std::endl;
			return 0;
		case '<':
			std::cout << (Bignum(num1)<Bignum(num2)) << std::endl;
			return 0;
		case '>':
			std::cout << (Bignum(num1)>Bignum(num2)) << std::endl;
			return 0;
		case '=':
			std::cout << (Bignum(num1)==Bignum(num2)) << std::endl;
			return 0;
		case '^':
			std::cout << Bignum(num1).expmod(num2, num3).to_string() << std::endl;
			return 0;
		case 'g':
			std::cout << (Bignum(num1).gcd(Bignum(num2))).to_string() << std::endl;
			return 0;
		case 'e':
			while(!std::cin.eof())
			{
				std::string next_line;
				std::getline(std::cin, next_line);
				// Many famous movie lines are too long to encrypt in one chunk, so we do all of them as two lines
				// A line too long for two-chunk encryption is truncated (this only affects one line)
				if(next_line.size()*3 > rsa_n.size())
				{
					std::cout << Bignum(to_numeric(next_line.substr(0, rsa_n.size()/3))).encrypt(rsa_n, rsa_e).to_string() << std::endl;
					std::cout << Bignum(to_numeric(next_line.substr(rsa_n.size()/3, rsa_n.size()/3))).encrypt(rsa_n, rsa_e).to_string() << std::endl;
				}
				else  // Fits on one line
				{
					std::cout << Bignum(to_numeric(next_line)).encrypt(rsa_n, rsa_e).to_string() << std::endl;
					std::cout << Bignum(++counter).encrypt(rsa_n, rsa_e).to_string() << std::endl;
				}
			}
			return 0;
		case 'd':
			while(!std::cin.eof())
			{
				std::string part1, part2;
				std::getline(std::cin, part1);
				std::getline(std::cin, part2);
				std::cout << to_chars(Bignum(part1).decrypt(rsa_n, rsa_d).to_string());
				Bignum p2 = Bignum(part2).decrypt(rsa_n, rsa_d);
				if(p2 > Bignum(100000))
				{
					std::cout << to_chars(p2.to_string());
				}
				std::cout << std::endl;
			}
			return 0;
		default:
			std::cout << "Error: " << op << " is not a supported operator\n" << std::endl;
			return 1;
	}

}


