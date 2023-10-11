#include<graphics.h>
#include<stdio.h>
#include<conio.h>

int main()
{
	initgraph(1000, 1000);
	setbkcolor(WHITE);
	cleardevice();
	int step = 10;//每圈半径加10
	int initial_color_1 = 115;
	int initial_color_2 = 0;
	int initial_color_3 = 200;
	int initial_color_4 = 0;
	/*第一个圆的初始RGB值（115，20，0），目标RGB（255，20，0）
	  第二个圆的初始RGB值（0，200，0），目标RGB（0，0，80）	*/
	
	int x_1 = 300;
	int x_2 = 700;
	int y_1 = 300;
	int y_2 = 300;
	//（300，300）（700，300）
	for (int r=5 ,i=0;i<20;r+=step,i++)
	{
		setlinecolor(RGB(initial_color_1,20,initial_color_2));//设置第一个渐变圆的边框色
		circle(x_1, y_1, r);
		setlinecolor(RGB(0,initial_color_3,initial_color_4));//设置第二个渐变圆的边框色
		circle(x_2, y_2, r);
		initial_color_1 += 7;
		initial_color_3 -= 10;
		initial_color_4 +=4;
		//修改两个圆的RGB值
		Sleep(100);
	}
	_getch();
	closegraph();
	return 0;

}
