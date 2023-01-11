#include "Common.h"

int pickRandom(int min, int max) {
	return rand() % (max - min + 1) + min;
}
int pickRandom(int min, int max, int exclude) {
	while (true) {
		int pick = pickRandom(min, max);
		if (pick != exclude) return pick;
	}
}

std::string genderToString(int gender) {
	if (gender == 0) return "남";
	else if (gender == 1) return "여";
	else return "?";
}

std::string dateToString(int month, int day) {
	return std::to_string(month) + "월 " + std::to_string(day) + "일";
}
std::string dateToString(int year, int month, int day) {
	return std::to_string(year) + "년 " + std::to_string(month) + "월 " + std::to_string(day) + "일";
}