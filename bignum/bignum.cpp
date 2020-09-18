/*
 * netID: sgp62
 * Name: Stefen Pegels
 * Class cpp file
 */
#include "bignum.hpp"
#include <iostream>
#include <string>

Bignum::Bignum(std::string data){
	for(int i = 0; i < (int) data.length(); i++){
		value.push_back(data[i]-'0');
	}
}
Bignum::Bignum(){
	value.push_back(0);
}


std::string Bignum::to_string(){
	std::string outstring = std::to_string(value[0]);
	for(int i = 1; i < (int) value.size(); i++){
		outstring += std::to_string(value[i]);
	}
	return outstring;
}

void Bignum::extend(Bignum b){//Used to add zero padding BEFORE operations
	int size = (b.value.size() > value.size()) ? b.value.size()-value.size() : value.size()-b.value.size();
	for(int i = 1; i <= size; i++){
		value.emplace(value.begin(), 0);
	}
}

Bignum Bignum::operator+(Bignum b){
	(b.value.size() > value.size()) ? extend(b) : b.extend(*this);
	for(int i = value.size()-1; i > 0; i--){
		value[i] += b.value[i];
			if(value[i] >= 10){
				value[i-1] += (value[i]) / 10;	//Overflow handling
				value[i] -= 10*(value[i]/10);
			}
	}
	value[0] += b.value[0];
	if(value[0] >= 10){//Overflow handling on last digit
	       	value.emplace(value.begin(), value[0] / 10);
		value[1] = value[1] % 10;
	}
	remove_zeroes();
	return *this;
}

bool Bignum::negative_check(Bignum b) {//Checks if subtraction by b from 'this' would be negative
	if(b.value.size() > value.size())return true;
	else{
		if(b.value.size() == value.size()){
			for(int i = 0;i < (int) value.size(); i++){
				if(value[i] <  b.value[i]) return true;
				else if(value[i] > b.value[i]) return false;
			}
		}
		return false;
	}
}

Bignum Bignum::operator-(Bignum b){
	if(negative_check(b)){
		std::cout << "Unsupported: Negative number\n";
		return Bignum();
	}
	b.extend(*this);	
	for(int i = value.size()-1; i >= 0; i--){
		value[i] -= b.value[i];
			if(value[i] < 0){
				int count = 1;
				while(value[i-count] == 0){//Multiple carries required
					value[i-count] = 9;
				       	count++;}
				value[i-count] -= 1;
				value[i] += 10;
			}
	}
	remove_zeroes();
	return *this;

}

Bignum Bignum::smul(int m){//Single digit multiplication, with added x10 case
	Bignum result = Bignum();
	if(m == 10){
		for(int i = value.size()-1; i >= 0; i--) result.value.emplace(result.value.begin(),value[i]);
	}
	else{
		int count = 0;
		for(int i = value.size()-1; i >= 0; i--){
			result.value[result.value.size()-1-count] += m*value[i];
			result.value.emplace(result.value.begin(),result.value[result.value.size()-1-count]/10);
			result.value[result.value.size()-1-count] %= 10;
			count++;
		}
	}
	return result;
}


Bignum Bignum::operator*(Bignum b){
	Bignum result = Bignum();
	for (int i = b.value.size()-1; i >= 0; i--){
		Bignum helper = Bignum();
		helper = smul(b.value[i]);
		int j = b.value.size()-1;
		while(j>i){
			helper = helper.smul(10);//Helper Bignum is extended with decimal place movement
			j--;
		}
		result = result + helper;
	}
	result.remove_zeroes();
	return result;
}

void Bignum::remove_zeroes(){//Used to remove leading zeroes from numbers, or extra zeroes in zero
	std::string nums = "123456789";
	int flag = 0;
	if(value[0] == 0 && value.size() > 1){
		for(int i = 0; i < (int) value.size(); i++){
			if(nums.find(to_string()[i]) != std::string::npos){
			       flag = i;
		       		break;
		 	}		
		}
		if(flag == 0) value.erase(value.begin()+1, value.end());
		else value.erase(value.begin(),value.begin()+ flag);
	}
}

Bignum Bignum::operator/(Bignum b){
	Bignum result = Bignum();
	if(negative_check(b)) return result;
	if(b.value[0] == 0){
		std::cout << "Error: Divide by zero\n";	
		return result;
	}
	Bignum helper = Bignum();
	for(int i = 0; i < (int) b.value.size(); i++){
		helper.value.push_back(value[i]);//Sets up helper as same size as b
	}
	helper.value.erase(helper.value.begin());
	int count = (int) helper.value.size();
	while((count < (int) value.size()) || !(helper.negative_check(b))){
		while(helper.negative_check(b) && count < (int) value.size()){
		       	helper.value.push_back(value[count]);//Helper extension after subtractions
			count++;
			result = result.smul(10);
		}
		if(!helper.negative_check(b)){
			helper = helper - b;
			result = result + Bignum("1");	//summation of quotient
		}
	}
	result.remove_zeroes();
	return result;
}

Bignum Bignum::operator%(Bignum b){
	Bignum im1 = *this / b;
	Bignum im2 = im1 * b;
	*this = *this - im2;
	return *this;
}

Bignum Bignum::gcd(Bignum a, Bignum b){
	if(b.to_string() == "0") return a;
	return gcd(b, a % b);
}

