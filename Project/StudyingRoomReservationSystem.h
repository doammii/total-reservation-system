#ifndef _STUDYING_ROOM_RESERVATION_SYSTEM_H_
#define _STUDYING_ROOM_RESERVATION_SYSTEM_H_
#include "BaseReservationSystem.h"
using namespace std;

namespace StudyingRoom {
	const int ROWS = 15;		// 세로 행 수
	const int COLUMNS = 15;		// 가로 열 수
	const int TIMES = 14;		// 사용 가능 시간대
	const int MAX_TIME = 4;		// 최대 예약 가능한 시간

	class Desk {
	private:
		static int _deskNo;

	public:
		int DeskNo;
		Client* Schedules[TIMES];	// 오전 9시부터 오후 11시까지의 예약자를 저장할 수 있도록 배열로 생성

		// 지정한 시간대에 예약된 회원 정보 반환
		// time: 09시~23시 사이의 값으로 지정할 것
		Client* GetClientByTime(int time) {
			if (time < 9 || 23 < time) return NULL;
			else return Schedules[time - 9];
		}
		// 지정한 시간대에 회원 예약
		// time: 09시~23시 사이의 값으로 지정할 것
		void SetClientToTime(int time, Client* client) {
			if (time < 9 || 23 < time) return;
			else Schedules[time - 9] = client;
		}
		// 현재 좌석에 지정한 회원이 총 예약한 시간 반환
		int GetClientTotalTime(Client* client) {
			int sum = 0;

			for (int i = 0; i < TIMES; i++) {
				if (Schedules[i] == client) sum++;
			}

			return sum;
		}
		bool IsFull() {
			for (int i = 0; i < TIMES; i++) {
				if (Schedules[i] == NULL) return false;
			}

			return true;
		}

		Desk() {
			DeskNo = ++_deskNo;
			for (int i = 0; i < TIMES; i++) Schedules[i] = NULL;
		}
	};

	class Reservation {
	public:
		int ClientNo;
		int Row;
		int Column;
		int Time;
	};

	class StudyingRoomReservationSystem : public BaseReservationSystem {
	private:
		Desk* _desk[ROWS][COLUMNS];
		vector<Reservation*> _reservations;

		void newReserve();
		void manageReserve();
		int getClientTotalTime(Client* client);

	public:
		StudyingRoomReservationSystem();
		void Run();
		void Dispose();
		string GetDeskString();
		string GetAllReservations();
	};
}
#endif