#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include "bignum.hpp"
#include <string>

//
// A Bignum is just a std::vector containing digits in the range 0-9, following the decimal
// convention that a number other than 0 has no leading 0's.  Oddly, that one rule made the
// code unexpectedly complicated; it would have been easier to just allow numbers like 0002!
//

Bignum::Bignum()
{
}

Bignum::Bignum(const Bignum& b)
{
	digits = b.digits;
}

Bignum::Bignum(const int& d): Bignum(std::to_string(d))
{
}

Bignum::Bignum(const std::string& s)
{
	if(Bignum::BASE == 10){
		for(auto c : s){
			digits.push_back(c-'0');
		}
	}
	else{
		std::string result = s;
		if(result.size() % 4 != 0){
			result = std::string(4 - result.size() % 4, '0') + result;
		}
		while(result.size() >= 4)	
		{
			digits.push_back(std::stoi(result.substr(0,4)));
			result = result.substr(4, result.size());
		}
	}
}

std::vector<int> Bignum::as_vec() const
{
	return digits;
}

std::string Bignum::to_string() const
{
	std::string s;
	for(auto d: digits)
	{
		s += std::to_string(d);
	}
	return s;
}

bool Bignum::operator<(const Bignum& b) const
{
	int alen = digits.size();
	int blen = b.digits.size();	
	int aindex = 0, bindex = 0;
	if(alen > blen)
	{
		return false;
	}
	if(blen > alen)
	{
		return true;
	}
	while(alen-- > 0)
	{
		if(digits[aindex] > b.digits[bindex])
		{
			return false;
		}
		if(digits[aindex++] < b.digits[bindex++])
		{
			return true;
		}
	}
	return false;
}

bool Bignum::operator>(const Bignum& b) const
{
	return b<*this;
}

bool Bignum::operator==(const Bignum& b) const
{
	return ! (*this<b || b<*this);
}

bool Bignum::operator<=(const Bignum& b) const
{
	return ! (*this>b);
}

bool Bignum::operator>=(const Bignum& b) const
{
	return ! (*this<b);
}

bool Bignum::operator!=(const Bignum& b) const
{
	return ! (*this==b);
}

Bignum Bignum::operator+(const Bignum& b) const
{
	Bignum res;
	std::deque<int> resqueue;
	int alen = digits.size();
	int blen = b.digits.size();
	int aindex = 0, bindex = 0;
	if(Bignum::OPT3){
		while(alen > blen)
		{
			resqueue.push_back(digits[aindex++]);
			--alen;
		}
		while(blen > alen)
		{
			resqueue.push_back(b.digits[bindex++]);
			--blen;
		}
		while(alen-- > 0)
		{
			resqueue.push_back(digits[aindex++] + b.digits[bindex++]);
		}
		int rlen = resqueue.size();
		int carry = 0;
		while(rlen > 0)
		{
			carry += resqueue[--rlen];
			resqueue[rlen] = carry%Bignum::BASE;
			carry /= Bignum::BASE;
		}
		while(carry > 0)
		{
			resqueue.emplace_front(carry%Bignum::BASE);
			carry /= Bignum::BASE;
		}
		res.digits = std::vector<int> (resqueue.begin(),resqueue.end());
	} else {
		while(alen > blen)
		{
			res.digits.push_back(digits[aindex++]);
			--alen;
		}
		while(blen > alen)
		{
			res.digits.push_back(b.digits[bindex++]);
			--blen;
		}
		while(alen-- > 0)
		{
			res.digits.push_back(digits[aindex++] + b.digits[bindex++]);
		}
		int rlen = res.digits.size();
		int carry = 0;
		while(rlen > 0)
		{
			carry += res.digits[--rlen];
			res.digits[rlen] = carry%Bignum::BASE;
			carry /= Bignum::BASE;
		
		}
		while(carry > 0)
		{
			res.digits.emplace(res.digits.begin(), carry%Bignum::BASE);
			carry /= Bignum::BASE;
		}
	}
	return res;
}

Bignum Bignum::operator-(const Bignum& b) const
{
	std::deque<int> resqueue;
	Bignum res;
	int alen = digits.size();
	int blen = b.digits.size();
	if(*this < b)
	{
		return BZero;
	}
	int borrow = 0;
	int zeros = 0;
	while(alen > 0 && blen > 0)
	{
		int d = digits[--alen]-b.digits[--blen]-borrow;
		borrow = 0;
		if(d < 0)
		{
			d += Bignum::BASE;
			borrow = 1;
		}
		if(d == 0)
		{
			++zeros;
		}
		else
		{
			if(Bignum::OPT3){

				while(zeros--)
				{
					resqueue.emplace_front(0);
				}
				zeros = 0;
				resqueue.emplace_front(d);
			}
			else {
				while(zeros--)
				{
					res.digits.emplace(res.digits.begin(), 0);
				}
				zeros = 0;
				res.digits.emplace(res.digits.begin(), d);
			}
		}
	}
	while(alen > 0)
	{
		int d = digits[--alen]-borrow;
		borrow = 0;
		if(d < 0)
		{
			d += Bignum::BASE;
			borrow = 1;
		}
		if(d == 0)
		{
			++zeros;
		}
		else
		{
			if(Bignum::OPT3){

				while(zeros--)
				{
					resqueue.emplace_front(0);
				}
				zeros = 0;
				resqueue.emplace_front(d);
			}
			else {
				while(zeros--)
				{
					res.digits.emplace(res.digits.begin(), 0);
				}
				zeros = 0;
				res.digits.emplace(res.digits.begin(), d);
			}
		}
	}
	if(Bignum::OPT3){
		res.digits = std::vector<int> (resqueue.begin(),resqueue.end());
	}
	if(res.digits.size() == 0)
	{
		return BZero;
	}
	return res;
}

// Helper procedure to multiple a Bignum by an integer.  This could be just a single
// digit, but in fact it will sometimes be 10 in my code.  We multiple all the digits
// first, then sweep from low to high digit applying the carries.
Bignum Bignum::operator*(const int& d) const
{
	if(d == 0)
	{
		return BZero;
	}
	if(d == 1)
	{
		return *this;
	}
	std::deque<int> resqueue;
	Bignum res;
	int alen = digits.size();
	int aindex = 0;
	while(alen--)
	{
		res.digits.push_back(digits[aindex++] * d);
	}
	int carry = 0;
	while(aindex-- > 0)
	{
		carry += res.digits[aindex];
		res.digits[aindex] = carry%Bignum::BASE;
		carry /= Bignum::BASE;
	}
	if(Bignum::OPT3) resqueue = std::deque<int> (res.digits.begin(), res.digits.end());
	while(carry > 0)
	{
		if(Bignum::OPT3){
			resqueue.emplace_front(carry%Bignum::BASE);
		} else {
			res.digits.emplace(res.digits.begin(), carry%Bignum::BASE);
		}
		carry /= Bignum::BASE;
	}
	if(Bignum::OPT3) res.digits = std::vector<int> (resqueue.begin(),resqueue.end());
	return res;
}

Bignum Bignum::operator*(const Bignum& b) const
{
	int blen = b.digits.size();
	Bignum tmp = *this;
	Bignum res = BZero;
	while(blen > 0)
	{
		int d = b.digits[--blen];
		if(d == 1)
		{
			res = res + tmp;
		}
		else if(d != 0)
		{
			res = res + tmp*d;
		}
		if(blen > 0)
		{
			tmp = tmp*Bignum::BASE;
		}
	}
	return res;
}

void Bignum::build_mul_table(const Bignum& a) const {
	Bignum multiple = Bignum(*this);
	int quick_two = 1;
	do{
		mul_table.push_back(multiple);
		multiple = multiple + multiple;
		pow2.push_back(quick_two);
		quick_two = quick_two + quick_two;
	}
	while(a > mul_table.back());
}

Bignum Bignum::operator/(const Bignum& b) const
{
	if(b > *this)
	{
		return BZero;
	}
	if(b == *this)
	{
		return BOne;
	}
	if(b == BZero)
	{
		std::cout << "Error: Divide by 0" << std::endl;
		exit(1);
	}
	Bignum res;
	if(Bignum::OPT4){
		Bignum tmp = Bignum(*this);
		if(Bignum::mul_table.size() == 0){
			b.build_mul_table(*this);
		}
		while(tmp >= b){
			int j = 0, k = mul_table.size()-1;
			while (j < k){
				int i = (j + k) / 2;
				if(Bignum::mul_table[i] < tmp){
					if(Bignum::mul_table[i+1] <= tmp){
						j = i+1;
					} else {
						tmp = tmp - Bignum::mul_table[i];
						res = res + Bignum(Bignum::pow2[i]);
						break;
					}	
				} else if(Bignum::mul_table[i] == tmp) {
					tmp = tmp - Bignum::mul_table[i];
					res = res + Bignum(Bignum::pow2[i]);
					break;
				} else {
					k = i;
				}
			}
		}
	}
	else{
		Bignum tmp;
		int alen = digits.size();
		int blen = b.digits.size();
		int aindex = 0;
		bool placed_first = false;
		while(aindex < blen)
		{
			int d = digits[aindex++];
			if(d != 0 || placed_first)
			{
				tmp.digits.push_back(d);
				placed_first = true;
			}
		}
		if(!placed_first)
		{
			tmp = BZero;
		}
		bool first = true;
		do
		{
			int d = 0;
			if(b > tmp && aindex < alen)
			{
				d = digits[aindex++];
				if(d != 0 || tmp != BZero)
				{
					if(tmp == BZero)
					{
						tmp.digits[0] = d;	
					}
					else
					{
						tmp.digits.push_back(d);
					}
				}
			}
			d = 0;
			while(b <= tmp)
			{
				++d;
				tmp = tmp-b;
			}
			if(d != 0 || !first)
			{
				first = false;
				res.digits.push_back(d);
			}
		}
		while(aindex < alen);
		if(res.digits.size() == 0)
		{
			return BZero;
		}
	}
	return res;
}

std::pair<Bignum, Bignum> Bignum::pair_divide(const Bignum& b) const{
	if(b > *this)
	{
		return { BZero, *this};
	}
	if(b == *this)
	{
		return { BOne, BZero };
	}
	if(b == BZero)
	{
		std::cout << "Error: Divide by 0" << std::endl;
		exit(1);
	}
	if(Bignum::OPT4){
		Bignum res;
		Bignum tmp = Bignum(*this);
		if(Bignum::mul_table.size() == 0){
			b.build_mul_table(*this);
		}
		while(tmp >= b){
			int j = 0, k = mul_table.size()-1;
			while (j < k){
				int i = (j + k) / 2;
				if(Bignum::mul_table[i] < tmp){
					if(Bignum::mul_table[i+1] <= tmp){
						j = i+1;
					} else {
						tmp = tmp - Bignum::mul_table[i];
						res = res + Bignum(Bignum::pow2[i]);
						break;
					}	
				} else if(Bignum::mul_table[i] == tmp) {
					tmp = tmp - Bignum::mul_table[i];
					res = res + Bignum(Bignum::pow2[i]);
					break;
				} else {
					k = i;
				}
			}
		}
		return {res, tmp};
	}
	else{
		Bignum res;
		Bignum tmp;
		int alen = digits.size();
		int blen = b.digits.size();
		int aindex = 0;
		bool placed_first = false;
		while(aindex < blen)
		{
			int d = digits[aindex++];
			if(d != 0 || placed_first)
			{
				tmp.digits.push_back(d);
				placed_first = true;
			}
		}
		if(!placed_first)
		{
			tmp = BZero;
		}
		bool first = true;
		do
		{
			int d = 0;
			if(b > tmp && aindex < alen)
			{
				d = digits[aindex++];
				if(d != 0 || tmp != BZero)
				{
					if(tmp == BZero)
					{
						tmp.digits[0] = d;	
					}
					else
					{
						tmp.digits.push_back(d);
					}
				}
			}
			d = 0;
			while(b <= tmp)
			{
				++d;
				tmp = tmp-b;
			}
			if(d != 0 || !first)
			{
				first = false;
				res.digits.push_back(d);
			}
		}
		while(aindex < alen);
		if(res.digits.size() == 0)
		{
			return { BZero, tmp };
		}
		return { res, tmp};
	}
}

Bignum Bignum::operator%(const Bignum& b) const{
	if(Bignum::OPT2){
		return pair_divide(b).second;
	}
	else{
		return *this - (*this/b)*b;
	}
}

Bignum Bignum::expmod(Bignum exp, const Bignum& mod) const
{
	if(exp == BZero)
	{
		return BOne;
	}
	Bignum two = Bignum(2);
	Bignum res = BOne;
	Bignum tmp = *this;
	while(exp > 0)
	{
		if(Bignum::OPT1){
			if(exp.digits.back() & BOne.digits.back()){
				res = res*tmp;
				res = res % mod;
			}
			tmp = tmp*tmp % mod;
			exp = exp/two;
		}
		else{
			if(exp % two == BOne)
			{
				res = res*tmp;
				res = res % mod;
			}
			tmp = tmp*tmp % mod;
			exp = exp/two;
		}
	}
	return res;
}

Bignum Bignum::gcd(const Bignum& b) const
{
	if (b == BZero)
		return *this;
	return b.gcd(*this % b);
}

Bignum Bignum::encrypt(const std::string& rsa_n, const std::string& rsa_e) const
{
	return expmod(Bignum(rsa_e), Bignum(rsa_n)).to_string();
}

Bignum Bignum::decrypt(const std::string& rsa_n, const std::string& rsa_d) const
{
	return expmod(Bignum(rsa_d), Bignum(rsa_n));
}

