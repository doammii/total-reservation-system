#include "Menu.h"
#include <conio.h>
using namespace std;

Menu::Menu() {

}
Menu::Menu(MenuType menuType) {
	_menuType = menuType;
}
Menu::Menu(MenuType menuType, string title) {
	_menuType = menuType;
	_title = title;
}

void Menu::AddMenu(string msg) {
	_menus.push_back(msg);
}

void printMenu(const vector<string> menu) {
	cout << "=======================" << endl;

	if (menu.size() == 0) {
		cout << " [0] 돌아가기" << endl;
	}
	else {
		for (int i = 0; i < menu.size(); i++) {
			cout << " [" << (i + 1) << "] " << menu.at(i) << endl;
		}
	}

	cout << "=======================" << endl;
}
void printMenu(string msg) {
	cout << msg << endl;
}

// min~max 사이의 숫자 한 개를 입력받아 반환한다.
int enterNumber(int min, int max) {
	if (min == 1 && max == 0) {
		min = max = 0;
	}

	while (true) {
		cout << "Enter >> ";

		int num;
		cin >> num;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(256, '\n');
			cout << "잘못 입력하셨습니다. " << min << " 이상, " << max << " 이하의 숫자를 입력하세요." << endl << endl;
		}
		else if (num < min || num > max) {
			cout << "잘못 입력하셨습니다. " << min << " 이상, " << max << " 이하의 숫자를 입력하세요." << endl << endl;
		}
		else {
			cin.ignore(256, '\n');

			return num;
		}
	}
}
// 숫자 한 개를 입력받아 반환한다.
int enterNumber() {
	while (true) {
		cout << "Enter >> ";

		int num;
		cin >> num;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(256, '\n');
			cout << "잘못 입력하셨습니다. 숫자만 입력하세요." << endl << endl;
		}
		else {
			cin.ignore(256, '\n');

			return num;
		}
	}
}
// 문자열 한 줄을 입력받아 반환한다.
string enterString() {
	cout << "Enter >> ";

	string str;
	getline(cin, str);

	return str;
}

string enterPassword() {
	cout << "Enter >> ";

	string str = "";
	char buf;
	do {
		buf = _getch();

		if (buf == '\r') {
			cout << endl;
			break;
		}
		else {
			str += buf;
			cout << "*";
		}
	} while (true);

	return str;
}

// y 또는 n을 입력받아 반환한다. (대소문자 상관 없음)
int enterYorN() {

	while (true) {
		cout << "Enter >> ";

		char yn;
		cin >> yn;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(256, '\n');
			cout << "잘못 입력하셨습니다. Y 또는 N을 입력하세요." << endl << endl;
		}
		else if (yn == 'y' || yn == 'Y' || yn == 'n' || yn == 'N') {
			return (yn == 'y' || yn == 'Y') ? 1 : 0;
		}
		else {
			cout << "잘못 입력하셨습니다. Y 또는 N을 입력하세요." << endl << endl;
		}
	}
}
void enterAnyKey() {
	cout << "계속하려면 아무 키나 입력하십시오.";
	_getch();
	cout << endl;
}

void Menu::Process() {
	try {
		switch (_menuType) {
		case YesNo:
			printMenu(_title);
			ResultInteger = enterYorN();
			break;

		case ShowAndInput:
			printMenu(_title);
			enterAnyKey();
			ResultInteger = 1;
			break;

		/*case Show:
			printMenu(_title);
			ResultInteger = 1;
			break;*/

		case Select:
			printMenu(_title);
			printMenu(_menus);
			ResultInteger = enterNumber(1, _menus.size());
			break;

		case AnswerStr:
			printMenu(_title);
			ResultString = enterString();
			break;

		case AnswerInt:
			printMenu(_title);
			ResultInteger = enterNumber();
			break;

		case AnswerPassword:
			printMenu(_title);
			ResultString = enterPassword();
			break;
		}

		ProcessResult = true;
		system("cls");
	}
	catch (int error_no) {
		ProcessResult = false;
	}
}