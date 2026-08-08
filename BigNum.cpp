#include "BigNum.h"

Arithmetic BigNum::arithmetic(BASENUM);

BigNum::BigNum(const std::string &number, intType base, bool isPositive) : storage(number, isPositive), base(base) {}

BigNum::BigNum(const std::string &number, intType base) : storage(number), base(base) {}

BigNum::BigNum(BigNum &&moved)
{
    this->storage = std::move(moved.storage);
    this->base = moved.base;
}

BigNum::BigNum(std::vector<intType>&& digits, intType base, bool isPositive) : storage(std::move(digits), isPositive), base(base) {}

BigNum &BigNum::operator=(BigNum &&moved) {
    if (this != &moved)
    {
        this->storage = std::move(moved.storage);
        this->base = moved.base;
    }
    return *this;
}

void gcd(const BigNum &a, const BigNum &b, BigNum &result) {
    itPair leftDigits = a.storage.getConvertedDigits();
    std::vector<intType> leftDigitsVector(leftDigits.first, leftDigits.second);
    itPair rightDigits = b.storage.getConvertedDigits();
    std::vector<intType> rightDigitsVector(rightDigits.first, rightDigits.second);
    std::vector<intType> resultVector;
    BigNum::arithmetic.gcd(leftDigitsVector, rightDigitsVector, resultVector);
    result = std::move(BigNum(std::move(resultVector), BASENUM, true));
}

BigNum operator+(const BigNum &a, const BigNum &b)
{
    itPair leftDigits = a.storage.getConvertedDigits();
    itPair rightDigits = b.storage.getConvertedDigits();
    std::vector<intType> result;
    bool isLeftPos = a.storage.isPos();
    bool isRightPos = b.storage.isPos();
    bool isPositive = (isLeftPos == isRightPos);
    BigNum::arithmetic.add(leftDigits.first, leftDigits.second, rightDigits.first, rightDigits.second, result, isLeftPos, isRightPos, isPositive, 0);
    BigNum newNum(std::move(result), BASENUM, isPositive);
    return newNum;
}

BigNum operator-(const BigNum &a, const BigNum &b)
{
    itPair leftDigits = a.storage.getConvertedDigits();
    itPair rightDigits = b.storage.getConvertedDigits();
    std::vector<intType> result;
    bool isPositive, isLeftPos, isRightPos;
    isLeftPos = a.storage.isPos();
    isRightPos = b.storage.isPos();
    isPositive = (isLeftPos == isRightPos);
    BigNum::arithmetic.substract(leftDigits.first, leftDigits.second, rightDigits.first, rightDigits.second, result, isLeftPos, isRightPos, isPositive);
    BigNum newNum(std::move(result), BASENUM, isPositive);
    return newNum;
}

BigNum operator*(const BigNum &a, const BigNum &b)
{
    itPair leftDigits = a.storage.getConvertedDigits();
    itPair rightDigits = b.storage.getConvertedDigits();
    std::vector<intType> result;
    bool isLeftPos = a.storage.isPos();
    bool isRightPos = b.storage.isPos();
    bool isPositive = (isLeftPos == isRightPos);
    BigNum::arithmetic.simpleMultiplication(leftDigits.first, leftDigits.second, rightDigits.first, rightDigits.second, result);
    BigNum newNum(std::move(result), BASENUM, isPositive);
    return newNum;
}

BigNum BigNum::operator+(const BigNum& right) {
    return ::operator+(*this,right);
}

BigNum BigNum::operator-(const BigNum& right) {
    return ::operator-(*this, right);
}

BigNum BigNum::operator*(const BigNum &right) {
    return ::operator*(*this, right);
}

std::pair<BigNum, BigNum> BigNum::operator/(const BigNum &right)
{
    itPair leftDigits = this->storage.getConvertedDigits();
    itPair rightDigits = right.storage.getConvertedDigits();
    vecPair result;
    bool isLeftPos = this->storage.isPos();
    bool isRightPos = right.isPos();
    #if DEBUG
    std::cerr << isLeftPos << isRightPos << std::endl;
    std::cerr << isLeftPos << isRightPos << std::endl;
    #endif
    bool isPositive = (isLeftPos == isRightPos);
    BigNum::arithmetic.longDivision(leftDigits.first, leftDigits.second, rightDigits.first, rightDigits.second, result);
    BigNum Q(std::move(result.first), base, isPositive);
    BigNum R(std::move(result.second), base, isLeftPos);
    std::pair<BigNum, BigNum> dividedNum(std::move(Q), std::move(R));
    return dividedNum;
}

std::ostream& operator<<(std::ostream& os, const BigNum& num) {
    itPair vect = num.storage.getConvertedDigits();
    std::vector<intType> digits(vect.first, vect.second);
    if(digits.empty()) {
        os << "0";
        return os;
    }

    if (digits.size() > 1 || (digits.size() == 1 && digits[0] != 0))
    {
        if(num.isPos() == false) {
            os << '-';
        }
        else {
            os << '+';
        }
    }

    os << digits.back();

    for (int i = digits.size() - 2; i >= 0; --i) {
        os << std::setw(9) << std::setfill('0') << digits[i];
    }

    return os;
}

std::ostream &operator<<(std::ostream &os, const std::pair<BigNum,BigNum> &num)
{
    os << "---|---" << std::endl;
    os << "-------" << std::endl;
    os << "Quotient: " << num.first << std::endl;
    os << "-------" << std::endl;
    os << "Remainder: " << num.second << std::endl;
    os << "-------" << std::endl;
    os << "---|---" << std::endl << std::endl;
    return os;
}
