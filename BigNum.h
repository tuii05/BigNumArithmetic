#pragma once
#include "Arithmetic.h"
#include "Storage.h"

class BigNum {
    public:
        BigNum() : storage("0"), base(BASENUM) {}
        BigNum(const std::string &, intType);
        BigNum(const std::string &, intType, bool);
        BigNum(std::vector<uint32_t> &&, intType, bool);
        BigNum(BigNum&&);
        BigNum(const BigNum &) = delete;

    public:
        void setBase(uint32_t base) { this->base = base; }
        bool isPos() const { return storage.isPos(); }
        itPair getDigits() const { return storage.getConvertedDigits(); }
        void setNumber(std::string&& numberString, intType base) {
            storage.setNumber(std::move(numberString), base);
        }
        void setNumber(std::vector<intType>&& digits) {
            storage.setNumber(std::move(digits));
        }

    public:
        friend void gcd(const BigNum &, const BigNum &, BigNum &);

    public:
        BigNum& operator=(BigNum&&);
        BigNum& operator=(const BigNum &) = delete;

        BigNum& operator+=(const BigNum&);
        BigNum& operator-=(const BigNum&);

        BigNum &operator*=(const BigNum&);
        BigNum &operator/=(const BigNum&);

        friend std::ostream& operator<<(std::ostream&, const BigNum&);
        friend std::ostream& operator<<(std::ostream &, const std::pair<BigNum, BigNum>&);

        BigNum operator+(const BigNum &);
        BigNum operator-(const BigNum &);
        BigNum operator*(const BigNum &);
        std::pair<BigNum,BigNum> operator/(const BigNum &);

    public:
        friend BigNum operator+(const BigNum &, const BigNum &);
        friend BigNum operator-(const BigNum &, const BigNum &);
        friend BigNum operator*(const BigNum &, const BigNum &);
        friend std::pair<BigNum, BigNum> operator/(const BigNum &, const BigNum &);

    private:
        Storage storage;
        static Arithmetic arithmetic;
        std::uint32_t base;
};
