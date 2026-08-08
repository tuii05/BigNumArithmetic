#include "Arithmetic.h"
#include <algorithm>


// Checks the relation between two big integer (bigger, less, equal), doesn't care about the sign.
void Arithmetic::compare(vecIt aBegin, vecIt aEnd, vecIt bBegin, vecIt bEnd, bool &isABigger, bool &isEqual)
{
    size_t sizeA = aEnd - aBegin;
    size_t sizeB = bEnd - bBegin;

    if (sizeA > sizeB)
    {
        isABigger = true;
        isEqual = false;
        return;
    }
    else if (sizeA < sizeB)
    {
        isABigger = false;
        isEqual = false;
        return;
    }

    for (size_t i = 0; i < sizeA; ++i)
    {
        size_t idx = sizeA - 1 - i;

        intType a = *(aBegin + idx);
        intType b = *(bBegin + idx);

        if (a > b)
        {
            isABigger = true;
            isEqual = false;
            return;
        }
        else if (a < b)
        {
            isABigger = false;
            isEqual = false;
            return;
        }
    }
    isABigger = false;
    isEqual = true;
}

//Perform basic addition for two large integers.
//Checks the signs, thus it will correctly handle the output.
//Uses the standard algorithm for addition, which is performed in everyday life.
void Arithmetic::add(vecIt aBegin, vecIt aEnd, vecIt bBegin, vecIt bEnd, std::vector<intType> &result, bool isPositiveA, bool isPositiveB, bool &isPositiveSign, size_t offset = 0)
{
    if(isPositiveA && !isPositiveB) {
        substract(aBegin, aEnd, bBegin, bEnd, result, isPositiveA, true, isPositiveSign);
        return;
    }
    else if(!isPositiveA && isPositiveB) {
        substract(bBegin, bEnd, aBegin, aEnd, result, isPositiveB, true, isPositiveSign);
        return;
    }
    else if(isPositiveA && isPositiveB) {
        isPositiveSign = true;
    }
    else if(!isPositiveA && !isPositiveB) {
        isPositiveSign = false;
    }
    size_t sizeA = aEnd - aBegin;
    size_t sizeB = bEnd - bBegin;
    size_t size = std::max(sizeA, sizeB);
    if(result.size() < offset + size + 1) {
        result.resize(offset + size + 1, 0);
    }
    uint64_t carry = 0;
    for (size_t i = 0; i < size; ++i) {
        uint64_t a_ = (i < sizeA) ? *(aBegin + i) : 0;
        uint64_t b_ = (i < sizeB) ? *(bBegin + i) : 0;

        uint64_t sum = static_cast<uint64_t>(result[i+offset]) +  a_ + b_ + carry; // Needed the offset for a different multiplication algorithm, however I used the basic multiplication algorithm instead so it is unused.
        carry = sum / base;
        result[i + offset] = static_cast<intType>(sum % base);
    }
    if(carry != 0) {
        result[offset + size] = static_cast<intType>(carry);
    }

    while(result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }
}

//Runs the iterator type of addition to prevent unnecessary copies for huge numbers.
std::vector<intType> Arithmetic::add(const std::vector<intType>& a, const std::vector<intType>& b, bool isPositiveA, bool isPositiveB, bool& isPositiveSign) {
    std::vector<intType> result;
    add(a.begin(), a.end(), b.begin(), b.end(), result, isPositiveA, isPositiveB, isPositiveSign, 0);
    return result;
}

//Performs substraction between two large numbers using the basic substraction algorithm.
//Check signs and handles them correctly and computes the overall sign of the outcome.
void Arithmetic::substract(vecIt aBegin, vecIt aEnd, vecIt bBegin, vecIt bEnd, std::vector<intType> &result, bool isPositiveA, bool isPositiveB, bool &isPositiveSign)
{
    if (!isPositiveB)
    {
        add(aBegin, aEnd, bBegin, bEnd, result, isPositiveA, true, isPositiveSign);
        return;
    }

    if (!isPositiveA && isPositiveB)
    {
        add(aBegin, aEnd, bBegin, bEnd, result, true, true, isPositiveSign);
        isPositiveSign = false;
        return;
    }

    if (!isPositiveA && !isPositiveB)
    {
        substract(bBegin, bEnd, aBegin, aEnd, result, true, true, isPositiveSign);
        return;
    }

    bool isABigger, isEqual;
    compare(aBegin, aEnd, bBegin, bEnd, isABigger, isEqual);

    if (isEqual)
    {
        result = {0};
        isPositiveSign = true;
        return;
    }

    vecIt bigBegin, bigEnd, smallBegin, smallEnd;
    if (isABigger)
    {
        isPositiveSign = true;
        bigBegin = aBegin;
        bigEnd = aEnd;
        smallBegin = bBegin;
        smallEnd = bEnd;
    }
    else
    {
        isPositiveSign = false;
        bigBegin = bBegin;
        bigEnd = bEnd;
        smallBegin = aBegin;
        smallEnd = aEnd;
    }

    size_t bigSize = bigEnd - bigBegin;
    size_t smallSize = smallEnd - smallBegin;

    result.assign(bigSize, 0);

    uint64_t carry = 0;

    for (size_t i = 0; i < bigSize; ++i)
    {
        int64_t A = *(bigBegin + i);
        int64_t B = (i < smallSize) ? *(smallBegin + i) : 0;

        int64_t diff = A - B - carry;

        if (diff < 0)
        {
            diff += base;
            carry = 1;
        }
        else
        {
            carry = 0;
        }

        result[i] = static_cast<intType>(diff);
    }

    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }
}

//Runs the iterator type of substraction algorithm to prevent unnecessary copies for large numbers which can be costly and quite tedious for nested operators (a-b-c-d....).
std::vector<intType> Arithmetic::substract(const std::vector<intType>& a, const std::vector<intType>& b, bool isPositiveA, bool isPositiveB, bool& isPositiveSign) {
    std::vector<intType> result;
    substract(a.begin(), a.end(), b.begin(), b.end(), result, isPositiveA, isPositiveB, isPositiveSign);
    return result;
}


//Uses the standard multiplication algorithm for two large integers numbers.
void Arithmetic::simpleMultiplication(vecIt aBegin, vecIt aEnd, vecIt bBegin, vecIt bEnd, std::vector<intType> &result)
{
    size_t sizeA = aEnd - aBegin;
    size_t sizeB = bEnd - bBegin;

    std::vector<intType> a(aBegin, aEnd);
    std::vector<intType> b(bBegin, bEnd);

    result.resize(sizeA + sizeB, 0);
    uint64_t carry = 0;
    for (size_t i = 0; i < sizeA; ++i)
    {
        carry = 0;
        for (size_t j = 0; j < sizeB; ++j)
        {
            uint64_t sum = uint64_t(a[i]) * b[j] + result[i + j] + carry;
            result[i + j] = sum % base;
            carry = sum / base;
        }
        result[i + b.size()] = carry;
    }
    while (result.size() > 1 && result.back() == 0)
        result.pop_back();
}

// Runs the standard multiplication algorithm after checking the sign and setting the overall sign of the outcome to the proper one.
void Arithmetic::multiply(vecIt aBegin, vecIt aEnd, vecIt bBegin, vecIt bEnd, std::vector<intType>& result, bool isPositiveA, bool isPositiveB, bool& isPositiveSign) {
    if(!isPositiveA && isPositiveB) {
        isPositiveSign = false;
    }
    else if(isPositiveA && !isPositiveB) {
        isPositiveSign = false;
    }
    else {
        isPositiveSign = true;
    }
    simpleMultiplication(aBegin, aEnd, bBegin, bEnd, result);
}

// Runs the iterator type of multiplication to prevent unnecessary copies for huge numbers.
std::vector<intType> Arithmetic::multiply(const std::vector<intType> &a, const std::vector<intType> &b, bool isPositiveA, bool isPositiveB, bool& isPositiveSign)
{
    std::vector<intType> result;
    multiply(a.begin(), a.end(), b.begin(), b.end(), result, isPositiveA, isPositiveB, isPositiveSign);
    return result;
}

//Handles the case for multiplication when: a.size() == 1, b.size() > 1
std::vector<intType> Arithmetic::multiply(intType a, const std::vector<intType> &b) {
    size_t size = b.size() + 1;
    std::vector<intType> result(size, 0);
    uint64_t carry = 0;
    for(size_t i = 0; i < b.size(); ++i) {
        uint64_t product = static_cast<uint64_t>(a) * b[i] + carry;
        result[i] = static_cast<intType>(product % base);
        carry = product / base;
    }
    if(carry != 0 ) {
        result[size - 1] = static_cast<intType>(carry);
    }
    else {
        result.pop_back();
    }
    return result;
}

//Handles the case for multiplication when: a.size() > 1, b.size() == 1
std::vector<intType> Arithmetic::multiply(const std::vector<intType> &b, intType a)
{
    size_t size = b.size() + 1;
    std::vector<intType> result(size, 0);
    uint64_t carry = 0;
    for (size_t i = 0; i < b.size(); ++i)
    {
        uint64_t product = static_cast<uint64_t>(a) * b[i] + carry;
        result[i] = static_cast<intType>(product % base);
        carry = product / base;
    }
    if (carry != 0)
    {
        result[size - 1] = static_cast<intType>(carry);
    }
    else
    {
        result.pop_back();
    }
    return result;
}

// Runs the iterator type of division to prevent unnecessary copies for huge numbers.
vecPair Arithmetic::divide(const std::vector<intType> &a, const std::vector<intType> &b) {
    vecPair result;
    divide(a.begin(), a.end(), b.begin(), b.end(), result);
    return result;
}

// Calls the school divison algorithm. I made a function for this since I can change the division algorithm at any point I want.
void Arithmetic::divide(vecIt aBegin, vecIt aEnd, vecIt bBegin, vecIt bEnd, vecPair& result) {
    longDivision(aBegin, aEnd, bBegin, bEnd, result);
}

// Performs school division for two large integer numbers.
void Arithmetic::longDivision(vecIt aBegin, vecIt aEnd, vecIt bBegin, vecIt bEnd, vecPair &result)
{
    result.first.clear();
    result.second.clear();

    std::vector<intType> dividend(aBegin, aEnd);
    std::vector<intType> divisor(bBegin, bEnd);

    if(dividend.size() == 1 && divisor.size() == 1) {
        intType a, b;
        a = *dividend.begin();
        b = *divisor.begin();
        result.first = {static_cast<intType>(a / b)};
        result.second = {static_cast<intType>(a % b)};
        return;
    }

    while (!dividend.empty() && dividend.back() == 0) {
        dividend.pop_back();
    }
    while (!divisor.empty() && divisor.back() == 0) {
        divisor.pop_back();
    }

    if (divisor.empty()) {
        throw std::runtime_error("Division with zero is not possible.");
    }

    bool isABigger, areTheyEqual;
    compare(dividend.begin(), dividend.end(), divisor.begin(), divisor.end(), isABigger, areTheyEqual);

    if (dividend.empty() || !isABigger)
    {
        result.first = {0};
        result.second = dividend;
        return;
    }

    std::vector<intType> quotient;
    std::vector<intType> remainder;

    for (size_t i = dividend.size(); i-- > 0;) {
        remainder.insert(remainder.begin(), dividend[i]);

        while (remainder.size() > 1 && remainder.back() == 0)
            remainder.pop_back();

        intType q = 0;

        bool isBigger, isEqual;
        compare(remainder.begin(), remainder.end(), divisor.begin(), divisor.end(), isBigger, isEqual);

        if (isBigger || isEqual)
        {
            intType low = 1, high = base - 1;
            intType best = 0;
            while (low <= high) // Searching for the integer of floor({current number} / divisor) and calculating a remainder for that.
            {
                intType mid = low + (high - low) / 2;
                std::vector<intType> prod = multiply(divisor, mid);

                compare(remainder.begin(), remainder.end(), prod.begin(), prod.end(), isBigger, isEqual);

                if (isBigger || isEqual)
                {
                    best = mid;
                    low = mid + 1;
                }
                else
                {
                    high = mid - 1;
                }
            }
            q = best;

            bool isPos;
            remainder = substract(remainder, multiply(divisor, q), true, true, isPos);
        }

        quotient.insert(quotient.begin(), q);
    }

    while (quotient.size() > 1 && quotient.back() == 0) {
        quotient.pop_back();
    }
    result.first = quotient;
    result.second = remainder.empty() ? std::vector<intType>{0} : remainder;
}

void Arithmetic::gcd(const std::vector<intType>& a, const std::vector<intType>&b, std::vector<intType>& result) {

    auto isZero = [](const std::vector<intType>& vec) { return (vec.size() == 1) && (*vec.begin() == 0); };

    if (isZero(b))
    {
        throw std::runtime_error("You can't divide with 0!");
    }

    std::vector<intType> a_ = a;
    std::vector<intType> b_ = b;

    vecPair divResult;

    while(!isZero(b_)) {
        divide(a_.begin(), a_.end(), b_.begin(), b_.end(), divResult);

        std::vector<intType> remainder = std::move(divResult.second);

        a_ = std::move(b_);
        b_ = std::move(remainder);
    }

    result = std::move(a_);
}