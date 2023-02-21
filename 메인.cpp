#include <iostream>
#include <Windows.h>
#include <conio.h>

using namespace std;

#define LEFT 75 // Ű���� ����
#define RIGHT 77 // Ű���� ������
#define UP 72 //Ű���� ����
#define DOWN 80 // Ű���� �Ʒ���
#define SELECT 13 // ����


void gotoxy(int x, int y) {
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void CursorView(){
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1; 
	cursorInfo.bVisible = FALSE; 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}


int GameMap[4][4]; // ������ ���� �� ������ ���ÿ� ��ġ�ϴ� ��
int FixedMap[4][4]; // ���������� ���� �����ϸ� �̰��� �迭


class EventHandler {
private:
	int x; // ���� x��ǥ��
	int y; // ���� y��ǥ��
	float score; // ������ Ƚ��
	int CheckRandomNumber[8] = { 0, }; // 1~8���� ������ ���� ���� 2�� �ʰ��ؼ� ��ġ���� �ʱ� ���� üũ�뵵�� �����ص� �迭
	int RandomNumber; // ���� �ʿ� ������ ���� ��ġ�ϱ� ���� �迭 1~8���� ���� �ΰ���
	int LimitNumber[6] = { 0, }; // ���õ� �ٸ� �� ī���� ���ڸ� �񱳰� �� ��ġ���� �����ϱ� ���� �迭, 0���� ù��° ī���� ����, 1�� 2���� ���� x���� y���� ����, 3~5�� �Ȱ��� ���
	bool Limiter = false; // ����Ʈ �ѹ��� ������ true�� �ٲ�鼭 �� �� �����ֱ� ���� �뵵
	bool Ending = false; // ���� �ڵ鷯�� ������ ������ �����Ű�� ���� �뵵
public:
	EventHandler(int x, int y) { // ������
		this->x = x;
		this->y = y;
		score = 0;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				GameMap[i][j] = 0;
				FixedMap[i][j] = 0;
			}
		}
		
		for (int i = 0; i < 4; i++) { // ���� �� �����ؼ� ���Ӹ� �迭�� �ν־� 1~8 ���� ����
			for (int j = 0; j < 4; j++) {
				RandomNumber = rand() % 8 + 1;
				if (GameMap[i][j] == 0) {
					if (CheckRandomNumber[RandomNumber - 1] >= 2) {
						j--;
						continue;
					}
					GameMap[i][j] = RandomNumber;
					j--;
					CheckRandomNumber[RandomNumber - 1]+=1;
				}	
			}
		}
		DrawStartMenu(); // ���� �޴�
		system("cls"); 
		DrawGameMap(); // ���� �� �׸���
		DrawCursor(); // ���� ���� �ִ� ��ġ�� Ŀ�� �׸���
	}

	void SetX(int x) {
		this->x = x;
	}
	
	void SetY(int y) {
		this->y = y;
	}

	int GetX() {
		return x;
	}

	int GetY() {
		return y;
	}

	bool GetEnding() {
		return Ending;
	}

	void SetScore(float s) {
		score += s;
	}

	float GetScore() {
		return score;
	}

	void DrawStartMenu() {
		cout << "\n\n\n\n";
		cout << "\t Card Number Match Game";
		Sleep(3000);

	}

	void DrawGameMap() {
		cout << "\n";
		for (int i = 0; i < 4; i++) {
			cout << "\t";
			for (int j = 0; j < 4; j++) {
				
				if (FixedMap[i][j] == 0) { // ���� �� ���� 0�̸� [?] ���·� �׷��ֱ�
					cout << "[?]   ";
				}
				else {
					cout << "[" << GameMap[i][j] << "]   "; // ���� �� ���� 0�� �ƴϸ� [1]�� ���� ���� �ʿ� �ִ� ���� ǥ��
				}
			}
			
			cout << "\n\n";
			
		}

	}

	void DrawCursor() {
		gotoxy(x, y); // x���� 6ĭ�� y���� 2�� ����
		cout << "��"; 
	}

	void DeleteCursor() {
		gotoxy(x, y);
		cout << "  "; // �̵��ϸ� ���� �ִ� Ŀ�� �����ֱ� ���� �뵵
	}

	void LimitSelect() { // ���ð��� �ΰ��� �������� ���� �޼ҵ�
		if (LimitNumber[0] == 0) { // ù��° ������ �ƴ��� �Ǵ� �뵵
			LimitNumber[0] = GameMap[y / 2][x / 6 - 1]; // ù��° ���õ� �� 
			LimitNumber[1] = x/6-1; // ù��° ���õ� x��ǥ
			LimitNumber[2] = y/2; // ù��° ���õ� y��ǥ
		}
		else {
			LimitNumber[3] = GameMap[y / 2][x / 6 - 1]; // �ι�° ���õ� ��
			LimitNumber[4] = x/6-1; // �ι�° ���õ� x��ǥ
			LimitNumber[5] = y/2; // �ι�° ���õ� y��ǥ
			Limiter = true; // ������ �ι��ߴٰ� �˷��ֱ� ���� �뵵
		}
	}

	void CheckLimit() {
		if (Limiter == true) { // �����Ͱ� true�� �Ǹ� �ι� ���õǾ��ٴ� ���̹Ƿ� ���� ���õ� �� �� ��
			if (LimitNumber[0] == LimitNumber[3]) { // ���� ���ؼ� ������ �ٸ��� Ȯ��
				Limiter = false; // �ʱ�ȭ
				for (int i = 0; i < 6; i++) { //�����ϱ� �ش� �� �״�� ���� -> ���õɶ� ���� ������ �ڵ����� ���� �迭���ֱ⶧��
					LimitNumber[i] = 0; // �ʱ�ȭ
				}
			}
			else { // �ٸ���
				system("cls");
				DrawGameMap();
				DrawCursor();
				Sleep(1000);
				FixedMap[LimitNumber[2]][LimitNumber[1]] = 0; // �ش� ������ ��ġ�� �ִ� ���� �ٽ� 0���� �ʱ�ȭ
				FixedMap[LimitNumber[5]][LimitNumber[4]] = 0; // ��
				Limiter = false; // �ʱ�ȭ
				for (int i = 0; i < 6; i++) {
					LimitNumber[i] = 0; // �ʱ�ȭ
				}
			}
		}
	}

	void EndingChecker() { // ���� �ʿ� �ִ� ������ ���� 0�� �ƴϸ� ���� ���� 
		bool checker = false;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (FixedMap[i][j] == 0) {  
					checker = true;
					Ending = false;
					break;
				}
				else {
					Ending = true; 
				}
			}
			if (checker == true) {
				break;
			}
		}
	}
};


class PlayHandler {
private:
	EventHandler* ev = new EventHandler(6, 1); // �̺�Ʈ �ڵ鷯 
public:
	int keyEvent() { // Ű���� �Է� �� �޴� �뵵
		int temp = _getwch();
		if (temp == 224) {
			switch (temp) {
			case 75:
				return LEFT;
				break;
			case 77:
				return RIGHT;
				break;
			case 72:
				return UP;
				break;
			case 80:
				return DOWN;
				break;
			};
		}
	}

	void Progress() { // ���� ������ ���� �޼ҵ�
		while (true) { // ���� ������ �ɶ����� Ű���� �Է� �������� ����
			int n = keyEvent();
			switch (n) {
			case UP: // Ű���� ����
				if (ev->GetY() == 1) { // �Ѱ輱 �Ѿ ������ �ϸ� �� ����
					continue;
				}else{
					ev->DeleteCursor();
					ev->SetY(ev->GetY() - 2);
					ev->DrawCursor();
					
				}
				break;
			case LEFT: // Ű���� ����
				if (ev->GetX() == 6) { // �Ѱ輱 �Ѿ ������ �ϸ� �� ����// �Ѱ輱
					continue;
				}
				else {
					ev->DeleteCursor();
					ev->SetX(ev->GetX() - 6);
					ev->DrawCursor();
					
				}
				break;
			case RIGHT: // Ű���� ������ Ű
				if (ev->GetX() == 24) { // �Ѱ輱 �Ѿ ������ �ϸ� �� ����
					continue;
				}
				else {
					ev->DeleteCursor();
					ev->SetX(ev->GetX() + 6);
					ev->DrawCursor();
					
				}
				break;
			case DOWN: // Ű���� �Ʒ� Ű
				if (ev->GetY() == 7) { // �Ѱ輱 �Ѿ ������ �ϸ� �� ����
					continue;
				}
				else {
					ev->DeleteCursor();
					ev->SetY(ev->GetY() + 2);
					ev->DrawCursor();
					
				}
				break;
			case SELECT: // Ű���� ���� Ű
				if (FixedMap[ev->GetY() / 2][ev->GetX() / 6 - 1] != 0) { // ���� ������ ��ġ�� ���� 0�� �ƴϸ� ���� -> �̹� ���õǾ��ų� ¦�� ã�Ƽ� ���� ���
					continue;
				}
				ev->SetScore(0.5); // ���� ������ �� 0.5 �ΰ� �������� 1
				FixedMap[ev->GetY()/2][ev->GetX()/6-1] = GameMap[ev->GetY()/2][ev->GetX() / 6-1]; // ���ð� ���ÿ� �ϴ� ���� �ʿ� �־��ش�
				ev->LimitSelect(); 
				ev->CheckLimit();   
				system("cls");
				ev->DrawGameMap();
				ev->DrawCursor();
				
			}
			ev->EndingChecker(); // ���� �ڵ鷯 ���� ���� ����
			if (ev->GetEnding()) {
				
				break;
			}
		}
	}
	
	void End() { // ����
		system("cls"); // ���� �� ���� ��
		cout << "\n\n\n\n";
		cout << "\t\tYOU WIN";  // �¸� ����
		cout << "\n\n\t   Number of Flips: " << ev->GetScore(); // ī�� ������ Ƚ�� ǥ�� �ѹ� �������� 0.5���� �ö���� �ΰ� �����Ҷ� 1�� ������ ������ ��
		Sleep(5000); // ȭ�� 5�ʰ� �����ִ� �뵵
		delete ev;
		exit(0);
	}
};


int main() {
	system("mode con cols=40 lines=10 | title ī�� ¦ ���߱� ����"); // ȭ�� ũ�� ����
	CursorView(); // �����̴� Ŀ�� �����
	srand((unsigned)time(NULL)); // ������ �ο� ����
	PlayHandler *ph = new PlayHandler(); // ���� ���� �ڵ鷯
	ph->Progress(); // Ű���� �Է� ���� ���� ���� ���� �ڵ鷯, ����üĿ�� ���ؼ� �ش� ���� ���
	ph->End(); // ���� â ����ִ� �뵵
	delete ph; // �޸� ����
}
