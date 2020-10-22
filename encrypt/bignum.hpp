class Bignum
{
	public:
		Bignum();
		Bignum(const Bignum&);
		Bignum(const int&);
		Bignum(const std::string&);
		std::string to_string() const;
		void check(std::string) const;
		bool operator<(const Bignum&) const;
		bool operator>(const Bignum&) const;
		bool operator==(const Bignum&) const;
		bool operator<=(const Bignum&) const;
		bool operator>=(const Bignum&) const;
		bool operator!=(const Bignum&) const;
		Bignum operator+(const Bignum&) const;
		Bignum operator-(const Bignum&) const;
		Bignum operator*(const int&) const;
		Bignum operator*(const Bignum&) const;
		Bignum operator/(const Bignum&) const;
		Bignum operator%(const Bignum&) const;
		std::pair<Bignum, Bignum> pair_divide(const Bignum&) const;
		Bignum expmod(Bignum, const Bignum &) const;
		Bignum gcd(const Bignum&) const;
		Bignum encrypt(const std::string&, const std::string&) const;
		Bignum decrypt(const std::string&, const std::string&) const;
		std::vector<int> as_vec() const;
		static bool OPT1;
		static bool OPT2;
		static bool OPT3;
		static bool OPT4;
		static int BASE;
		static Bignum BOne;
		static Bignum BZero;
		static std::vector<Bignum> mul_table;
		void build_mul_table(const Bignum &) const;
		static std::vector<Bignum> pow2;
		Bignum back_to_ten(const Bignum&) const;
		//Consider std::move()
	private:
		std::vector<int> digits;
		void append_digit(int);
};
