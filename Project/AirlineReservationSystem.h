#ifndef _AIRPLANE_RESERVATION_SYSTEM_H_
#define _AIRPLANE_RESERVATION_SYSTEM_H_
#include "BaseReservationSystem.h"
using namespace std;

namespace Airline {
	class Airplane;

	// 좌석 가격 설정
	const int BUSINESS_PRICE = 1200000;
	const int ECONOMY_PRICE = 800000;
	// Km당 마일리지 (비즈니스, 이코노미 각각)
	const int MILEAGE_PER_KM_BUSINESS = 200;
	const int MILEAGE_PER_KM_ECONOMY = 100;

	class Seat {
	public:
		Client* client;

		Seat() {
			client = NULL;
		}
	};

	class Airport {
	public:
		vector<Airplane*> Airplanes;
		string Name;
		string Country;

		Airport(string name, string country) {
			Name = name;
			Country = country;
		}

		void PrintInfo();
		void Dispose();
	};


	class Airplane {
		int countRemain(vector<Seat*> seats) {
			int remains = 0;
			for (int i = 0; i < seats.size(); i++) if (seats.at(i)->client == NULL) remains++;

			return remains;
		}
	public:
		vector<Seat*> EconomySeats;
		vector<Seat*> BusinessSeats;

		string Name;

		int Year;
		int Month;
		int Day;

		class Airport* From;
		class Airport* To;

		void Dispose() {
			for (int i = 0; i < EconomySeats.size(); i++) {
				delete EconomySeats.at(i);
			}
			for (int i = 0; i < BusinessSeats.size(); i++) {
				delete BusinessSeats.at(i);
			}

			EconomySeats.clear();
			BusinessSeats.clear();
		}

		int RemainBusiness() {
			return countRemain(BusinessSeats);
		}
		int RemainEconomy() {
			return countRemain(EconomySeats);
		}

		string GetSimpleInfo() {
			int remains = RemainBusiness() + RemainEconomy();

			return Name + " " + From->Name + "(" + From->Country + ") " + " -> " + To->Name + "(" + To->Country + ") " + to_string(Year) + "-" + to_string(Month) + "-" + to_string(Day) + " (잔여:" + to_string(remains) + "/" + to_string(BusinessSeats.size() + EconomySeats.size()) + ")";
		}
	};

	class Reservation {
	public:
		int ClientNo;
		int BusinessCount;
		int EconomyCount;

		Airplane* Plane;
		vector<Seat*> Seats;

		int TotalPrice;
		int TotalMileage;
	};

	class AirlineReservationSystem : public BaseReservationSystem {
	private:
		// 공항 목록
		vector<Airport*> _airports;
		vector<Reservation*> _reservations;

		void showAllAirlines();
		void newReserve();
		void manageReserve();


	public:
		AirlineReservationSystem();
		void Run();
		void Dispose();

		string GetAllReservations();
	};
}
#endif