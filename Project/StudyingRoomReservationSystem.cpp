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
// 00시~23시까지 입력 가능
string timeToString(int time) {
	string str = (time < 10 ? "0" : "") + to_string(time) + ":00 ~ " + (time + 1 < 10 ? "0" : "") + to_string(time + 1) + ":00";
	return str;
}
StudyingRoomReservationSystem::StudyingRoomReservationSystem() : BaseReservationSystem() {
	_clientList.push_back(new Client("test", "123123", "test", 0, 25, MALE));	// Test 용 계정

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
			_menu = Menu(Select, "독서실 예약 시스템");
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
			_menu = Menu(Select, "독서실 예약 시스템");
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
		_menu = Menu(ShowAndInput, "만 13세 이하는 예약 기능을 사용할 수 없습니다.");
		_menu.Process();
		return;
	}

	cout << "잔여 시간 : 최대 " << to_string(MAX_TIME) << "시간 중 " << to_string(MAX_TIME - getClientTotalTime(_clientList[_client])) << "시간 이용 가능" << endl;
	cout << GetDeskString();
	_menu = Menu(AnswerStr, "예약할 좌석을 선택하세요. [ex: A9]");
	_menu.Process();

	if (_menu.ProcessResult) {
		string input = _menu.ResultString;

		if (input.size() != 2) {
			_menu = Menu(ShowAndInput, "잘못 입력하셨습니다.");
			_menu.Process();
			return;
		}
		else {
			int row = input[0];
			int col = input[1];

			// 첫번째 글자가 알파벳인 경우 0~25의 숫자로 변환, 알파벳이 아닌경우 -1로 만든다.
			if ('A' <= row && row <= 'Z') row -= 'A';
			else if ('a' <= row && row <= 'z') row -= 'a';
			else row = -1;

			// 두번째 글자가 숫자 또는 : ; < = > 인 경우 0~COLUMNS - 1의 숫자로 변환, 숫자가 아닌경우 -1로 만든다.
			if ('0' <= col && col <= '>') col -= '0';
			else col = -1;

			if (row == -1 || row >= ROWS || col == -1 || col >= COLUMNS) {
				_menu = Menu(ShowAndInput, "잘못 입력하셨습니다.");
				_menu.Process();
				return;
			}

			if (_desk[row][col]->IsFull()) {
				_menu = Menu(ShowAndInput, "선택하신 좌석에는 이용 가능한 시간대가 없습니다.");
				_menu.Process();
				return;
			}

			_menu = Menu(Select, "이용할 시간을 선택하세요.");
			for (int i = 0; i < TIMES; i++) {
				Client* client = _desk[row][col]->GetClientByTime(i + 9);
				_menu.AddMenu(timeToString(i + 9) + " " + (client != NULL ? client->GetName(true) + " (" + genderToString(client->GetGender()) + ")" : "공석"));
			}
			_menu.Process();
			
			int select = _menu.ResultInteger;
			if (_desk[row][col]->GetClientByTime(select + 8) != NULL) {
				_menu = Menu(ShowAndInput, "선택하신 시간은 이미 예약된 시간입니다.");
				_menu.Process();
				return;
			}

			// 상하좌우 좌석 성별 검사 - 배열 인덱스가 넘어가지 않도록 주의해야 한다.
				// 상 = row - 1
			if ((row - 1 >= 0 && _desk[row - 1][col]->GetClientByTime(select + 8) != NULL && _desk[row - 1][col]->GetClientByTime(select + 8)->GetGender() == _clientList.at(_client)->GetGender()) ||
				// 하 = row + 1
				(row + 1 < ROWS && _desk[row + 1][col]->GetClientByTime(select + 8) != NULL && _desk[row + 1][col]->GetClientByTime(select + 8)->GetGender() == _clientList.at(_client)->GetGender()) ||
				// 좌 = col - 1
				(col - 1 >= 0 && _desk[row][col - 1]->GetClientByTime(select + 8) != NULL && _desk[row][col - 1]->GetClientByTime(select + 8)->GetGender() == _clientList.at(_client)->GetGender()) ||
				// 우 = col + 1
				(col + 1 < COLUMNS && _desk[row][col + 1]->GetClientByTime(select + 8) != NULL && _desk[row][col + 1]->GetClientByTime(select + 8)->GetGender() == _clientList.at(_client)->GetGender())) {
				_menu = Menu(ShowAndInput, "선택하신 시간에 좌석의 주변에 동일한 성별의 이용자가 존재합니다.");
				_menu.Process();
				return;
			}

			if (getClientTotalTime(_clientList[_client]) >= MAX_TIME) {
				_menu = Menu(ShowAndInput, "예약 가능한 잔여 시간이 없습니다. 예약 가능한 최대 시간은 계정당 " + to_string(MAX_TIME) + "시간입니다.");
				_menu.Process();
				return;
			}

			string recipe = "예약 내용을 확인해주세요.\n";
			recipe += "좌석 번호 : " + rowColToString(row, col) + "\n";
			recipe += "예약 시간 : " + timeToString(select + 8) + "\n";
			recipe += "예약 후 잔여 시간 : " + to_string(MAX_TIME - getClientTotalTime(_clientList[_client]) - 1) + "\n";
			recipe += "이대로 예약하시겠습니까? (y/n)";

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

				_menu = Menu(ShowAndInput, "예약되었습니다.");
			}
			else {
				_menu = Menu(ShowAndInput, "취소되었습니다.");
			}
			_menu.Process();
		}
	}
}

void StudyingRoomReservationSystem::manageReserve() {
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

	if (tmp.size() != 0) {
		_menu = Menu(Select, "예약을 선택하세요.");
		for (int i = 0; i < tmp.size(); i++) {
			_menu.AddMenu("좌석 " + rowColToString(tmp.at(i)->Row, tmp.at(i)->Column) + "(" + timeToString(tmp.at(i)->Time + 9) + ")");
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
					"좌석 : " + rowColToString(res->Row, res->Column) + "\n" +
					"예약 시간 : " + timeToString(res->Time + 9));
				_menu.Process();
				break;

			case 2:
				// 예약 취소
				_menu = Menu(YesNo, "정말 해당 예약을 취소하시겠습니까? [y/n]");
				_menu.Process();

				if (_menu.ResultInteger) {
					res = _reservations.at(index);
;					_desk[res->Row][res->Column]->SetClientToTime(res->Time + 9, NULL);
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
	str += "(- : 잔여 좌석 있음, X : 잔여 좌석 없음)\n";

	return str;
}

string StudyingRoomReservationSystem::GetAllReservations() {
	string str = "============ 도서관 예약 시스템 ============\n";

	if (_reservations.empty()) {
		str += " 예약 목록이 비었습니다.\n";
	}

	for (int i = 0; i < _reservations.size(); i++) {
		Reservation* res = _reservations.at(i);
		str += "[" + to_string(i) + "] " + _clientList.at(res->ClientNo)->GetName() + " 좌석 번호: " + rowColToString(res->Row, res->Column) + " (" + timeToString(res->Time + 9) + ")\n";
	}

	return str;
}