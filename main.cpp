#include <ncurses.h>
#include <list>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

bool quit = false;
int way = 2; // way : head 방향
int stage = 0;
bool stage_clear = false; //모든 스테이지를 클리어했는지 여부
int snake_s,  growth_s, poison_s; //속도 조절

//item, gate 좌표
int growth_x, growth_y;
int poison_x, poison_y;
int gate1_x, gate1_y;
int gate2_x, gate2_y;

int item_g, item_p, use_g;
char check_b, check_gr, check_p, check_g;

void move();
void show();
void get_growth();
void get_poison();
void get_gate();
void set_stage(int stage);

int map[4][21][21] =
	{
		{
			{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2}
		},
		{
			{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2}
		},
		{
			{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2}
		},
		{
			{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2}
		}
};

class snake {
public:
	int x, y;
	snake(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

//body들의 각 좌표들까지 저장하기 위함
list<snake> body;
list<snake>::iterator it;

int main() {
	initscr();
	noecho(); //입력 자동으로 화면 출력 X
	curs_set(0);
	nodelay(stdscr, true);
	
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_WHITE);
	init_pair(2, COLOR_CYAN, COLOR_CYAN);
	init_pair(3, COLOR_BLUE, COLOR_BLUE);
	init_pair(4, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(5, COLOR_GREEN, COLOR_GREEN);
	init_pair(6, COLOR_RED, COLOR_RED);
	init_pair(7, COLOR_MAGENTA, COLOR_MAGENTA);
	
	keypad(stdscr, TRUE);
	set_stage(stage);

	while(quit != true) {
		snake_s --;
		growth_s --;
		poison_s --;
		
		int key = getch(); //키패드 입력을 받아옴
		switch(key) {
			case KEY_LEFT:
				if(way == 2)	
					quit = true;
				else
					way = 1;
				break;
			case KEY_RIGHT:
				if(way == 1)	
					quit = true;
				else
					way = 2;
				break;
			case KEY_UP:
				if(way == 4)	
					quit = true;
				else	
					way = 3;
				break;
			case KEY_DOWN:
				if(way == 3)	
					quit = true;
				else
					way = 4;
				break;
			case 'q':
				quit = true;
				break;
		}
		
		if(snake_s == 0) { //snake 이동 속도 조절
			snake_s = 5;
			move();
		}

		if(growth_s == 0) { //growth 생성 속도 조절
			growth_s = 100;
			map[stage][growth_y][growth_x] = 0;
			get_growth();
		}
		
		if(poison_s == 0) { //poison 생성 속도 조절
			poison_s = 70;
			map[stage][poison_y][poison_x] = 0;
			get_poison();
		}
	
		show();
		usleep(50000);
	}
	
	if(stage_clear == false) {
		clear();
		mvprintw(10, 15, "GAME OVER\n");
	}
	
	timeout(-1); //자동 종료 방지
	refresh();
	getch();  // 사용자 입력 대기 -> 아무거나 눌러도 종료
	endwin();
	return 0;
}

void set_stage(int n) {
	clear();
	mvprintw(10, 15, "Stage %d", stage + 1);
	refresh();
	
	//속도 조절위함(값 변경 가능)
	snake_s = 5;
	growth_s = 100;
	poison_s = 70;
	
	item_g = 0;
	item_p = 0;
	use_g = 0;
	
	check_b = ' ';
	check_gr = ' ';
	check_p = ' ';
	check_g = ' ';
	
	body.clear();
	for(int i = 0; i < 3; i++)
		body.push_front(snake(i+9, 10));
	
	get_growth();
	get_poison();
	get_gate();
	
	if(stage == 0)
		way = 1;
	else if(stage == 1)
		way = 2;
	else if(stage == 2)
		way = 3;
	else if(stage == 3)
		way = 4;
	
	usleep(1000000);
}

void move() {
	snake head = body.front(); // snake의 head 부분
	int x = head.x;
	int y = head.y;
		
	snake tail = body.back(); // snake의 tail 부분
	map[stage][tail.y][tail.x] = 0;
	body.pop_back(); //가장 마지막 좌표 삭제

	if(way == 1) //왼쪽
		x--;
	else if(way == 2) //오른쪽
		x++;
	else if(way == 3) //위쪽
		y--;
	else if (way == 4) //아래쪽
		y++;
			
	body.push_front(snake(x, y)); //새로 입력 받은 방향에 따른 좌표 추가

	if(map[stage][y][x] == 1) // map 밖으로 나가게 되는 경우 게임 오버 처리(벽에 부딪히는 경우)
		quit = true;
	
	bool growth = false, poison = false;
	//growth 아이템을 먹게 되는 경우
	if(map[stage][y][x] == 5) {
		growth = true;
		item_g++;
		body.push_back(snake(tail.x, tail.y)); //지웠던 마지막 좌표를 다시 추가해 길이를 1 증가시킴
	}
	//poison 아이템을 먹게 되는 경우
	else if (map[stage][y][x] == 6) {
		if(body.size() <= 3) //poison 아이템을 먹고 길이가 3보다 작아지는 경우 게임 오버
			quit = true;
		else {
			poison = true;
			item_p++;
			map[stage][body.back().y][body.back().x] = 0;
			body.pop_back();
		}
	}

	//snake 화면에 표시
	for(it = body.begin(); it != body.end(); it++)
		map[stage][(*it).y][(*it).x] = 4;
	map[stage][body.front().y][body.front().x] = 3;
	
	if(growth == true) { //새 growth 표시
		get_growth();
		growth_s = 100;
	}
	
	if(poison == true) { //새 poison 표시
		get_poison();
		poison_s = 70;
	}
}

void show() {
	clear();
	
	//map, snake, grwoth, poison, gate 화면에 색을 이용해 표시
	for(int i = 0; i < 21; i++) {
		for(int j = 0; j <21; j++) {
			if(map[stage][i][j] == 0)
				printw(" ");
			else if(map[stage][i][j] == 1) {
				attron(COLOR_PAIR(1));
				printw("-");
				attroff(COLOR_PAIR(1));
			}
			else if(map[stage][i][j] == 2) {
				attron(COLOR_PAIR(2));
				printw("+");
				attroff(COLOR_PAIR(2));
			}
			else if(map[stage][i][j] == 3) {
				attron(COLOR_PAIR(3));
				printw("H");
				attroff(COLOR_PAIR(3));
			}
			else if(map[stage][i][j] == 4) {
				attron(COLOR_PAIR(4));
				printw("B");
				attroff(COLOR_PAIR(4));
			}
			else if(map[stage][i][j] == 5) {
				attron(COLOR_PAIR(5));
				printw("G");
				attroff(COLOR_PAIR(5));
			}
			else if(map[stage][i][j] == 6) {
				attron(COLOR_PAIR(6));
				printw("P");
				attroff(COLOR_PAIR(6));
			}
			else if(map[stage][i][j] == 7) {
				attron(COLOR_PAIR(7));
				printw("D");
				attroff(COLOR_PAIR(7));
			}
		}
		printw("\n");
	}
	
	mvprintw(1, 25, "Score Board");
	mvprintw(2, 25, "B : %d/ (Max_Length)", body.size());
	mvprintw(3, 25, "+ : %d", item_g);
	mvprintw(4, 25, "- : %d", item_p);
	mvprintw(5, 25, "G : %d", use_g);
	mvprintw(7, 25, "Mission");
	mvprintw(8, 25, "B : 10 (%c)", check_b);
	mvprintw(9, 25, "+ : 5 (%c)", check_gr);
	mvprintw(10, 25, "- : 2 (%c)", check_p);
	mvprintw(11, 25, "G : 1 (%c)", check_g);
	refresh();
}

void get_growth() {
	while(true) {
		srand(time(NULL));
		growth_x = (rand() % 19) + 1;
		growth_y = (rand() % 19) + 1;

		if(map[stage][growth_y][growth_x] == 0) { //growth 아이템이 빈 공간에 생기도록 조정
			map[stage][growth_y][growth_x] = 5;
			break;
		}
	}
}

void get_poison() {
	while(true) {
		srand(time(NULL));
		poison_x = (rand() % 19) + 1;
		poison_y = (rand() % 19) + 1;

		if(map[stage][poison_y][poison_x] == 0) { //poison 아이템이 빈 공간에 생성되도록 조정
			map[stage][poison_y][poison_x] = 6;
			break;
		}
	}
}

void get_gate() {
	srand(time(NULL));
	while(true) {
		gate1_x = (rand() % 21);
		gate1_y = (rand() % 21);

		if(map[stage][gate1_y][gate1_x] == 1) { //gate가 wall에 생성되도록 조정
			map[stage][gate1_y][gate1_x] = 7;
			break;
		}
	}
	while(true) {
		gate2_x = (rand() % 21);
		gate2_y = (rand() % 21);

		if(map[stage][gate2_y][gate2_x] == 1 && map[stage][gate2_y][gate2_x] != 7) {
			map[stage][gate2_y][gate2_x] = 7; //gate가 wall에 생성되고 다른 gate와 겹치지 않도록 조정
			break;
		}
	}
}
