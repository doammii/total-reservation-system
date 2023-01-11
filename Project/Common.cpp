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
	if (gender == 0) return "��";
	else if (gender == 1) return "��";
	else return "?";
}

std::string dateToString(int month, int day) {
	return std::to_string(month) + "�� " + std::to_string(day) + "��";
}
std::string dateToString(int year, int month, int day) {
	return std::to_string(year) + "�� " + std::to_string(month) + "�� " + std::to_string(day) + "��";
}