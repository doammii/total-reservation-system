#include "RestaurantReservationSystem.h"
using namespace std;
using namespace Restaurant;

int Table::_tableNo = 0;
RestaurantReservationSystem::RestaurantReservationSystem() : BaseReservationSystem() {
	_clientList.push_back(new Client("test", "123123", "test", 0, 24, 0));	// Test �� ����

	srand(time(NULL));

	int tables = 8;	// ���̺� �� (8�� ����)
	for (int i = 0; i < tables; i++) {
		_tables.push_back(new Table(pickRandom(1, 4) * 2));	// ���̺�� �¼� �� (2,4,6,8 �� ����)
	}
}

void RestaurantReservationSystem::Dispose() {
	for (int i = 0; i < _tables.size(); i++) delete _tables.at(i);
	_tables.clear();

	for (int i = 0; i < _reservations.size(); i++) delete _reservations.at(i);
	_reservations.clear();
}

void RestaurantReservationSystem::Run() {
	bool end = false;

	do {
		if (_client == -1) {
			_menu = Menu(Select, "�Ĵ� ���� �ý���");
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
			_menu = Menu(Select, "�Ĵ� ���� �ý���");
			_menu.AddMenu("����");
			_menu.AddMenu("Walk-In");
			_menu.AddMenu("���� ����");
			_menu.AddMenu("�α׾ƿ�");
			_menu.Process();

			switch (_menu.ResultInteger) {
			case 1:
				newReserve();
				break;

			case 2:
				walkIn();
				break;

			case 3:
				manageReserve();
				break;

			case 4:
				SignOut();
				break;
			}
		}
	} while (!end);

	_menu = Menu(ShowAndInput, "�α׾ƿ��Ǿ����ϴ�.");
	_menu.Process();
}

void RestaurantReservationSystem::newReserve() {
	if (_clientList[_client]->GetAge() <= 7) {
		_menu = Menu(ShowAndInput, "�� 7�� ���ϴ� ���� ����� ����� �� �����ϴ�.");
		_menu.Process();
		return;
	}

	_menu = Menu(Select, "������ ��¥�� �����ϼ���.");
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 0));
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 1));
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 2));
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 3));
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 4));
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 5));
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 6));
	_menu.Process();

	int day = _menu.ResultInteger - 1;

	_menu = Menu(AnswerInt, "���� �ο��� �Է��ϼ���.");
	_menu.Process();

	int seats = _menu.ResultInteger;

	// �̿� ������ �¼� Ž��
	vector<Table*> tmp;
	for (int i = 0; i < _tables.size(); i++) {
		if (_tables.at(i)->client[day] == NULL &&
			_tables.at(i)->Seats >= seats) {
			tmp.push_back(_tables.at(i));
		}
	}

	if (tmp.size() != 0) {
		_menu = Menu(Select, "���� ������ �¼� ����� �Ʒ��� �����ϴ�. �¼��� �����ϼ���.");
		for (int i = 0; i < tmp.size(); i++) {
			_menu.AddMenu(tmp.at(i)->GetSimpleInfo(day));
		}
		_menu.Process();

		int seat = _menu.ResultInteger - 1;

		Table* p = tmp.at(seat);
		string recipe = "���� ������ Ȯ�����ּ���.\n";
		recipe += "���̺� ��ȣ : " + to_string(tmp.at(seat)->TableNo) + "\n";
		recipe += "�ο� : " + to_string(seats) + "\n";
		recipe += "�̴�� �����Ͻðڽ��ϱ�? (y/n)";

		_menu = Menu(YesNo, recipe);
		_menu.Process();


		if (_menu.ResultInteger) {
			Client* client = _clientList.at(_client);
			Reservation* res = new Reservation();
			res->Table = p;
			res->ClientNo = _client;
			res->Seats = seats;
			res->Day = day + TODAY_DAY;

			p->client[day] = client;
			_reservations.push_back(res);

			_menu = Menu(ShowAndInput, "����Ǿ����ϴ�.");
		}
		else {
			_menu = Menu(ShowAndInput, "��ҵǾ����ϴ�.");
		}
		_menu.Process();
	}
	else {
		_menu = Menu(ShowAndInput, "�Է��Ͻ� ������ ���� ������ �¼��� �������� �ʽ��ϴ�.");
		_menu.Process();
	}
}

void RestaurantReservationSystem::manageReserve() {
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

	if (!tmp.empty()) {
		_menu = Menu(Select, "������ �����ϼ���.");
		for (int i = 0; i < tmp.size(); i++) {
			_menu.AddMenu(to_string(tmp.at(i)->Table->TableNo) + "�� ���̺� (" + to_string(tmp.at(i)->Seats) + "��)");
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
					"���̺� : " + to_string(res->Table->TableNo) + "�� ���̺� (" + to_string(res->Seats) + "��)\n" +
					"���� ��¥ : " + to_string(TODAY_YEAR) + "�� " + to_string(TODAY_MONTH) + "�� " + to_string(res->Day) + "��");
				_menu.Process();
				break;

			case 2:
				// ���� ���
				_menu = Menu(YesNo, "���� �ش� ������ ����Ͻðڽ��ϱ�? [y/n]");
				_menu.Process();

				if (_menu.ResultInteger) {
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

void RestaurantReservationSystem::walkIn() {
	while (true) {
		_menu = Menu(Select, "Walk-In �޴�");
		_menu.AddMenu("Walk-In Ȯ��");
		_menu.AddMenu("Walk-In ��û");
		_menu.AddMenu("Walk-In ���");
		_menu.AddMenu("���ư���");

		_menu.Process();

		int select = _menu.ResultInteger;
		int alreadyIn = -1;

		switch (select) {
		case 1:
			alreadyIn = -1;
			for (int i = 0; i < _waiting.size(); i++) {
				if (_waiting.at(i) == _clientList[_client]) {
					alreadyIn = i;
					break;
				}
			}

			_menu = Menu(ShowAndInput, "���� ����� : " + to_string(_waiting.size()) + "��" + (alreadyIn != -1 ? "\n����� ���� : " + to_string(alreadyIn + 1) : ""));
			_menu.Process();
			break;

		case 2:
			_menu = Menu(YesNo, "���� ����� : " + to_string(_waiting.size()) + "��\n" + "Walk-In ����Ͻðڽ��ϱ�? [y/n]");
			_menu.Process();

			if (_menu.ResultInteger) {
				alreadyIn = -1;
				for (int i = 0; i < _waiting.size(); i++) {
					if (_waiting.at(i) == _clientList[_client]) {
						alreadyIn = i;
						break;
					}
				}

				if (alreadyIn != -1) {
					_menu = Menu(ShowAndInput, "�̹� ��ϵǾ� �ֽ��ϴ�. ����� ������ " + to_string(alreadyIn + 1) + "�� �Դϴ�.");
					_menu.Process();
				}
				else {
					_waiting.push_back(_clientList[_client]);

					_menu = Menu(ShowAndInput, "��ϵǾ����ϴ�.");
					_menu.Process();
				}
			}
			break;

		case 3:
			for (int i = 0; i < _waiting.size(); i++) {
				if (_waiting.at(i) == _clientList[_client]) {
					_menu = Menu(YesNo, "����� ������ " + to_string(i+1) + "�� �Դϴ�. ���� ����Ͻðڽ��ϱ� ? [y/n]");
					_menu.Process();

					if (_menu.ResultInteger) {
						_waiting.erase(_waiting.begin() + i);
						_menu = Menu(ShowAndInput, "��ҵǾ����ϴ�.");
						_menu.Process();
						break;
					}
				}
			}
			break;

		case 4:
			_menu = Menu(ShowAndInput, "���� �޴��� ���ư��ϴ�.");
			_menu.Process();
			return;
		}
	}
}

string RestaurantReservationSystem::GetAllReservations() {
	string str = "============= �Ĵ� ���� �ý��� ============\n";

	if (_reservations.empty()) {
		str += " ���� ����� ������ϴ�.\n";
	}

	for (int i = 0; i < _reservations.size(); i++) {
		Reservation* res = _reservations.at(i);
		str += "[" + to_string(i) + "] " + _clientList.at(res->ClientNo)->GetName() + " ������: " + dateToString(TODAY_MONTH, res->Day) + " ���̺� ��ȣ: " + to_string(res->Table->TableNo) + " �ο���: " + to_string(res->Seats) + "��\n";
	}

	return str;
}