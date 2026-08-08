#include <vector>
#include <cstdint>
#include <algorithm>

#define DEBUG false
#define BASENUM 1000000000

using intType = uint32_t;
using uintMatrix = std::vector<std::vector<intType>>;
using vecIt = std::vector<intType>::const_iterator;
using itPair = std::pair<vecIt, vecIt>;
using vecPair = std::pair<std::vector<intType>, std::vector<intType>>;
