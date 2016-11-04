#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include<time.h>
#include<stdlib.h>
#include<ctype.h>
#include<deque>

using namespace std;

struct Bullet {
	int x, y, d;
};

deque<Bullet> bullets;

void  gotoxy(int x, int y) {
	COORD c = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void setcursor(bool visible) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}

void setcolor(int fg, int bg) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}

void draw_ship(int x, int y) {
	setcolor(9, 0);
	gotoxy(x, y);
	printf(" <=0=> ");
}

void draw_ship2(int x, int y) {
	setcolor(2, 0);
	gotoxy(x - 2, 2);
	printf(" -<0>- ");
}

void erase_ship(int x, int y) {
	gotoxy(x, y);
	printf("       ");
}

void draw_bullet(int x, int y) {
	Bullet b;
	b.x = x;
	b.y = y;
	b.d = 1;
	bullets.push_back(b);
}

void draw_bullet1(int x, int y) {
	Bullet b;
	b.x = x;
	b.y = y;
	b.d = -1;
	bullets.push_back(b);
}

void clear_bullet(int x, int y) {
	gotoxy(x, y);
	printf(" ");
}

void draw_star(int x, int y) {
	setcolor(6, 0);
	gotoxy(x, y);
	printf("*");
}
void erase_star(int x,int y){
	gotoxy(x,y);
	printf(" ");
}

void draw_star2(int x, int y) {
	setcolor(7, 0);
	gotoxy(x , y );
	printf("O");
}

void clear_star(int x, int y) {
	gotoxy(x, y);
	printf(" ");
}

void new_star(int x, int y) {
	setcolor(6, 0);
	gotoxy((rand() % 60) + 10, (rand() % 4) + 2);
	printf("*");
}

void new_star2(int x, int y) {
	x = (rand() % 60) + 10;
	y = (rand() % 4) + 2;
	setcolor(7, 0);
	gotoxy(x - 1, y + 1);
	printf("O");
}

char cursor(int x, int y) {
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	char buf[2]; COORD c = { x,y }; DWORD num_read;
	if (!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read))
		return '\0';
	else
		return buf[0];
}

double count_time(double current_time) {
	clock_t tt;
	current_time = clock();
	tt = clock() - current_time;
	double time_taken = ((double)tt) / CLOCKS_PER_SEC;
	return time_taken + 1;

}


void homedisplay() {
	system("cls");
	printf("\n\n");
	printf("\t _________   ___        _________\t\t\t   _________________________________\n");
	printf("\t |   __   | |   |      |         |\t\t\t  |                                 \\\n");
	printf("\t |  |__|  | |   |      |   ___   |\t\t\t  | Credit by                        \\\n");
	printf("\t |   _____| |   |      |  |___|  |\t\t\t  | Chananya  Choosak 1D 59010272    /\n");
	printf("\t |  |       |   |____  |   ___   |\t\t\t  |_________________________________/\n");
	printf("\t |__|       |________| |__|   |__|\n\n");
	printf("\t  ________   ___    ___\n");
	printf("\t |__    __| |   |  |   |\t\t\t\t\t  Press 'P' to play.\n");
	printf("\t    |  |    |   |__|   |\n");
	printf("\t    |  |    |___    ___|\n");
	printf("\t    |  |        |  |\t\t\t\t\t\t  Press 'H' for help.\n");
	printf("\t    |__|        |__|\n\n");
	printf("\t _________   __    __   _________\t\t\t\t  Press 'X' to exit.\n");
	printf("\t |   __   | |  |  |  | |   ______|\n");
	printf("\t |  |__|  | |  |  |  | |  |______\n");
	printf("\t |   _____| |  |  |  | |_______  |\n");
	printf("\t |  |       |  |__|  |  _______| |\n");
	printf("\t |__|       |________| |_________|\n\n");
}

void endgame(int ss) {
	system("cls");
	gotoxy(40, 10); printf("Game Ended.");
	gotoxy(30, 15); printf("Your score: %d",ss);
	gotoxy(30, 17); printf("Time: ");
	getch();
}

void gameover(int ss) {
	system("cls");

	char ch;

	printf("\n\n");
	printf("\t  _______   _______   ___    ___   _______\t ________   ___   ___    ________    ________\n");
	printf("\t |  _____| |  ___  | |   \\  \/   | |   ____|\t|  ____  | |   | |   |  |    ____|  |  ____  |\n");
	printf("\t | |  ___  | |___| | |    \\\/    | |  |____\t| |    | | |   | |   |  |   |____   | |____| |\n");
	printf("\t | | |_  | |  ___  | |          | |   ____|\t| |    | | |   | |   |  |    ____|  |  ____  |\n");
	printf("\t | |___| | | |   | | |  |\\  \/|  | |  |____\t| |____| | |   \\ \/   |  |   |____   | |    \\ \\\n");
	printf("\t |_______| |_|   |_| |__| \\\/ |__| |_______|\t|________|  \\_______\/   |________|  |_|     \\_\\");
	printf("\n\n\n");
	printf("Press any key to continue.");

	getch();
	endgame(ss);
}

void gameplay() {
	system("cls");
	setcursor(0);
	char ch = '.';
	double current_time;
	int x = 38, y = 20, i, j, bullet = 0, score = 0, bx, by, life = 5;
	int xbullet = 0, ybullet = 0;
	gotoxy(90, 1); printf("Score: ");
	gotoxy(100, 1); printf("%d", score);
	gotoxy(90, 3); printf("Time:   %d",(int)count_time(current_time));
	//gotoxy(100, 3); printf("");
	gotoxy(90, 5); printf("Life: ");
	gotoxy(100, 5); printf("%d", life);

	char c = '.';
	//create stars and ships
	srand(time(NULL));
	struct _coord{
		int x,y,dir,dist;
	}star[20],star2[10],bot[100];
	for(i=0;i<100;i++)
		bot[i].dir=0;
	int countBot=0;
	
	for (i = 1; i <= 20; i++) { 
		star[i-1].x=rand()%60+5;star[i-1].y=rand()%20+2;
			
		draw_star(star[i-1].x, star[i-1].y); }
	for (i = 1; i <= 10; i++) { 
		star2[i-1].x=rand()%60+5;
		star2[i-1].y=rand()%10+1;
		
		draw_star2(star2[i-1].x, star2[i-1].y); }
	
	
	
	draw_ship(x, y);
	//draw_ship2((rand() % 60) + 10, 2);
	//draw_ship2(++x, y);
	int ss;
	unsigned int mod=0;
	do {
		mod++;
		for(i=0;i<20;i++)
			if(mod%2==0){
				erase_star(star[i].x,star[i].y);
				if(++star[i].y>24){
					star[i].y=0;
					star[i].x=rand()%60+5;
				}
				draw_star(star[i].x,star[i].y);
			}
			
		
		for(i=0;i<10;i++)
			if(mod%2==0){
				erase_star(star2[i].x,star2[i].y);
				if(++star2[i].y>24){
				star2[i].y=0;
				star2[i].x=rand()%60+5;
				}
				draw_star2(star2[i].x,star2[i].y);
			}
		
			
		
		for(i=0;i<7;i++)
			{
				if(cursor(x+i,y)=='*')
					life--;
				
				if(cursor(x+i,y)=='O')
					life-=2;
			}
		
		gotoxy(100, 5); printf("%d", life); 	
			
		if (kbhit()) {
			ch = getch();
			if (ch == 'a') { draw_ship(--x, y); c = 'L'; }
			if (ch == 'd') { draw_ship(++x, y); c = 'R'; }
			if (ch == 'w') {
				
			 	draw_ship(x, --y); erase_ship(x, y + 1); }
			if (ch == 's') { 
				
				draw_ship(x, ++y); erase_ship(x, y - 1); }
			if (ch == 'r') { c = 'S'; }
			if (/*bullet != 1 &&*/ ch == ' ') {
				//bullet = 1; 
				bx = x + 3; by = y - 1;
				{
					xbullet = x + 3; ybullet = y-1;
					draw_bullet(xbullet, ybullet);}
				
			}
			
		}
		fflush(stdin);
		//automatic move
		if (c == 'L') {  draw_ship(--x, y); }
		else if (c == 'R') { draw_ship(++x, y); }
		else if (c == 'S') { draw_ship(x, y); }

		//bullet
		for (int i = 0; i < bullets.size(); i++) {
			Bullet *b = &bullets[i];
			if (b->y == 1) {
					clear_bullet(b->x, b->y);
					bullets.erase(bullets.begin() + i);
				}
				else {
//					if (cursor(b->x+3, b-> y - 2) == '*') {
//						Beep(800, 100);
//						//new_star(x, y);
//						
//						score++;
//						setcolor(7, 0); gotoxy(100, 1); printf("%d", score);
//					}
//					if (cursor(b->x + 3, b->y - 2) == 'O') {
//						Beep(800, 100);
//						//new_star2(x, y);
//						score++;
//						setcolor(7, 0); gotoxy(100, 1); printf("%d", score);
//					}
//					if (cursor(b->x + 3, b->y - 2) == '0' || cursor(b->x + 3, b->y - 2) == '-' || cursor(b->x + 3, b->y - 2) == '<' || cursor(b->x + 3, b->y - 2) == '>') {
//						Beep(500, 80);
//						erase_ship(bx, by);
//						score = score + 2;
//						setcolor(7, 0); gotoxy(100, 1); printf("%d", score);
//					}
					clear_bullet(b->x, b->y);
					b->y--;
					gotoxy(b->x, b->y);
					setcolor(4, 0);
					printf("^");

				}
			
		}
		
		for(i=0;i<20;i++)
			if(cursor(star[i].x,star[i].y)=='^'||cursor(star[i].x,star[i].y-1)=='^')
				{
					Beep(800, 100);
					star[i].x=rand()%60+5;star[i].y=0;
					score++;
					setcolor(7, 0); gotoxy(100, 1); printf("%d", score);
				}
		
		for(i=0;i<10;i++)
			if(cursor(star2[i].x,star2[i].y)=='^'||cursor(star2[i].x,star2[i].y-1)=='^')
				{
					Beep(800, 100);
					star2[i].x=rand()%60+5;star2[i].y=0;
					score++;
					setcolor(7, 0); gotoxy(100, 1); printf("%d", score);
				}
		
		

				
		Sleep(100);
		if (x <= 1) { draw_ship(++x, y); }
		if (x >= 80) { draw_ship(--x, y); }
		if (y <= 2) { draw_ship(x, ++y); erase_ship(x, 10); }
		if (y >= 25) { draw_ship(x, --y); erase_ship(x, 25); }

		if (score != 0 && score % 10 == 0)  
			for (i = 0; i < 3; i++) { 
				bot[countBot].x=rand()%60+10;bot[countBot].y=2;
				draw_ship2(bot[countBot].x, bot[countBot].y);
				bot[countBot].dir=rand()%4+1;
				bot[countBot].dist=rand()%10+1;
				countBot++;
			}
		
		for(i=0;i<20;i++)
		{
			
			if(bot[i].dist--<=0 && bot[i].dir!=0)
			{
				bot[i].dir=rand()%4+1;
				bot[i].dist=rand()%10+1;	
			}
			
			if(bot[i].dist>0){
			
			
				erase_ship(bot[i].x,bot[i].y);
				switch (bot[i].dir){
					case 1:if(--bot[i].y<1)bot[i].y=1;break;
					case 2:if(++bot[i].x>70)bot[i].x=70;break;
					case 3:if(++bot[i].y>15)bot[i].y=15;break;
					case 4:if(--bot[i].x<2)bot[i].x=2;break;
				}
				draw_ship2(bot[i].x,bot[i].y);
			}
		}
		
		

		ss=score;
		if (life <= 0 || cursor(x, y) == 'v') { gameover(ss); }
	} while (ch != 'q');
	endgame(ss);
}

void help() {
	system("cls");
	gotoxy(5, 1); printf("Press 'M' to back to menu.");
	gotoxy(22, 5); printf("--- HELP ---");
	gotoxy(20, 7); printf("'w' move forward");
	gotoxy(20, 8); printf("'s' move backward");
	gotoxy(20, 9); printf("'d' move right");
	gotoxy(20, 10); printf("'a' move left");
	gotoxy(20, 11); printf("'space bar' shoot");
	gotoxy(20, 12); printf("'q' quit game");
	gotoxy(5, 15); printf("shoot stars = 1 point  shoot another ship = 2 point");
	gotoxy(16, 16); printf("if you fired = GAME OVER!");
	gotoxy(22, 18); printf("Good Luck :)");

	char ch;
	ch = toupper(getch());
	if (ch == 'M') {
		system("cls");
		homedisplay();
		ch = getch();
		if (ch == 'P'); gameplay();
	}
}

int main() {
	homedisplay();
	char select;
	select = toupper(getch());
	switch (select) {
	case 'P': gameplay(); break;
	case 'X': break;
	case 'H': help(); break;
	default: 
	gotoxy(74, 20); printf("Please try again."); Sleep(2000); main(); 
		
	}
}
