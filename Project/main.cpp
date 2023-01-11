#include "Common.h"
#include "AirlineReservationSystem.h"
#include "RestaurantReservationSystem.h"
#include "StudyingRoomReservationSystem.h"

void admin(BaseReservationSystem* system) {
	cout << system->GetAllReservations();
	cout << endl;
}

int main() {
	Airline::AirlineReservationSystem airplane;
	Restaurant::RestaurantReservationSystem restaurant;
	StudyingRoom::StudyingRoomReservationSystem studyingRoom;

	BaseReservationSystem* system;

	int select = 0;
	Menu menu;

	do{
	menu = Menu(Select, "통합 예약 시스템");
	menu.AddMenu("비행기 예약 시스템");
	menu.AddMenu("도서관 예약 시스템");
	menu.AddMenu("식당 예약 시스템");
	menu.AddMenu("관리자 메뉴");
	menu.AddMenu("종료");

		menu.Process();
		select = menu.ResultInteger;

		switch (select) {
			case 1:
				system = &airplane;
				system->Run();
				break;

			case 2:
				system = &studyingRoom;
				system->Run();
				break;

			case 3:
				system = &restaurant;
				system->Run();
				break;

			case 4:
				admin(&airplane);
				admin(&restaurant);
				admin(&studyingRoom);
				menu = Menu(ShowAndInput, "");
				menu.Process();
				break;
		}
	} while (select != 5);

	menu = Menu(ShowAndInput, "통합 예약 시스템을 종료합니다.");
	menu.Process();

	airplane.Dispose();
	restaurant.Dispose();

	return 0;
}