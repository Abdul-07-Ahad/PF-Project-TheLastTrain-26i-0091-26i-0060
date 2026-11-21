#ifndef UTIL_H
#define UTIL_H
#include "state.h"

// Helper utilities for clamping, string comparisons, and screen control
int clampInt(int val, int minVal, int maxVal);
void trimWhitespace(char* str);
bool stringEquals(const char* a, const char* b);
void printHeader(const char* title);
void clearScreen();
const char* getValue(const char* line, const char* key); //added helper function myself that was not already given
const char* getObjectiveName(int index);

#endif // UTIL_H
