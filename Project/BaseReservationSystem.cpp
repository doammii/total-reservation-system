#include "BaseReservationSystem.h"

string toUpper(string str) {
	string result = str;
	for (int i = 0; i < result.size(); i++) {
		if (isalpha(result[i])) {
			result[i] = toupper(result[i]);
		}
	}

	return result;
}

BaseReservationSystem::BaseReservationSystem() {
	_client = -1;
}

int BaseReservationSystem::findClientById(string id) {
	for (int i = 0; i < _clientList.size(); i++) {
		if (toUpper(_clientList.at(i)->GetId()) == toUpper(id)) {
			return i;
		}
	}

	return -1;
}

const int ID = 0;
const int PASSWORD = 1;
const int PASSWORD_RE = 2;
const int FAMILY_ID = 3;
const int NAME = 4;
const int AGE = 5;
const int GENDER = 6;
const int END = 7;

int BaseReservationSystem::SignUp() {
	string id, password, name, tmp;
	int familyId, age, gender;

	int state = ID;

	Menu menu;

	while (true) {
		switch (state) {
		case ID:
			menu = Menu(AnswerStr, "[1/7] ���̵� �Է��ϼ���.");
			menu.Process();

			if (menu.ProcessResult) {
				id = menu.ResultString;
			}
			else {
				return false;
			}

			if (id.size() < 4) {
				menu = Menu(ShowAndInput, "4���� �̻� �Է��ϼ���.");
				menu.Process();
			}
			else if (findClientById(id) != -1) {
				menu = Menu(ShowAndInput, "�̹� �����ϴ� ID�Դϴ�.");
				menu.Process();
				return false;
			}
			else {
				state = PASSWORD;
			}
			break;

		case PASSWORD:
			menu = Menu(AnswerPassword, "[2/7] ��й�ȣ�� �Է��ϼ���.");
			menu.Process();

			if (menu.ProcessResult) {
				password = menu.ResultString;
			}
			else {
				return false;
			}

			if (password.size() < 4) {
				menu = Menu(ShowAndInput, "4���� �̻� �Է��ϼ���.");
				menu.Process();
			}
			else {
				state = PASSWORD_RE;
			}
			break;

		case PASSWORD_RE:
			menu = Menu(AnswerPassword, "[3/7] ��й�ȣ�� ���Է��ϼ���. (Ȯ�ο�)");
			menu.Process();

			tmp;
			if (menu.ProcessResult) {
				tmp = menu.ResultString;
			}
			else {
				return false;
			}

			if (tmp != password) {
				menu = Menu(ShowAndInput, "��й�ȣ�� �������� �ʽ��ϴ�.");
				menu.Process();
				state = PASSWORD;
			}
			else {
				state = FAMILY_ID;
			}
			break;

		case FAMILY_ID:
			menu = Menu(AnswerInt, "[4/7] ���� ��ȣ�� �Է��ϼ���.");
			menu.Process();

			if (menu.ProcessResult) {
				familyId = menu.ResultInteger;
			}
			else {
				return false;
			}

			state = NAME;
			break;

		case NAME:
			menu = Menu(AnswerStr, "[5/7] �̸��� �Է��ϼ���.");
			menu.Process();

			if (menu.ProcessResult) {
				name = menu.ResultString;
			}
			else {
				return false;
			}

			if (name.size() < 1) {
				menu = Menu(ShowAndInput, "1���� �̻� �Է��ϼ���.");
				menu.Process();
			}
			else {
				state = AGE;
			}
			break;

		case AGE:
			menu = Menu(AnswerInt, "[6/7] ���̸� �Է��ϼ���.");
			menu.Process();

			if (menu.ProcessResult) {
				age = menu.ResultInteger;
			}
			else {
				return false;
			}

			if (age < 0) {
				menu = Menu(ShowAndInput, "�߸��� �����Դϴ�.");
				menu.Process();
			}
			else {
				state = GENDER;
			}
			break;

		case GENDER:
			menu = Menu(Select, "[7/7] ������ �Է��ϼ���.");
			menu.AddMenu("��");
			menu.AddMenu("��");
			menu.Process();

			if (menu.ProcessResult) {
				gender = menu.ResultInteger;
			}
			else {
				return false;
			}

			state = END;
			break;

		case END:
			_clientList.push_back(new Client(id, password, name, familyId, age, gender));
			menu = Menu(YesNo, "ȸ�������� �Ϸ�Ǿ����ϴ�. �ٷ� �α����Ͻðڽ��ϱ�? [y/n]");
			menu.Process();

			if (menu.ResultInteger) {
				return (_client = _clientList.size() - 1);
			}
			else {
				return -1;
			}
		}
	}
}

int BaseReservationSystem::SignIn() {
	string id, password;

	Menu menu = Menu(AnswerStr, "���̵� �Է��ϼ���.");
	menu.Process();
	id = menu.ResultString;

	menu = Menu(AnswerPassword, "��й�ȣ�� �Է��ϼ���.");
	menu.Process();
	password = menu.ResultString;

	int no = findClientById(id);
	if (no == -1 || !_clientList.at(no)->CheckPassword(password)) {
		menu = Menu(ShowAndInput, "���̵� �Ǵ� ��й�ȣ�� �ùٸ��� �ʽ��ϴ�.");
		menu.Process();
		return -1;
	}
	else {
		return (_client = no);
	}
}
bool BaseReservationSystem::SignOut() {
	_client = -1;
	return true;
}

