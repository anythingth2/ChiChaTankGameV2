#include <iostream>
#include <SFML/System.hpp>				//SFML ��� Library����Ѻ��¹��
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <stdlib.h>
#include <conio.h>
#include <SFML/OpenGL.hpp>
#include <string>
#include <vector>
#include <cmath>


#define empty 0			//define ����Ѻobj
#define bullet 4
#define player1bullet 1
#define player2bullet 2
#define botbullet 3
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

#define normal 1		//define����Ѻ��������ԧ
#define spread 2
#define reflect 3
const int delayBullet = 5;				//˹�ǧ���ҡ���ع����͹���
const int delayShootingBullet = 15;		//˹�ǧ���ҡ���ع����ԧ�������
const int delayTank = 15;				//˹�ǧ�������ǡ������͹���
const int numberOFmap = 336+21;			//�ӹǹ���͡�map������
const int rangeRadarBot = 1200;
int countbullet = 0;
int readyTank[2] = { 5, 5 };
int numberOFtank = 4;
int scoreTank[2] = { 0, 0 };
int bossPos;
int botTankremain = 50;
int botTarget[7];
sf::Time botTimingDelay = sf::milliseconds(500);
bool playingTankRunSound = false;		//boolean������§���ö�ѧ���
//����ԧẺ�١�з�͹ �ѧ�������

sf::RenderWindow windows(sf::VideoMode(1920,1080), "ChiChaTankGame_V2", sf::Style::Fullscreen);		//render window ��Ҵ 1920x1080 FullScreen
//sf::RenderWindow windows(sf::VideoMode(800, 600), "ChiChaTankGame_V2");
sf::RectangleShape botRec(sf::Vector2f(64, 64));

sf::Mutex mutex;
sf::Clock timing;
sf::Time botShootTiming[7];
struct _pos{
	int x, y;
}startMapEditor;
struct _tank
{
	int x, y, dir;
	int hp;
	int dmg;
	int modeShooting;
	int obj;
	int skin;
	//hp = health point ��ѧ���Ե�ͧ�ѵ�� 
	//dmg = damage ��ѧ����
	//modeShooting = ��������ԧ
	//x ��� y ��͵��˹觢ͧ�ѵ�� ,dir=direction ��ȷҧ ��8���
}player1,player2,bulle[100],bot[7];
// player1���player2 �纵��ö�ѧ ,bulle ���١����ع

struct _map
{
	//int x, y, hp,obj;
	int hp, obj;
}map[numberOFmap+10];
//map ���ѵ���Ἱ��� ,obj ��ͪ�Դ�ѵ��,hp �纾�ѧ���Ե�ͧ�ѵ��
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
}getMapPos;//���ͧ�ҡ���˹觢ͧ��ѵ�� ������˹�����ԡѴ ������array᷹
struct _checkCollision
{
	//�礪�
	bool bullets(int i, int j)
	{
		//����ҡ���ع���Ѻ�ѵ��㹴�ҹ�������
		if (bulle[j].obj == empty)return false;
		if (map[i].obj<28 && !(bulle[j].x - 5 > getMapPos.x(i) + 64
			|| bulle[j].x + 5 < getMapPos.x(i)
			|| bulle[j].y - 12 > getMapPos.y(i) + 64
			|| bulle[j].y + 12 < getMapPos.y(i)) &&map[i].hp>0)
		{
			if (map[i].obj == 13)exit(1);     
			map[i].hp--;
			if (map[i].hp <= 0)
				map[i].obj = empty;
			return true;
		}
		else return false;
	}

	bool tank(int no,int dir)
	{
		//�����ö�ѧ���ѵ���������
		//���礵����ȷ��ö�ѧ�ѹ����
		if (no == 1)
		{
			switch (dir)
			{
			case 1:
				for (int i = 0; i < numberOFmap; i++)
				{
					if (map[i].obj != empty&& map[i].obj<28)
						if (!(player1.x - 25 > getMapPos.x(i) + 60
							|| player1.x + 25 < getMapPos.x(i) + 4
							|| player1.y - 3 - 25 > getMapPos.y(i) + 60
							|| player1.y - 3 + 25 < getMapPos.y(i) + 4))
						{
							
							return true;
						}
							
				}
				return false;


			case 2:
				for (int i = 0; i < numberOFmap; i++)
				{
					
					if (map[i].obj != empty && map[i].obj<28)
						if (!(player1.x + 3 - 25 > getMapPos.x(i) + 60
							|| player1.x + 3 + 25 < getMapPos.x(i) + 4
							|| player1.y - 25 > getMapPos.y(i) + 60
							|| player1.y + 25 < getMapPos.y(i) + 4))
							return true;
				}
				return false;


			case 3:
				for (int i = 0; i < numberOFmap; i++)
					if (map[i].obj != empty&& map[i].obj<28)
						if (!(player1.x - 25 > getMapPos.x(i) + 60
							|| player1.x + 25 < getMapPos.x(i)+4
							|| player1.y + 3 - 25 > getMapPos.y(i) + 60
							|| player1.y + 3 + 25 < getMapPos.y(i)+4))
							return true;
				return false;


			case 4:
				for (int i = 0; i < numberOFmap; i++)
					if (map[i].obj != empty&& map[i].obj<28)
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
					if (map[i].obj != empty&& map[i].obj<28)
						if (!(player2.x - 25 > getMapPos.x(i) + 60
							|| player2.x + 25 < getMapPos.x(i)+4
							|| player2.y - 3 - 25 > getMapPos.y(i) + 60
							|| player2.y - 3 + 25 < getMapPos.y(i)+4))
							return true;
				return false;


			case 2:
				for (int i = 0; i < numberOFmap; i++)
					if (map[i].obj != empty&& map[i].obj<28)
						if (!(player2.x + 3 - 25 > getMapPos.x(i) + 60
							|| player2.x + 3 + 25 < getMapPos.x(i)+4
							|| player2.y - 25 > getMapPos.y(i) + 60
							|| player2.y + 25 < getMapPos.y(i)+4))
							return true;
				return false;


			case 3:
				for (int i = 0; i < numberOFmap; i++)
					if (map[i].obj != empty&& map[i].obj<28)
						if (!(player2.x - 25 > getMapPos.x(i) + 60
							|| player2.x + 25 < getMapPos.x(i)+4
							|| player2.y + 3 - 25 > getMapPos.y(i) + 60
							|| player2.y + 3 + 25 < getMapPos.y(i)+4))
							return true;
				return false;


			case 4:
				for (int i = 0; i < numberOFmap; i++)
					if (map[i].obj != empty&& map[i].obj<28)
						if (!(player2.x - 3 - 25 > getMapPos.x(i) + 60
							|| player2.x - 3 + 25 < getMapPos.x(i)+4
							|| player2.y - 25 > getMapPos.y(i) + 60
							|| player2.y + 25 < getMapPos.y(i)+4))
							return true;

				return false;
			}
			
		}

	}

	bool bots(int no, int dir)
	{
		switch (dir)
		{
		case 1:
			for (int i = 0; i < numberOFmap; i++)
				if (map[i].obj != empty&& map[i].obj<28)
					if (!(bot[no].x - 25 > getMapPos.x(i) + 60
						|| bot[no].x + 25 < getMapPos.x(i) + 4
						|| bot[no].y - 3 - 25 > getMapPos.y(i) + 60
						|| bot[no].y - 3 + 25 < getMapPos.y(i) + 4))
						return true;
			return false;


		case 2:
			for (int i = 0; i < numberOFmap; i++)
				if (map[i].obj != empty&& map[i].obj<28)
					if (!(bot[no].x + 3 - 25 > getMapPos.x(i) + 60
						|| bot[no].x + 3 + 25 < getMapPos.x(i) + 4
						|| bot[no].y - 25 > getMapPos.y(i) + 60
						|| bot[no].y + 25 < getMapPos.y(i) + 4))
						return true;
			return false;


		case 3:
			for (int i = 0; i < numberOFmap; i++)
				if (map[i].obj != empty && map[i].obj<28)
					if (!(bot[no].x - 25 > getMapPos.x(i) + 60
						|| bot[no].x + 25 < getMapPos.x(i) + 4
						|| bot[no].y + 3 - 25 > getMapPos.y(i) + 60
						|| bot[no].y + 3 + 25 < getMapPos.y(i) + 4))
						return true;
			return false;


		case 4:
			for (int i = 0; i < numberOFmap; i++)
				if (map[i].obj != empty&& map[i].obj<28)
					if (!(bot[no].x - 3 - 25 > getMapPos.x(i) + 60
						|| bot[no].x - 3 + 25 < getMapPos.x(i) + 4
						|| bot[no].y - 25 > getMapPos.y(i) + 60
						|| bot[no].y + 25 < getMapPos.y(i) + 4))
						return true;

			return false;
		}
	}


	bool bulletVSplayer1(int i)
	{
		if (bulle[i].obj == empty)return false;
		return !(bulle[i].x - 5 > player1.x + 25
			|| bulle[i].x + 5 < player1.x-25
			|| bulle[i].y - 12 > player1.y + 25
			|| bulle[i].y + 12 < player1.y-25);
	}

	bool bulletVSplayer2(int i)
	{
		if (bulle[i].obj == empty)return false;
		return !(bulle[i].x - 5 > player2.x + 25
			|| bulle[i].x + 5 < player2.x - 25
			|| bulle[i].y - 12 > player2.y + 25
			|| bulle[i].y + 12 < player2.y - 25);
	}
	
	bool bulletVSbot(int i, int j)
	{
		if (bulle[i].obj == empty)return false;
		return !(bulle[i].x - 5 > bot[j].x + 5
			|| bulle[i].x + 5 < bot[j].x - 25
			|| bulle[i].y - 12 > bot[j].y + 25
			|| bulle[i].y + 12 < bot[j].y - 25);
	}
	
}checkTouch;
struct _spawn
{
	void Bot(int x)
	{
		
		botTarget[x] = rand() % 2;
		botTankremain--;
		bot[x].obj = normal;
		bot[x].hp = 1;
		do
		{
			bot[x].x = rand() % 800 + 400;
			bot[x].y = rand() % 500 + 85;
		} while (checkTouch.bots(x, bot[x].dir));
	}

	void Player1()
	{
		readyTank[0]--;
		player1.obj = normal;
		player1.hp = 3;
		do
		{
			player1.x = rand() % 800 + 400;
			player1.y = rand() % 500 + 85;
		} while (checkTouch.tank(1, 1));
	}

	void Player2()
	{
		readyTank[1]--;
		player2.obj = normal;
		player2.hp = 3;
		do
		{
			player2.x = rand() % 800 + 400;
			player2.y = rand() % 500 + 85;
		} while (checkTouch.tank(2, 1));
	}
}spawn;

void playerControler()
{
	//Thread�Ǻ������ö�ѧ�ͧ2������
		//���ҧ10 sound buffers 
	sf::Sound shoot[10];		//���ҧ10 sounds
	sf::SoundBuffer buff[10];
	buff[0].loadFromFile("bin/shoot1.wav");
	buff[1].loadFromFile("bin/shoot2.wav");
	buff[2].loadFromFile("bin/shoot3.wav");
	buff[3].loadFromFile("bin/shoot4.wav");
	buff[4].loadFromFile("bin/shoot5.wav");
	buff[5].loadFromFile("bin/shoot6.wav");
	buff[6].loadFromFile("bin/shoot7.wav");
	buff[7].loadFromFile("bin/shoot8.wav");
	buff[8].loadFromFile("bin/shoot9.wav");
	buff[9].loadFromFile("bin/pew.wav");

	
	//Load ������§ŧbuffer

	for (int i = 0; i < 10; i++)
		shoot[i].setBuffer(buff[i]);//set buffer ���Ѻ���§
	std::cout << "Done Loading sound buffer" << std::endl;

	int mod1 = 0,mod2=0;


	do{
		playingTankRunSound = false;
		
		//player1
		if (keyW && !(keyD || keyA || keyS) && player1.hp>0)
		{
			//��ҡ� W ���player1�Թ˹�� ������������§ö�ѧ���
			player1.dir = 1;
			if (!checkTouch.tank(1, 1) )
			{
				
				player1.y -= 3;
				playingTankRunSound = true;
			}
			
		}
		if (keyD && !(keyW || keyA || keyS) && player1.hp > 0)
		{
			//��ҡ�D ���player1�Թ��� ������������§ö�ѧ���
			player1.dir = 2;
			if (!checkTouch.tank(1, 2) )
			{
				
				player1.x += 3;
				playingTankRunSound = true;
			}
		}
		if (keyS && !(keyD || keyA || keyW) && player1.hp > 0)
		{
			//��ҡ� S ���player1�Թ��� ������������§ö�ѧ���
			player1.dir = 3;
			if (!checkTouch.tank(1, 3) ){
				
				player1.y += 3;
				playingTankRunSound = true;
			}
		}
		if (keyA && !(keyD || keyW || keyS) && player1.hp > 0)
		{
			//��ҡ� A ���player1�Թ���� ������������§ö�ѧ���
			player1.dir = 4;
			if (!checkTouch.tank(1, 4) )
			{
				
				player1.x -= 3;
				playingTankRunSound = true;
			}
			
		}

		if (keySpace && player1.hp <= 0)spawn.Player1();
		if (keySpace && player1.hp > 0)
		{
			//�� Space player1�ԧ ���������§�ͧ����ع�ҡ���§���10���§
			//����mod㹡��˹�ǧ�������ǡ���ع
			if (mod1++%delayShootingBullet == 0)
			{
				shoot[rand() % 10].play();
				switch (player1.modeShooting)
				{
				case normal:
					
					bulle[countbullet++] = player1;
					switch (player1.dir)
					{
					case 1:bulle[countbullet - 1].y -= 48; break;
					case 2:bulle[countbullet - 1].x += 48; break;
					case 3:bulle[countbullet - 1].y += 48; break;
					case 4:bulle[countbullet - 1].x -= 48; break;
					}
					bulle[countbullet - 1].obj = player1bullet; break;
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
				
				if (++countbullet > 100)countbullet = 1;
			}

		}


		//player2
		if (keyUp && !(keyRight || keyDown || keyLeft) && player2.hp > 0)
		{
			//��ҡ� �١�â�� player2���Թ�� ���������§ö�ѧ���
			player2.dir = 1;
			if (!checkTouch.tank(2, 1) )
			{
				
				player2.y -= 3;
				playingTankRunSound = true;
			}

		}
		if (keyRight && !(keyUp || keyLeft || keyDown) && player2.hp > 0)
		{
			//��ҡ� �١�â�� player2���Թ��� ���������§ö�ѧ���
			player2.dir = 2;
			if (!checkTouch.tank(2, 2))
			{
				
				player2.x += 3;
				playingTankRunSound = true;
			}
			

		}
		if (keyDown && !(keyRight || keyLeft || keyUp) && player2.hp > 0)
		{
			//��ҡ� �١����ҧ player2���Թ��ҧ ���������§ö�ѧ���
			player2.dir = 3;
			if (!checkTouch.tank(2, 3) )
			{
				
				player2.y += 3;
				playingTankRunSound = true;
			}
			

		}
		if (keyLeft && !(keyRight || keyUp || keyDown) && player2.hp > 0)
		{
			//��ҡ� �١�ë��� player2���Թ���� ���������§ö�ѧ���
			player2.dir = 4;
			if (!checkTouch.tank(2, 4) )
			{
				
				player2.x -= 3;
				playingTankRunSound = true;
			}

		}
		if (keyRCtrl && player2.hp <= 0)spawn.Player2();
		if (keyRCtrl && player2.hp > 0)
		{
			//�� RightControl player2�ԧ����ع
			//����mod㹡��˹�ǧ�������ǡ���ع
			if (mod2++%delayShootingBullet == 0)
			{
				shoot[rand() % 10].play();
				switch (player2.modeShooting)
				{
				case normal:
					bulle[countbullet++] = player2;
					switch (player2.dir)
					{
					case 1:bulle[countbullet - 1].y -= 48; break;
					case 2:bulle[countbullet - 1].x += 48; break;
					case 3:bulle[countbullet - 1].y += 48; break;
					case 4:bulle[countbullet - 1].x -= 48; break;
					}
					bulle[countbullet - 1].obj = player2bullet; break;
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
				if (++countbullet > 100)countbullet = 1;
			}
		}
		
		sf::sleep(sf::milliseconds(delayTank)); //˹�ǧ����ö�ѧ����͹���
		playingTankRunSound = false;
	} while (!sf::Keyboard::isKeyPressed(sf::Keyboard::X));
	
}



void bulletStimulate()
{
	sf::Sound impact;
	sf::SoundBuffer impactBuff;
	impactBuff.loadFromFile("bin/impact.wav");
	impact.setBuffer(impactBuff);

	while (1)
	{
		//loop �ӧҹ�������array 
		for (int u = 0; u < 100; u++)
		{
			//����١����ع����͹�������ȷҧ�ͧ�����١
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
			if (checkTouch.bulletVSplayer1(u))
			{
				player1.hp--;
				
				bulle[u].obj = empty;
				impact.play();
			}
			//	

			if (checkTouch.bulletVSplayer2(u))
			{
				player2.hp--;
				
				bulle[u].obj = empty;
				impact.play();
			}

			for (int i = 0; i < numberOFtank; i++)
				if (checkTouch.bulletVSbot(u, i) && bulle[u].obj != empty)
				{
					bot[i].hp -= bulle[u].dmg;
					bot[i].obj = empty;
					if (bulle[u].obj == player1bullet)scoreTank[0] += 10;
					else if (bulle[u].obj == player2bullet)scoreTank[1] += 10;
					bulle[u].obj = empty;
					impact.play();
				}
			
			
		}
		sf::sleep(sf::milliseconds(delayBullet));	//˹�ǧ�����١����ع
	}
}

void playTankSound()
{
	//Thread ������§ö�ѧ
	sf::SoundBuffer tankRunBuf;			//���ҧ buffer sound�ͧö�ѧ
	sf::Sound tankRun;					//���ҧ sound ö�ѧ
	tankRunBuf.loadFromFile("bin/tank_sound.wav");		//Load���§ö�ѧ
	tankRun.setBuffer(tankRunBuf);					//set ���§�ö�ѧ


	while (1)
	{
		//������§ö�ѧ����� playingTankRunSound �� true
		if (playingTankRunSound){
			tankRun.play();		//������§ö�ѧ
			while (playingTankRunSound)sf::sleep(sf::milliseconds(16));	//�ͨ����� playingTankRunSound �� false
			tankRun.pause();	//��ش���§ö�ѧ
		}
		sf::sleep(sf::microseconds(1));
	}
}
int botDist[7];


int AI(int i)
{
	int bit = 0,dir=3,dist_nearest=9999,who=0;
	/*switch (rand() % 3)
	{
	case 0:
		if (sqrt(pow((bot[i].x - player1.x), 2) + pow((bot[i].y - player1.y), 2)) < rangeRadarBot)
		{
			bit += 1;
			if (bot[i].x > player1.x && bot[i].y < player1.x)dir = 1;
			if (bot[i].x > player1.x && bot[i].y > player1.x)dir = 2;
			if (bot[i].x < player1.x && bot[i].y > player1.x)dir = 3;
			if (bot[i].x < player1.x && bot[i].y < player1.x)dir = 4;
			break;
		}
	case 1:
		if (sqrt(pow((bot[i].x - player2.x), 2) + pow((bot[i].y - player2.y), 2)) < rangeRadarBot)
		{
			bit += 2;
			if (bot[i].x > player2.x && bot[i].y < player2.x)dir = 1;
			if (bot[i].x > player2.x && bot[i].y > player2.x)dir = 2;
			if (bot[i].x < player2.x && bot[i].y > player2.x)dir = 3;
			if (bot[i].x < player2.x && bot[i].y < player2.x)dir = 4;
			break;
		}
	case 2:
		if (sqrt(pow((bot[i].x - getMapPos.x(bossPos)), 2) + pow((bot[i].y - getMapPos.y(bossPos)), 2)) < rangeRadarBot)
		{
			bit += 4;
			if (bot[i].x > getMapPos.x(bossPos) && bot[i].y < player2.x)dir = 1;
			if (bot[i].x > getMapPos.x(bossPos) && bot[i].y > player2.x)dir = 2;
			if (bot[i].x < getMapPos.x(bossPos) && bot[i].y > player2.x)dir = 3;
			if (bot[i].x < getMapPos.x(bossPos) && bot[i].y < player2.x)dir = 4;
			break;
		}
	}*/
	
	
	
	


	switch (botTarget[i])
	{
	case 0:
		if (bot[i].x > player1.x && bot[i].y < player1.y)dir = 1;
		if (bot[i].x > player1.x && bot[i].y > player1.y)dir = 2;
		if (bot[i].x < player1.x && bot[i].y > player1.y)dir = 3;
		if (bot[i].x < player1.x && bot[i].y < player1.y)dir = 4;
		break;
	case 1:
		if (bot[i].x > player2.x && bot[i].y < player2.y)dir = 1;
		if (bot[i].x > player2.x && bot[i].y > player2.y)dir = 2;
		if (bot[i].x < player2.x && bot[i].y > player2.y)dir = 3;
		if (bot[i].x < player2.x && bot[i].y < player2.y)dir = 4;
		break;
	}
	int prob = rand() % 100;
	switch (dir)
	{
	case 1:
		if (prob < 10)return 1;
		else if (prob < 20)return 2;
		else if (prob < 60)return 3;
		else return 4;
		break;
	case 2:
		if (prob < 10)return 2;
		else if (prob < 20)return 3;
		else if (prob < 60)return 1;
		else return 4;
		break;
	case 3:
		if (prob < 10)return 3;
		else if (prob < 20)return 4;
		else if (prob < 60)return 1;
		else return 2;
		break;
	case 4:
		if (prob < 10)return 1;
		else if (prob < 20)return 4;
		else if (prob < 60)return 2;
		else return 3;
		break;
	}

	
	
}
bool checkPlayerFrontBot(int x)
{
	if (rand() % 2 == 1)return 0;
	switch (bot[x].dir)
	{
	case 1:
		if (bot[x].x + 48 > player1.x && bot[x].x - 48 < player1.x && bot[x].y > player1.y)
			return 1;
		if (bot[x].x + 48 > player2.x && bot[x].x - 48 < player2.x && bot[x].y > player2.y)
			return 1;
		break;
	case 2:
		if (bot[x].y + 48 > player1.y && bot[x].y - 48 < player1.y && bot[x].x < player1.x)
			return 1;
		if (bot[x].y + 48 > player2.y && bot[x].y - 48 < player2.y && bot[x].x < player2.x)
			return 1;
		break;
	case 3:
		if (bot[x].x + 48 > player1.x && bot[x].x - 48 < player1.x && bot[x].y < player1.y)
			return 1;
		if (bot[x].x + 48 > player2.x && bot[x].x - 48 < player2.x && bot[x].y < player2.y)
			return 1;
		break;
	case 4:
		if (bot[x].y + 48 > player1.y && bot[x].y - 48 < player1.y && bot[x].x > player1.x)
			return 1;
		if (bot[x].y + 48 > player2.y && bot[x].y - 48 < player2.y && bot[x].x > player2.x)
			return 1;
		break;
	}
	return 0;
}

bool checkBotReadyShoot(int x)
{
	if (timing.getElapsedTime() - botShootTiming[x] > botTimingDelay)
	{
		botShootTiming[x] = timing.getElapsedTime();
		return 1;
	}
	return 0;

}
void botTank()
{
	
	int i,k=0,j=0,countGoAway=0;
	
	for (i = 0; i < numberOFtank; i++)
	{
		if (bot[i].obj != empty)
		{
			if (botDist[i] < 0)
			{
				botDist[i] = rand() % 250;
				bot[i].dir = AI(i);
			}

			while (checkTouch.bots(i, bot[i].dir))
			{
				bot[i].dir = AI(i);
				if (++countGoAway>100){
					spawn.Bot(i);
					countGoAway = 0;
				}
			}


			switch (bot[i].dir)
			{
			case 1:bot[i].y -= 4; break;
			case 2:bot[i].x += 4; break;
			case 3:bot[i].y += 4; break;
			case 4:bot[i].x -= 4; break;
			}
			botDist[i] -= 4;

			if (checkPlayerFrontBot(i) && checkBotReadyShoot(i) && bot[i].hp>0)
			{	
				bulle[countbullet].obj = botbullet;
				bulle[countbullet].dir = bot[i].dir;
				bulle[countbullet].x = bot[i].x;
				bulle[countbullet].y = bot[i].y;
				switch (bot[i].dir)
				{
				case 1:bulle[countbullet].y -= 48; break;
				case 2:bulle[countbullet].x += 48; break;
				case 3:bulle[countbullet].y += 48; break;
				case 4:bulle[countbullet].x -= 48; break;
				}
				if (++countbullet>100)
					countbullet = 0;
			}
		}
		else
		{
			if (rand() % 50 == 0)
				spawn.Bot(i);
		}

	}
	//sf::sleep(sf::milliseconds(delayTank + 7));
		
		
		
	

}

void createMap(char name[])
{
	//function���ҧἹ��� ���Ѻ����Ἱ���
	FILE *mapFile;
	if (fopen(name, "r") == NULL)
		//����������͵������Ѻ�������� �����¹�Ѻ�ͧ���
		//�����ѧ�����������ҧ���������¹ ����Ҵ��ᾧ�ͺ�͡
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
	//��ҹἹ�������¹��������ͷ���Ѻ��
	FILE *mapFile;
	mapFile = fopen(name, "r");
	fread(map, sizeof(_map), numberOFmap, mapFile);
	fclose(mapFile);
	std::cout << "read map DONE!" << std::endl;
}
void intro()
{
	//intro ���ѡ�������ѧ�����е���Ţ ���ŧ�ҵ���Ǵ�� ���ѡ�÷����觨ж١����������� 
	//intro �Ф������˹ѧ Matrix
	sf::Music twd;
	twd.openFromFile("bin/TWD.wav");
	twd.play();
	twd.setLoop(true);
	sf::Text text;					//���ҧText����text
	sf::Font fonttex;				//���ҧ�������font
	fonttex.loadFromFile("bin/consola.ttf");	//load font
	text.setFont(fonttex);			//��fontŧtext
	text.setCharacterSize(18);		//��Ѻ��Ҵ�ѡ��
	
	std::string str ="" ;

	sf::Text nameGame,press;		
	sf::Font csFont;
	csFont.loadFromFile("bin/cs_regular.ttf");
	nameGame.setFont(csFont);
	nameGame.setCharacterSize(104);
	nameGame.setColor(sf::Color::White);		//set�բ��
	nameGame.setString("ChiChaTankGame_V2");
	press.setFont(csFont);
	press.setCharacterSize(52);
	press.setString("Press Space to Continue");
	
	nameGame.setPosition(1920 / 2-525, 1080/2 - 300);		//�絵��˹觢ͧ����� ChiChaTankGame_V2 ��˹�Ҩ�
	press.setPosition(1920 / 2-300, 1080 / 2 + 250);		//�絵��˹觢ͧ����� Press Space to Continue ��˹�Ҩ�

	int move = 7, whereX[200],whereY[200],i;

	//��whereX���whereY᷹�ԡѴ�ͧ����ѡ�÷�����ŧ��
	// move ���˹��������ŧ�ͧ�ѡ�� 7pixels
	for (int i = 0; i < 200; i++)
	{
		//�������˹�
		whereX[i] = rand() % 1920;
		whereY[i] = rand() % 1080;
	}
	unsigned int run = 0;

	sf::Event respondmeplz;

	while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		windows.clear();
		//�ѡ�è����ŧ�� 8����ѡ��������� ���ըФ����ҧ
		windows.pollEvent(respondmeplz);

		for (int j = 0; j < 200; j++)
		{
			for (int k = 0; k < 8; k++)
			{
				//���ŧ��8�ѡ�� �����ѡ���������
				if (rand() % 2 == 0)
					str = rand() % 26 + 65;
				else
					str = rand() % 10 + 48;

				text.setString(str);
				text.setColor(sf::Color(0, 255, 0, 255 - k * 32));	//������ Ŵ��������յ��������ҧ�ͧ�ѡ��
				if (j % 17 == 0)
					text.setColor(sf::Color(0, 255, 255, 255 - k * 32));	//�տ��
				else if (j % 19 == 0)
					text.setColor(sf::Color(255, 255, 255, 255 - k * 32));	//�բ��
				
				text.setPosition(whereX[j], whereY[j]-k*20);	//�絵��˹� 
				windows.draw(text);
			}
			whereY[j] += move;	//�ѡ�����ŧ
			if (whereY[j]>1080+8*20)
			{
				//����ѡ�����ŧ�ش��������������ش����
				whereX[j] = rand() % 1920;
				whereY[j] = 0;
			}
		}
		windows.draw(nameGame);	//�Ҵ������
		run++;
		
		if (run % 8 <6)windows.draw(press);	
		sf::sleep(sf::milliseconds(40));	//˹�ǧ����

		windows.display();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			exit(0);
		
	}
	std::cout << "Intro End" << std::endl;
	//exit(NULL);
}

void setHpMap()
{
	for (int i = 0; i < numberOFmap; i++)
		switch (map[i].obj)
		{
		case 11:map[i].hp = 999; break;
		case 13:map[i].hp = 1; bossPos = i; break;
		default:map[i].hp = 5;
		}

}
	


int main()
{
	for (int i = 0; i < 7; i++)
	{
		bot[i].dir = rand() % 4 + 1;
		botDist[i] = rand() % 500;
	}

	windows.setFramerateLimit(80);
	unsigned short int i = 0, s = 0;
	srand(time(NULL));
	intro();		//intro ��͹�����	
	sf::Vector2i mouse, mouse2;		//���纾ԡѴmouse

	glEnable(GL_TEXTURE_2D);		//�Դ��openGL
	srand(time(NULL));

	sf::Thread playerControler_Thread(&playerControler);			//���ҧThread �Ǻ���ö�ѧ
	playerControler_Thread.launch();								//���Thread �ӧҹ

	sf::Thread bulletThread(&bulletStimulate);						//���ҧThread �ԧ����ع
	bulletThread.launch();											//���Thread �ӧҹ

	sf::Thread TankSound_Thread(&playTankSound);					//���ҧThread ������§ö�ѧ
	TankSound_Thread.launch();										//���Thread �ӧҹ


	//sf::Thread botTank_Thread(&botTank);
	//botTank_Thread.launch();


	player1.x = 1440 / 2; player1.y = 1080 / 2;
	player2.x = 1440 / 2; player2.y = 1080 / 2 + 32 * 5;
	player1.modeShooting = normal; player2.modeShooting = normal;


	sf::RectangleShape player1Rec(sf::Vector2f(32, 32));			//���ҧ��������������Ѻplayer1
	sf::Texture player1Tex[7];										//���ҧTexture
	player1Rec.setOrigin(player1Rec.getSize().x / 2, player1Rec.getSize().y / 2);		//set �ش���Դ�ͧ����������
	player1Rec.setScale(2, 2);				//set ��Ҵ��2���


	sf::RectangleShape player2Rec(sf::Vector2f(32, 32));			//���ҧ��������������Ѻplayer2
	sf::Texture player2Tex[7];										//���ҧTexture
	player2Rec.setOrigin(player2Rec.getSize().x / 2, player2Rec.getSize().y / 2);		//set �ش���Դ�ͧ���§�������
	player2Rec.setScale(2, 2);				//set ��Ҵ��2���


	sf::Texture botTex[7][8];
	botRec.setOrigin(botRec.getSize().x / 2, botRec.getSize().y / 2);

	sf::Texture explTex[3];
	for (int i = 0; i < 3; i++)
		explTex[i].loadFromFile("bin/tiles.png", sf::IntRect(i * 32, 0, 32, 32));



	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 8; j++)
			botTex[i][j].loadFromFile("bin/MulticolorTanks.png", sf::IntRect(i * 32, j * 32, 32, 32));


	sf::RectangleShape bulletS(sf::Vector2f(32, 32));				//���ҧ��������������ع
	sf::Texture bulletTex;											//���ҧTexture
	bulletTex.loadFromFile("bin/tiles.png", sf::IntRect(9 * 32, 0, 32, 32));	//Load Texture �ҡ���
	bulletS.setTexture(&bulletTex);				//set Texture �Ѻ����ع
	bulletS.setOrigin(bulletS.getSize().x / 2, bulletS.getSize().y / 2);		//set �ش���Դ

	


	sf::RectangleShape mapEditor(sf::Vector2f(32, 32));
	startMapEditor.x = 500; startMapEditor.y = 500;
	sf::RectangleShape mapS(sf::Vector2f(64, 64));
	sf::Texture mapTex[6][12];
	
	for (int u = 0; u < 6; u++)
		for (int v = 0; v < 12; v++)
			mapTex[u][v].loadFromFile("bin/tiles.png", sf::IntRect(v * 32, (u + 1) * 32, 31, 32));
	int mapEditorObj = 11;

	//Load Texture �ҡ���
	for (i = 0; i < 7; i++)
	{
		player1Tex[i].loadFromFile("bin/tiles.png", sf::IntRect((15 + i) * 32 - 1, 0, 32, 32));
		if (i != 0)
			player2Tex[i - 1].loadFromFile("bin/tiles.png", sf::IntRect((15 + i) * 32, 9 * 32, 32, 32));
	}
	player2Tex[6].loadFromFile("bin/tiles.png", sf::IntRect((15 + 6) * 32, 9 * 32, 32, 32));

	sf::RectangleShape heart(sf::Vector2f(40, 40)), retank(sf::Vector2f(40, 40)), retank2(sf::Vector2f(40, 40));
	sf::RectangleShape logo(sf::Vector2f(128, 128)), logo2(sf::Vector2f(128, 128));

	sf::Texture heartTex;
	heartTex.loadFromFile("bin/heart.png");
	heart.setTexture(&heartTex);
	retank.setTexture(&player1Tex[0]); retank2.setTexture(&player2Tex[0]);
	sf::Text name, kmitlno, life, remaintank, life2, remaintank2, pointtext, scoretext, remainbotTankText, remainbot;
	std::string score;
	sf::Font cs;
	cs.loadFromFile("bin/cs_regular.ttf");
	name.setFont(cs); kmitlno.setFont(cs); life.setFont(cs); remaintank.setFont(cs); life2.setFont(cs); remaintank2.setFont(cs); pointtext.setFont(cs); scoretext.setFont(cs), remainbotTankText.setFont(cs), remainbot.setFont(cs);
	name.setString("ChatChai ShaeTan"); kmitlno.setString("59010240"); life.setString("Life ->"); remaintank.setString("Tank ->"); life2.setString("Life ->"); remaintank2.setString("Tank ->"); pointtext.setString("Points");
	remainbotTankText.setString("Remaining Bot ->"); remainbotTankText.setPosition(10, 1080 - 100);
	remainbot.setPosition(75, 1080 - 50);
	name.setPosition(1920 - 300, 1080 - 100); kmitlno.setPosition(1920 - 200, 1080 - 50);
	life.setPosition(25, 300); life2.setPosition(1920 - 300, 300); remaintank.setPosition(25, 700); remaintank2.setPosition(1920 - 300, 700);
	logo.setPosition(50, 150); logo2.setPosition(1920 - 240, 150);

	sf::Text win, lose;
	win.setFont(cs); lose.setFont(cs);
	win.setString("YOU WIN!!!"); lose.setString("You Lose");
	win.setPosition(1920 / 2 - 300, 1080 / 2 - 100); lose.setPosition(1920 / 2 - 300, 1080 / 2 - 100);
	win.setOutlineColor(sf::Color::Black); lose.setOutlineColor(sf::Color::Black);
	win.setOutlineThickness(1.5); lose.setOutlineThickness(1.5);
	win.setScale(4, 4); lose.setScale(4, 4);
	//createMap("Hi.txt");
	readMap("bin/Hi.txt");
	
	sf::Event events;


	player1.dmg = 1;
	player1.hp = 3;
	player1.obj = normal;
	player2.dmg = 1;
	player2.hp = 3;
	player2.obj = normal;

	for (int i = 0; i < numberOFtank; i++)
	{
		bot[i].obj = normal;
		bot[i].dmg = 1;
		bot[i].hp = 1;
	}

	int countExplTex[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	std::cout << "Start running game" << std::endl;
	int switchmapEditor = 0;
	for (int i = 0; i < numberOFtank;i++)
		spawn.Bot(i);
	setHpMap();
	
	while (1)
	{
		
		windows.clear();			//Clear window
		//draw map
		logo.setTexture(&player1Tex[s]); logo2.setTexture(&player2Tex[s]);
		windows.draw(name); windows.draw(kmitlno); windows.draw(life); windows.draw(life2); windows.draw(remaintank); windows.draw(remaintank2);
		windows.draw(logo); windows.draw(logo2); windows.draw(remainbotTankText);
		score = std::to_string(scoreTank[0]); scoretext.setString(score); scoretext.setPosition(15, 550); windows.draw(scoretext);
		score = std::to_string(scoreTank[1]); scoretext.setString(score); scoretext.setPosition(1920-235, 550); windows.draw(scoretext);
		pointtext.setPosition(100, 550); windows.draw(pointtext);
		pointtext.setPosition(1920-150, 550); windows.draw(pointtext);
		score = std::to_string(botTankremain); remainbot.setString(score); windows.draw(remainbot);
		for (int i = 0; i < player1.hp; i++){ heart.setPosition(25 + i * 72, 400); windows.draw(heart); }
		for (int i = 0; i < player2.hp; i++){ heart.setPosition(1920-300 + i * 72, 400); windows.draw(heart); }
		for (int i = 0; i < readyTank[0]; i++){ retank.setPosition(10 + i * 45, 800); windows.draw(retank); }
		for (int i = 0; i < readyTank[1]; i++){ retank2.setPosition(1920-300 + i * 45, 800); windows.draw(retank2); }
		
		
		
		for (int ob = 0; ob < numberOFmap; ob++)
		{
			if (map[ob].obj == 13 && map[ob].hp<0)exit(0); 
			if (map[ob].obj != 42 && map[ob].obj != 43 && map[ob].obj != 72 && map[ob].obj != empty&&map[ob].hp > 0)
			{
				//��
				
				mapS.setTexture(&mapTex[map[ob].obj/ 12][map[ob].obj % 12 - 1]);
				mapS.setPosition(getMapPos.x(ob), getMapPos.y(ob));
				windows.draw(mapS);
			}
		}
		if (++i % 10 == 0)
		{
			//����¹Texture ���Ѻö�ѧ�������� ������������͹���ö�ѧ���ѧ�������
			player1Rec.setTexture(&player1Tex[s]);
			player2Rec.setTexture(&player2Tex[s]);
			if (++s > 6)s = 0;
		}
		
		//draw tank1
		if (player1.obj != empty && player1.hp>0)
		{
			switch (player1.dir)
			{
				//��عö�ѧ��ͧ�ҵ����ȷҧ
			case 1:player1Rec.setRotation(0); break;
			case 2:player1Rec.setRotation(90); break;
			case 3:player1Rec.setRotation(180); break;
			case 4:player1Rec.setRotation(270); break;

			}
			
			player1Rec.setPosition(player1.x, player1.y);		//��˹����˹�ö�ѧplayer1
			windows.draw(player1Rec);							//�Ҵö�ѧplayer1������˹���з�ȷҧ

		}
		
		
		//draw tank2
		if (player2.obj != empty && player2.hp>0)
		{
			switch (player2.dir)
			{
				//��عö�ѧ��ͧ�ҵ����ȷҧ
			case 1:player2Rec.setRotation(0); break;
			case 2:player2Rec.setRotation(90); break;
			case 3:player2Rec.setRotation(180); break;
			case 4:player2Rec.setRotation(270); break;

			}
			player2Rec.setPosition(player2.x, player2.y);		//��˹����˹�ö�ѧplayer2
			windows.draw(player2Rec);							//�Ҵö�ѧplayer2������˹���з�ȷҧ
		}
		
		for (int i = 0; i < 9; i++)
			if (countExplTex[i] != 0 && countExplTex[i] < 7 )
			{ 
				if (i <= 1)
				{
					if (i == 0)
						if (countExplTex[i] <= 3)
							player1Rec.setTexture(&explTex[countExplTex[i]-1]);
						else
							player1Rec.setTexture(&explTex[6-countExplTex[i]]);
					else
						if (countExplTex[i] <= 3)
							player2Rec.setTexture(&explTex[countExplTex[i] - 1]);
						else
							player2Rec.setTexture(&explTex[6 - countExplTex[i]]);
				}
				
				
			}
		if (i % 2 == 0)botTank();
		for (int ii = 0; ii < numberOFtank; ii++)
		{
			if (bot[ii].obj != empty && bot[ii].hp>0)
			{
				switch (bot[ii].dir)
				{
					//��عö�ѧ��ͧ�ҵ����ȷҧ
				case 1:botRec.setRotation(270); break;
				case 2:botRec.setRotation(0); break;
				case 3:botRec.setRotation(90); break;
				case 4:botRec.setRotation(180); break;
				}

				botRec.setPosition(bot[ii].x, bot[ii].y);		//��˹����˹�ö�ѧbot
				botRec.setTexture(&botTex[s][ii]);
				windows.draw(botRec);
			}
			
		}//�Ҵö�ѧbot������˹���з�ȷҧ


		//draw bullet
		for (int y = 0; y < 100; y++)
		{
			if (bulle[y].obj !=empty){
				switch (bulle[y].dir)
				{
					//��ع��ȷҧ����ع
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
			
			bulletS.setPosition(bulle[y].x, bulle[y].y);	//��˹����˹觡���ع
			if (bulle[y].obj != empty)
				windows.draw(bulletS);						//�Ҵ����ع
											
		}
		
		
		
		
		//draw mouse position
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)){ switchmapEditor++; while (sf::Keyboard::isKeyPressed(sf::Keyboard::P)); }
		windows.pollEvent(events);
		if (events.type == sf::Event::MouseButtonPressed){
			mouse = sf::Mouse::getPosition(windows);


			//map editor
			
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && switchmapEditor % 2 == 1){
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
			//create block of map from mapEditor
			for (int u = 0; u < 15; u++)
				for (int v = 0; v < 19; v++)
				{
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						mouse = sf::Mouse::getPosition();
						if (303 + v * 64 < mouse.x && 303 + v * 64 + 64 > mouse.x
							&& 64 + u * 64 < mouse.y && 64 + u * 64 + 64 > mouse.y)

							map[v + 1 + (u + 1) * 21].obj = mapEditorObj;
					}
					if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
					{
						mouse = sf::Mouse::getPosition();
						if (303 + v * 64 < mouse.x && 303 + v * 64 + 64 > mouse.x
							&& 64 + u * 64 < mouse.y && 64 + u * 64 + 64 > mouse.y)

							map[v + 1 + (u + 1) * 21].obj = empty;
					}
				}
		}
		
			
		//draw mapEditor
		if (switchmapEditor % 2 == 1)
			for (int u = 0; u < 6; u++)
				for (int v = 0; v < 12; v++)
				{
					mapEditor.setTexture(&mapTex[u][v]);
					mapEditor.setPosition(startMapEditor.x + v* (32 + 5), startMapEditor.y + u* (32 + 5));
					windows.draw(mapEditor);
				}
		

		
		if (readyTank[0] < 0 && readyTank[1] < 0 )
		{
			playerControler_Thread.terminate();
			bulletThread.terminate();
			TankSound_Thread.terminate();
			windows.draw(lose);
			
		}

		if (botTankremain<0)
		{
			playerControler_Thread.terminate();
			bulletThread.terminate();
			TankSound_Thread.terminate();
			windows.draw(win);
			
		}
		windows.display();							//�ʴ��ŷ���Ҵ���ҧ˹�Ҩ�
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			//createMap("bin/Hi.txt");
			exit(-1);
		}
		sf::sleep(sf::microseconds(1));
		
	}
	getch();
}