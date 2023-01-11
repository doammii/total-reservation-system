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

	// 회원 등록
	// 회원 등록 실패한 경우 : -1
	// 회원 등록 후 바로 로그인한 경우 : _clientList에서의 회원 인덱스
	// 회원 등록 후 바로 로그인하지 않은 경우 : -1
	int SignUp();
	// 로그인
	// 로그인 성공한 경우 : 회원 인덱스
	// 로그인 실패한 경우 : -1
	int SignIn();
	bool SignOut();

	virtual void Run() = 0;
	virtual void Dispose() = 0;
	virtual string GetAllReservations() = 0;
};
#endif
