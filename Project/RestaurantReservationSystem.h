#ifndef _RESTAURANT_RESERVATION_SYSTEM_H_
#define _RESTAURANT_RESERVATION_SYSTEM_H_
#include "BaseReservationSystem.h"
using namespace std;

namespace Restaurant {
	class Table {
	private:
		static int _tableNo;

	public:
		int TableNo;
		Client* client[7];
		int Seats;

		string GetSimpleInfo(int day) {
			string str = "";
			str += to_string(Seats) + "인석, 예약자 : ";

			if (client[day] == NULL) str += "없음";
			else str += client[day]->GetName();

			return str;
		}

		Table(int seats) {
			TableNo = ++_tableNo;
			Seats = seats;
			for(int i = 0; i < 7; i++) client[i] = NULL;
		}
	};

	class Reservation {
	public:
		int ClientNo;
		int Seats;
		int Day;
		
		Table* Table;
	};

	class RestaurantReservationSystem : public BaseReservationSystem {
	private:
		vector<Table*> _tables;
		vector<Reservation*> _reservations;
		vector<Client*> _waiting;

		void newReserve();
		void manageReserve();
		void walkIn();

	public:
		RestaurantReservationSystem();

		void Run();
		void Dispose();
		string GetAllReservations();
	};
}
#endif