#ifndef _BASE_RESERVATION_SYSTEM_H_
#define _BASE_RESERVATION_SYSTEM_H_
#include "Common.h"
#include "Menu.h"
#include "Client.h"

const int MALE = 0;
const int FEMALE = 1;

const int TODAY_YEAR = 2022;
const int TODAY_MONTH = 5;
const int TODAY_DAY = 15;

class BaseReservationSystem abstract {
private:
	int findClientById(string);

protected:
	Menu _menu;
	int _client = -1;
	vector<Client*> _clientList;

public:
	BaseReservationSystem();

	// ȸ�� ���
	// ȸ�� ��� ������ ��� : -1
	// ȸ�� ��� �� �ٷ� �α����� ��� : _clientList������ ȸ�� �ε���
	// ȸ�� ��� �� �ٷ� �α������� ���� ��� : -1
	int SignUp();
	// �α���
	// �α��� ������ ��� : ȸ�� �ε���
	// �α��� ������ ��� : -1
	int SignIn();
	bool SignOut();

	virtual void Run() = 0;
	virtual void Dispose() = 0;
	virtual string GetAllReservations() = 0;
};
#endif
