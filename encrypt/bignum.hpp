class Bignum
{
	public:
		Bignum();
		Bignum(const Bignum&);
		Bignum(int);
		Bignum(std::string);
		std::string to_string();
		void check(std::string);
		bool operator<(Bignum);
		bool operator>(Bignum);
		bool operator==(Bignum);
		bool operator<=(Bignum);
		bool operator>=(Bignum);
		bool operator!=(Bignum);
		Bignum operator+(Bignum);
		Bignum operator-(Bignum);
		Bignum operator*(int);
		Bignum operator*(Bignum);
		Bignum operator/(Bignum);
		Bignum operator%(Bignum);
		Bignum expmod(Bignum, Bignum);
		Bignum gcd(Bignum);
		Bignum encrypt(std::string, std::string);
		Bignum decrypt(std::string, std::string);
		std::vector<int> as_vec();
	private:
		std::vector<int> digits;
		void append_digit(int);
};
