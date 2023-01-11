#include "StudyingRoomReservationSystem.h"
using namespace std;
using namespace StudyingRoom;

int Desk::_deskNo = 0;
string rowColToString(int row, int col) {
	string str;
	str = str + (char)(row + 65);
	str = str + (char)(col + 48);
	return str;
}
// 00��~23�ñ��� �Է� ����
string timeToString(int time) {
	string str = (time < 10 ? "0" : "") + to_string(time) + ":00 ~ " + (time + 1 < 10 ? "0" : "") + to_string(time + 1) + ":00";
	return str;
}
StudyingRoomReservationSystem::StudyingRoomReservationSystem() : BaseReservationSystem() {
	_clientList.push_back(new Client("test", "123123", "test", 0, 25, MALE));	// Test �� ����

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			_desk[i][j] = new Desk();
		}
	}
}

void StudyingRoomReservationSystem::Dispose() {
	for (int i = 0; i < _reservations.size(); i++) delete _reservations.at(i);
	_reservations.clear();

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			delete _desk[i][j];
		}
	}
}

void StudyingRoomReservationSystem::Run() {
	bool end = false;

	do {
		if (_client == -1) {
			_menu = Menu(Select, "������ ���� �ý���");
			_menu.AddMenu("�α���");
			_menu.AddMenu("ȸ������");
			_menu.AddMenu("����");
			_menu.Process();

			switch (_menu.ResultInteger) {
			case 1:
				SignIn();
				break;

			case 2:
				SignUp();
				break;

			case 3:
				end = true;
				break;
			}
		}
		else {
			_menu = Menu(Select, "������ ���� �ý���");
			_menu.AddMenu("����");
			_menu.AddMenu("���� ����");
			_menu.AddMenu("�α׾ƿ�");
			_menu.Process();

			switch (_menu.ResultInteger) {
			case 1:
				newReserve();
				break;

			case 2:
				manageReserve();
				break;

			case 3:
				SignOut();
				break;
			}
		}
	} while (!end);

	_menu = Menu(ShowAndInput, "�α׾ƿ��Ǿ����ϴ�.");
	_menu.Process();
}

int StudyingRoomReservationSystem::getClientTotalTime(Client* client) {
	int sum = 0;

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			sum += _desk[i][j]->GetClientTotalTime(client);
		}
	}

	return sum;
}

void StudyingRoomReservationSystem::newReserve() {
	if (_clientList[_client]->GetAge() <= 13) {
		_menu = Menu(ShowAndInput, "�� 13�� ���ϴ� ���� ����� ����� �� �����ϴ�.");
		_menu.Process();
		return;
	}

	cout << "�ܿ� �ð� : �ִ� " << to_string(MAX_TIME) << "�ð� �� " << to_string(MAX_TIME - getClientTotalTime(_clientList[_client])) << "�ð� �̿� ����" << endl;
	cout << GetDeskString();
	_menu = Menu(AnswerStr, "������ �¼��� �����ϼ���. [ex: A9]");
	_menu.Process();

	if (_menu.ProcessResult) {
		string input = _menu.ResultString;

		if (input.size() != 2) {
			_menu = Menu(ShowAndInput, "�߸� �Է��ϼ̽��ϴ�.");
			_menu.Process();
			return;
		}
		else {
			int row = input[0];
			int col = input[1];

			// ù��° ���ڰ� ���ĺ��� ��� 0~25�� ���ڷ� ��ȯ, ���ĺ��� �ƴѰ�� -1�� �����.
			if ('A' <= row && row <= 'Z') row -= 'A';
			else if ('a' <= row && row <= 'z') row -= 'a';
			else row = -1;

			// �ι�° ���ڰ� ���� �Ǵ� : ; < = > �� ��� 0~COLUMNS - 1�� ���ڷ� ��ȯ, ���ڰ� �ƴѰ�� -1�� �����.
			if ('0' <= col && col <= '>') col -= '0';
			else col = -1;

			if (row == -1 || row >= ROWS || col == -1 || col >= COLUMNS) {
				_menu = Menu(ShowAndInput, "�߸� �Է��ϼ̽��ϴ�.");
				_menu.Process();
				return;
			}

			if (_desk[row][col]->IsFull()) {
				_menu = Menu(ShowAndInput, "�����Ͻ� �¼����� �̿� ������ �ð��밡 �����ϴ�.");
				_menu.Process();
				return;
			}

			_menu = Menu(Select, "�̿��� �ð��� �����ϼ���.");
			for (int i = 0; i < TIMES; i++) {
				Client* client = _desk[row][col]->GetClientByTime(i + 9);
				_menu.AddMenu(timeToString(i + 9) + " " + (client != NULL ? client->GetName(true) + " (" + genderToString(client->GetGender()) + ")" : "����"));
			}
			_menu.Process();
			
			int select = _menu.ResultInteger;
			if (_desk[row][col]->GetClientByTime(select + 8) != NULL) {
				_menu = Menu(ShowAndInput, "�����Ͻ� �ð��� �̹� ����� �ð��Դϴ�.");
				_menu.Process();
				return;
			}

			// �����¿� �¼� ���� �˻� - �迭 �ε����� �Ѿ�� �ʵ��� �����ؾ� �Ѵ�.
				// �� = row - 1
			if ((row - 1 >= 0 && _desk[row - 1][col]->GetClientByTime(select + 8) != NULL && _desk[row - 1][col]->GetClientByTime(select + 8)->GetGender() == _clientList.at(_client)->GetGender()) ||
				// �� = row + 1
				(row + 1 < ROWS && _desk[row + 1][col]->GetClientByTime(select + 8) != NULL && _desk[row + 1][col]->GetClientByTime(select + 8)->GetGender() == _clientList.at(_client)->GetGender()) ||
				// �� = col - 1
				(col - 1 >= 0 && _desk[row][col - 1]->GetClientByTime(select + 8) != NULL && _desk[row][col - 1]->GetClientByTime(select + 8)->GetGender() == _clientList.at(_client)->GetGender()) ||
				// �� = col + 1
				(col + 1 < COLUMNS && _desk[row][col + 1]->GetClientByTime(select + 8) != NULL && _desk[row][col + 1]->GetClientByTime(select + 8)->GetGender() == _clientList.at(_client)->GetGender())) {
				_menu = Menu(ShowAndInput, "�����Ͻ� �ð��� �¼��� �ֺ��� ������ ������ �̿��ڰ� �����մϴ�.");
				_menu.Process();
				return;
			}

			if (getClientTotalTime(_clientList[_client]) >= MAX_TIME) {
				_menu = Menu(ShowAndInput, "���� ������ �ܿ� �ð��� �����ϴ�. ���� ������ �ִ� �ð��� ������ " + to_string(MAX_TIME) + "�ð��Դϴ�.");
				_menu.Process();
				return;
			}

			string recipe = "���� ������ Ȯ�����ּ���.\n";
			recipe += "�¼� ��ȣ : " + rowColToString(row, col) + "\n";
			recipe += "���� �ð� : " + timeToString(select + 8) + "\n";
			recipe += "���� �� �ܿ� �ð� : " + to_string(MAX_TIME - getClientTotalTime(_clientList[_client]) - 1) + "\n";
			recipe += "�̴�� �����Ͻðڽ��ϱ�? (y/n)";

			_menu = Menu(YesNo, recipe);
			_menu.Process();

			if (_menu.ResultInteger) {
				_desk[row][col]->SetClientToTime(select + 8, _clientList[_client]);

				Reservation* res = new Reservation();
				res->ClientNo = _client;
				res->Row = row;
				res->Column = col;
				res->Time = select;
				_reservations.push_back(res);

				_menu = Menu(ShowAndInput, "����Ǿ����ϴ�.");
			}
			else {
				_menu = Menu(ShowAndInput, "��ҵǾ����ϴ�.");
			}
			_menu.Process();
		}
	}
}

void StudyingRoomReservationSystem::manageReserve() {
	// ���� �α��ε� ȸ���� ����� ���� ����� �ӽ÷� ������ ����
	vector<Reservation*> tmp;
	// ������ ���� ����� ������ �� �ش� ���� ����� �ε���(��ü ���� ��ϻ���)�� �Բ� ������ ���� - ���� ������ �� �ʿ� 
	vector<int> indices;
	for (int i = 0; i < _reservations.size(); i++) {
		if (_reservations.at(i)->ClientNo == _client) {
			tmp.push_back(_reservations.at(i));
			indices.push_back(i);
		}
	}

	if (tmp.size() != 0) {
		_menu = Menu(Select, "������ �����ϼ���.");
		for (int i = 0; i < tmp.size(); i++) {
			_menu.AddMenu("�¼� " + rowColToString(tmp.at(i)->Row, tmp.at(i)->Column) + "(" + timeToString(tmp.at(i)->Time + 9) + ")");
		}
		_menu.AddMenu("���");

		_menu.Process();

		int index = _menu.ResultInteger - 1;
		if (index != tmp.size()) {
			_menu = Menu(Select, "���� ���� �޴�");
			_menu.AddMenu("�� ��ȸ");
			_menu.AddMenu("���� ���");
			_menu.AddMenu("���ư���");

			_menu.Process();

			int select = _menu.ResultInteger;
			Reservation* res;

			switch (select)
			{
			case 1:
				// �� ��ȸ
				res = tmp.at(index);

				_menu = Menu(ShowAndInput, string("�� ��ȸ ���\n") +
					"������ : " + _clientList.at(res->ClientNo)->GetName() + "\n" +
					"�¼� : " + rowColToString(res->Row, res->Column) + "\n" +
					"���� �ð� : " + timeToString(res->Time + 9));
				_menu.Process();
				break;

			case 2:
				// ���� ���
				_menu = Menu(YesNo, "���� �ش� ������ ����Ͻðڽ��ϱ�? [y/n]");
				_menu.Process();

				if (_menu.ResultInteger) {
					res = _reservations.at(index);
;					_desk[res->Row][res->Column]->SetClientToTime(res->Time + 9, NULL);
					_reservations.erase(_reservations.begin() + indices.at(index));

					_menu = Menu(ShowAndInput, "���������� ��ҵǾ����ϴ�.");
					_menu.Process();
				}
				break;

			case 3:
				// ���ư��� - Do nothing
				break;
			}
		}
	}
	else {
		_menu = Menu(ShowAndInput, "������ ����ֽ��ϴ�.");
		_menu.Process();
	}
}

string StudyingRoomReservationSystem::GetDeskString() {
	string str;

	str += "  0 1 2 3 4 5 6 7 8 9 : ; < = >\n";
	//for (int i = 0; i < COLUMNS; i++) str += 
	for (int i = 0; i < ROWS; i++) {
		str = str + (char)(65 + i);
		str = str + " ";
		for (int j = 0; j < COLUMNS; j++) {
			if (_desk[i][j]->IsFull()) str += "X ";
			else str += "- ";
		}
		str += "\n";
	}
	str += "(- : �ܿ� �¼� ����, X : �ܿ� �¼� ����)\n";

	return str;
}

string StudyingRoomReservationSystem::GetAllReservations() {
	string str = "============ ������ ���� �ý��� ============\n";

	if (_reservations.empty()) {
		str += " ���� ����� ������ϴ�.\n";
	}

	for (int i = 0; i < _reservations.size(); i++) {
		Reservation* res = _reservations.at(i);
		str += "[" + to_string(i) + "] " + _clientList.at(res->ClientNo)->GetName() + " �¼� ��ȣ: " + rowColToString(res->Row, res->Column) + " (" + timeToString(res->Time + 9) + ")\n";
	}

	return str;
}