#ifndef _MENU_H_
#define _MENU_H_
#include "Common.h"
#include <vector>
#include <stdlib.h>
using namespace std;

enum MenuType {
	// ��/�ƴϿ� ������
	YesNo,
	// �޽��� �����ֱ� �� �ƹ�Ű�� ������ ����
	ShowAndInput,
	// �޽����� �����ְ� �Ѿ
	//Show,
	// n���� ������ �����ְ� ��ȣ�� ����
	Select,
	// �޽��� �����ְ� ���ڿ� �Է�
	AnswerStr,
	// �޽��� �����ְ� ���� �Է�
	AnswerInt,
	// AnswerStr�� ������ �Է��� ���ڰ� �н����� ĳ����(*)�� ����ũ�Ǿ� ǥ�õ�
	AnswerPassword,
};

class Menu {
private:
	vector<string> _menus;
	MenuType _menuType;
	string _title;

public:
	Menu();
	Menu(MenuType);
	Menu(MenuType, string);
	void AddMenu(string);

	// ó�� ������ true (�����ؼ��� true)
	bool ProcessResult;

	// MenuType�� AnswerStr�� ��� �̰��� ����� ����
	string ResultString;
	// MenuType�� AnswerStr�� �ƴ� ��� ��� �̰��� ����� ����
	int ResultInteger;

	// �޴��� �����ְ� MenuType�� ���� ó��
	void Process();
};

#endif