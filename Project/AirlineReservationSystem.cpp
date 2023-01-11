#include "AirlineReservationSystem.h"
#include <ctime>
using namespace std;
using namespace Airline;

Airplane* makePlane(Airport* from, Airport* to, int day, int seats) {
	Airplane* plane = new Airplane();

	// 이름 랜덤 생성 
	string name = "";
	// 대문자 2개 + 숫자 3자리로 구성
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

	// 좌석수의 20%는 비즈니스석, 80%는 이코노미석
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
	cout << "항공편 : " << to_string(Airplanes.size()) << endl;
}

AirlineReservationSystem::AirlineReservationSystem() : BaseReservationSystem() {
	_clientList.push_back(new Client("test", "123123", "test", 0, 0, 0));	// Test 용 계정

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
		// 각 공항에 10개~20개 가량의 비행기를 랜덤하게 생성
		// 목적지는 랜덤
		// 년, 월은 고정시키고 날짜는 랜덤하게 15일~21일 부여
		// 좌석 수는 8로 고정
		int count = pickRandom(10, 20);
		Airport* curr = _airports.at(i);

		for (int j = 0; j < count; j++) {
			curr->Airplanes.push_back(makePlane(curr,	// 출발 공항
				_airports.at(pickRandom(0, 5, i)),		// 도착 공항 (전체 공항 중 출발 공항을 제외하고 랜덤)
				pickRandom(TODAY_DAY, TODAY_DAY + 6),	// 출발 날짜 (15~21일 중 랜덤)
				8));									// 좌석수 (8개 고정)
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
			_menu = Menu(Select, "비행기 예약 시스템");
			_menu.AddMenu("로그인");
			_menu.AddMenu("회원가입");
			_menu.AddMenu("종료");
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
			_menu = Menu(Select, "비행기 예약 시스템");
			_menu.AddMenu("예약");
			_menu.AddMenu("예약 관리");
			_menu.AddMenu("로그아웃");
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

	_menu = Menu(ShowAndInput, "로그아웃되었습니다.");
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
	_menu = Menu(Select, "출발할 공항을 선택하세요.");
	for (int i = 0; i < _airports.size(); i++) {
		if (_airports.at(i)->Country == "Republic of Korea") {
			_menu.AddMenu(_airports.at(i)->Name);
		}
	}
	_menu.Process();

	int from = _menu.ResultInteger - 1;

	_menu = Menu(Select, "출발할 날짜를 선택하세요.");
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 0));
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 1));
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 2));
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 3));
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 4));
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 5));
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 6));
	_menu.Process();

	int day = _menu.ResultInteger + 14;	// 선택된 값은 1~7의 값이므로 15~21일로 환산하기 위해 14 더함

	_menu = Menu(Select, "도착할 공항을 선택하세요.");
	for (int i = 0; i < _airports.size(); i++) {
		if (i != from) {	// 출발지로 선택한 공항 제외
			_menu.AddMenu(_airports.at(i)->Name);
		}
	}
	_menu.Process();

	int to = _menu.ResultInteger - 1;

	_menu = Menu(AnswerInt, "예약할 이코노미 좌석 개수를 입력하세요.");
	_menu.Process();

	int economy = _menu.ResultInteger;

	_menu = Menu(AnswerInt, "예약할 비즈니스 좌석 개수를 입력하세요.");
	_menu.Process();

	int business = _menu.ResultInteger;

	// 이용 가능한 비행기 탐색
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
		string recipe = "예약 내용을 확인해주세요.\n";
		recipe += "항공기 번호 : " + p->Name + "\n";
		recipe += "날짜 : " + dateToString(p->Year, p->Month, p->Day) + "\n";
		recipe += "출발지 : " + p->From->Name + "\n";
		recipe += "도착지 : " + p->To->Name + "\n";
		recipe += "예약 좌석 : 비즈니스 " + to_string(business) + "매, 이코노미 " + to_string(economy) + "매\n";
		recipe += "가격 : " + to_string(business * BUSINESS_PRICE + economy * ECONOMY_PRICE) + "원\n";
		recipe += "이대로 예약하시겠습니까? (y/n)";

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


			_menu = Menu(ShowAndInput, "예약되었습니다.");
			_menu.Process();
		}
		else {
			_menu = Menu(ShowAndInput, "취소되었습니다.");
			_menu.Process();
		}
	}
	else {
		_menu = Menu(ShowAndInput, "입력하신 정보로 예약 가능한 비행기가 존재하지 않습니다.");
		_menu.Process();
	}
}

void AirlineReservationSystem::manageReserve() {
	// 현재 로그인된 회원이 등록한 예약 목록을 임시로 저장할 벡터
	vector<Reservation*> tmp;
	// 위에서 예약 목록을 저장할 때 해당 예약 목록의 인덱스(전체 예약 목록상의)를 함께 저장할 벡터 - 예약 삭제할 때 필요 
	vector<int> indices;
	for (int i = 0; i < _reservations.size(); i++) {
		if (_reservations.at(i)->ClientNo == _client) {
			tmp.push_back(_reservations.at(i));
			indices.push_back(i);
		}
	}

	if (!tmp.empty()) {
		_menu = Menu(Select, "예약을 선택하세요.");
		for (int i = 0; i < tmp.size(); i++) {
			_menu.AddMenu(tmp.at(i)->Plane->From->Name + "(" + tmp.at(i)->Plane->From->Country + ") -> " +
				tmp.at(i)->Plane->To->Name + "(" + tmp.at(i)->Plane->To->Country + ") : " +
				"비즈니스 : " + to_string(tmp.at(i)->BusinessCount) + "석, 이코노미 : " + to_string(tmp.at(i)->EconomyCount) + "석");
		}
		_menu.AddMenu("취소");

		_menu.Process();

		int index = _menu.ResultInteger - 1;
		if (index != tmp.size()) {
			_menu = Menu(Select, "예약 관리 메뉴");
			_menu.AddMenu("상세 조회");
			_menu.AddMenu("예약 취소");
			_menu.AddMenu("돌아가기");

			_menu.Process();

			int select = _menu.ResultInteger;
			Reservation* res;

			switch (select)
			{
			case 1:
				// 상세 조회
				res = tmp.at(index);

				_menu = Menu(ShowAndInput, string("상세 조회 결과\n") +
									"예약자 : " + _clientList.at(res->ClientNo)->GetName() + "\n" +
									"출발지 : " + res->Plane->From->Name + "(" + res->Plane->From->Country + ")\n" +
									"도착지 : " + res->Plane->To->Name + "(" + res->Plane->To->Country + ")\n" + 
									"예약 좌석 : 비즈니스 " + to_string(res->BusinessCount) + "석, 이코노미 " + to_string(res->EconomyCount) + "석\n" + 
									"출발일 : " + dateToString(res->Plane->Year, res->Plane->Month, res->Plane->Day) + "\n" +
									"총 결제 금액 : " + to_string(res->TotalPrice) + "원\n" +
									"적립 마일리지 : " + to_string(res->TotalMileage) + "마일리지\n");
				_menu.Process();
				break;

			case 2:
				// 예약 취소
				_menu = Menu(YesNo, "정말 해당 예약을 취소하시겠습니까? [y/n]");
				_menu.Process();

				if (_menu.ResultInteger) {
					_reservations.erase(_reservations.begin() + indices.at(index));
					_clientList.at(tmp.at(index)->ClientNo)->SubMileage(tmp.at(index)->TotalMileage);

					_menu = Menu(ShowAndInput, "정상적으로 취소되었습니다.");
					_menu.Process();
				}
				break;

			case 3:
				// 돌아가기 - Do nothing
				break;
			}
		}
	}
	else {
		_menu = Menu(ShowAndInput, "예약이 비어있습니다.");
		_menu.Process();
	}
}

string AirlineReservationSystem::GetAllReservations() {
	string str = "============ 비행기 예약 시스템 ============\n";

	if (_reservations.empty()) {
		str += " 예약 목록이 비었습니다.\n";
	}

	for (int i = 0; i < _reservations.size(); i++) {
		Reservation* res = _reservations.at(i);
		str += "[" + to_string(i) + "] " + _clientList.at(res->ClientNo)->GetName() + " " + res->Plane->Name + " 비즈니스 :" + to_string(res->BusinessCount) + "석, 이코노미 : " + to_string(res->EconomyCount) + "석\n";
	}

	return str;
}