#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<Windows.h>
#include<time.h>
#include<stdlib.h>
#include<tchar.h>
#include<algorithm>
#include<graphics.h>
#include<vector>
#include<math.h>
#include<tchar.h>
#include<string>
using namespace std;
#pragma comment(lib,"msimg32.lib")
#pragma comment(lib,"winmm.lib")
#define High 750
#define Width 500                                                     // 画布大小
#define PI 3.1415926

/*----------------------------定义结构体-------------------------------*/
struct Bullet {                                                       // 子弹结构体
	double x, y;                                                      // 子弹位置              
	int type;                                                         // 子弹种类
	double speed;                                                     // 子弹速度
	int high, width;                                                  // 子弹图片的大小
	Bullet(double x1, double y1, int type1,                           //
		int high1, int width1, double speed1 = 0.9) {                 //
		x = x1; y = y1; type = type1; speed = speed1;                 //
		high = high1; width = width1;                                 //
	}                                                                 //
};                                                                    //
																	  //
struct Enemy_Bullet {                                                 //敌人子弹结构体
	double x, y, r;                                                   //敌人子弹位置，大小
	double speed;                                                     // 子弹速度
	double dx, dy;                                                    //
	Enemy_Bullet(double x1, double y1, double r1, double speed1       //
		, double dx1 = 0, double dy1 = 1) {                           //
		x = x1; y = y1; r = r1; speed = speed1;                       //
		dx = dx1; dy = dy1;
	}                                          //
};                                                                    //
																	  //
struct Plane {                                                        //飞机结构体
	double x, y;                                                      // 飞机位置
	int health;                                                       // 血量
	int weapon;                                                       // 武器种类
	int damage;                                                       // 伤害
	int firespeed;                                                    // 开火间隔
	int high, width;                                                  // 图片大小
	Plane(double x1 = Width / 2, double y1 = High,                    // 
		int health1 = 100, int weapon1 = 0,                           // 
		int damage1 = 50, int firespeed1 = 450,                       //
		int high1 = 57, int width1 = 45)                              //
	{                                                                 //
		x = x1; y = y1; health = health1;                             //
		weapon = weapon1; damage = damage1;                           //
		firespeed = firespeed1;  high = high; width = width1;         //
	}                                                                 //
};                                                                    // 
																	  //
struct Enemy {                                                        // 敌机结构体
	double x, y;                                                      // 敌机位置
	int max_health, health;                                           // 血量
	int damage;                                                       // 伤害
	int type;                                                         // 敌机种类
	double speed;                                                     // 移动速度
	int high, width;                                                  // 图片大小
	bool show_health;                                                 // 是否显示血量
	Enemy(double x1, double y1, int type1, int high1, int width1,     //
		double speed1, int max_health1, int health1, int damage1,     //
		bool show_health1)                                            //
	{                                                                 //
		x = x1; y = y1; max_health = max_health1; type = type1;       //
		damage = damage1;  high = high1; width = width1;              //
		speed = speed1; show_health = show_health1; health = health1; //
	}                                                                 //
};                                                                    //
																	  //
struct Boom_Enemy {                                                   // 爆炸动画结构体
	double x, y;                                                      // 爆炸动画位置
	int type;                                                         // 种类
	double time;                                                      // 第几帧
	Boom_Enemy(int x1, int y1, int type1, double time1) {             //
		x = x1; y = y1; type = type1; time = time1;
	}                  //
};                                                                    //
																	  //
struct Reward {                                                       // 奖励道具结构体
	double x, y;                                                      // 位置
	int dx, dy;                                                       // 方向
	double speed;                                                     // 移动速度
	int type;                                                         // 种类
	int r;                                                            // 大小
	Reward(double x1, double y1, int dx1, int dy1,                    //
		double speed1, int type1, int r1 = 40) {                      //
		x = x1; y = y1; dx = dx1; dy = dy1;                           //
		speed = speed1; type = type1; r = r1;
	}                        //
};                                                                    //
/*-------------------------------------------------------------------*/

/*------------------自定义函数------------------*/
void start();                                  // 初始化
void load_image();                             // 载入图片
void hello();                                  // 欢迎界面
bool bye();                                    // 结束界面
void show();                                   // 显示主画面
void show_bullet();                            // 显示子弹
void show_enemy_bullet();                      // 显示敌人子弹
void show_reward();                            // 显示奖励
void show_enemy();                             // 显示敌机
void show_enemy_boom();                        // 显示敌机爆炸动画
void show_plane();                             // 显示玩家飞机
void show_health();                            // 显示生命
void show_score();                             // 显示分数
void manuallcontrol();                         // 鼠标控制
void automaticcontrol();                       // 自动更新动画
void is_shot();                                // 是否射中
void is_live();                                // 是否死亡
void auto_fire();                              // 自动射击并自动销毁子弹
void eat_reward();                             // 吃奖励
void reward_move();                            // 奖励移动
void enemy_fire();                             // 敌人开火
void enemy_move();                             // 敌机移动
void change_difficulty();                      // 随着时间改变难度
void create_enemy();                           // 创造敌机
void enemy_boom(Enemy boom_enemy, int n);      // 敌机爆炸后的事件
void create_reward(Enemy e);                   // 击杀奖励
void plane_hurt(int damage);                   // 飞机受伤
int rnd(int n, double a, double b, double c);  // 产生3种控制概率的随机数
/*---------------------------------------------*/

/*----------------------------------全局变量-----------------------------------*/
double width, high;                                                           // W->X      H->Y
int score, difficulty;                                                        // 分数
bool live;                                                                    // 游戏结束开始
int fire_cnt;                                                                 // 开火速度
int sum_hurt;                                                                 //承受伤害
IMAGE img_bk;                                                                 // 背景图片
IMAGE img_title1, img_title2;                                                 // 标题图片
IMAGE img_start1, img_start2;                                                 // 开始按钮
IMAGE img_close1, img_close2;                                                 // 关闭按钮
IMAGE img_plane1, img_plane2;                                                 // 飞机图片
IMAGE img_bullet1, img_bullet2, img_bullet3, img_bullet4;                     // 子弹图片
IMAGE img_enemy1, img_enemy2;                                                 // 敌机1图片
IMAGE img_enemy3, img_enemy4;                                                 // 敌机2图片
IMAGE img_enemy5, img_enemy6;                                                 // 敌机3图片
IMAGE img_enemy1_boom1, img_enemy1_boom2, img_enemy1_boom3, img_enemy1_boom4; // 敌机1爆炸动画
IMAGE img_enemy2_boom1, img_enemy2_boom2, img_enemy2_boom3, img_enemy2_boom4; // 敌机1爆炸动画遮罩
IMAGE img_enemy3_boom1, img_enemy3_boom2, img_enemy3_boom3, img_enemy3_boom4; // 敌机2爆炸动画
IMAGE img_enemy4_boom1, img_enemy4_boom2, img_enemy4_boom3, img_enemy4_boom4; // 敌机2爆炸动画遮罩
IMAGE img_enemy5_boom1, img_enemy5_boom2, img_enemy5_boom3, img_enemy5_boom4; // 敌机3爆炸动画
IMAGE img_enemy6_boom1, img_enemy6_boom2, img_enemy6_boom3, img_enemy6_boom4; // 敌机3爆炸动画遮罩
IMAGE img_heal1, img_heal2, img_weapon1, img_weapon2, img_speed1, img_speed2; // 奖励道具图片
vector<Enemy> enemy;                                                          // 敌机序列
vector<Bullet> bullet;                                                        // 子弹序列
vector<Enemy_Bullet> enemy_bullet;                                            // 敌机序列
vector<Boom_Enemy> boom_enemy;                                                // 敌机爆炸序列
vector<Reward> reward;                                                        // 奖励道具序列
Plane plane;                                                                  // 玩家控制飞机
/*----------------------------------------------------------------------------*/

int main()
{
game_start:
	start();
	hello(); // 开始界面
	while (live)
	{
		show();
		manuallcontrol();
		automaticcontrol();
	}
	if (!live) {
		if (bye()) {
			live = true;
			goto game_start;
		}
	}
	closegraph(); // 关闭绘图环境
	return 0;
}

void start()
{
	mciSendString(TEXT("open picture\\game_music.mp3 alias bgm"), NULL, 0, NULL);
	mciSendString(TEXT("play bgm repeat"), NULL, 0, NULL);
	initgraph(Width, High);                     //绘图环境初始化
	load_image();                               // 载入图片
	score = 0;                                  // 分数初始化0
	live = true;                                // 或者
	plane.health = 100, plane.weapon = 0;       // 初始化飞机数据，方便重开
	plane.damage = 50, plane.firespeed = 450;   //
	difficulty = 2;                             // 最低2，最高10
	srand(unsigned(time(0)));                   // 初始化随机种子
	fire_cnt = 0;                               // 控制开火速度
	sum_hurt = 0;                               // 承受伤害
	width = Width; high = High;                 // 初始化循环大小
	BeginBatchDraw();
}

void load_image() {
	/*--------------------------载入图片-----------------------------*/
	loadimage(&img_bk, _T("picture\\background.png"));               // 背景
	loadimage(&img_title1, _T("picture\\title1.jpg"));               // 标题
	loadimage(&img_title2, _T("picture\\title2.jpg"));               //
	loadimage(&img_start1, _T("picture\\start1.jpg"));               // 开始按钮
	loadimage(&img_start2, _T("picture\\start2.jpg"));               //
	loadimage(&img_close1, _T("picture\\close1.jpg"));               // 结束按钮
	loadimage(&img_close2, _T("picture\\close2.jpg"));               //
	loadimage(&img_plane1, _T("picture\\plane1.jpg"));               // 飞机
	loadimage(&img_plane2, _T("picture\\plane2.jpg"));               // 飞机遮罩
	loadimage(&img_bullet1, _T("picture\\bullet1.jpg"));             // 子弹1
	loadimage(&img_bullet2, _T("picture\\bullet2.jpg"));             // 子弹1遮罩
	loadimage(&img_bullet3, _T("picture\\bullet3.jpg"));             // 子弹2
	loadimage(&img_bullet4, _T("picture\\bullet4.jpg"));             // 子弹2遮罩
	loadimage(&img_enemy1, _T("picture\\enemy1.jpg"));               // 敌机1
	loadimage(&img_enemy2, _T("picture\\enemy2.jpg"));               // 敌机1遮罩
	loadimage(&img_enemy1_boom1, _T("picture\\enemy1_boom1.jpg"));   // 敌机1爆炸1
	loadimage(&img_enemy2_boom1, _T("picture\\enemy2_boom1.jpg"));   // 敌机1爆炸1遮罩
	loadimage(&img_enemy1_boom2, _T("picture\\enemy1_boom2.jpg"));   // 敌机1爆炸2
	loadimage(&img_enemy2_boom2, _T("picture\\enemy2_boom2.jpg"));   // 敌机1爆炸2遮罩
	loadimage(&img_enemy1_boom3, _T("picture\\enemy1_boom3.jpg"));   // 敌机1爆炸3
	loadimage(&img_enemy2_boom3, _T("picture\\enemy2_boom3.jpg"));   // 敌机1爆炸3遮罩
	loadimage(&img_enemy1_boom4, _T("picture\\enemy1_boom4.jpg"));   // 敌机1爆炸4
	loadimage(&img_enemy2_boom4, _T("picture\\enemy2_boom4.jpg"));   // 敌机1爆炸4遮罩
	loadimage(&img_enemy3, _T("picture\\enemy3.jpg"));               // 敌机2
	loadimage(&img_enemy4, _T("picture\\enemy4.jpg"));               // 敌机2遮罩
	loadimage(&img_enemy3_boom1, _T("picture\\enemy3_boom1.jpg"));   // 敌机2爆炸1
	loadimage(&img_enemy4_boom1, _T("picture\\enemy4_boom1.jpg"));   // 敌机2爆炸1遮罩
	loadimage(&img_enemy3_boom2, _T("picture\\enemy3_boom2.jpg"));   // 敌机2爆炸2
	loadimage(&img_enemy4_boom2, _T("picture\\enemy4_boom2.jpg"));   // 敌机2爆炸2遮罩
	loadimage(&img_enemy3_boom3, _T("picture\\enemy3_boom3.jpg"));   // 敌机2爆炸3
	loadimage(&img_enemy4_boom3, _T("picture\\enemy4_boom3.jpg"));   // 敌机2爆炸3遮罩
	loadimage(&img_enemy3_boom4, _T("picture\\enemy3_boom4.jpg"));   // 敌机2爆炸4
	loadimage(&img_enemy4_boom4, _T("picture\\enemy4_boom4.jpg"));   // 敌机2爆炸4遮罩
	loadimage(&img_enemy5, _T("picture\\enemy5.jpg"));               // 敌机3
	loadimage(&img_enemy6, _T("picture\\enemy6.jpg"));               // 敌机3遮罩
	loadimage(&img_enemy5_boom1, _T("picture\\enemy5_boom1.jpg"));   // 敌机3爆炸1
	loadimage(&img_enemy6_boom1, _T("picture\\enemy6_boom1.jpg"));   // 敌机3爆炸1遮罩
	loadimage(&img_enemy5_boom2, _T("picture\\enemy5_boom2.jpg"));   // 敌机3爆炸2
	loadimage(&img_enemy6_boom2, _T("picture\\enemy6_boom2.jpg"));   // 敌机3爆炸2遮罩
	loadimage(&img_enemy5_boom3, _T("picture\\enemy5_boom3.jpg"));   // 敌机3爆炸3
	loadimage(&img_enemy6_boom3, _T("picture\\enemy6_boom3.jpg"));   // 敌机3爆炸3遮罩
	loadimage(&img_enemy5_boom4, _T("picture\\enemy5_boom4.jpg"));   // 敌机3爆炸4
	loadimage(&img_enemy6_boom4, _T("picture\\enemy6_boom4.jpg"));   // 敌机3爆炸4遮罩
	loadimage(&img_heal1, _T("picture\\heal_bag1.jpg"));             // 治疗包
	loadimage(&img_heal2, _T("picture\\heal_bag2.jpg"));             // 治疗包遮罩
	loadimage(&img_weapon1, _T("picture\\weapon_bag1.jpg"));         // 武器包
	loadimage(&img_weapon2, _T("picture\\weapon_bag2.jpg"));         // 武器包遮罩
	loadimage(&img_speed1, _T("picture\\speed_bag1.jpg"));           // 射速包
	loadimage(&img_speed2, _T("picture\\speed_bag2.jpg"));           // 射速包遮罩
	/*--------------------------------------------------------------*/
}

void show() {
	putimage(0, 0, &img_bk);
	show_bullet();
	show_enemy_bullet();
	show_enemy_boom();
	show_reward();
	show_enemy();
	show_plane();
	show_health();
	show_score();
	FlushBatchDraw();
}

void show_bullet() {
	for (auto i = bullet.begin(); i != bullet.end(); i++) { // 显示子弹
		if (i->type == 0) {                                 // 不同种类
			putimage(i->x, i->y, &img_bullet2, NOTSRCERASE);
			putimage(i->x, i->y, &img_bullet1, SRCINVERT);
		}
		else if (i->type == 1 || i->type == 2) {
			putimage(i->x, i->y, &img_bullet4, NOTSRCERASE);
			putimage(i->x, i->y, &img_bullet3, SRCINVERT);
		}

	}
}

void show_enemy_bullet() {
	setfillcolor(RED);
	for (auto i = enemy_bullet.begin(); i != enemy_bullet.end(); i++) {// 显示敌机子弹
		fillcircle(i->x, i->y, i->r);
	}
}

void show_enemy_boom() {
	for (auto i = boom_enemy.begin(); i != boom_enemy.end();) {// 敌机爆炸动画
		if (i->type == 0) {
			if (i->time >= 0 && i->time < 5) {
				putimage(i->x, i->y, &img_enemy2_boom1, NOTSRCERASE);
				putimage(i->x, i->y, &img_enemy1_boom1, SRCINVERT);
			}
			else if (i->time >= 5 && i->time < 10) {
				putimage(i->x, i->y, &img_enemy2_boom2, NOTSRCERASE);
				putimage(i->x, i->y, &img_enemy1_boom2, SRCINVERT);
			}
			else if (i->time >= 10 && i->time < 15) {
				putimage(i->x, i->y, &img_enemy2_boom3, NOTSRCERASE);
				putimage(i->x, i->y, &img_enemy1_boom3, SRCINVERT);
			}
			else if (i->time >= 15 && i->time < 20) {
				putimage(i->x, i->y, &img_enemy2_boom4, NOTSRCERASE);
				putimage(i->x, i->y, &img_enemy1_boom4, SRCINVERT);
			}
		}
		else if (i->type == 1) {
			if (i->time >= 0 && i->time < 5) {
				putimage(i->x, i->y, &img_enemy4_boom1, NOTSRCERASE);
				putimage(i->x, i->y, &img_enemy3_boom1, SRCINVERT);
			}
			else if (i->time >= 5 && i->time < 10) {
				putimage(i->x, i->y, &img_enemy4_boom2, NOTSRCERASE);
				putimage(i->x, i->y, &img_enemy3_boom2, SRCINVERT);
			}
			else if (i->time >= 10 && i->time < 15) {
				putimage(i->x, i->y, &img_enemy4_boom3, NOTSRCERASE);
				putimage(i->x, i->y, &img_enemy3_boom3, SRCINVERT);
			}
			else if (i->time >= 15 && i->time < 20) {
				putimage(i->x, i->y, &img_enemy4_boom4, NOTSRCERASE);
				putimage(i->x, i->y, &img_enemy3_boom4, SRCINVERT);
			}
		}
		else if (i->type == 2) {
			if (i->time >= 0 && i->time < 5) {
				putimage(i->x, i->y, &img_enemy6_boom1, NOTSRCERASE);
				putimage(i->x, i->y, &img_enemy5_boom1, SRCINVERT);
			}
			else if (i->time >= 5 && i->time < 10) {
				putimage(i->x, i->y, &img_enemy6_boom2, NOTSRCERASE);
				putimage(i->x, i->y, &img_enemy5_boom2, SRCINVERT);
			}
			else if (i->time >= 10 && i->time < 15) {
				putimage(i->x, i->y, &img_enemy6_boom3, NOTSRCERASE);
				putimage(i->x, i->y, &img_enemy5_boom3, SRCINVERT);
			}
			else if (i->time >= 15 && i->time < 20) {
				putimage(i->x, i->y, &img_enemy6_boom4, NOTSRCERASE);
				putimage(i->x, i->y, &img_enemy5_boom4, SRCINVERT);
			}
		}
		if (i->time >= 20) {
			boom_enemy.erase(i);
			i = boom_enemy.begin();
		}
		else {
			i->time += 0.1;
			i++;
		}
	}

}

void show_reward() {
	for (auto i = reward.begin(); i != reward.end(); i++) { // 显示奖励
		if (i->type == 0) {
			putimage(i->x, i->y, &img_heal2, NOTSRCERASE);
			putimage(i->x, i->y, &img_heal1, SRCINVERT);
		}
		else if (i->type == 1) {
			putimage(i->x, i->y, &img_speed2, NOTSRCERASE);
			putimage(i->x, i->y, &img_speed1, SRCINVERT);
		}
		else if (i->type == 2) {
			putimage(i->x, i->y, &img_weapon2, NOTSRCERASE);
			putimage(i->x, i->y, &img_weapon1, SRCINVERT);
		}

	}
}

void show_enemy() {
	for (auto i = enemy.begin(); i != enemy.end(); i++) { // 显示敌机
		if (i->type == 0) {
			putimage(i->x, i->y, &img_enemy2, NOTSRCERASE);
			putimage(i->x, i->y, &img_enemy1, SRCINVERT);
		}
		else if (i->type == 1) {
			putimage(i->x, i->y, &img_enemy4, NOTSRCERASE);
			putimage(i->x, i->y, &img_enemy3, SRCINVERT);
		}
		else if (i->type == 2) {
			putimage(i->x, i->y, &img_enemy6, NOTSRCERASE);
			putimage(i->x, i->y, &img_enemy5, SRCINVERT);
		}
		if (i->show_health) { // 如果没击落，显示血条
			setlinecolor(BLACK);
			int left = i->x + i->width / 6 - 3;
			int top = i->y - 8;
			int bottom = top + 5;
			int right = left + i->max_health * double(double((i->width) * 5 / 6) / i->max_health);
			rectangle(left - 1, top, right + 1, bottom); // 血条框  
			setfillcolor(RED);
			solidrectangle(left, top + 1, right - (i->max_health - i->health)      // 自己凑的非常复杂的公式
				* double(double((i->width) * 5 / 6) / i->max_health), bottom - 1); // 将血量变成百分比长度
		}
	}
}

void show_plane() {
	putimage(plane.x, plane.y, &img_plane2, NOTSRCERASE); //
	putimage(plane.x, plane.y, &img_plane1, SRCINVERT); // 显示飞机, 减去的数字保证坐标在飞机中间
}

void show_health() {
	setlinecolor(BLACK);
	rectangle(10, 299, 25, 501);                         // 边框
	setfillcolor(RED);
	plane.health = plane.health < 0 ? 0 : plane.health;  // 保证血条最小为 0
	solidrectangle(11, 500 - plane.health * 2, 24, 500); // 血条
	TCHAR text[20];
	_stprintf_s(text, _T("%d"), plane.health);
	outtextxy(10, 510, text);
}

void show_score() {
	TCHAR info[100];
	setfillcolor(BLACK);
	_stprintf_s(info, _T("分数：%d"), score);
	outtextxy(0, 0, info);
}



void manuallcontrol()
{
	MOUSEMSG mouse;                       // 鼠标消息
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
	is_live();
	auto_fire();
	eat_reward();
	reward_move();
	enemy_fire();
	enemy_move();
	change_difficulty();
	if (enemy.size() < difficulty) {
		create_enemy();
	}
}

void is_shot() {
	for (int i = 0; i < enemy.size();) { // 我方是否击中
		int j;
		for (j = 0; j < bullet.size();) {
			if (bullet[j].x + bullet[j].width >= enemy[i].x
				&& bullet[j].x <= enemy[i].x + enemy[i].width
				&& bullet[j].y >= enemy[i].y - enemy[i].high
				&& bullet[j].y - bullet[j].high <= enemy[i].y) // 子弹击中敌机
			{
				bullet.erase(bullet.begin() + j);
				j = 0;
				enemy[i].health -= plane.damage;
				if (enemy[i].health <= 0) { //如果敌机生命小于0则摧毁
					score += enemy[i].damage;
					enemy_boom(*(enemy.begin() + i), i);
					i = 0;
				}
				else { // 否则显示血量
					enemy[i].show_health = true;
				}
			}
			else {
				j++;
			}

		}
		if (j == bullet.size()) {
			i++;
		}
	}
	for (int i = 0; i < enemy_bullet.size();) { // 敌方是否击中
		if (enemy_bullet[i].x - enemy_bullet[i].r <= plane.x + plane.width
			&& enemy_bullet[i].x + enemy_bullet[i].r >= plane.x
			&& enemy_bullet[i].y - enemy_bullet[i].r <= plane.y + plane.high
			&& enemy_bullet[i].y + enemy_bullet[i].r >= plane.y)
		{
			plane_hurt(10);
			enemy_bullet.erase(enemy_bullet.begin() + i);
		}
		else {
			i++;
		}
	}

	for (auto i = 0; i < enemy.size();) { // 是否碰撞
		if (enemy[i].x <= plane.x + plane.width && enemy[i].x + enemy[i].width >= plane.x
			&& enemy[i].y <= plane.y + plane.high && enemy[i].y + enemy[i].high >= plane.y) {
			plane_hurt(enemy[i].damage);
			enemy_boom(*(enemy.begin() + i), i);
		}
		else {
			i++;
		}
	}
}

void is_live() {
	if (plane.health <= 0) {
		live = false;
	}
}

void auto_fire() {                     // 自动射击并自动销毁子弹
	fire_cnt++;
	if (fire_cnt % plane.firespeed == 1) {
		if (plane.weapon == 0) {       //根据武器push不同的子弹
			Bullet new_bullet(plane.x + plane.width / 2 - 4, plane.y, 0, 21, 9);
			bullet.push_back(new_bullet);
		}
		else if (plane.weapon == 1) {
			Bullet new_bullet(plane.x + plane.width / 2 - 9, plane.y, 1, 17, 18);
			bullet.push_back(new_bullet);
		}
		else if (plane.weapon == 2) {
			Bullet new_bullet1(plane.x, plane.y, 1, 17, 18);
			Bullet new_bullet2(plane.x + plane.width - 18, plane.y, 1, 17, 18);
			bullet.push_back(new_bullet1);
			bullet.push_back(new_bullet2);
		}
		fire_cnt %= plane.firespeed;
	}

	for (int i = 0; i < bullet.size();) {
		bullet[i].y -= bullet[i].speed;
		if (bullet[i].y <= -bullet[i].high) {
			bullet.erase(bullet.begin() + i);
		}
		else {
			i++;
		}
	}
}

void eat_reward() {
	for (int i = 0; i < reward.size();) { // 是否吃到奖励
		if (reward[i].x <= plane.x + plane.width
			&& reward[i].x + reward[i].r >= plane.x
			&& reward[i].y <= plane.y + plane.high
			&& reward[i].y + reward[i].r >= plane.y)
		{
			if (reward[i].type == 0) {
				plane.health = min(plane.health + (max(1, (100 - plane.health) / 2)), 100); // 根据已损失生命值回血,最少回1
				reward.erase(reward.begin() + i);
				i = 0;
			}
			else if (reward[i].type == 1) {
				plane.firespeed = max(plane.firespeed - 150, 150);
				reward.erase(reward.begin() + i);
				i = 0;
			}
			else if (reward[i].type == 2) {
				plane.weapon = min(plane.weapon + 1, 2);
				if (plane.weapon == 1) {
					plane.damage = 75;
				}
				else if (plane.weapon == 2) {
					plane.damage += 10;    // 武器2级后再升级只加伤害
				}
				reward.erase(reward.begin() + i);
				i = 0;
			}
		}
		else {
			i++;
		}
	}
}

void reward_move() {
	for (int i = 0; i < reward.size();) {
		reward[i].x += reward[i].speed * reward[i].dx;
		reward[i].y += reward[i].speed * reward[i].dy;
		if (reward[i].x <= 0 || reward[i].x >= width - reward[i].r) {
			reward[i].dx *= -1;
		}
		if (reward[i].y <= 0) {
			reward[i].dy *= -1;
		}
		if (reward[i].y >= high) { // 超出屏幕外自动销毁
			reward.erase(reward.begin() + i);
			i = 0;
		}
		else {
			i++;
		}
	}
}

void enemy_fire() {
	for (auto i = enemy.begin(); i != enemy.end(); i++) {
		if (i->type == 1 && i->y >= high / 10 && i->y <= high / 10 + 0.1) {
			Enemy_Bullet new_enemy_bullet(i->x + i->width / 2, i->y + i->high, 5, i->speed * 2);
			enemy_bullet.push_back(new_enemy_bullet);
		}
		if (i->type == 2 && i->y >= high / 20 && i->y <= high / 20 + 0.05) { // 厉害的敌机一次发射9个子弹
			// dx = sin( 2pi*4/5/6/7/8 / 60 )
			Enemy_Bullet new_enemy_bullet1(i->x + i->width / 2, i->y + i->high, 5, i->speed * 2, sin(8 * PI / 60), cos(8 * PI / 60));
			Enemy_Bullet new_enemy_bullet2(i->x + i->width / 2, i->y + i->high, 5, i->speed * 2, sin(-8 * PI / 60), cos(-8 * PI / 60));
			Enemy_Bullet new_enemy_bullet3(i->x + i->width / 2, i->y + i->high, 5, i->speed * 2, sin(6 * PI / 60), cos(6 * PI / 60));
			Enemy_Bullet new_enemy_bullet4(i->x + i->width / 2, i->y + i->high, 5, i->speed * 2, sin(-6 * PI / 60), cos(-6 * PI / 60));
			Enemy_Bullet new_enemy_bullet5(i->x + i->width / 2, i->y + i->high, 5, i->speed * 2, sin(4 * PI / 60), cos(4 * PI / 60));
			Enemy_Bullet new_enemy_bullet6(i->x + i->width / 2, i->y + i->high, 5, i->speed * 2, sin(-4 * PI / 60), cos(-4 * PI / 60));
			Enemy_Bullet new_enemy_bullet7(i->x + i->width / 2, i->y + i->high, 5, i->speed * 2, sin(2 * PI / 60), cos(2 * PI / 60));
			Enemy_Bullet new_enemy_bullet8(i->x + i->width / 2, i->y + i->high, 5, i->speed * 2, sin(-2 * PI / 60), cos(-2 * PI / 60));
			Enemy_Bullet new_enemy_bullet9(i->x + i->width / 2, i->y + i->high, 5, i->speed * 2, sin(0 * PI / 60), cos(0 * PI / 60));
			enemy_bullet.push_back(new_enemy_bullet1);
			enemy_bullet.push_back(new_enemy_bullet2);
			enemy_bullet.push_back(new_enemy_bullet3);
			enemy_bullet.push_back(new_enemy_bullet4);
			enemy_bullet.push_back(new_enemy_bullet5);
			enemy_bullet.push_back(new_enemy_bullet6);
			enemy_bullet.push_back(new_enemy_bullet7);
			enemy_bullet.push_back(new_enemy_bullet8);
			enemy_bullet.push_back(new_enemy_bullet9);
		}
	}

	for (int i = 0; i < enemy_bullet.size();) {
		enemy_bullet[i].x += enemy_bullet[i].speed * enemy_bullet[i].dx; // 子弹移动
		enemy_bullet[i].y += enemy_bullet[i].speed * enemy_bullet[i].dy;

		if (enemy_bullet[i].y >= high + enemy_bullet[i].r) { // 子弹超出屏幕就销毁
			enemy_bullet.erase(enemy_bullet.begin() + i);
		}
		else {
			i++;
		}
	}
}

void enemy_move() {
	for (int i = 0; i < enemy.size();) {
		enemy[i].y += enemy[i].speed;
		if (enemy[i].y >= high) { // 超出屏幕外自动销毁
			enemy.erase(enemy.begin() + i);
		}
		else {
			i++;
		}
	}
}

void change_difficulty()
{
	static int tempscore = 500; // 难度系数
	if (score / tempscore >= 1) {
		difficulty++; // 难度增加，敌机血量会增加，同屏最大敌机数量也会增加
		tempscore += 500;
	}
	if (difficulty > 10) { // 最大难度为10
		difficulty = 10;
	};
}

void create_enemy() { // 制造敌机并自动下落

	int type = rnd(1000, 0.6, 0.3, 0.1); // 生成种类
	/* 0 -> 单个生成  1 -> 斜着快速出现  2 -> 三角队列 */

	if (type == 0) {                                                             // 单个生成
		int enemy_type = rnd(1000, 0.7 - ((difficulty - 3) * 0.013), 0.3, ((difficulty - 3) * 0.013));
		double x = rand() % ((int)width - 50);                                   // 随机敌机的坐标 并 防止敌机出生在屏幕外面
		if (enemy_type == 0) {
			double speed = (rnd(1000, 0.9, 0.1, 0) + 1) * 0.2;                   // 随机敌机速度
			Enemy new_enemy(x, 0, 0, 35, 51, speed, 50 + ((difficulty - 3) * 10) // 血量随难度上升
				, 50 + ((difficulty - 3) * 10), 10, false);                      // 0号敌机的位置和大小
			enemy.push_back(new_enemy);
		}
		if (enemy_type == 1) {
			Enemy new_enemy(x, 0, 1, 79, 69, 0.1, 75 + ((difficulty - 3) * 15)
				, 75 + ((difficulty - 3) * 15), 20, false); // 1号敌机的位置和大小
			enemy.push_back(new_enemy);
		}
		if (enemy_type == 2) {
			Enemy new_enemy(x, 0, 2, 113, 83, 0.05, 125 + ((difficulty - 3) * 20)
				, 125 + ((difficulty - 3) * 20), 30, false); // 1号敌机的位置和大小
			enemy.push_back(new_enemy);
		}
	}
	else if (type == 1) {

	}



}

void enemy_boom(Enemy dead_enemy, int n) {
	double x = dead_enemy.x, y = dead_enemy.y;
	if (reward.size() < 4) { // 同屏不能超过3个奖励
		create_reward(dead_enemy);
	}
	int type = dead_enemy.type;
	enemy.erase(enemy.begin() + n);
	Boom_Enemy new_boom_enemy(x, y, type, 0);
	boom_enemy.push_back(new_boom_enemy);
	//mciSendString(TEXT("close boom"), NULL, 0, NULL);
	//mciSendString(TEXT("open enemy1_down.mp3 alias boom"), NULL, 0, NULL);
	//mciSendString(TEXT("play boom"), NULL, 0, NULL);
	//mciSendString(TEXT("close boom"), NULL, 0, NULL);
}

void create_reward(Enemy e) {
	int type;
	/* 0-> 血包 1-> 开火速度包 2-> 武器包*/
	// 随着难度增加增加爆率
	if (e.type == 0) { //打死杂兵，奖励少
		type = rnd(10000, 0.01 + ((difficulty - 3) * 0.001)
			, 0.012 + ((difficulty - 3) * 0.0012)
			, 0.012 + ((difficulty - 3) * 0.0011));
		if (type == -1) return;
		Reward new_reward(e.x, e.y, -1, -1, e.speed / 0.7, type);
		reward.push_back(new_reward);
	}
	else if (e.type == 1) { //稍微提高点爆率
		type = rnd(10000, 0.04 + ((difficulty - 3) * 0.002)
			, 0.024 + ((difficulty - 3) * 0.0024)
			, 0.024 + ((difficulty - 3) * 0.0022));
		if (type == -1) return;
		Reward new_reward(e.x, e.y, -1, -1, e.speed / 0.7, type);
		reward.push_back(new_reward);
	}
	else if (e.type == 2) { // 最厉害的直接奖励
		type = rnd(10000, 0.3, 0.4, 0.4);
		Reward new_reward(e.x, e.y, -1, -1, e.speed / 0.7, type);
		reward.push_back(new_reward);
	}


}

void plane_hurt(int damage) {
	plane.health -= damage;
	sum_hurt += damage;
	if (sum_hurt >= 20) { // 承受伤害到达20后,清除奖励
		plane.firespeed = 450;
		plane.weapon = 0;
		plane.damage = 50;
		sum_hurt = 0;
	}
}

int rnd(int n, double a, double b, double c) {
	int t = rand() % n;
	int res = -1;
	if (t < n * a) { // 控制几率
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
		putimage(width / 2 - 214, 100, &img_title2, NOTSRCERASE);
		putimage(width / 2 - 214, 100, &img_title1, SRCINVERT);
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

bool bye() {
	char result[100];
	sprintf(result, "本次得分：%d分，请再接再厉。是否重新开始游戏？", score);
	if (MessageBox(GetHWnd(), result, "游戏结束", MB_YESNO) == IDYES)
	{
		enemy.clear();
		bullet.clear();
		enemy_bullet.clear();
		boom_enemy.clear();
		reward.clear();
		return true;
	}
	return false;
}
