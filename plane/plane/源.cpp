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
#define Width 500                                                     // ������С
#define PI 3.1415926

/*----------------------------����ṹ��-------------------------------*/
struct Bullet {                                                       // �ӵ��ṹ��
	double x, y;                                                      // �ӵ�λ��              
	int type;                                                         // �ӵ�����
	double speed;                                                     // �ӵ��ٶ�
	int high, width;                                                  // �ӵ�ͼƬ�Ĵ�С
	Bullet(double x1, double y1, int type1,                           //
		int high1, int width1, double speed1 = 0.9) {                 //
		x = x1; y = y1; type = type1; speed = speed1;                 //
		high = high1; width = width1;                                 //
	}                                                                 //
};                                                                    //
																	  //
struct Enemy_Bullet {                                                 //�����ӵ��ṹ��
	double x, y, r;                                                   //�����ӵ�λ�ã���С
	double speed;                                                     // �ӵ��ٶ�
	double dx, dy;                                                    //
	Enemy_Bullet(double x1, double y1, double r1, double speed1       //
		, double dx1 = 0, double dy1 = 1) {                           //
		x = x1; y = y1; r = r1; speed = speed1;                       //
		dx = dx1; dy = dy1;
	}                                          //
};                                                                    //
																	  //
struct Plane {                                                        //�ɻ��ṹ��
	double x, y;                                                      // �ɻ�λ��
	int health;                                                       // Ѫ��
	int weapon;                                                       // ��������
	int damage;                                                       // �˺�
	int firespeed;                                                    // ������
	int high, width;                                                  // ͼƬ��С
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
struct Enemy {                                                        // �л��ṹ��
	double x, y;                                                      // �л�λ��
	int max_health, health;                                           // Ѫ��
	int damage;                                                       // �˺�
	int type;                                                         // �л�����
	double speed;                                                     // �ƶ��ٶ�
	int high, width;                                                  // ͼƬ��С
	bool show_health;                                                 // �Ƿ���ʾѪ��
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
struct Boom_Enemy {                                                   // ��ը�����ṹ��
	double x, y;                                                      // ��ը����λ��
	int type;                                                         // ����
	double time;                                                      // �ڼ�֡
	Boom_Enemy(int x1, int y1, int type1, double time1) {             //
		x = x1; y = y1; type = type1; time = time1;
	}                  //
};                                                                    //
																	  //
struct Reward {                                                       // �������߽ṹ��
	double x, y;                                                      // λ��
	int dx, dy;                                                       // ����
	double speed;                                                     // �ƶ��ٶ�
	int type;                                                         // ����
	int r;                                                            // ��С
	Reward(double x1, double y1, int dx1, int dy1,                    //
		double speed1, int type1, int r1 = 40) {                      //
		x = x1; y = y1; dx = dx1; dy = dy1;                           //
		speed = speed1; type = type1; r = r1;
	}                        //
};                                                                    //
/*-------------------------------------------------------------------*/

/*------------------�Զ��庯��------------------*/
void start();                                  // ��ʼ��
void load_image();                             // ����ͼƬ
void hello();                                  // ��ӭ����
bool bye();                                    // ��������
void show();                                   // ��ʾ������
void show_bullet();                            // ��ʾ�ӵ�
void show_enemy_bullet();                      // ��ʾ�����ӵ�
void show_reward();                            // ��ʾ����
void show_enemy();                             // ��ʾ�л�
void show_enemy_boom();                        // ��ʾ�л���ը����
void show_plane();                             // ��ʾ��ҷɻ�
void show_health();                            // ��ʾ����
void show_score();                             // ��ʾ����
void manuallcontrol();                         // ������
void automaticcontrol();                       // �Զ����¶���
void is_shot();                                // �Ƿ�����
void is_live();                                // �Ƿ�����
void auto_fire();                              // �Զ�������Զ������ӵ�
void eat_reward();                             // �Խ���
void reward_move();                            // �����ƶ�
void enemy_fire();                             // ���˿���
void enemy_move();                             // �л��ƶ�
void change_difficulty();                      // ����ʱ��ı��Ѷ�
void create_enemy();                           // ����л�
void enemy_boom(Enemy boom_enemy, int n);      // �л���ը����¼�
void create_reward(Enemy e);                   // ��ɱ����
void plane_hurt(int damage);                   // �ɻ�����
int rnd(int n, double a, double b, double c);  // ����3�ֿ��Ƹ��ʵ������
/*---------------------------------------------*/

/*----------------------------------ȫ�ֱ���-----------------------------------*/
double width, high;                                                           // W->X      H->Y
int score, difficulty;                                                        // ����
bool live;                                                                    // ��Ϸ������ʼ
int fire_cnt;                                                                 // �����ٶ�
int sum_hurt;                                                                 //�����˺�
IMAGE img_bk;                                                                 // ����ͼƬ
IMAGE img_title1, img_title2;                                                 // ����ͼƬ
IMAGE img_start1, img_start2;                                                 // ��ʼ��ť
IMAGE img_close1, img_close2;                                                 // �رհ�ť
IMAGE img_plane1, img_plane2;                                                 // �ɻ�ͼƬ
IMAGE img_bullet1, img_bullet2, img_bullet3, img_bullet4;                     // �ӵ�ͼƬ
IMAGE img_enemy1, img_enemy2;                                                 // �л�1ͼƬ
IMAGE img_enemy3, img_enemy4;                                                 // �л�2ͼƬ
IMAGE img_enemy5, img_enemy6;                                                 // �л�3ͼƬ
IMAGE img_enemy1_boom1, img_enemy1_boom2, img_enemy1_boom3, img_enemy1_boom4; // �л�1��ը����
IMAGE img_enemy2_boom1, img_enemy2_boom2, img_enemy2_boom3, img_enemy2_boom4; // �л�1��ը��������
IMAGE img_enemy3_boom1, img_enemy3_boom2, img_enemy3_boom3, img_enemy3_boom4; // �л�2��ը����
IMAGE img_enemy4_boom1, img_enemy4_boom2, img_enemy4_boom3, img_enemy4_boom4; // �л�2��ը��������
IMAGE img_enemy5_boom1, img_enemy5_boom2, img_enemy5_boom3, img_enemy5_boom4; // �л�3��ը����
IMAGE img_enemy6_boom1, img_enemy6_boom2, img_enemy6_boom3, img_enemy6_boom4; // �л�3��ը��������
IMAGE img_heal1, img_heal2, img_weapon1, img_weapon2, img_speed1, img_speed2; // ��������ͼƬ
vector<Enemy> enemy;                                                          // �л�����
vector<Bullet> bullet;                                                        // �ӵ�����
vector<Enemy_Bullet> enemy_bullet;                                            // �л�����
vector<Boom_Enemy> boom_enemy;                                                // �л���ը����
vector<Reward> reward;                                                        // ������������
Plane plane;                                                                  // ��ҿ��Ʒɻ�
/*----------------------------------------------------------------------------*/

int main()
{
game_start:
	start();
	hello(); // ��ʼ����
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
	closegraph(); // �رջ�ͼ����
	return 0;
}

void start()
{
	mciSendString(TEXT("open picture\\game_music.mp3 alias bgm"), NULL, 0, NULL);
	mciSendString(TEXT("play bgm repeat"), NULL, 0, NULL);
	initgraph(Width, High);                     //��ͼ������ʼ��
	load_image();                               // ����ͼƬ
	score = 0;                                  // ������ʼ��0
	live = true;                                // ����
	plane.health = 100, plane.weapon = 0;       // ��ʼ���ɻ����ݣ������ؿ�
	plane.damage = 50, plane.firespeed = 450;   //
	difficulty = 2;                             // ���2�����10
	srand(unsigned(time(0)));                   // ��ʼ���������
	fire_cnt = 0;                               // ���ƿ����ٶ�
	sum_hurt = 0;                               // �����˺�
	width = Width; high = High;                 // ��ʼ��ѭ����С
	BeginBatchDraw();
}

void load_image() {
	/*--------------------------����ͼƬ-----------------------------*/
	loadimage(&img_bk, _T("picture\\background.png"));               // ����
	loadimage(&img_title1, _T("picture\\title1.jpg"));               // ����
	loadimage(&img_title2, _T("picture\\title2.jpg"));               //
	loadimage(&img_start1, _T("picture\\start1.jpg"));               // ��ʼ��ť
	loadimage(&img_start2, _T("picture\\start2.jpg"));               //
	loadimage(&img_close1, _T("picture\\close1.jpg"));               // ������ť
	loadimage(&img_close2, _T("picture\\close2.jpg"));               //
	loadimage(&img_plane1, _T("picture\\plane1.jpg"));               // �ɻ�
	loadimage(&img_plane2, _T("picture\\plane2.jpg"));               // �ɻ�����
	loadimage(&img_bullet1, _T("picture\\bullet1.jpg"));             // �ӵ�1
	loadimage(&img_bullet2, _T("picture\\bullet2.jpg"));             // �ӵ�1����
	loadimage(&img_bullet3, _T("picture\\bullet3.jpg"));             // �ӵ�2
	loadimage(&img_bullet4, _T("picture\\bullet4.jpg"));             // �ӵ�2����
	loadimage(&img_enemy1, _T("picture\\enemy1.jpg"));               // �л�1
	loadimage(&img_enemy2, _T("picture\\enemy2.jpg"));               // �л�1����
	loadimage(&img_enemy1_boom1, _T("picture\\enemy1_boom1.jpg"));   // �л�1��ը1
	loadimage(&img_enemy2_boom1, _T("picture\\enemy2_boom1.jpg"));   // �л�1��ը1����
	loadimage(&img_enemy1_boom2, _T("picture\\enemy1_boom2.jpg"));   // �л�1��ը2
	loadimage(&img_enemy2_boom2, _T("picture\\enemy2_boom2.jpg"));   // �л�1��ը2����
	loadimage(&img_enemy1_boom3, _T("picture\\enemy1_boom3.jpg"));   // �л�1��ը3
	loadimage(&img_enemy2_boom3, _T("picture\\enemy2_boom3.jpg"));   // �л�1��ը3����
	loadimage(&img_enemy1_boom4, _T("picture\\enemy1_boom4.jpg"));   // �л�1��ը4
	loadimage(&img_enemy2_boom4, _T("picture\\enemy2_boom4.jpg"));   // �л�1��ը4����
	loadimage(&img_enemy3, _T("picture\\enemy3.jpg"));               // �л�2
	loadimage(&img_enemy4, _T("picture\\enemy4.jpg"));               // �л�2����
	loadimage(&img_enemy3_boom1, _T("picture\\enemy3_boom1.jpg"));   // �л�2��ը1
	loadimage(&img_enemy4_boom1, _T("picture\\enemy4_boom1.jpg"));   // �л�2��ը1����
	loadimage(&img_enemy3_boom2, _T("picture\\enemy3_boom2.jpg"));   // �л�2��ը2
	loadimage(&img_enemy4_boom2, _T("picture\\enemy4_boom2.jpg"));   // �л�2��ը2����
	loadimage(&img_enemy3_boom3, _T("picture\\enemy3_boom3.jpg"));   // �л�2��ը3
	loadimage(&img_enemy4_boom3, _T("picture\\enemy4_boom3.jpg"));   // �л�2��ը3����
	loadimage(&img_enemy3_boom4, _T("picture\\enemy3_boom4.jpg"));   // �л�2��ը4
	loadimage(&img_enemy4_boom4, _T("picture\\enemy4_boom4.jpg"));   // �л�2��ը4����
	loadimage(&img_enemy5, _T("picture\\enemy5.jpg"));               // �л�3
	loadimage(&img_enemy6, _T("picture\\enemy6.jpg"));               // �л�3����
	loadimage(&img_enemy5_boom1, _T("picture\\enemy5_boom1.jpg"));   // �л�3��ը1
	loadimage(&img_enemy6_boom1, _T("picture\\enemy6_boom1.jpg"));   // �л�3��ը1����
	loadimage(&img_enemy5_boom2, _T("picture\\enemy5_boom2.jpg"));   // �л�3��ը2
	loadimage(&img_enemy6_boom2, _T("picture\\enemy6_boom2.jpg"));   // �л�3��ը2����
	loadimage(&img_enemy5_boom3, _T("picture\\enemy5_boom3.jpg"));   // �л�3��ը3
	loadimage(&img_enemy6_boom3, _T("picture\\enemy6_boom3.jpg"));   // �л�3��ը3����
	loadimage(&img_enemy5_boom4, _T("picture\\enemy5_boom4.jpg"));   // �л�3��ը4
	loadimage(&img_enemy6_boom4, _T("picture\\enemy6_boom4.jpg"));   // �л�3��ը4����
	loadimage(&img_heal1, _T("picture\\heal_bag1.jpg"));             // ���ư�
	loadimage(&img_heal2, _T("picture\\heal_bag2.jpg"));             // ���ư�����
	loadimage(&img_weapon1, _T("picture\\weapon_bag1.jpg"));         // ������
	loadimage(&img_weapon2, _T("picture\\weapon_bag2.jpg"));         // ����������
	loadimage(&img_speed1, _T("picture\\speed_bag1.jpg"));           // ���ٰ�
	loadimage(&img_speed2, _T("picture\\speed_bag2.jpg"));           // ���ٰ�����
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
	for (auto i = bullet.begin(); i != bullet.end(); i++) { // ��ʾ�ӵ�
		if (i->type == 0) {                                 // ��ͬ����
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
	for (auto i = enemy_bullet.begin(); i != enemy_bullet.end(); i++) {// ��ʾ�л��ӵ�
		fillcircle(i->x, i->y, i->r);
	}
}

void show_enemy_boom() {
	for (auto i = boom_enemy.begin(); i != boom_enemy.end();) {// �л���ը����
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
	for (auto i = reward.begin(); i != reward.end(); i++) { // ��ʾ����
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
	for (auto i = enemy.begin(); i != enemy.end(); i++) { // ��ʾ�л�
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
		if (i->show_health) { // ���û���䣬��ʾѪ��
			setlinecolor(BLACK);
			int left = i->x + i->width / 6 - 3;
			int top = i->y - 8;
			int bottom = top + 5;
			int right = left + i->max_health * double(double((i->width) * 5 / 6) / i->max_health);
			rectangle(left - 1, top, right + 1, bottom); // Ѫ����  
			setfillcolor(RED);
			solidrectangle(left, top + 1, right - (i->max_health - i->health)      // �Լ��յķǳ����ӵĹ�ʽ
				* double(double((i->width) * 5 / 6) / i->max_health), bottom - 1); // ��Ѫ����ɰٷֱȳ���
		}
	}
}

void show_plane() {
	putimage(plane.x, plane.y, &img_plane2, NOTSRCERASE); //
	putimage(plane.x, plane.y, &img_plane1, SRCINVERT); // ��ʾ�ɻ�, ��ȥ�����ֱ�֤�����ڷɻ��м�
}

void show_health() {
	setlinecolor(BLACK);
	rectangle(10, 299, 25, 501);                         // �߿�
	setfillcolor(RED);
	plane.health = plane.health < 0 ? 0 : plane.health;  // ��֤Ѫ����СΪ 0
	solidrectangle(11, 500 - plane.health * 2, 24, 500); // Ѫ��
	TCHAR text[20];
	_stprintf_s(text, _T("%d"), plane.health);
	outtextxy(10, 510, text);
}

void show_score() {
	TCHAR info[100];
	setfillcolor(BLACK);
	_stprintf_s(info, _T("������%d"), score);
	outtextxy(0, 0, info);
}



void manuallcontrol()
{
	MOUSEMSG mouse;                       // �����Ϣ
	while (MouseHit()) {
		mouse = GetMouseMsg();
		if (mouse.uMsg == WM_MOUSEMOVE) { // ��������괫���ɻ�
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
	for (int i = 0; i < enemy.size();) { // �ҷ��Ƿ����
		int j;
		for (j = 0; j < bullet.size();) {
			if (bullet[j].x + bullet[j].width >= enemy[i].x
				&& bullet[j].x <= enemy[i].x + enemy[i].width
				&& bullet[j].y >= enemy[i].y - enemy[i].high
				&& bullet[j].y - bullet[j].high <= enemy[i].y) // �ӵ����ел�
			{
				bullet.erase(bullet.begin() + j);
				j = 0;
				enemy[i].health -= plane.damage;
				if (enemy[i].health <= 0) { //����л�����С��0��ݻ�
					score += enemy[i].damage;
					enemy_boom(*(enemy.begin() + i), i);
					i = 0;
				}
				else { // ������ʾѪ��
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
	for (int i = 0; i < enemy_bullet.size();) { // �з��Ƿ����
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

	for (auto i = 0; i < enemy.size();) { // �Ƿ���ײ
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

void auto_fire() {                     // �Զ�������Զ������ӵ�
	fire_cnt++;
	if (fire_cnt % plane.firespeed == 1) {
		if (plane.weapon == 0) {       //��������push��ͬ���ӵ�
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
	for (int i = 0; i < reward.size();) { // �Ƿ�Ե�����
		if (reward[i].x <= plane.x + plane.width
			&& reward[i].x + reward[i].r >= plane.x
			&& reward[i].y <= plane.y + plane.high
			&& reward[i].y + reward[i].r >= plane.y)
		{
			if (reward[i].type == 0) {
				plane.health = min(plane.health + (max(1, (100 - plane.health) / 2)), 100); // ��������ʧ����ֵ��Ѫ,���ٻ�1
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
					plane.damage += 10;    // ����2����������ֻ���˺�
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
		if (reward[i].y >= high) { // ������Ļ���Զ�����
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
		if (i->type == 2 && i->y >= high / 20 && i->y <= high / 20 + 0.05) { // �����ĵл�һ�η���9���ӵ�
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
		enemy_bullet[i].x += enemy_bullet[i].speed * enemy_bullet[i].dx; // �ӵ��ƶ�
		enemy_bullet[i].y += enemy_bullet[i].speed * enemy_bullet[i].dy;

		if (enemy_bullet[i].y >= high + enemy_bullet[i].r) { // �ӵ�������Ļ������
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
		if (enemy[i].y >= high) { // ������Ļ���Զ�����
			enemy.erase(enemy.begin() + i);
		}
		else {
			i++;
		}
	}
}

void change_difficulty()
{
	static int tempscore = 500; // �Ѷ�ϵ��
	if (score / tempscore >= 1) {
		difficulty++; // �Ѷ����ӣ��л�Ѫ�������ӣ�ͬ�����л�����Ҳ������
		tempscore += 500;
	}
	if (difficulty > 10) { // ����Ѷ�Ϊ10
		difficulty = 10;
	};
}

void create_enemy() { // ����л����Զ�����

	int type = rnd(1000, 0.6, 0.3, 0.1); // ��������
	/* 0 -> ��������  1 -> б�ſ��ٳ���  2 -> ���Ƕ��� */

	if (type == 0) {                                                             // ��������
		int enemy_type = rnd(1000, 0.7 - ((difficulty - 3) * 0.013), 0.3, ((difficulty - 3) * 0.013));
		double x = rand() % ((int)width - 50);                                   // ����л������� �� ��ֹ�л���������Ļ����
		if (enemy_type == 0) {
			double speed = (rnd(1000, 0.9, 0.1, 0) + 1) * 0.2;                   // ����л��ٶ�
			Enemy new_enemy(x, 0, 0, 35, 51, speed, 50 + ((difficulty - 3) * 10) // Ѫ�����Ѷ�����
				, 50 + ((difficulty - 3) * 10), 10, false);                      // 0�ŵл���λ�úʹ�С
			enemy.push_back(new_enemy);
		}
		if (enemy_type == 1) {
			Enemy new_enemy(x, 0, 1, 79, 69, 0.1, 75 + ((difficulty - 3) * 15)
				, 75 + ((difficulty - 3) * 15), 20, false); // 1�ŵл���λ�úʹ�С
			enemy.push_back(new_enemy);
		}
		if (enemy_type == 2) {
			Enemy new_enemy(x, 0, 2, 113, 83, 0.05, 125 + ((difficulty - 3) * 20)
				, 125 + ((difficulty - 3) * 20), 30, false); // 1�ŵл���λ�úʹ�С
			enemy.push_back(new_enemy);
		}
	}
	else if (type == 1) {

	}



}

void enemy_boom(Enemy dead_enemy, int n) {
	double x = dead_enemy.x, y = dead_enemy.y;
	if (reward.size() < 4) { // ͬ�����ܳ���3������
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
	/* 0-> Ѫ�� 1-> �����ٶȰ� 2-> ������*/
	// �����Ѷ��������ӱ���
	if (e.type == 0) { //�����ӱ���������
		type = rnd(10000, 0.01 + ((difficulty - 3) * 0.001)
			, 0.012 + ((difficulty - 3) * 0.0012)
			, 0.012 + ((difficulty - 3) * 0.0011));
		if (type == -1) return;
		Reward new_reward(e.x, e.y, -1, -1, e.speed / 0.7, type);
		reward.push_back(new_reward);
	}
	else if (e.type == 1) { //��΢��ߵ㱬��
		type = rnd(10000, 0.04 + ((difficulty - 3) * 0.002)
			, 0.024 + ((difficulty - 3) * 0.0024)
			, 0.024 + ((difficulty - 3) * 0.0022));
		if (type == -1) return;
		Reward new_reward(e.x, e.y, -1, -1, e.speed / 0.7, type);
		reward.push_back(new_reward);
	}
	else if (e.type == 2) { // ��������ֱ�ӽ���
		type = rnd(10000, 0.3, 0.4, 0.4);
		Reward new_reward(e.x, e.y, -1, -1, e.speed / 0.7, type);
		reward.push_back(new_reward);
	}


}

void plane_hurt(int damage) {
	plane.health -= damage;
	sum_hurt += damage;
	if (sum_hurt >= 20) { // �����˺�����20��,�������
		plane.firespeed = 450;
		plane.weapon = 0;
		plane.damage = 50;
		sum_hurt = 0;
	}
}

int rnd(int n, double a, double b, double c) {
	int t = rand() % n;
	int res = -1;
	if (t < n * a) { // ���Ƽ���
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
	MOUSEMSG mouse; // �����Ϣ
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
	sprintf(result, "���ε÷֣�%d�֣����ٽ��������Ƿ����¿�ʼ��Ϸ��", score);
	if (MessageBox(GetHWnd(), result, "��Ϸ����", MB_YESNO) == IDYES)
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
