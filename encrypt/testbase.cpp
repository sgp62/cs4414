//Decimal to X base

#include <string>
#include <iostream>

std::string dec_to_10000(int num){
	std::string result;
	while(num>0){
		result = std::to_string(num%10000) + result;
		num = num/10000;
	}
	return result;
}

int main(){
	std::cout << dec_to_10000(7) << std::endl;
	std::cout << dec_to_10000(20000) << std::endl;
	std::cout << dec_to_10000(2040200) << std::endl;
	return 0;
}
