#ifndef _CLIENT_H_
#define _CLIENT_H_
#include "Common.h"

using namespace std;

class Client {
private:
	string _identifier;
	string _password;

	string _name;
	int _familyId;
	int _age;
	// 0: 남자, 1: 여자
	int _gender;

	int _mileage;

public:
	Client(string id, string password, string name, int familyId, int age, int gender);

	string GetId();
	string GetName(bool hide = false);
	int GetFamilyId();
	int GetAge();
	int GetGender();
	bool CheckPassword(string);

	int GetMileage();
	void SetMileage(int);
	void AddMileage(int);
	void SubMileage(int);
};
#endif