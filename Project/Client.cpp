#include "Client.h"

Client::Client(string id, string password, string name, int familyId, int age, int gender) {
	_identifier = id;
	_password = password;
	_name = name;
	_familyId = familyId;
	_age = age;
	_gender = gender;
	_mileage = 0;
}

string Client::GetId() { return _identifier; }
string Client::GetName(bool hide)
{
	if (hide && _name.size() >= 2) {
		string res = _name;
		for (int i = 1; i < _name.size(); i++) {
			_name[i] = '*';
		}

		return res;
	}
	else {
		return _name;
	}
}
int Client::GetFamilyId() { return _familyId; }
int Client::GetAge() { return _age; }
int Client::GetGender() { return _gender; }
int Client::GetMileage() { return _mileage; }
void Client::SetMileage(int value) { _mileage = value; }
void Client::AddMileage(int amount) { _mileage += amount; }
void Client::SubMileage(int amount) { _mileage -= amount; }
bool Client::CheckPassword(string password) {
	return password == _password;
}