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
		cout << " [0] ���ư���" << endl;
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

// min~max ������ ���� �� ���� �Է¹޾� ��ȯ�Ѵ�.
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
			cout << "�߸� �Է��ϼ̽��ϴ�. " << min << " �̻�, " << max << " ������ ���ڸ� �Է��ϼ���." << endl << endl;
		}
		else if (num < min || num > max) {
			cout << "�߸� �Է��ϼ̽��ϴ�. " << min << " �̻�, " << max << " ������ ���ڸ� �Է��ϼ���." << endl << endl;
		}
		else {
			cin.ignore(256, '\n');

			return num;
		}
	}
}
// ���� �� ���� �Է¹޾� ��ȯ�Ѵ�.
int enterNumber() {
	while (true) {
		cout << "Enter >> ";

		int num;
		cin >> num;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(256, '\n');
			cout << "�߸� �Է��ϼ̽��ϴ�. ���ڸ� �Է��ϼ���." << endl << endl;
		}
		else {
			cin.ignore(256, '\n');

			return num;
		}
	}
}
// ���ڿ� �� ���� �Է¹޾� ��ȯ�Ѵ�.
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

// y �Ǵ� n�� �Է¹޾� ��ȯ�Ѵ�. (��ҹ��� ��� ����)
int enterYorN() {

	while (true) {
		cout << "Enter >> ";

		char yn;
		cin >> yn;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(256, '\n');
			cout << "�߸� �Է��ϼ̽��ϴ�. Y �Ǵ� N�� �Է��ϼ���." << endl << endl;
		}
		else if (yn == 'y' || yn == 'Y' || yn == 'n' || yn == 'N') {
			return (yn == 'y' || yn == 'Y') ? 1 : 0;
		}
		else {
			cout << "�߸� �Է��ϼ̽��ϴ�. Y �Ǵ� N�� �Է��ϼ���." << endl << endl;
		}
	}
}
void enterAnyKey() {
	cout << "����Ϸ��� �ƹ� Ű�� �Է��Ͻʽÿ�.";
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