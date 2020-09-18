/*
 * netID: sgp62
 * Name: Stefen Pegels
 * Header file
 */
#ifndef _BIGNUM_H
#define _BIGNUM_H
#include <vector>
#include <string>

class Bignum
{
	private:
		std::vector<int> value;

	public:
		std::string check();

		Bignum(std::string data);
		Bignum();

		bool negative_check(Bignum b);
		Bignum operator+ (Bignum b);

		Bignum operator- (Bignum b);
		Bignum operator* (Bignum b);
		Bignum operator/ (Bignum b);
		Bignum operator% (Bignum b);
		Bignum gcd (Bignum a, Bignum b);
		
		void remove_zeroes();
		Bignum smul(int m);
		int single_dig_mul (int digit);
		std::string to_string();
		void extend(Bignum b);



};
#endif
