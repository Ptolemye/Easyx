#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include<math.h>
#include<string.h>
#include <stdlib.h>
#include <time.h>
#include<vector>
#include<math.h>
#define WIDTH	30
#define	HEIGHT	20
bool start = false;
int Box[9][17] = { 0 };
int GiftMap[9][17] = { 0 };
bool win = false;
bool Key_get = false;
bool Bomb_up = false;
bool Speed_up = false;
class Gift
{
public:
	int row;
	int col;
	int style;
	bool is_got = false;
	void draw()
	{
		int x = col * 50 + 25;
		int y = row * 50 + 25;
		if (style == 1)//钥匙
		{
			settextstyle(20, 0, TEXT("微软雅黑"));
			outtextxy(x - 10, y-10, TEXT("KEY"));
		}
		if (style == 2)//门
		{
			settextstyle(20, 0, TEXT("微软雅黑"));
			outtextxy(x-18, y-10, TEXT("DOOR"));
		}
		if (style == 3)//超级炸弹
		{
			settextstyle(20, 0, TEXT("微软雅黑"));
			outtextxy(x-20, y-20, TEXT("BOMB"));
			outtextxy(x - 10, y, TEXT("UP"));
		}
		if (style == 4)//移速增加
		{
			settextstyle(20, 0, TEXT("微软雅黑"));
			outtextxy(x-20, y-20, TEXT("SPEED"));
			outtextxy(x - 10, y, TEXT("UP"));
		}
	}
};
Gift gift[5];
class Map
{
public:
	void setstones() {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 8; j++)
			{
				Box[2 * i + 1][2 * j + 1] = 1;//固定砖块
			}

	}
	void setbricks()
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 17; j++)
			{
				int t = rand() % 4;
				if (t == 0)Box[i][j] = 2;
			}
		}//可破坏砖块
	}
	void draw() {
		setbkcolor(RGB(0, 100, 0));
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 17; j++)
			{
				if (Box[i][j] == 1) {
					setfillcolor(RGB(128, 128, 128));
					setlinecolor(RGB(180, 180, 180));
					fillrectangle(50 * j, 50 * i, 50 * j + 50, 50 * i + 50);
				}
				if (Box[i][j] == 2) {
					setfillcolor(RGB(0, 0, 0));
					setlinecolor(RGB(0, 0, 0));
					fillrectangle(50 * j + 5, 50 * i + 5, 50 * j + 45, 50 * i + 45);
				}
			}
	}
};
Map map;
class Enemy
{
public:
		int x;
		int y;
		int direct=rand()%4;
		bool direct_change = true;//0为上，1为下，2为左，3为右
		int row_now;
		int col_now;
		int row;
		int col;
		int stepcount = 0;
		void draw()
		{
			setlinecolor(BLACK);
			setfillcolor(BLACK);
			/*fillcircle(x, y, 10);
			line(x - 25, y - 25, x + 25, y + 25);
			line(x - 25, y + 25, x + 25, y - 25);
			line(x - 25, y, x + 25, y);
			line(x, y - 25, x, y + 25);*/
			POINT pts1[] = { {x - 25, y - 25}, {x + 25, y - 25}, {x, y+25} };
			polygon(pts1, 3);
			POINT pts2[] = { {x - 25, y + 25}, {x + 25, y + 25}, {x, y - 25} };
			polygon(pts2, 3);
			fillcircle(x, y, 5);
		}
		Enemy();
		Enemy(int row,int col)
		{
			x = col * 50 + 25;
			y = row * 50 + 25;
			row_now = y / 50;
			col_now = x / 50;
		}
		void mov()
		{
			if (direct == 0 && Box[row_now - 1][col_now] != 1 && Box[row_now - 1][col_now] != 2)
			{
				y -= 5;
				stepcount++;
			}
			else if (direct == 1&& Box[row_now + 1][col_now] != 1 && Box[row_now + 1][col_now] != 2)
			{
				y += 5;
				stepcount++;
			}
			else if (direct == 2 && Box[row_now ][col_now-1] != 1 && Box[row_now][col_now-1] != 2)
			{
				x -= 5;
				stepcount++;
			}
			else if (direct == 3 && Box[row_now][col_now+1] != 1 && Box[row_now][col_now+1] != 2)
			{
				x += 5;
				stepcount++;
			}
			else direct = rand() % 4;
			if (stepcount==10) {
				direct = rand() % 4;
				row_now = y / 50;
				col_now = x / 50;
				stepcount = 0;
			}
			if (x <= 25)x = 25;
			if (x >= 825)x = 825;
			if (y <= 25)y = 25;
			if (y >= 425)y = 425;
			col = x / 50;
			row = y / 50;
		}
};
std::vector<Enemy> enemies;
class Bomb {
public:
	int row;
	int col;
	int time = 0;
	int exp_time = 0;
	bool damage_check = false;
	int mod=3;//1为横向爆炸，2为纵向爆炸，3为十字爆
	int scale=2;
	Bomb();
	Bomb(int a, int b)
	{
		row = a;
		col = b;
	}
	void draw()
	{
		//爆炸前
		if (time < 40)
		{
			setfillcolor(WHITE);
			if (Bomb_up)setfillcolor(RED);
			settextstyle(16, 0, _T("微软雅黑"));
			setbkmode(OPAQUE);
			fillrectangle(col * 50 + 10, row * 50 + 10, col * 50 + 40, row * 50 + 40);
			outtextxy(col * 50 + 10, row * 50 + 10, TEXT("TNT"));
			time++;
		}
		//爆炸后
		if (time == 40&&exp_time<10) {
			mod = 3;
			scale = 2;
			if (Bomb_up)scale = 20;
			setlinecolor(YELLOW);
			setfillcolor(YELLOW);
			exp_time++;
			fillrectangle(col * 50 + 10, row * 50 + 10, col * 50 + 40, row * 50 + 40);
			for (int i = row+1; i <= min(row + scale,8); i++)
			{
				if (Box[i][col] != 1) {
					fillrectangle(col * 50+10, i * 50, col * 50 + 40, i * 50 + 50);
				}
				else {
					mod = 1;
					break;
				}
			}
			for (int i = row-1; i >= max(row - scale,0); i--)
			{
				if (Box[i][col] != 1) {
					fillrectangle(col * 50+10, i * 50, col * 50 + 40, i * 50 + 50);
				}
				else {
					mod = 1;
					break;
				}
			}
			for (int i = col+1; i <=min(col + scale,16); i++)
			{
				if (Box[row][i] != 1) {
					fillrectangle(i * 50, row * 50 + 10, i * 50 + 50, row * 50 + 40);
				}
				else {
					mod = 2;
					break;
				}
			}
			for (int i = col-1; i >=max(col - scale,0); i--)
			{
				if (Box[row][i] != 1) {
					fillrectangle(i * 50, row * 50 + 10, i * 50 + 50, row * 50 + 40);
				}
				else {
					mod = 2;
					break;
				}
			}
			if (time==40) {
				for (int i =max(col - scale,0); i <=min(col + scale,16); i++)
				{
					if (Box[row][i] == 2 && (mod == 1 || mod == 3))Box[row][i] = 0;
					for (int j = 0; j < enemies.size(); j++)
					{
						if (enemies[j].col == i && enemies[j].row == row&&(mod==1||mod==3))
						{
							auto iter = enemies.erase(enemies.begin() + j);
						}
					}
				}
				for (int i = max(row - scale,0); i <=min(row + scale,8); i++)
				{
					if (Box[i][col] == 2 && (mod == 2 || mod == 3))Box[i][col] = 0;
					for (int j = 0; j < enemies.size(); j++)
					{
						if (enemies[j].col == col && enemies[j].row == i && (mod == 2 || mod == 3))
						{
							auto iter = enemies.erase(enemies.begin() + j);
						}
					}
				}
			}
			return;
		}
	}
};
std::vector<Bomb> bombs;
class Player
{
public:
	int x;
	int y;
	int row;
	int col;
	bool alive = true;
	void set_player()
	{
		int i, j;
		while (1)
		{
			i = rand() % 9;
			j = rand() % 17;
			if (Box[i][j] == 0)break;
		}
		Box[i][j] = 3;
		row = i;
		col = j;
		x = 25 + 50 * j;
		y = 25 + 50 * i;
	}
	void draw()
	{
		setfillcolor(RED);
		fillcircle(x, y, 20);
	}
	void mov()
	{
		row = y / 50;
		col = x / 50;
		if (_kbhit())
		{
			char input = 0;
			int speed = 5;
			if (Speed_up)speed = 8;
			if (y%50!=0) {
				if (GetAsyncKeyState(VK_LEFT))
				{
					if (x - 25 <= 50 * col && (Box[row][col - 1] == 1 || Box[row][col - 1] == 2))
						x += 0;
					else if(y % 25 == 0) x -= speed;
					if (abs(y - (row * 50 + 25)) <= 15)y = row * 50 + 25;
				}
				if (GetAsyncKeyState(VK_RIGHT))
				{
					if (x + 25 >= 50 * (col + 1) && (Box[row][col + 1] == 1 || Box[row][col + 1] == 2))
						x += 0;
					else if(y % 25 == 0) x += speed;
					if (abs(y - (row * 50 + 25)) <= 15)y = row * 50 + 25;
				}
			}
			if (x % 50 != 0) {
				if (GetAsyncKeyState(VK_UP))
				{
					if (y - 25 <= 50 * row && (Box[row - 1][col] == 1 || Box[row - 1][col] == 2))
						y -= 0;
					else if(x % 25 == 0) y -= speed;
					if (abs(x - (col * 50 + 25)) <= 15)x = col * 50 + 25;
				}
				if (GetAsyncKeyState(VK_DOWN))
				{
					if (y + 25 >= 50 * (row + 1) && (Box[row + 1][col] == 1 || Box[row + 1][col] == 2))
						y += 0;
					else if(x % 25 == 0)y += speed;
					if (abs(x - (col * 50 + 25)) <= 15)x = col * 50 + 25;
				}
			}
			if (GetAsyncKeyState(VK_SPACE))
			{
				Bomb bomb(row, col);
				bombs.push_back(bomb);
			}
			
		}
		if (x <= 25)x = 25;
		if (x >= 825)x = 825;
		if (y <= 25)y = 25;
		if (y >= 425)y = 425;
	}
};
Player player;

void initialize()
{
	int num = 0;
	player.set_player(); 
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 17; j++) {
			int t = rand() % 15;
			if (t == 0 && Box[i][j] == 0&&abs(i-player.row)>=2&& abs(j- player.col) >= 2&&num<5) {
				num++;
				Enemy t(i, j);
				enemies.push_back(t);
				Box[i][j] = 4;
			}
			if (i == 8 && j == 16 && num < 5)
			{
				i = 0;
				j = 0;
			}
		}
	start = true;
	for(int i=player.row-1;i<=player.row+1;i++)
		for (int j = player.col - 1; j <= player.col + 1; j++) {
			if (Box[i][j] == 2)Box[i][j] = 0;
		}
	for (int i = 1; i < 5; i++)
	{
		gift[i].style = i;
		while (1) {
			int row = rand() % 9;
			int col = rand() % 17;
			if (Box[row][col] == 2 && GiftMap[row][col] == 0)
			{
				GiftMap[row][col] = i;
				gift[i].row = row;
				gift[i].col = col;
				break;
			}
		}
	}
} 
void flush()
{
	
}
void lose_print()
{
	if (!player.alive)
	{
		setbkmode(TRANSPARENT);
		settextstyle(50, 0, _T("微软雅黑"));
		outtextxy(400, 200, TEXT("LOSE"));
	}
}
void win_print()
{
	if (win&&player.alive)
	{
		setbkmode(TRANSPARENT);
		settextstyle(50, 0, _T("微软雅黑"));
		outtextxy(400, 200, TEXT("WIN"));
	}
}
int main()
{
	initgraph(850, 450);
	srand((unsigned int)time(NULL));
	map.setbricks();
	map.setstones();
	BeginBatchDraw();
	while (1)
	{
		if (!start)initialize();
		map.draw();
		if(player.alive&&!win)
		{
			player.mov();
			player.draw();
		}
		for (int i = 0; i < bombs.size(); i++)
		{
			bombs[i].draw();
			int col = bombs[i].col;
			int row = bombs[i].row;
			int mod = bombs[i].mod;
			if (bombs[i].time==40&&bombs[i].exp_time<10) {
				for (int j = col - bombs[i].scale; j <=col + bombs[i].scale; j++)
				{
					if (player.row == row && player.col == j && (mod == 1 ||mod == 3))player.alive = false;
				}
				for (int j = row - bombs[i].scale; j <=row + bombs[i].scale; j++)
				{
					if (player.row == j && player.col == col&&(mod == 2 || mod == 3))player.alive = false;
				}
			}
		}
		for (int i = 0; i < enemies.size(); i++)
		{
			enemies[i].draw();
			if(!win&& player.alive)enemies[i].mov();
			if (abs(enemies[i].x - player.x) < 50 && abs(enemies[i].y - player.y) < 50)
				player.alive = false;
		}
		for (int i = 1; i < 5; i++)
		{
			int row = gift[i].row;
			int col = gift[i].col;
			if (player.row == gift[i].row && player.col == gift[i].col&&i!=2) {
				gift[i].is_got = true;
				if (i == 1)Key_get = true;
				if (i == 3)Bomb_up = true;
				if (i == 4) Speed_up = true;
			}
			if (Box[row][col] == 0&&!gift[i].is_got) {
				gift[i].draw();
			}
		}
		if (enemies.size() == 0)win = true;
		if (Key_get && player.row == gift[2].row && player.col == gift[2].col)win = true;
		win_print();
		lose_print();
		FlushBatchDraw();
		Sleep(30);
		cleardevice();                                                                                                                                                                                      
	}
}