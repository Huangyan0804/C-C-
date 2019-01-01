#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<Windows.h>
#include<time.h>
#include<stdlib.h>
#include<tchar.h>
#include<graphics.h>
#include<vector>
#include<tchar.h>
using namespace std;

#define High 750
#define Width 500 // 画布大小

/*------------------------结构体---------------------------*/
struct Bullet {                                               //
	double x, y;                                              // 子弹位置              
	double speed;                                             // 子弹速度
	int high, width;                                          // 子弹图片的大小
	Bullet(double i, double j, double spee = 0.9,             //
		int hig = 21, int widt = 9) {                         //
		x = i; y = j; speed = spee;                           //
		high = hig; width = widt;                             //
	}                                                         //
};                                                            // 子弹结构体
															  //
struct Plane {                                                //
	double x, y;                                              // 飞机位置
	int health;                                               // 血量
	int weapon;                                               // 武器种类
	int damage;                                               // 伤害
	int firespeed;
	int high, width;                                          // 图片大小
	Plane(double i = Width / 2, double j = High,              // 
		int healt = 100, int weapo = 0,                       // 
		int damag = 50, int firespee = 500,
		int hig = 62, int widt = 50)
	{                                                         //
		x = i; y = j; health = healt;                         //
		weapon = weapo; damage = damag;
		firespeed = firespee;  high = high; width = widt;     //
	}                                                         //
};                                                            // 创建飞机结构体
struct Enemy {                                                // 
	double x, y;                                              // 敌机位置
	int health;                                               // 血量
	int damage;                                               // 伤害
	int type;                                                 // 敌机种类
	double speed;                                             // 移动速度
	int high, width;                                          // 图片大小
	bool show_health; // 是否显示血量
	Enemy(double i, double j, int typ, int hig, int widt,     //
		double spee, int healt, int damag, bool show)    //
	{                                                         //
		x = i; y = j; health = healt; type = typ;             //
		damage = damag;  high = hig; width = widt;            //
		speed = spee; show_health = show;                     //
	}                                                         //
};                                                            //
/*--------------------------------------------------------*/

/*------------------自定义函数------------------*/
void start();                                  // 初始化
void hello();                                  // 欢迎界面
void bye();                                    // 结束界面
void show();                                   // 显示主画面
void show_bullet();                            // 显示子弹
void show_enemy();                             // 显示敌机
void show_plane();                             // 显示玩家飞机
void manuallcontrol();                         // 鼠标控制
void automaticcontrol();                       // 自动更新动画
void create_enemy();                           // 创造敌机
void enemy_move();                             // 敌机移动
void show_health();                            // 显示生命
void show_score();                             // 显示分数
void auto_fire();                              // 自动射击并自动销毁子弹
void is_shot();                                // 是否射中
void is_live();                                // 是否死亡
int rnd(int n, double a, double b, double c);  // 产生3种控制概率的随机数
/*---------------------------------------------*/

/*------------------全局变量------------------*/
double width, high;                         // W->X      H->Y
int score;                                  // 分数
bool live = true;                           // 游戏结束开始
int fire_cnt;                               // 开火速度
IMAGE img_bk;                               // 背景图片
IMAGE img_start1, img_start2;
IMAGE img_close1, img_close2;
IMAGE img_plane1, img_plane2;               // 飞机图片
IMAGE img_bullet1, img_bullet2;             // 子弹图片
IMAGE img_enemy1, img_enemy2;               // 敌机1图片
IMAGE img_enemy3, img_enemy4;               // 敌机2图片
vector<Enemy> enemy;                        // 敌机序列
vector<Bullet> bullet;                      // 子弹序列
Plane plane;                                // 玩家控制飞机
/*----------------------------------------*/

int main()
{
	start();
	hello();
	while (live)
	{
		manuallcontrol();
		show();
		automaticcontrol();
	}
	if (!live) {
		bye();
	}
	closegraph(); // 关闭绘图环境
	return 0;
}

void start()
{
	initgraph(Width, High);                     //绘图环境初始化
/*------------------载入图片--------------------*/
	loadimage(&img_bk, _T("background.png"));   // 背景
	loadimage(&img_start1, _T("start1.jpg"));
	loadimage(&img_start2, _T("start2.jpg"));
	loadimage(&img_close1, _T("close1.jpg"));
	loadimage(&img_close2, _T("close2.jpg"));
	loadimage(&img_plane1, _T("plane1.jpg"));   // 飞机
	loadimage(&img_plane2, _T("plane2.jpg"));   // 飞机遮罩
	loadimage(&img_bullet1, _T("bullet1.jpg")); // 子弹
	loadimage(&img_bullet2, _T("bullet2.jpg")); // 子弹遮罩
	loadimage(&img_enemy1, _T("enemy1.jpg"));   // 敌机1
	loadimage(&img_enemy2, _T("enemy2.jpg"));   // 敌机1遮罩
	loadimage(&img_enemy3, _T("enemy3.jpg"));   // 敌机2
	loadimage(&img_enemy4, _T("enemy4.jpg"));   // 敌机2遮罩
/*---------------------------------------------*/

	score = 0;                                  // 分数初始化0
	srand(time(0));
	fire_cnt = 0;
	width = Width; high = High;                 // 初始化循环大小


	BeginBatchDraw();
}

void show() {

	putimage(0, 0, &img_bk);
	show_bullet();
	show_plane();
	show_enemy();
	show_health();
	show_score();
	FlushBatchDraw();
}

void show_plane() {
	putimage(plane.x, plane.y, &img_plane2, NOTSRCERASE); //
	putimage(plane.x, plane.y, &img_plane1, SRCINVERT); //显示飞机, 减去的数字保证坐标在飞机中间
}

void show_bullet() {
	for (auto i = bullet.begin(); i != bullet.end(); i++) {//显示子弹
		putimage(i->x, i->y, &img_bullet2, NOTSRCERASE);
		putimage(i->x, i->y, &img_bullet1, SRCINVERT);
	}
}

void show_enemy() {
	for (auto i = enemy.begin(); i != enemy.end(); i++) {//显示敌机
		if (i->type == 0) {
			putimage(i->x, i->y, &img_enemy2, NOTSRCERASE);
			putimage(i->x, i->y, &img_enemy1, SRCINVERT);
		}
		else if (i->type == 1) {
			putimage(i->x, i->y, &img_enemy4, NOTSRCERASE);
			putimage(i->x, i->y, &img_enemy3, SRCINVERT);
		}
		if (i->show_health) {
			setlinecolor(BLACK);
			int left = i->x + i->width / 6 - 5;
			int top = i->y - 8;
			rectangle(left - 1, top, left + i->width / 6 * 5 + 1, top + 5);
			setfillcolor(RED);
			solidrectangle(left, top, left + (i->health) / 6 * 5, top + 5);
		}
	}
}

void show_health() {
	setlinecolor(BLACK);
	rectangle(10, 299, 25, 501); // 边框
	setfillcolor(RED);
	plane.health = plane.health < 0 ? 0 : plane.health; // 保证血条最小为 0
	solidrectangle(11, 500 - plane.health * 2, 24, 500); // 血条
	TCHAR text[20];
	_stprintf_s(text, _T("%d"), plane.health);
	outtextxy(10, 510, text);
}

void show_score() {
	TCHAR info[100];
	_stprintf_s(info, _T("分数：%d"), score);
	outtextxy(0, 0, info);
}

void is_live() {
	if (plane.health <= 0) {
		live = false;
	}
}

void manuallcontrol()
{
	MOUSEMSG mouse; // 鼠标消息
	while (MouseHit()) {
		mouse = GetMouseMsg();
		if (mouse.uMsg == WM_MOUSEMOVE) { // 将鼠标坐标传给飞机
			plane.x = mouse.x - plane.width / 2;
			plane.y = mouse.y;
		}

	}

}

void automaticcontrol()
{
	is_shot();
	//is_live();
	auto_fire();
	enemy_move();
	if (enemy.size() < 3) {
		create_enemy();
	}
}

void auto_fire() { // 自动射击并自动销毁子弹
	fire_cnt++;
	if (fire_cnt % plane.firespeed == 0) {
		if (plane.weapon == 0) {
			Bullet new_bullet(plane.x + plane.width / 2 - 4, plane.y);
			bullet.push_back(new_bullet);
		}
		fire_cnt %= plane.firespeed;
	}

	for (int i = 0; i < bullet.size(); i++) {
		(bullet.begin() + i)->y -= (bullet.begin() + i)->speed;
		if ((bullet.begin() + i)->y <= -(bullet.begin() + i)->high) {
			bullet.begin() + i = bullet.erase(bullet.begin() + i);
			i--;
		}
	}
}

void create_enemy() { // 制造敌机并自动下落

	int type = rnd(1000, 0.6, 0.3, 0.1); // 生成种类
	/* 0 -> 单个生成  1 -> 斜着快速出现  2 -> 三角队列 */

	if (type == 0) { // 单个生成
		int enemy_type = rnd(1000, 0.7, 0.3, 0);
		double x = rand() % ((int)width - 50); // 随机敌机的坐标 并 防止敌机出生在屏幕外面
		if (enemy_type == 0) {
			double speed = (rnd(1000, 0.9, 0.1, 0) + 1) * 0.2; // 随机敌机速度
			Enemy new_enemy(x, 0, 0, 39, 51, speed, 50, 10, false); // 0号敌机的位置和大小
			enemy.push_back(new_enemy);
		}
		if (enemy_type == 1) {
			Enemy new_enemy(x, 0, 1, 89, 69, 0.1, 75, 20, false); // 1号敌机的位置和大小
			enemy.push_back(new_enemy);
		}
	}
	else if (type == 1) {

	}



}
void enemy_move() {
	for (int i = 0; i < enemy.size(); i++) {
		(enemy.begin() + i)->y += (enemy.begin() + i)->speed;
		if ((enemy.begin() + i)->y >= high) { // 超出屏幕外自动销毁
			enemy.begin() + i = enemy.erase(enemy.begin() + i);
			i--;
		}
	}
}
void is_shot() { // 是否击中
	for (int i = 0; i < enemy.size();) {
		auto eit = enemy.begin() + i;
		int j;
		for (j = 0; j < bullet.size(); ) {
			auto bit = bullet.begin() + j;
			if (bit->x + bit->width >= (eit)->x
				&& bit->x <= eit->x + eit->width
				&& bit->y >= eit->y - eit->high
				&& bit->y - bit->high <= eit->y) // 子弹击中敌机
			{
				bullet.erase(bullet.begin() + j);
				(enemy.begin() + i)->health -= plane.damage;
				if ((enemy.begin() + i)->health <= 0) { //如果敌机生命小于0则摧毁
					score += eit->damage;
					enemy.erase(enemy.begin() + i);
				}
				else { // 否则显示血量
					eit->show_health = true;
				}
			}
			else {
				j++;
			}
		}
		if (j == bullet.size())
			i++;

	}
	for (int i = 0; i < enemy.size();) { // 是否碰撞
		auto eit = enemy.begin() + i;
		if (eit->x <= plane.x + plane.width && eit->x + eit->width >= plane.x
			&& eit->y <= plane.y + plane.high && eit->y + eit->high >= plane.y) {
			plane.health -= eit->damage;
			enemy.erase(enemy.begin() + i);
		}
		else {
			i++;
		}
	}
}

int rnd(int n, double a, double b, double c) {
	int t = rand() % n;
	int res;
	if (t < n * a) { //控制几率
		res = 0;
	}
	else if (t < n * (a + b)) {
		res = 1;
	}
	else if (t < n * (a + b + c)) {
		res = 2;
	}
	return res;
}

void hello() {

	MOUSEMSG mouse; // 鼠标消息
	while (true) {
		putimage(0, 0, &img_bk);
		putimage(width / 2 - 75, high / 2 - 50, &img_start2, NOTSRCERASE);
		putimage(width / 2 - 75, high / 2 - 50, &img_start1, SRCINVERT);
		putimage(width / 2 - 75, high / 2 + 50, &img_close2, NOTSRCERASE);
		putimage(width / 2 - 75, high / 2 + 50, &img_close1, SRCINVERT);
		FlushBatchDraw();
		mouse = GetMouseMsg();
		if (mouse.mkLButton && mouse.x >= width / 2 - 75 && mouse.y >= high / 2 - 50
			&& mouse.x <= width / 2 + 75 && mouse.y <= high / 2 + 50)
			break;
		else if (mouse.mkLButton && mouse.x >= width / 2 - 75 && mouse.y >= high / 2 + 50
			&& mouse.x <= width / 2 + 75 && mouse.y <= high / 2 + 150)
		{
			closegraph();
			exit(0);
		}
	}
}

void bye() {

}
