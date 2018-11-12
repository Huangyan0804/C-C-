#include<graphics.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
using namespace std;
#define High 480
#define Width 640

int robot, people;
int robot_win = 0, people_win = 0;
int click = 0;
void choose() {
	cleardevice();
	MOUSEMSG m;
	IMAGE img_st, img_jd, img_bu;
	int img_width = 150, img_high = 160;
	loadimage(&img_st, "st.png");
	loadimage(&img_jd, "jd.png");
	loadimage(&img_bu, "bu.png");
	LOGFONT ft;        //字体
	gettextstyle(&ft);
	ft.lfHeight = 30;
	ft.lfWeight = 200;
	_tcscpy_s(ft.lfFaceName, _T("黑体"));
	ft.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&ft);
	outtextxy(Width / 2 - 50, High / 8, "请选择你要出的拳");
	putimage(Width / 9 * 1 - 50, High / 4, &img_st);
	putimage(Width / 9 * 4 - 50, High / 4, &img_jd);
	putimage(Width / 9 * 7 - 50, High / 4, &img_bu);
	FlushBatchDraw();
	if (MouseHit()) {
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN) {
			click = 1;
			if (m.x >= Width / 9 * 1 - 50 && m.x <= Width / 9 * 1 - 50 + img_width
				&& m.y >= High / 4 && m.y <= High / 4 + img_high)
				people = 0;
			else if (m.x >= Width / 9 * 4 - 50 && m.x <= Width / 9 * 4 - 50 + img_width
				&& m.y >= High / 4 && m.y <= High / 4 + img_high)
				people = 1;
			else if (m.x >= Width / 9 * 7 - 50 && m.x <= Width / 9 * 7 - 50 + img_width
				&& m.y >= High / 4 && m.y <= High / 4 + img_high)
				people = 2;
		}
	}
}

int main() {

	initgraph(Width, High);
	BeginBatchDraw();
	srand(time(0));

	outtextxy(Width / 6, High / 8 , "电脑");
	outtextxy(Width / 6*4, High / 8, "玩家");
	IMAGE img_st, img_jd, img_bu;
	loadimage(&img_st, "st.png");
	loadimage(&img_jd, "jd.png");
	loadimage(&img_bu, "bu.png");
	LOGFONT ft;        //字体
	gettextstyle(&ft);
	ft.lfHeight = 25;
	ft.lfWeight = 200;
	_tcscpy_s(ft.lfFaceName, _T("黑体"));
	ft.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&ft);
	// 石头0 剪刀1 布2
	while (robot_win < 2 && people_win < 2) {
		choose();
		if (click) {
			cleardevice();
			robot = rand() % 3;
			if (robot == 0 && people == 1 || robot == 1 && people == 2 || robot == 2 && people == 0) {
				robot_win++;
				outtextxy(Width / 3 + 30, High / 4 * 3, "你输了！");
			}

			else if (robot == 1 && people == 0 || robot == 2 && people == 1 || robot == 0 && people == 2) {
				people_win++;
				outtextxy(Width / 3 + 30, High / 4 * 3, "你赢了！");
			}
			else
				outtextxy(Width / 3 + 30, High / 4 * 3, "平局");
			/*--------------------------------------------------------*/
			if (robot == 0)
				putimage(Width / 4 - 100, High / 4, &img_st);
			else if (robot == 1)
				putimage(Width / 4 - 100, High / 4, &img_jd);
			else if (robot == 2)
				putimage(Width / 4 - 100, High / 4, &img_bu);

			if (people == 0)
				putimage(Width / 4 * 3 - 100, High / 4, &img_st);
			else if (people == 1)
				putimage(Width / 4 * 3 - 100, High / 4, &img_jd);
			else if (people == 2)
				putimage(Width / 4 * 3 - 100, High / 4, &img_bu);
			FlushBatchDraw();
			Sleep(2000);
		}
		click = 0;
	}
	cleardevice();
	ft.lfHeight = 40;
	settextstyle(&ft);
	if(people_win>=2)
		outtextxy(Width / 6 , High /3 , "恭喜你，你获胜了！");
	else
		outtextxy(Width / 6 , High / 3, "游戏结束，电脑获胜。");
	FlushBatchDraw();
	_getch();
	EndBatchDraw();
	closegraph();
	return 0;
}