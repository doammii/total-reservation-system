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
			menu = Menu(AnswerStr, "[1/7] 아이디를 입력하세요.");
			menu.Process();

			if (menu.ProcessResult) {
				id = menu.ResultString;
			}
			else {
				return false;
			}

			if (id.size() < 4) {
				menu = Menu(ShowAndInput, "4글자 이상 입력하세요.");
				menu.Process();
			}
			else if (findClientById(id) != -1) {
				menu = Menu(ShowAndInput, "이미 존재하는 ID입니다.");
				menu.Process();
				return false;
			}
			else {
				state = PASSWORD;
			}
			break;

		case PASSWORD:
			menu = Menu(AnswerPassword, "[2/7] 비밀번호를 입력하세요.");
			menu.Process();

			if (menu.ProcessResult) {
				password = menu.ResultString;
			}
			else {
				return false;
			}

			if (password.size() < 4) {
				menu = Menu(ShowAndInput, "4글자 이상 입력하세요.");
				menu.Process();
			}
			else {
				state = PASSWORD_RE;
			}
			break;

		case PASSWORD_RE:
			menu = Menu(AnswerPassword, "[3/7] 비밀번호를 재입력하세요. (확인용)");
			menu.Process();

			tmp;
			if (menu.ProcessResult) {
				tmp = menu.ResultString;
			}
			else {
				return false;
			}

			if (tmp != password) {
				menu = Menu(ShowAndInput, "비밀번호가 동일하지 않습니다.");
				menu.Process();
				state = PASSWORD;
			}
			else {
				state = FAMILY_ID;
			}
			break;

		case FAMILY_ID:
			menu = Menu(AnswerInt, "[4/7] 세대 번호를 입력하세요.");
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
			menu = Menu(AnswerStr, "[5/7] 이름을 입력하세요.");
			menu.Process();

			if (menu.ProcessResult) {
				name = menu.ResultString;
			}
			else {
				return false;
			}

			if (name.size() < 1) {
				menu = Menu(ShowAndInput, "1글자 이상 입력하세요.");
				menu.Process();
			}
			else {
				state = AGE;
			}
			break;

		case AGE:
			menu = Menu(AnswerInt, "[6/7] 나이를 입력하세요.");
			menu.Process();

			if (menu.ProcessResult) {
				age = menu.ResultInteger;
			}
			else {
				return false;
			}

			if (age < 0) {
				menu = Menu(ShowAndInput, "잘못된 나이입니다.");
				menu.Process();
			}
			else {
				state = GENDER;
			}
			break;

		case GENDER:
			menu = Menu(Select, "[7/7] 성별을 입력하세요.");
			menu.AddMenu("남");
			menu.AddMenu("여");
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
			menu = Menu(YesNo, "회원가입이 완료되었습니다. 바로 로그인하시겠습니까? [y/n]");
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

	Menu menu = Menu(AnswerStr, "아이디를 입력하세요.");
	menu.Process();
	id = menu.ResultString;

	menu = Menu(AnswerPassword, "비밀번호를 입력하세요.");
	menu.Process();
	password = menu.ResultString;

	int no = findClientById(id);
	if (no == -1 || !_clientList.at(no)->CheckPassword(password)) {
		menu = Menu(ShowAndInput, "아이디 또는 비밀번호가 올바르지 않습니다.");
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

