#include <ncurses.h>
#include <list>
#include <time.h>
using namespace std;

class snake {
public:
	int x, y;
	snake(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

int main() {
	initscr();
	WINDOW *w = newwin(23, 32, 0, 0);
	
	noecho(); //입력 자동으로 화면 출력 X
	curs_set(0);
	
	keypad(w, TRUE);

	//body들의 각 좌표들까지 저장하기 위함
	list<snake> body;
	list<snake>::iterator it;
 
 	wtimeout(w, 150); // snake의 이동 속도 조절
 	
 	// head body 초기 생성
	for(int i = 0; i < 3; i++)
		body.push_front(snake(i+2, 5));
	
	bool quit = false;
	int key;
	int way = 2; // way : head 방향
	while(quit != true) {
		box(w, 0, 0);
		key = wgetch(w); //키패드 입력을 받아옴
		
		switch(key) {
			case KEY_LEFT:
				way = 1;
				break;
			case KEY_RIGHT:
				way = 2;
				break;
			case KEY_UP:
				way = 3;
				break;
			case KEY_DOWN:
				way = 4;
				break;
			case 'q':
				quit = true;
				break;
		}

		snake head = body.front(); // snake의 head 부분
		int x = head.x;
		int y = head.y;

		if(way == 1) //왼쪽
			x--;
		else if(way == 2) //오른쪽
			x++;
		else if(way == 3) //위쪽
			y--;
		else if (way == 4) //아래쪽
			y++;
    
		body.push_front(snake(x, y)); //새로 입력 받은 방향에 따른 좌표 추가
		body.pop_back(); //가장 마지막 좌표 삭제
		wclear(w);
		
		// map 밖으로 나가게 되는 경우 게임 오버 처리
		if(x < 0 || 30 < x || y < 0 || 21 < y)
			quit = true;
		
		for(it = body.begin(); it != body.end(); it++) {
			mvwaddch(w, (*it).y, (*it).x, 'O'); //snake 화면에 표시
			
			//head 방향과 입력 받은 키의 방향이 같으면 game over 처리
			if((*it).y == y && (*it).x == x && it != body.begin())
				quit = true;
		}
	}
	wclear(w);
	delwin(w);
	
	//게임 오버 시 화면
	timeout(-1); //자동 종료 방지
	mvprintw(10, 15, "GAME OVER\n");
	refresh();
	
	getch(); // 사용자 입력 대기 -> 아무거나 눌러도 종료
	endwin();
	return 0;
}
