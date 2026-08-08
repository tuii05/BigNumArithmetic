#pragma once
#include <iostream>
#include "Definitions.h"

class Arithmetic {
    public:
        explicit Arithmetic(std::uint32_t base) : base(base) {}
        Arithmetic() : base(2) {}

        Arithmetic(const Arithmetic&) = delete;
        Arithmetic operator=(const Arithmetic &) = delete;

        Arithmetic(Arithmetic&&) = delete;
        Arithmetic operator=(Arithmetic &&) = delete;

    public:
        void compare(vecIt,vecIt,vecIt,vecIt, bool&, bool&);

        void add(vecIt, vecIt, vecIt, vecIt, std::vector<uint32_t> &, bool, bool, bool &, size_t);
        std::vector<uint32_t> add(const std::vector<uint32_t> &, const std::vector<uint32_t> &, bool, bool, bool&);

        void substract(vecIt, vecIt, vecIt, vecIt, std::vector<uint32_t>&, bool, bool, bool&);
        std::vector<uint32_t> substract(const std::vector<uint32_t> &, const std::vector<uint32_t> &, bool, bool, bool&);

        void simpleMultiplication(vecIt, vecIt, vecIt, vecIt, std::vector<uint32_t> &);
        void multiply(vecIt, vecIt, vecIt, vecIt, std::vector<uint32_t>&, bool, bool, bool&);
        std::vector<uint32_t> multiply(const std::vector<uint32_t> &, const std::vector<uint32_t> &, bool, bool, bool&);
        std::vector<uint32_t> multiply(uint32_t, const std::vector<uint32_t> &);
        std::vector<uint32_t> multiply(const std::vector<uint32_t>&, uint32_t);

        vecPair divide(const std::vector<uint32_t> &, const std::vector<uint32_t> &);
        void divide(vecIt, vecIt, vecIt, vecIt, vecPair &);
        void longDivision(vecIt, vecIt, vecIt, vecIt, vecPair&);

        void gcd(const std::vector<intType> &, const std::vector<intType> &, std::vector<intType> &);

    private:
        const std::uint32_t base;
};