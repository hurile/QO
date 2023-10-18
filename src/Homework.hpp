#include "HomeworkLogic.hpp"
#define HWTASK(a, b) static size_t hw_ ## a = tinydb::HomeworkLogic::getInstance().addHandler((#a), [](int, const char* const*) -> std::string { return (b); });