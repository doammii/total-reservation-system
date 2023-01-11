#ifndef _STUDYING_ROOM_RESERVATION_SYSTEM_H_
#define _STUDYING_ROOM_RESERVATION_SYSTEM_H_
#include "BaseReservationSystem.h"
using namespace std;

namespace StudyingRoom {
	const int ROWS = 15;		// ���� �� ��
	const int COLUMNS = 15;		// ���� �� ��
	const int TIMES = 14;		// ��� ���� �ð���
	const int MAX_TIME = 4;		// �ִ� ���� ������ �ð�

	class Desk {
	private:
		static int _deskNo;

	public:
		int DeskNo;
		Client* Schedules[TIMES];	// ���� 9�ú��� ���� 11�ñ����� �����ڸ� ������ �� �ֵ��� �迭�� ����

		// ������ �ð��뿡 ����� ȸ�� ���� ��ȯ
		// time: 09��~23�� ������ ������ ������ ��
		Client* GetClientByTime(int time) {
			if (time < 9 || 23 < time) return NULL;
			else return Schedules[time - 9];
		}
		// ������ �ð��뿡 ȸ�� ����
		// time: 09��~23�� ������ ������ ������ ��
		void SetClientToTime(int time, Client* client) {
			if (time < 9 || 23 < time) return;
			else Schedules[time - 9] = client;
		}
		// ���� �¼��� ������ ȸ���� �� ������ �ð� ��ȯ
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