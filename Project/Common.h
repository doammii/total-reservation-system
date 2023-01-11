#ifndef _COMMON_H_
#define _COMMON_H_
#include <iostream>
#include <string>
#include <vector>

int pickRandom(int min, int max);
int pickRandom(int min, int max, int exclude);
std::string genderToString(int gender);
std::string dateToString(int month, int day);
std::string dateToString(int year, int month, int day);
#endif