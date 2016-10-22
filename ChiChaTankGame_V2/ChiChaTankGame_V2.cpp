#include <iostream>
#include <SFML/System.hpp>				//SFML คือ Libraryสำหรับเขียนเกม
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <stdlib.h>
#include <conio.h>
#include <SFML/OpenGL.hpp>
#include <string>
#include <vector>
#include <cmath>


#define empty 0
#define bullet 1

#define keyW sf::Keyboard::isKeyPressed(sf::Keyboard::W)
#define keyD sf::Keyboard::isKeyPressed(sf::Keyboard::D)
#define keyS sf::Keyboard::isKeyPressed(sf::Keyboard::S)
#define keyA sf::Keyboard::isKeyPressed(sf::Keyboard::A)
#define keySpace sf::Keyboard::isKeyPressed(sf::Keyboard::Space)

#define keyUp sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
#define keyRight sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
#define keyDown sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
#define keyLeft sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
#define keyRCtrl sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)

#define normal 1
#define spread 2
#define reflect 3
const int delayBullet = 5;
const int delayShootingBullet = 25;
const int delayTank = 15;
const int numberOFmap = 336+21;
int countbullet = 0;
bool playingTankRunSound = false;
//การยิงแบบลูกสะท้อน ยังไม่เสร็จ

sf::RenderWindow windows(sf::VideoMode(1920,1080), "ChiChaTankGame_V2", sf::Style::Fullscreen);		//render window ขนาด 1920x1080

struct _pos{
	int x, y;
}startMapEditor;
struct _tank
{
	int obj;
	int hp;
	int dmg;
	int modeShooting;
	int x, y, dir;
	//hp = health point พลังชีวิตของวัตถุ 
	//dmg = damage พลังโจมตี
	//modeShooting = โหมดการยิง
	//x และ y คือตำแหน่งของวัตถุ ,dir=direction ทิศทาง มี8ทิศ
}player1,player2,bulle[30];
// player1และplayer2 เก็บตัวรถถัง ,bulle เก็บลูกกระสุน

struct _map
{
	//int x, y, hp,obj;
	int hp, obj;
}map[numberOFmap+10];
//map เก็บวัตถุในแผนที่ ,obj คือชนิดวัตถุ
struct _posMap
{
	int x(int i)
	{
		return 239 + (i % 21) * 64;
	};

	int y(int i)
	{
		return (i / 21) * 64;
	}
}getMapPos;
struct _checkCollision
{
	bool bullets(int i, int j)
	{
		
		return !(bulle[j].x - 5 > getMapPos.x(i) + 64
			|| bulle[j].x + 5 < getMapPos.x(i)
			|| bulle[j].y - 12 > getMapPos.y(i) + 64
			|| bulle[j].y + 12 < getMapPos.y(i));
	}

	bool tank(int no,int dir)
	{
		if (no == 1)
		{
			switch (dir)
			{
			case 1:
				for (int i = 0; i < numberOFmap; i++)
					if (map[i].obj != empty)
						if (!(player1.x - 25 > getMapPos.x(i) + 60
							|| player1.x + 25 < getMapPos.x(i)+4
							|| player1.y - 3 - 25 > getMapPos.y(i) + 60
							|| player1.y - 3 + 25 < getMapPos.y(i)+4))
							return true;
				return false;


			case 2:
				for (int i = 0; i < numberOFmap; i++)
					if (map[i].obj != empty)
						if (!(player1.x + 3 - 25 > getMapPos.x(i) + 60
							|| player1.x + 3 +25 < getMapPos.x(i)+4
							|| player1.y - 25 > getMapPos.y(i) + 60
							|| player1.y + 25 < getMapPos.y(i)+4))
							return true;
				return false;


			case 3:
				for (int i = 0; i < numberOFmap; i++)
					if (map[i].obj != empty)
						if (!(player1.x - 25 > getMapPos.x(i) + 60
							|| player1.x + 25 < getMapPos.x(i)+4
							|| player1.y + 3 - 25 > getMapPos.y(i) + 60
							|| player1.y + 3 + 25 < getMapPos.y(i)+4))
							return true;
				return false;


			case 4:
				for (int i = 0; i < numberOFmap; i++)
					if (map[i].obj != empty)
						if (!(player1.x - 3 - 25 > getMapPos.x(i) + 60
							|| player1.x - 3 + 25 < getMapPos.x(i)+4
							|| player1.y - 25 > getMapPos.y(i) + 60
							|| player1.y + 25 < getMapPos.y(i)+4))
							return true;
				
				return false;
			}
			
		}
		else
		{
			switch (dir)
			{
			case 1:
				for (int i = 0; i < numberOFmap; i++)
					if (map[i].obj != empty)
						if (!(player2.x - 25 > getMapPos.x(i) + 60
							|| player2.x + 25 < getMapPos.x(i)+4
							|| player2.y - 3 - 25 > getMapPos.y(i) + 60
							|| player2.y - 3 + 25 < getMapPos.y(i)+4))
							return true;
				return false;


			case 2:
				for (int i = 0; i < numberOFmap; i++)
					if (map[i].obj != empty)
						if (!(player2.x + 3 - 25 > getMapPos.x(i) + 60
							|| player2.x + 3 + 25 < getMapPos.x(i)+4
							|| player2.y - 25 > getMapPos.y(i) + 60
							|| player2.y + 25 < getMapPos.y(i)+4))
							return true;
				return false;


			case 3:
				for (int i = 0; i < numberOFmap; i++)
					if (map[i].obj != empty)
						if (!(player2.x - 25 > getMapPos.x(i) + 60
							|| player2.x + 25 < getMapPos.x(i)+4
							|| player2.y + 3 - 25 > getMapPos.y(i) + 60
							|| player2.y + 3 + 25 < getMapPos.y(i)+4))
							return true;
				return false;


			case 4:
				for (int i = 0; i < numberOFmap; i++)
					if (map[i].obj != empty)
						if (!(player2.x - 3 - 25 > getMapPos.x(i) + 60
							|| player2.x - 3 + 25 < getMapPos.x(i)+4
							|| player2.y - 25 > getMapPos.y(i) + 60
							|| player2.y + 25 < getMapPos.y(i)+4))
							return true;

				return false;
			}
			
		}

	}
}checkTouch;
void playerControler()
{
	//Threadควบคุมตัวรถถังของ2ผู้เล่น
	sf::SoundBuffer buff[10];	//สร้าง10 sound buffers 
	sf::Sound shoot[10];		//สร้าง10 sounds

	buff[0].loadFromFile("shoot1.wav");
	buff[1].loadFromFile("shoot2.wav");
	buff[2].loadFromFile("shoot3.wav");
	buff[3].loadFromFile("shoot4.wav");
	buff[4].loadFromFile("shoot5.wav");
	buff[5].loadFromFile("shoot6.wav");
	buff[6].loadFromFile("shoot7.wav");
	buff[7].loadFromFile("shoot8.wav");
	buff[8].loadFromFile("shoot9.wav");
	buff[9].loadFromFile("pew.wav");
	//Load ไฟล์เสียงลงbuffer

	for (int i = 0; i < 10; i++)
		shoot[i].setBuffer(buff[i]);//set buffer ให้กับเสียง
	std::cout << "Done Loading sound buffer" << std::endl;

	int mod1 = 0,mod2=0;


	do{
		playingTankRunSound = false;

		//player1
		if (keyW && !(keyD || keyA || keyS))
		{
			//ถ้ากด W คือplayer1เดินหน้า และให้เล่นเสียงรถถังวิ่ง
			player1.dir = 1;
			if (!checkTouch.tank(1, 1))
			{
				
				player1.y -= 3;
				playingTankRunSound = true;
			}
			
		}
		if (keyD && !(keyW || keyA || keyS))
		{
			//ถ้ากดD คือplayer1เดินขวา และให้เล่นเสียงรถถังวิ่ง
			player1.dir = 2;
			if (!checkTouch.tank(1,2))
			{
				
				player1.x += 3;
				playingTankRunSound = true;
			}
		}
		if (keyS && !(keyD || keyA || keyW))
		{
			//ถ้ากด S คือplayer1เดินขวา และให้เล่นเสียงรถถังวิ่ง
			player1.dir = 3;
			if (!checkTouch.tank(1,3)){
				
				player1.y += 3;
				playingTankRunSound = true;
			}
		}
		if (keyA && !(keyD || keyW || keyS))
		{
			//ถ้ากด A คือplayer1เดินซ้าย และให้เล่นเสียงรถถังวิ่ง
			player1.dir = 4;
			if (!checkTouch.tank(1, 4))
			{
				
				player1.x -= 3;
				playingTankRunSound = true;
			}
			
		}
		//if (keyW && keyD && !(keyS || keyA))
		//{
		//	//ถ้ากด W และ D คือplayer1เดินเฉียงบนขวา และให้เล่นเสียงรถถังวิ่ง
		//	player1.dir = 5;
		//	player1.x += 2; player1.y -= 2;
		//	playingTankRunSound = true;

		//}
		//if (keyS && keyD && !(keyA || keyW))
		//{
		//	//ถ้ากด D และ S คือplayer1เดินเฉียงล่างขวาและให้เล่นเสียงรถถังวิ่ง
		//	player1.dir = 6;
		//	player1.x += 2; player1.y += 2;
		//	playingTankRunSound = true;

		//}
		//if (keyS && keyA && !(keyW || keyD))
		//{
		//	//ถ้ากด S และ A คือplayer1เดินเฉียงล่างซ้าย และให้เล่นเสียงรถถังวิ่ง
		//	player1.dir = 7;
		//	player1.x -= 2; player1.y += 2;
		//	playingTankRunSound = true;

		//}
		//if (keyW && keyA && !(keyS || keyD))
		//{
		//	//ถ้ากด A และ W คือplayer1เดินเฉียงบนซ้าย และให้เล่นเสียงรถถังวิ่ง
		//	player1.dir = 8;
		//	player1.x -= 2; player1.y -= 2;
		//	playingTankRunSound = true;

		//}
		if (keySpace)
		{
			//กด Space player1ยิง โดยสุ่มเสียงของกระสุนจากเสียงทั้ง10เสียง
			//ใช้การmodในการหน่วงความเร็วกระสุน
			if (mod1++%delayShootingBullet == 0)
			{
				shoot[rand() % 10].play();
				switch (player1.modeShooting)
				{
				case normal:
					bulle[countbullet++] = player1;
					bulle[countbullet - 1].obj = bullet; break;
				case spread:
					bulle[countbullet++] = player1;
					bulle[countbullet - 1].obj = bullet;
					bulle[countbullet++] = player1;
					bulle[countbullet - 1].obj = bullet;
					bulle[countbullet++] = player1;
					bulle[countbullet - 1].obj = bullet;
					switch(player1.dir)
					{
					case 1:
						bulle[countbullet - 1].dir = 8;
						bulle[countbullet - 2].dir = 5;
						break;
					case 2:
						bulle[countbullet - 1].dir = 5;
						bulle[countbullet - 2].dir = 6;
						break;
					case 3:
						bulle[countbullet - 1].dir = 7;
						bulle[countbullet - 2].dir = 6;
						break;
					case 4:
						bulle[countbullet - 1].dir = 7;
						bulle[countbullet - 2].dir = 8;
						break;
					/*case 5:
						bulle[countbullet - 1].dir = 1;
						bulle[countbullet - 2].dir = 2;
						break;
					case 6:
						bulle[countbullet - 1].dir = 2;
						bulle[countbullet - 2].dir = 3;
						break;
					case 7:
						bulle[countbullet - 1].dir = 3;
						bulle[countbullet - 2].dir = 4;
						break;
					case 8:
						bulle[countbullet - 1].dir = 4;
						bulle[countbullet - 2].dir = 1;
						break;*/
					}
					break;
				
				}
				
				if (++countbullet > 30)countbullet = 1;
			}

		}


		//player2
		if (keyUp && !(keyRight || keyDown || keyLeft))
		{
			//ถ้ากด ลูกศรขึ้น player2จะเดินบน และเล่นเสียงรถถังวิ่ง
			player2.dir = 1;
			if (!checkTouch.tank(2, 1))
			{
				
				player2.y -= 3;
				playingTankRunSound = true;
			}

		}
		if (keyRight && !(keyUp || keyLeft || keyDown))
		{
			//ถ้ากด ลูกศรขวา player2จะเดินขวา และเล่นเสียงรถถังวิ่ง
			player2.dir = 2;
			if (!checkTouch.tank(2, 2))
			{
				
				player2.x += 3;
				playingTankRunSound = true;
			}
			

		}
		if (keyDown && !(keyRight || keyLeft || keyUp))
		{
			//ถ้ากด ลูกศรล่าง player2จะเดินล่าง และเล่นเสียงรถถังวิ่ง
			player2.dir = 3;
			if (!checkTouch.tank(2, 3))
			{
				
				player2.y += 3;
				playingTankRunSound = true;
			}
			

		}
		if (keyLeft && !(keyRight || keyUp || keyDown))
		{
			//ถ้ากด ลูกศรซ้าย player2จะเดินซ้าย และเล่นเสียงรถถังวิ่ง
			player2.dir = 4;
			if (!checkTouch.tank(2, 4))
			{
				
				player2.x -= 3;
				playingTankRunSound = true;
			}

		}
		//if (keyUp && keyRight && !(keyDown || keyLeft))
		//{
		//	//ถ้ากด ลูกศรขึ้น และ ลูกศรขวา player2จะเดินเฉียงบนขวา และเล่นเสียงรถถังวิ่ง
		//	player2.dir = 5;
		//	player2.x += 2; player2.y -= 2;
		//	playingTankRunSound = true;

		//}
		//if (keyDown && keyRight && !(keyLeft || keyUp))
		//{
		//	//ถ้ากด ลูกศรล่าง และ ลูกศรขวา player2จะเดินเฉียงล่างขวา และเล่นเสียงรถถังวิ่ง
		//	player2.dir = 6;
		//	player2.x += 2; player2.y += 2;
		//	playingTankRunSound = true;

		//}
		//if (keyDown && keyLeft && !(keyUp || keyRight))
		//{
		//	//ถ้ากด ลูกศรล่าง และ ลูกศรซ้าย player2จะเดินเฉียงล่างซ้าย และเล่นเสียงรถถังวิ่ง
		//	player2.dir = 7;
		//	player2.x -= 2; player2.y += 2;
		//	playingTankRunSound = true;

		//}
		//if (keyUp && keyLeft && !(keyRight || keyDown))
		//{
		//	//ถ้ากด ลูกศรขึ้น และ ลูกศรซ้าย player2จะเดินเฉียงบนซ้าย และเล่นเสียงรถถังวิ่ง
		//	player2.dir = 8;
		//	player2.x -= 2; player2.y -= 2;
		//	playingTankRunSound = true;

		//}
		if (keyRCtrl)
		{
			//กด RightControl player2ยิงกระสุน
			//ใช้การmodในการหน่วงความเร็วกระสุน
			if (mod2++%delayShootingBullet == 0)
			{
				shoot[rand() % 10].play();
				switch (player2.modeShooting)
				{
				case normal:
					bulle[countbullet++] = player2; bulle[countbullet - 1].obj = bullet; break;
				case spread:
					bulle[countbullet++] = player2;
					bulle[countbullet - 1].obj = bullet;
					bulle[countbullet++] = player2;
					bulle[countbullet - 1].obj = bullet;
					bulle[countbullet++] = player2;
					bulle[countbullet - 1].obj = bullet;
					switch (player2.dir)
					{
					case 1:
						bulle[countbullet - 1].dir = 8;
						bulle[countbullet - 2].dir = 5;
						break;
					case 2:
						bulle[countbullet - 1].dir = 5;
						bulle[countbullet - 2].dir = 6;
						break;
					case 3:
						bulle[countbullet - 1].dir = 7;
						bulle[countbullet - 2].dir = 6;
						break;
					case 4:
						bulle[countbullet - 1].dir = 7;
						bulle[countbullet - 2].dir = 8;
						break;
					/*case 5:
						bulle[countbullet - 1].dir = 1;
						bulle[countbullet - 2].dir = 2;
						break;
					case 6:
						bulle[countbullet - 1].dir = 2;
						bulle[countbullet - 2].dir = 3;
						break;
					case 7:
						bulle[countbullet - 1].dir = 3;
						bulle[countbullet - 2].dir = 4;
						break;
					case 8:
						bulle[countbullet - 1].dir = 4;
						bulle[countbullet - 2].dir = 1;
						break;*/
					}
					break;

				}
				if (++countbullet > 30)countbullet = 1;
			}
		}
		
		sf::sleep(sf::milliseconds(delayTank)); //หน่วงเวลารถถังเคลื่อนที่
		playingTankRunSound = false;
	} while (!sf::Keyboard::isKeyPressed(sf::Keyboard::X));
	
}



void bulletStimulate()
{
	while (1)
	{
		//loop ทำงานโดยไล่ตามarray 
		for (int u = 0; u < 30; u++)
		{
			//ให้ลูกกระสุนเคลื่อนที่ตามทิศทางของแต่ละลูก
			switch (bulle[u].dir)
			{
				case 1:bulle[u].y -= 3; break;
				case 2:bulle[u].x += 3; break;
				case 3:bulle[u].y += 3; break;
				case 4:bulle[u].x -= 3; break;
				/*case 5:bulle[u].y -= 2; bulle[u].x += 2; break;
				case 6:bulle[u].x += 2; bulle[u].y += 2; break;
				case 7:bulle[u].y += 2; bulle[u].x -= 2; break;
				case 8:bulle[u].x -= 2; bulle[u].y -= 2; break;*/
			}

			
			
		}
		sf::sleep(sf::milliseconds(delayBullet));	//หน่วงเวลาลูกกระสุน
	}
}

void playTankSound()
{
	//Thread เล่นเสียงรถถัง
	sf::SoundBuffer tankRunBuf;			//สร้าง buffer soundของรถถัง
	sf::Sound tankRun;					//สร้าง sound รถถัง
	tankRunBuf.loadFromFile("tank_sound.wav");		//Loadเสียงรถถัง
	tankRun.setBuffer(tankRunBuf);					//set เสียงรรถถัง


	while (1)
	{
		//เล่นเสียงรถถังเมื่อ playingTankRunSound เป็น true
		if (playingTankRunSound){
			tankRun.play();		//เล่นเสียงรถถัง
			while (playingTankRunSound)sf::sleep(sf::milliseconds(16));	//รอจนกว่า playingTankRunSound เป็น false
			tankRun.pause();	//หยุดเสียงรถถัง
		}
	}
}

void botTank(int numberOFtank,sf::RectangleShape *bot)
{
	int dir,range;
	while (1)
	{

		for (int i = 0; i < numberOFtank; i++)
		{
			dir = rand() % 4 + 1;
			switch (dir)
			{
			case 1:
				
				bot->setPosition(bot->getPosition().x, bot->getPosition().y - 3);

			}
		}
			
		
	}

}

void createMap(char name[])
{
	
	FILE *mapFile;
	if (fopen(name, "r") == NULL)

	{
		mapFile = fopen(name, "a");
		for (int i = 0; i < numberOFmap; i++)
			map[i].obj = empty;
		for (int i = 0; i < 17; i++)
		{
			map[i * 21].obj = 11;
			map[i * 21 + 20].obj = 11;
		}
		for (int i = 0; i < 20; i++)
		{
			map[i + 1].obj = 11;
			map[i + 315 + 21].obj = 11;
		}
	}
	else
		mapFile = fopen(name, "w");
	fwrite(map, sizeof(_map), numberOFmap, mapFile);
	fclose(mapFile);
}
void readMap(char name[])
{
	FILE *mapFile;
	mapFile = fopen(name, "r");
	fread(map, sizeof(_map), numberOFmap, mapFile);
	fclose(mapFile);
}
void intro()
{
	sf::Text text;
	sf::Font fonttex;
	fonttex.loadFromFile("consola.ttf");
	text.setFont(fonttex);
	text.setCharacterSize(18);
	text.setColor(sf::Color(sf::Color(0,255,0,32)));
	sf::Color();
	//text.setOutlineColor(sf::Color(0,255,0,255));
	
	std::string str ="" ;
	int move = 7, whereX[200],whereY[200],i;
	for (int i = 0; i < 200; i++)
	{
		whereX[i] = rand() % 1920;
		whereY[i] = rand() % 1080;
	}
	int randomLength = rand() % (1080 / move);
	unsigned int run = 0;
	while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		windows.clear();
		
		for (int j = 0; j < 200; j++)
		{
			for (int k = 0; k < 8; k++)
			{
				if (rand() % 2 == 0)
					str = rand() % 26 + 65;
				else
					str = rand() % 10 + 48;

				text.setString(str);
				/*if (j % 3 == 0)
					text.setColor(sf::Color(0, 255, 0, 255 - k * 32));
				else if (j % 3 == 1)
					text.setColor(sf::Color(0, 255, 255, 255 - k * 32));
				else
					text.setColor(sf::Color(255, 255, 255, 255 - k * 32));*/
				text.setColor(sf::Color(255, 255, 255, 255 - k * 32));
				text.setPosition(whereX[j], whereY[j]-k*20);
				windows.draw(text);
			}
			whereY[j] += move;
			if (whereY[j]>1080+8*20)
			{
				whereX[j] = rand() % 1920;
				whereY[j] = 0;
			}
		}
		sf::sleep(sf::milliseconds(20));
		windows.display();

	}
	exit(-1);
		
		
		
	
	
	


}
int main()
{
	
	windows.setFramerateLimit(80);
	unsigned short int i = 0,s=0;
	srand(time(NULL));
	intro();
	sf::Vector2i mouse,mouse2;
	
	glEnable(GL_TEXTURE_2D);		//เปิดใช้openGL
	srand(time(NULL));
	
	sf::Thread playerControler_Thread(&playerControler);			//สร้างThread ควบคุมรถถัง
	playerControler_Thread.launch();								//สั่งThread ทำงาน

	sf::Thread bulletThread(&bulletStimulate);						//สร้างThread ยิงกระสุน
	bulletThread.launch();											//สั่งThread ทำงาน
	
	sf::Thread TankSound_Thread(&playTankSound);					//สร้างThread เล่นเสียงรถถัง
	TankSound_Thread.launch();										//สั่งThread ทำงาน
	


	player1.x = 1440 / 2; player1.y = 1080 / 2;					
	player2.x = 1440 / 2; player2.y = 1080 / 2 + 32*5;
	player1.modeShooting = normal; player2.modeShooting = normal;


	sf::RectangleShape player1Rec(sf::Vector2f(32, 32));			//สร้างสี่เหลื่ยมสำหรับplayer1
	sf::Texture player1Tex[7];										//สร้างTexture
	player1Rec.setOrigin(player1Rec.getSize().x / 2, player1Rec.getSize().y / 2);		//set จุดกำเนิดของสี่เหลื่ยม
	player1Rec.setScale(2, 2);				//set ขนาดเป็น2เท่า


	sf::RectangleShape player2Rec(sf::Vector2f(32, 32));			//สร้างสี่เหลื่ยมสำหรับplayer2
	sf::Texture player2Tex[7];										//สร้างTexture
	player2Rec.setOrigin(player2Rec.getSize().x / 2, player2Rec.getSize().y / 2);		//set จุดกำเนิดของสี่ยงเหลื่ยม
	player2Rec.setScale(2, 2);				//set ขนาดเป็น2เท่า

	sf::RectangleShape botRec(sf::Vector2f(64, 64));
	sf::Texture botTex[7][8];
	botRec.setOrigin(botRec.getSize().x / 2, botRec.getSize().y / 2);
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 8; j++)
			botTex[i][j].loadFromFile("MulticolorTanks.png", sf::IntRect(i * 32, j * 32, 32, 32));


	sf::RectangleShape bulletS(sf::Vector2f(32, 32));				//สร้างสี่เหลื่ยมกระสุน
	sf::Texture bulletTex;											//สร้างTexture
	bulletTex.loadFromFile("tiles.png", sf::IntRect(9 * 32, 0, 32, 32));	//Load Texture จากไฟล์
	bulletS.setTexture(&bulletTex);				//set Texture กับกระสุน
	bulletS.setOrigin(bulletS.getSize().x / 2, bulletS.getSize().y / 2);		//set จุดกำเนิด
	


	sf::RectangleShape mapEditor(sf::Vector2f(32, 32));
	startMapEditor.x = 500; startMapEditor.y = 500;
	sf::RectangleShape mapS(sf::Vector2f(64, 64));
	sf::Texture mapTex[6][12];

	for (int u = 0; u < 6; u++) 
		for (int v = 0; v < 12; v++)
			mapTex[u][v].loadFromFile("tiles.png", sf::IntRect(v * 32, (u + 1) * 32, 31, 32));
	int mapEditorObj=11;
	
	//Load Texture จากไฟล์
	for (i = 0; i < 7; i++)
	{
		player1Tex[i].loadFromFile("tiles.png", sf::IntRect((15 + i) * 32-1, 0, 32, 32));
		if (i != 0)
			player2Tex[i - 1].loadFromFile("tiles.png", sf::IntRect((15 + i) * 32, 9 * 32, 32, 32));
	}
	player2Tex[6].loadFromFile("tiles.png", sf::IntRect((15+6) * 32, 9 * 32, 32, 32));
	
	
	
	

	
	std::string posOfmouse;
	sf::Text pixel;
	sf::Font testFont;
	testFont.loadFromFile("SuperMario256.ttf");
	pixel.setFont(testFont);
	pixel.setCharacterSize(32);
	pixel.setPosition(1700, 1080/2);
	pixel.setColor(sf::Color::White);
	

	//createMap("testMap.txt");
	readMap("Hi.txt");
	


	while (1)
	{
		
		windows.clear();			//Clear window
		//draw map
		for (int ob = 0; ob < numberOFmap; ob++)
			if (map[ob].obj != 42 && map[ob].obj != 43 && map[ob].obj != 72 && map[ob].obj != empty)
			{
				mapS.setTexture(&mapTex[map[ob].obj / 12][map[ob].obj % 12 - 1]);
				mapS.setPosition(getMapPos.x(ob), getMapPos.y(ob));
				windows.draw(mapS);
			}
		if (++i % 10 == 0)
		{
			//เปลี่ยนTexture ให้กับรถถังไปเรื่อยๆ ทำให้เห็นเหมือนล้อรถถังกำลังวิ่งอยู่
			player1Rec.setTexture(&player1Tex[s]);
			player2Rec.setTexture(&player2Tex[s]);
			if (++s > 6)s = 0;
		}
		
		//draw tank1
		switch (player1.dir)
		{
			//หมุนรถถังเป็นองศาตามทิศทาง
			case 1:player1Rec.setRotation(0); break;
			case 2:player1Rec.setRotation(90); break;
			case 3:player1Rec.setRotation(180); break;
			case 4:player1Rec.setRotation(270); break;
			case 5:player1Rec.setRotation(45); break;
			case 6:player1Rec.setRotation(135); break;
			case 7:player1Rec.setRotation(225); break;
			case 8:player1Rec.setRotation(315); break;
		}

		player1Rec.setPosition(player1.x, player1.y);		//กำหนดตำแหน่งรถถังplayer1
		windows.draw(player1Rec);							//วาดรถถังplayer1ตามตำแหน่งและทิศทาง
		
		
		//draw tank2
		switch (player2.dir)
		{
			//หมุนรถถังเป็นองศาตามทิศทาง
			case 1:player2Rec.setRotation(0); break;
			case 2:player2Rec.setRotation(90); break;
			case 3:player2Rec.setRotation(180); break;
			case 4:player2Rec.setRotation(270); break;
			case 5:player2Rec.setRotation(45); break;
			case 6:player2Rec.setRotation(135); break;
			case 7:player2Rec.setRotation(225); break;
			case 8:player2Rec.setRotation(315); break;
		}
		player2Rec.setPosition(player2.x, player2.y);		//กำหนดตำแหน่งรถถังplayer2
		windows.draw(player2Rec);							//วาดรถถังplayer2ตามตำแหน่งและทิศทาง
			
		//draw bullet
		for (int y = 0; y < 30; y++)
		{
			if (bulle[y].obj == bullet){
				switch (bulle[y].dir)
				{
					//หมุนทิศทางกระสุน
				case 1: bulletS.setRotation(0); break;
				case 2: bulletS.setRotation(90); break;
				case 3: bulletS.setRotation(180); break;
				case 4: bulletS.setRotation(270); break;
				
				}
				for (int v = 0; v < numberOFmap; v++)
					if (map[v].obj != empty)
						if (checkTouch.bullets(v, y))
							bulle[y].obj = empty;
			}
			
			bulletS.setPosition(bulle[y].x, bulle[y].y);	//กำกนดตำแหน่งกระสุน
			if (bulle[y].obj == bullet)
				windows.draw(bulletS);						//วาดกระสุน
											
		}
		
		
		
		
		//draw mouse position
		mouse = sf::Mouse::getPosition(windows);
		posOfmouse = std::to_string(mouse.x) + " " + std::to_string(mouse.y);
		pixel.setString(posOfmouse);
		windows.draw(pixel);
		
		//map editor
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			if ((mouse.x > startMapEditor.x + (32 + 5) * 5 && mouse.x < startMapEditor.x + (32 + 5) * 7
				&& mouse.y>startMapEditor.y + (32 + 5) * 3 && mouse.y < startMapEditor.y + (32 + 5) * 4))
			{

				while (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					windows.clear();
					mouse = sf::Mouse::getPosition(windows);
					startMapEditor.x = mouse.x - (32 + 5) * 6;
					startMapEditor.y = mouse.y - (32 + 5) * 4 + 16;
					for (int u = 0; u < 6; u++)
					{
						for (int v = 0; v < 12; v++)
						{
							mapEditor.setTexture(&mapTex[u][v]);
							mapEditor.setPosition(startMapEditor.x + v* (32 + 5), startMapEditor.y + u* (32 + 5));
							windows.draw(mapEditor);
						}
					}
					windows.display();
				}
				windows.clear();
			}
			else
			{
				for (int i = 0; i < 72; i++)
					if (mouse.x>startMapEditor.x + i % 12 * (32 + 5) && mouse.x<startMapEditor.x + i % 12 * (32 + 5) + 32
						&& mouse.y>startMapEditor.y + (i / 12) * (32 + 5) && mouse.y < startMapEditor.y + (i / 12) * (32 + 5) + 32)
					{
						mapEditorObj = i + 1;
						break;
					}

			}
		}
			
		//draw mapEditor
		for (int u = 0; u < 6; u++)
		{
			for (int v = 0; v < 12; v++)
			{
				mapEditor.setTexture(&mapTex[u][v]);
				mapEditor.setPosition(startMapEditor.x + v* (32 + 5), startMapEditor.y + u* (32 + 5));
				windows.draw(mapEditor);
			}
		}

		//create block of map from mapEditor
		for (int u = 0; u < 15; u++)
			for (int v = 0; v < 19; v++)
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					mouse = sf::Mouse::getPosition();
					if (303 + v * 64 < mouse.x && 303 + v * 64 + 64 > mouse.x
						&& 64 + u * 64 < mouse.y && 64 + u * 64 + 64 > mouse.y)

						map[v +1 + (u+1 ) * 21].obj = mapEditorObj;
				}
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				{
					mouse = sf::Mouse::getPosition();
					if (303 + v * 64 < mouse.x && 303 + v * 64 + 64 > mouse.x
						&& 64 + u * 64 < mouse.y && 64 + u * 64 + 64 > mouse.y)

						map[v + 1 + (u + 1) * 21].obj = empty;
				}
			}
		
		windows.display();							//แสดงผลที่วาดไว้ทางหน้าจอ
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			createMap("Hi.txt");
			exit(-1);
		}
	}

}