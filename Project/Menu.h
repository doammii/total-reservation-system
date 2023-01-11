#ifndef _MENU_H_
#define _MENU_H_
#include "Common.h"
#include <vector>
#include <stdlib.h>
using namespace std;

enum MenuType {
	// 예/아니오 선택지
	YesNo,
	// 메시지 보여주기 후 아무키나 눌러서 종료
	ShowAndInput,
	// 메시지만 보여주고 넘어감
	//Show,
	// n개의 선택지 보여주고 번호로 선택
	Select,
	// 메시지 보여주고 문자열 입력
	AnswerStr,
	// 메시지 보여주고 숫자 입력
	AnswerInt,
	// AnswerStr과 같으나 입력한 문자가 패스워드 캐릭터(*)로 마스크되어 표시됨
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

	// 처리 성공시 true (웬만해서는 true)
	bool ProcessResult;

	// MenuType이 AnswerStr인 경우 이곳에 결과값 저장
	string ResultString;
	// MenuType이 AnswerStr이 아닌 모든 경우 이곳에 결과값 저장
	int ResultInteger;

	// 메뉴를 보여주고 MenuType에 따라 처리
	void Process();
};

#endif