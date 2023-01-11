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
	menu = Menu(Select, "���� ���� �ý���");
	menu.AddMenu("����� ���� �ý���");
	menu.AddMenu("������ ���� �ý���");
	menu.AddMenu("�Ĵ� ���� �ý���");
	menu.AddMenu("������ �޴�");
	menu.AddMenu("����");

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

	menu = Menu(ShowAndInput, "���� ���� �ý����� �����մϴ�.");
	menu.Process();

	airplane.Dispose();
	restaurant.Dispose();

	return 0;
}