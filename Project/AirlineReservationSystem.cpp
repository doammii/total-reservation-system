#include "AirlineReservationSystem.h"
#include <ctime>
using namespace std;
using namespace Airline;

Airplane* makePlane(Airport* from, Airport* to, int day, int seats) {
	Airplane* plane = new Airplane();

	// �̸� ���� ���� 
	string name = "";
	// �빮�� 2�� + ���� 3�ڸ��� ����
	name += (char)pickRandom(65, 90);
	name += (char)pickRandom(65, 90);
	name += (char)pickRandom(48, 57);
	name += (char)pickRandom(48, 57);
	name += (char)pickRandom(48, 57);

	plane->Name = name;


	plane->From = from;
	plane->To = to;

	plane->Year = TODAY_YEAR;
	plane->Month = TODAY_MONTH;
	plane->Day = day;

	// �¼����� 20%�� ����Ͻ���, 80%�� ���ڳ�̼�
	for (int i = 0; i < round(seats * 0.2); i++) {
		plane->BusinessSeats.push_back(new Seat());
	}
	for (int i = 0; i < round(seats * 0.8); i++) {
		plane->EconomySeats.push_back(new Seat());
	}

	return plane;
}

void Airport::Dispose() {
	for (int i = 0; i < Airplanes.size(); i++) {
		Airplanes.at(i)->Dispose();
		delete Airplanes.at(i);
	}

	Airplanes.clear();
}
void Airport::PrintInfo() {
	cout << Name << " International Airport (" << Country << ")" << endl;
	cout << "�װ��� : " << to_string(Airplanes.size()) << endl;
}

AirlineReservationSystem::AirlineReservationSystem() : BaseReservationSystem() {
	_clientList.push_back(new Client("test", "123123", "test", 0, 0, 0));	// Test �� ����

	srand(time(NULL));

	_airports.push_back(new Airport("Incheon", "Republic of Korea"));
	_airports.push_back(new Airport("Gimpo", "Republic of Korea"));
	_airports.push_back(new Airport("Jeju", "Republic of Korea"));
	_airports.push_back(new Airport("Gimhae", "Republic of Korea"));
	_airports.push_back(new Airport("Pohang", "Republic of Korea"));

	_airports.push_back(new Airport("Dokyo", "Japan"));
	_airports.push_back(new Airport("London City", "United Kingdom"));
	_airports.push_back(new Airport("Dubai", "United Arab Emirates"));

	for (int i = 0; i < _airports.size(); i++) {
		// �� ���׿� 10��~20�� ������ ����⸦ �����ϰ� ����
		// �������� ����
		// ��, ���� ������Ű�� ��¥�� �����ϰ� 15��~21�� �ο�
		// �¼� ���� 8�� ����
		int count = pickRandom(10, 20);
		Airport* curr = _airports.at(i);

		for (int j = 0; j < count; j++) {
			curr->Airplanes.push_back(makePlane(curr,	// ��� ����
				_airports.at(pickRandom(0, 5, i)),		// ���� ���� (��ü ���� �� ��� ������ �����ϰ� ����)
				pickRandom(TODAY_DAY, TODAY_DAY + 6),	// ��� ��¥ (15~21�� �� ����)
				8));									// �¼��� (8�� ����)
		}
	}
}

void AirlineReservationSystem::Dispose() {
	for (int i = 0; i < _clientList.size(); i++) {
		delete _clientList.at(i);
	}

	_clientList.clear();

	for (int i = 0; i < _airports.size(); i++) {
		_airports.at(i)->Dispose();
		delete _airports.at(i);
	}

	_airports.clear();
}

void AirlineReservationSystem::Run() {
	bool end = false;

	do {
		if (_client == -1) {
			_menu = Menu(Select, "����� ���� �ý���");
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
			_menu = Menu(Select, "����� ���� �ý���");
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

void AirlineReservationSystem::showAllAirlines() {
	for (int i = 0; i < _airports.size(); i++) {
		_airports.at(i)->PrintInfo();

		for (int j = 0; j < _airports.at(i)->Airplanes.size(); j++) {
			cout << _airports.at(i)->Airplanes.at(j)->GetSimpleInfo();
			cout << endl;
		}

		cout << endl;
	}
}

void AirlineReservationSystem::newReserve() {
	_menu = Menu(Select, "����� ������ �����ϼ���.");
	for (int i = 0; i < _airports.size(); i++) {
		if (_airports.at(i)->Country == "Republic of Korea") {
			_menu.AddMenu(_airports.at(i)->Name);
		}
	}
	_menu.Process();

	int from = _menu.ResultInteger - 1;

	_menu = Menu(Select, "����� ��¥�� �����ϼ���.");
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 0));
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 1));
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 2));
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 3));
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 4));
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 5));
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 6));
	_menu.Process();

	int day = _menu.ResultInteger + 14;	// ���õ� ���� 1~7�� ���̹Ƿ� 15~21�Ϸ� ȯ���ϱ� ���� 14 ����

	_menu = Menu(Select, "������ ������ �����ϼ���.");
	for (int i = 0; i < _airports.size(); i++) {
		if (i != from) {	// ������� ������ ���� ����
			_menu.AddMenu(_airports.at(i)->Name);
		}
	}
	_menu.Process();

	int to = _menu.ResultInteger - 1;

	_menu = Menu(AnswerInt, "������ ���ڳ�� �¼� ������ �Է��ϼ���.");
	_menu.Process();

	int economy = _menu.ResultInteger;

	_menu = Menu(AnswerInt, "������ ����Ͻ� �¼� ������ �Է��ϼ���.");
	_menu.Process();

	int business = _menu.ResultInteger;

	// �̿� ������ ����� Ž��
	vector<Airplane*> tmp;
	for (int i = 0; i < _airports.at(from)->Airplanes.size(); i++) {
		if (_airports.at(from)->Airplanes.at(i)->To == _airports.at(to) && 
			_airports.at(from)->Airplanes.at(i)->Day == day &&
			_airports.at(from)->Airplanes.at(i)->RemainBusiness() >= business &&
			_airports.at(from)->Airplanes.at(i)->RemainEconomy() >= economy) {
			tmp.push_back(_airports.at(from)->Airplanes.at(i));
		}
	}

	if (tmp.size() != 0) {
		for (int i = 0; i < tmp.size(); i++) {
			_menu.AddMenu(tmp.at(i)->GetSimpleInfo());
		}
		_menu.Process();

		int plane = _menu.ResultInteger - 1;

		Airplane* p = tmp.at(plane);
		string recipe = "���� ������ Ȯ�����ּ���.\n";
		recipe += "�װ��� ��ȣ : " + p->Name + "\n";
		recipe += "��¥ : " + dateToString(p->Year, p->Month, p->Day) + "\n";
		recipe += "����� : " + p->From->Name + "\n";
		recipe += "������ : " + p->To->Name + "\n";
		recipe += "���� �¼� : ����Ͻ� " + to_string(business) + "��, ���ڳ�� " + to_string(economy) + "��\n";
		recipe += "���� : " + to_string(business * BUSINESS_PRICE + economy * ECONOMY_PRICE) + "��\n";
		recipe += "�̴�� �����Ͻðڽ��ϱ�? (y/n)";

		_menu = Menu(YesNo, recipe);
		_menu.Process();

		Client* client = _clientList.at(_client);
		Reservation* res = new Reservation();
		res->Plane = p;
		res->ClientNo = _client;
		res->BusinessCount = business;
		res->EconomyCount = economy;
		res->TotalPrice = business * BUSINESS_PRICE + economy * ECONOMY_PRICE;
		res->TotalMileage = (business * MILEAGE_PER_KM_BUSINESS * 100) + (economy * MILEAGE_PER_KM_ECONOMY * 100);
		client->AddMileage(res->TotalMileage);

		if (_menu.ResultInteger) {
			for (int i = 0; i < p->BusinessSeats.size(); i++) {
				if (business == 0) break;
				else if (p->BusinessSeats.at(i)->client == NULL) {
					p->BusinessSeats.at(i)->client = client;
					res->Seats.push_back(p->BusinessSeats.at(i));
					business--;
				}
			}

			for (int i = 0; i < p->EconomySeats.size(); i++) {
				if (business == 0) break;
				else if (p->EconomySeats.at(i)->client == NULL) {
					p->EconomySeats.at(i)->client = client;
					res->Seats.push_back(p->EconomySeats.at(i));
					business--;
				}
			}

			_reservations.push_back(res);


			_menu = Menu(ShowAndInput, "����Ǿ����ϴ�.");
			_menu.Process();
		}
		else {
			_menu = Menu(ShowAndInput, "��ҵǾ����ϴ�.");
			_menu.Process();
		}
	}
	else {
		_menu = Menu(ShowAndInput, "�Է��Ͻ� ������ ���� ������ ����Ⱑ �������� �ʽ��ϴ�.");
		_menu.Process();
	}
}

void AirlineReservationSystem::manageReserve() {
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
			_menu.AddMenu(tmp.at(i)->Plane->From->Name + "(" + tmp.at(i)->Plane->From->Country + ") -> " +
				tmp.at(i)->Plane->To->Name + "(" + tmp.at(i)->Plane->To->Country + ") : " +
				"����Ͻ� : " + to_string(tmp.at(i)->BusinessCount) + "��, ���ڳ�� : " + to_string(tmp.at(i)->EconomyCount) + "��");
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
									"����� : " + res->Plane->From->Name + "(" + res->Plane->From->Country + ")\n" +
									"������ : " + res->Plane->To->Name + "(" + res->Plane->To->Country + ")\n" + 
									"���� �¼� : ����Ͻ� " + to_string(res->BusinessCount) + "��, ���ڳ�� " + to_string(res->EconomyCount) + "��\n" + 
									"����� : " + dateToString(res->Plane->Year, res->Plane->Month, res->Plane->Day) + "\n" +
									"�� ���� �ݾ� : " + to_string(res->TotalPrice) + "��\n" +
									"���� ���ϸ��� : " + to_string(res->TotalMileage) + "���ϸ���\n");
				_menu.Process();
				break;

			case 2:
				// ���� ���
				_menu = Menu(YesNo, "���� �ش� ������ ����Ͻðڽ��ϱ�? [y/n]");
				_menu.Process();

				if (_menu.ResultInteger) {
					_reservations.erase(_reservations.begin() + indices.at(index));
					_clientList.at(tmp.at(index)->ClientNo)->SubMileage(tmp.at(index)->TotalMileage);

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

string AirlineReservationSystem::GetAllReservations() {
	string str = "============ ����� ���� �ý��� ============\n";

	if (_reservations.empty()) {
		str += " ���� ����� ������ϴ�.\n";
	}

	for (int i = 0; i < _reservations.size(); i++) {
		Reservation* res = _reservations.at(i);
		str += "[" + to_string(i) + "] " + _clientList.at(res->ClientNo)->GetName() + " " + res->Plane->Name + " ����Ͻ� :" + to_string(res->BusinessCount) + "��, ���ڳ�� : " + to_string(res->EconomyCount) + "��\n";
	}

	return str;
}