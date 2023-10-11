
#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <time.h>
#include <list>
#include<string.h>
using namespace std;

#define WIN_WIDTH	600
#define	WIN_HEIGHT	800
bool lose = false;
bool weapon_reinforced_0 = false;
bool weapon_reinforced_1 = false;
int enemy_killed = 0;
//Definition of coordinate of both planes and bullets
class Gift
{
public:
	int x;
	int y;
	int style;
	Gift();
	Gift(int xx, int yy,int s)
	{
		x = xx;
		y = yy;
		style = s;
	}
	void draw()
	{
		if (style == 0)setfillcolor(YELLOW);
		if (style == 1)setfillcolor(BLUE);
		POINT points[] = { {x,y + 10},{x + 10,y + 20},{x + 20,y + 10},{x + 10,y } };
		fillpolygon(points, 4);
		y++;
	}
};
list<Gift>giftlist;
class Bullet {
public:
	int x;
	int y;
	Bullet();
	Bullet(int xx,int yy)
	{
		x = xx;
		y = yy;
	}
};
list<Bullet>Enemybulletlist;
list<Bullet>Bossbulletlist_0;
list<Bullet>Bossbulletlist_1;
list<Bullet>Bossbulletlist_2;
class Enemy {
public:
	int x;
	int y;
	int direct;
	DWORD t1 = GetTickCount(); 
	DWORD t2;
	Enemy();
	Enemy(int xx,int yy)
	{
		x = xx;
		y = yy;
	}
	void draw()
	{
		roundrect(x, y, x + 30, y + 20, 5, 5);
	}
	void mov()
	{
		t2 = GetTickCount();
		if (t2 - t1 > 1000) {
			direct = rand() % 2;
			t1 = t2;
		}
		if (direct == 1) {
			x++;
		}
		if (direct == 0) {
			x--;
		}
	}
};
list<Enemy>EnemyList;
class BOSS
{
public:
	int x;
	int y;
	bool alive=true;
	int life=100;
	int direct=1;
	DWORD t1 = GetTickCount();
	DWORD t2;
	DWORD tt1 = GetTickCount();
	DWORD tt2;
	void shoot()
	{
		t2 = GetTickCount();
		if (t2 - t1 > 500&&life>0) {
			Bossbulletlist_1.push_back(Bullet(x + 100, y + 100));
			Bossbulletlist_0.push_back(Bullet(x + 50, y + 100));
			Bossbulletlist_2.push_back(Bullet(x + 150, y + 100));
			t1 = t2;
		}
		setfillcolor(RED);
		for (auto& p : Bossbulletlist_0) {
			fillcircle(p.x, p.y, 5);
			p.y += 3;
			p.x -= 3;
		}
		for (auto& p : Bossbulletlist_1) {
			fillcircle(p.x, p.y, 5);
			p.y += 3;
		}
		for (auto& p : Bossbulletlist_2) {
			fillcircle(p.x, p.y, 5);
			p.y += 3;
			p.x += 3;
		}
	}
	void life_print()
	{
		setfillcolor(RED);
		fillrectangle(x , y -15, x + 2 * life, y -5);
	}
	BOSS();
	BOSS(int xx, int yy)
	{
		x = xx;
		y = yy;
	}
	void draw()
	{
		setfillcolor(WHITE);
		fillroundrect(x, y, x + 200, y + 100, 10, 10);
		settextstyle(50, 0, TEXT("黑体"));
		outtextxy(x +50,y+20,TEXT("BOSS"));
	}
	void mov()
	{
		if (y < 130)y += 3;
		tt2 = GetTickCount();
		if (tt2 - tt1 > 1000) {
			direct = rand() % 2;
			tt1 = tt2;
		}
		if (direct == 1) {
			x++;
		}
		if (direct == 0) {
			x--;
		}
		if (x >=400)direct = 0;
		if (x <= 0)direct = 1;
	}
};
BOSS boss(200, 0);
class Player
{
public:
	int x;
	int y;
	int life = 10;
	list<Bullet>bulletlist;
	Player();
	Player(int xx,int yy)
	{
		x = xx;
		y = yy;
	}
	void draw()
	{
		setfillcolor(WHITE);
		setlinecolor(WHITE);
		fillrectangle(x, y, x + 32, y + 18);
		for (auto& p : bulletlist) {
			circle(p.x, p.y, 5); 
			p.y-=3;
		}
	}
	void mov()
	{
		if (_kbhit())			
		{
			//char key;
			//key = _getch();		//	Obtain key info

			if (GetAsyncKeyState(VK_UP) && y >= 8)     //Up
				y -= 3;
			if (GetAsyncKeyState(VK_DOWN) && y < WIN_HEIGHT - 18) {   //Down
				y += 3;
				if (y > WIN_HEIGHT - 18)y = WIN_HEIGHT - 20;
			}
			if (GetAsyncKeyState(VK_LEFT) && x >= 6)   //Left
				x -= 3;
			if (GetAsyncKeyState(VK_RIGHT) && x < WIN_WIDTH - 32) { //Right
				x += 3;
				if (x > WIN_WIDTH - 32)x = WIN_WIDTH - 36;
			}
		}
	}
	void life_print()
	{
		settextstyle(20, 0, TEXT("黑体"));
		char num[20];
		sprintf_s(num, "%d",life);
	/*	outtextxy(100, 100, num);*/
		outtextxy(30, 30, TEXT("HP"));
		setfillcolor(RED);
		fillrectangle(80, 30, 80 + 10 * life, 50);
	}
};
Player player(260, 740);
void Shoot1(){

	list<Enemy>::iterator pDj = EnemyList.begin();
	list<Bullet>::iterator pZd = player.bulletlist.begin();

	//	Iterate all planes 
	bool flag = false;
	while (pDj != EnemyList.end()) {
		pZd = player.bulletlist.begin();
		//	Iterate all bullets
		while (pZd !=player. bulletlist.end()) {
			//	Detection of Collision
			if ((*pZd).y < 0)
			{
				player.bulletlist.erase(pZd);
				break;
			}
			if ((*pZd).x >= ((*pDj).x - 10) && (*pZd).x <= ((*pDj).x + 50) &&
				(*pZd).y >= ((*pDj).y - 15) && (*pZd).y <= ((*pDj).y + 30)
				) {
				//	If collide, them eliminate both the plane and the bullet
				int t = rand() % 5;
				int style = rand() % 2;
				if (t == 0)giftlist.push_back(Gift((*pZd).x, (*pZd).y, style));
				EnemyList.erase(pDj);
				player.bulletlist.erase(pZd);
				enemy_killed++;
				flag = true;
				break;
			}
			else {
				++pZd;
			}
		}
		if (flag) {
			break;
		}
		++pDj;
	}
}
void Shoot2() {
	list<Bullet>::iterator t = player.bulletlist.begin();
	for (auto& p : player.bulletlist) {
		if (p.x >= (boss.x - 10) && p.x <= (boss.x + 200) &&
			p.y >= (boss.y - 5) && p.y <= (boss.y + 105)
			) {
			boss.life--;
			player.bulletlist.erase(t);
			break;
		}
		t++;
	}
}
void Damage(list<Bullet> &Bulletlist)
{
	list<Bullet>::iterator t = Bulletlist.begin();
	for (auto& p : Bulletlist) {
		if (p.y > 800) {
			Bulletlist.erase(t);
			break;
		}
		if (p.x >= (player.x - 10) && p.x <= (player.x + 50) &&
			p.y >= (player.y - 15) && p.y <= (player.y + 30)
			) {
			//	If collide, them eliminate both the plane and the bullet
			player.life--;
			Bulletlist.erase(t);
			break;
		}
		if(t!=Bulletlist.end())t++;
	}
}
void giftget()
{
	list<Gift>::iterator t = giftlist.begin();
	for (auto& p : giftlist) {
		if (p.x >= (player.x - 10) && p.x <= (player.x + 50) &&
			p.y >= (player.y - 15) && p.y <= (player.y + 30)
			) {
			if(p.style==0)weapon_reinforced_0=true;
			if (p.style == 1)weapon_reinforced_1 = true;
			giftlist.erase(t);
			break;
		}
		if (t !=giftlist.end())t++;
	}
}
void lose_print()
{
	if (lose) {
		settextstyle(40, 0, TEXT("微软雅黑"));
		outtextxy(200, 400, TEXT("LOST"));
	}
}
int main()
{
	srand((unsigned int)time(NULL));
	initgraph(WIN_WIDTH, WIN_HEIGHT, SHOWCONSOLE);
	DWORD t1, t2;			//	敌机速度
	DWORD tt1, tt2;			//	子弹速度
	DWORD T1, T2;
	t1 = GetTickCount();			//	Starting time of the plane
	tt1 = GetTickCount();			//	Starting time of the bullet
	T1 = GetTickCount();
	unsigned int gap = 500;
	while (1)
	{

		//	生成敌机 every 1000ms
		t2 = GetTickCount();
		if (t2 - t1 >= 1000) {
			EnemyList.push_back(Enemy(rand() % (WIN_WIDTH - 50), 0));
			
			t1 = t2;
		}
		T2 = GetTickCount();
		if (T2 - T1 >= 2000)
		{
			for (auto& p : EnemyList) {
				Enemybulletlist.push_back(Bullet(p.x + 20, p.y + 20));
			}
			T1 = T2;
		}
		//	添加弹药 every 500ms
		tt2 = GetTickCount();
		if (weapon_reinforced_1)gap = 250;
		if (tt2 - tt1 >gap) {
			player.bulletlist.push_back(Bullet(player.x + 20, player.y - 20));
			if (weapon_reinforced_0) {
				player.bulletlist.push_back(Bullet(player.x + 10, player.y - 20));
				player.bulletlist.push_back(Bullet(player.x + 30, player.y - 20));
			}
			tt1 = tt2;
		}
		if(!lose)player.mov();
		BeginBatchDraw();		  //Starting batchdraw
		cleardevice();
		player.life_print();
		//画出玩家飞机 (x=260, y=740)
		if(!lose)player.draw();

		

		// 绘制敌机
		for (auto& p : EnemyList) {
			p.mov();
			p.draw();
			p.y++;
		}
		//绘制boss
		if (enemy_killed >= 10&&boss.life>0) {
			boss.mov();
			boss.draw();
			boss.life_print();
			Shoot2();
		}
		if(enemy_killed>=10)boss.shoot();
		//绘制子弹
		for (auto& p : Enemybulletlist) {
			setfillcolor(RED);
			fillcircle(p.x, p.y, 5);
			p.y += 2;
		}
		//绘制奖励
		for (auto& p : giftlist) {
			setfillcolor(YELLOW);
			p.draw();
		}
		giftget();
		if(!lose)
		{
			Shoot1(); // detection of collision
			Damage(Enemybulletlist);
			Damage(Bossbulletlist_0);
			Damage(Bossbulletlist_1);
			Damage(Bossbulletlist_2);
		}
		if (player.life == 0)lose = true;
		lose_print();
		Sleep(5);
		EndBatchDraw(); // end batchdraw

	}

	getchar();

	return 0;
}
