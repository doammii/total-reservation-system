#include "RestaurantReservationSystem.h"
using namespace std;
using namespace Restaurant;

int Table::_tableNo = 0;
RestaurantReservationSystem::RestaurantReservationSystem() : BaseReservationSystem() {
	_clientList.push_back(new Client("test", "123123", "test", 0, 24, 0));	// Test 용 계정

	srand(time(NULL));

	int tables = 8;	// 테이블 수 (8개 고정)
	for (int i = 0; i < tables; i++) {
		_tables.push_back(new Table(pickRandom(1, 4) * 2));	// 테이블당 좌석 수 (2,4,6,8 중 랜덤)
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
			_menu = Menu(Select, "식당 예약 시스템");
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
			_menu = Menu(Select, "식당 예약 시스템");
			_menu.AddMenu("예약");
			_menu.AddMenu("Walk-In");
			_menu.AddMenu("예약 관리");
			_menu.AddMenu("로그아웃");
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

	_menu = Menu(ShowAndInput, "로그아웃되었습니다.");
	_menu.Process();
}

void RestaurantReservationSystem::newReserve() {
	if (_clientList[_client]->GetAge() <= 7) {
		_menu = Menu(ShowAndInput, "만 7세 이하는 예약 기능을 사용할 수 없습니다.");
		_menu.Process();
		return;
	}

	_menu = Menu(Select, "예약할 날짜를 선택하세요.");
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 0));
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 1));
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 2));
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 3));
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 4));
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 5));
	_menu.AddMenu(dateToString(TODAY_MONTH, TODAY_DAY + 6));
	_menu.Process();

	int day = _menu.ResultInteger - 1;

	_menu = Menu(AnswerInt, "예약 인원을 입력하세요.");
	_menu.Process();

	int seats = _menu.ResultInteger;

	// 이용 가능한 좌석 탐색
	vector<Table*> tmp;
	for (int i = 0; i < _tables.size(); i++) {
		if (_tables.at(i)->client[day] == NULL &&
			_tables.at(i)->Seats >= seats) {
			tmp.push_back(_tables.at(i));
		}
	}

	if (tmp.size() != 0) {
		_menu = Menu(Select, "예약 가능한 좌석 목록은 아래와 같습니다. 좌석을 선택하세요.");
		for (int i = 0; i < tmp.size(); i++) {
			_menu.AddMenu(tmp.at(i)->GetSimpleInfo(day));
		}
		_menu.Process();

		int seat = _menu.ResultInteger - 1;

		Table* p = tmp.at(seat);
		string recipe = "예약 내용을 확인해주세요.\n";
		recipe += "테이블 번호 : " + to_string(tmp.at(seat)->TableNo) + "\n";
		recipe += "인원 : " + to_string(seats) + "\n";
		recipe += "이대로 예약하시겠습니까? (y/n)";

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

			_menu = Menu(ShowAndInput, "예약되었습니다.");
		}
		else {
			_menu = Menu(ShowAndInput, "취소되었습니다.");
		}
		_menu.Process();
	}
	else {
		_menu = Menu(ShowAndInput, "입력하신 정보로 예약 가능한 좌석이 존재하지 않습니다.");
		_menu.Process();
	}
}

void RestaurantReservationSystem::manageReserve() {
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
			_menu.AddMenu(to_string(tmp.at(i)->Table->TableNo) + "번 테이블 (" + to_string(tmp.at(i)->Seats) + "석)");
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
					"테이블 : " + to_string(res->Table->TableNo) + "번 테이블 (" + to_string(res->Seats) + "석)\n" +
					"예약 날짜 : " + to_string(TODAY_YEAR) + "년 " + to_string(TODAY_MONTH) + "월 " + to_string(res->Day) + "일");
				_menu.Process();
				break;

			case 2:
				// 예약 취소
				_menu = Menu(YesNo, "정말 해당 예약을 취소하시겠습니까? [y/n]");
				_menu.Process();

				if (_menu.ResultInteger) {
					_reservations.erase(_reservations.begin() + indices.at(index));

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

void RestaurantReservationSystem::walkIn() {
	while (true) {
		_menu = Menu(Select, "Walk-In 메뉴");
		_menu.AddMenu("Walk-In 확인");
		_menu.AddMenu("Walk-In 신청");
		_menu.AddMenu("Walk-In 취소");
		_menu.AddMenu("돌아가기");

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

			_menu = Menu(ShowAndInput, "현재 대기자 : " + to_string(_waiting.size()) + "명" + (alreadyIn != -1 ? "\n당신의 순번 : " + to_string(alreadyIn + 1) : ""));
			_menu.Process();
			break;

		case 2:
			_menu = Menu(YesNo, "현재 대기자 : " + to_string(_waiting.size()) + "명\n" + "Walk-In 등록하시겠습니까? [y/n]");
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
					_menu = Menu(ShowAndInput, "이미 등록되어 있습니다. 당신의 순번은 " + to_string(alreadyIn + 1) + "번 입니다.");
					_menu.Process();
				}
				else {
					_waiting.push_back(_clientList[_client]);

					_menu = Menu(ShowAndInput, "등록되었습니다.");
					_menu.Process();
				}
			}
			break;

		case 3:
			for (int i = 0; i < _waiting.size(); i++) {
				if (_waiting.at(i) == _clientList[_client]) {
					_menu = Menu(YesNo, "당신의 순번은 " + to_string(i+1) + "번 입니다. 정말 취소하시겠습니까 ? [y/n]");
					_menu.Process();

					if (_menu.ResultInteger) {
						_waiting.erase(_waiting.begin() + i);
						_menu = Menu(ShowAndInput, "취소되었습니다.");
						_menu.Process();
						break;
					}
				}
			}
			break;

		case 4:
			_menu = Menu(ShowAndInput, "이전 메뉴로 돌아갑니다.");
			_menu.Process();
			return;
		}
	}
}

string RestaurantReservationSystem::GetAllReservations() {
	string str = "============= 식당 예약 시스템 ============\n";

	if (_reservations.empty()) {
		str += " 예약 목록이 비었습니다.\n";
	}

	for (int i = 0; i < _reservations.size(); i++) {
		Reservation* res = _reservations.at(i);
		str += "[" + to_string(i) + "] " + _clientList.at(res->ClientNo)->GetName() + " 예약일: " + dateToString(TODAY_MONTH, res->Day) + " 테이블 번호: " + to_string(res->Table->TableNo) + " 인원수: " + to_string(res->Seats) + "명\n";
	}

	return str;
}