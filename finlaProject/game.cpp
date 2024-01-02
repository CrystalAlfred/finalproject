#include "game.h"
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

int score = 0;//得分
int historyscore[3] = { 0 };
int paused = 0;


void StartScene() {
	float w, h;//游戏画面宽高与重力加速度
	w = 600;
	h = 400;
	initgraph(w, h);//创建一个窗口
	settextstyle(40, 0, _T("Arial"));
	outtextxy(w / 10, h / 2, _T("Press any key to start the game"));


	_getch(); // 等待用戶按下任意鍵
	cleardevice(); // 清除畫面
}

void TogglePause() {
	paused = !paused;
	if (paused) {
		settextstyle(20, 0, _T("Arial"));
		outtextxy(50, 50, _T("遊戲已暫停，按 'P' 鍵繼續..."));
	}
}

void GameRunning() {

	float w, h, g;//游戏画面宽高与重力加速度
	float ball_x, ball_y, ball_vy, r;//小球圆心坐标，y方向速度和半径
	float rect1_x1, rect1_y1, rect1_x2, rect1_y2, rect_vx, rect_vy;//上方块的横纵坐标,因为画面向左移动，我们只要让方块向左移就可以实现，所以需要x方向速度
	float rect2_x1, rect2_y1, rect2_x2, rect2_y2;//下方块的横纵坐标
	float rect_h = 125;//因为随机方块高度的需要，单独设置一个高度

	//下面给具体参数
	w = 600;
	h = 400;
	g = 0.4;
	initgraph(w, h);//创建一个窗口

	//小球参数
	r = 18;
	ball_x = w / 4;//让小球在画面左边1/4处
	ball_y = h / 2;//让小球贴地，离地面一个半径距离
	ball_vy = 0;

	//上方块参数
	rect1_x1 = w * 3 / 4;//方块在画面3/4处
	rect1_y1 = 0;//上方块左上角y坐标为0
	rect1_x2 = rect1_x1 + 30;//+30就是指宽度为30
	rect1_y2 = rect1_y1 + rect_h;//高度为130

	//下方块参数
	rect2_x1 = w * 3 / 4;
	rect2_y1 = h - rect_h;//下方块的左上角y坐标离右下角差一个高度
	rect2_x2 = rect2_x1 + 30;
	rect2_y2 = h;//下方块贴地

	//暫停
	/*
	while (1) {
		if (!_kbhit()) {
			// 檢查是否按下 P 鍵，用於暫停和恢復
			if (_kbhit() && _getch() == 'p') {
				TogglePause();
			}
		}

		if (!paused) {
			// 遊戲運行
			// ...（之前的遊戲邏輯碼）
		}

		cleardevice();

		// 繪制小球
		setfillcolor(GREEN);
		fillcircle(ball_x, ball_y - 10, r);

		// 繪制方塊
		setfillcolor(BROWN);
		fillrectangle(rect1_x1, rect1_y1, rect1_x2, rect1_y2);
		fillrectangle(rect2_x1, rect2_y1, rect2_x2, rect2_y2);

		// 繪制分數
		TCHAR s[20];
		_stprintf(s, _T("%d"), score);
		settextstyle(40, 0, _T("Arial"));
		outtextxy(50, 30, s);

		// 如果遊戲暫停，顯示暫停提示
		if (paused) {
			settextstyle(20, 0, _T("Arial"));
			outtextxy(50, 50, _T("遊戲已暫停，按 'P' 鍵繼續..."));
		}

		Sleep(10);
	}*/

	//速度
	rect_vx = -2.5;
	rect_vy = -2;

	score = 0;//分數歸零

	while (1)
	{
		//当摁下空格，小球向上跳跃，给一个反向速度
		if (_kbhit())
		{
			char input = _getch();
			if (input == ' ')
			{
				ball_vy = -8;
			}
			if (input == 'p') {
				while (1) {
					if (_kbhit())
					{
						char input = _getch();
						if (input == 'p')
						{
							ball_vy = -8;
							break;
						}
					}
				}
			}
		}

		ball_vy = ball_vy + g;//y方向初速度为0，再加上一个重力加速度
		ball_y = ball_y + ball_vy;//根据小球y方向速度更新y坐标

		if (ball_y >= h - r)
		{
			ball_vy = 0;
			ball_y = h - r;
		}

		//更新两个方块的位置坐标
		rect1_x1 += rect_vx;
		rect1_x2 += rect_vx;
		rect2_x1 += rect_vx;
		rect2_x2 += rect_vx;

		if (rect1_x1 <= 0 && rect2_x1 <= 0)//方块来到最左边
		{
			//让他们出现在最右侧
			rect1_x1 = rect2_x1 = w - 30;
			rect1_x2 = rect2_x2 = w;
			score += 1;//得分+1
			rect_h = rand() % int(h / 5.5) + h / 5.5;//设置随即高度

			rect1_y2 = rect_h;
			rect2_y1 = h - rect_h;

			if (score > 0)
				rect_vx -= 0.1;//如果速度一直大于0，左移速度加大。

		}

		if (((ball_x + r >= rect2_x1)
			&& (ball_y + r >= rect2_y1)
			&& (ball_x - r <= rect2_x2))//碰到下方块
			|| ((ball_x + r >= rect1_x1)
				&& (ball_y - r <= rect1_y2)
				&& (ball_x - r <= rect1_x2))//碰到上方块
			|| (ball_y + r >= h - 10)//碰到底部
			)
		{
			Sleep(100);//给个慢速回放，看看怎么死的
			rect_vx = -3;//碰到就速度还原
			break;
		}

		if (ball_y - r <= 10)//如果碰到顶部，直接回到底部，分数为0
		{
			ball_vy += 100 * g;
			ball_y += ball_vy;
			break;
		}
		if (score > 10)
		{
			rect1_y2 += rect_vy;
			rect2_y1 += rect_vy;
			if(rect1_y2<10|| rect2_y1>h-10)rect_vy*=-1;
		}


        if (ball_y - r <= 10)//如果碰到顶部，直接回到底部，分数为0
        {
            ball_vy += 100 * g;
            ball_y += ball_vy;
            break;
        }
        if (score > 10)
        {
            rect1_y2 += rect_vy;
            rect2_y1 += rect_vy;
            if(rect1_y2<10|| rect2_y1>h-10)rect_vy*=-1;
        }

		cleardevice();


		cleardevice();

		//绘制小球
		setfillcolor(GREEN);//填上绿色
		fillcircle(ball_x, ball_y - 10, r);

		//绘制方块
		setfillcolor(BROWN);//填上棕色
		fillrectangle(rect1_x1, rect1_y1, rect1_x2, rect1_y2);
		fillrectangle(rect2_x1, rect2_y1, rect2_x2, rect2_y2);

		setfillcolor(RED);
		fillrectangle(0, 0, w, 10);
		fillrectangle(0, h - 10, w, h);

		//打印分数
		TCHAR s[20];
		_stprintf(s, _T("%d"), score);
		settextstyle(40, 0, _T("Arial"));
		outtextxy(50, 30, s);
		Sleep(10);
	}
	closegraph();
}

void GameOver() {
	float w, h;//游戏画面宽高与重力加速度
	w = 600;
	h = 400;
	initgraph(w, h);//创建一个窗口
	settextstyle(20, 0, _T("Arial"));
	outtextxy(w / 10, h / 4 - 20, _T("歷史成績"));
	TCHAR text[50];
	_stprintf(text, _T("#1:%d"), historyscore[2]);
	outtextxy(w / 10, h / 4, text);
	_stprintf(text, _T("#2:%d"), historyscore[1]);
	outtextxy(w / 10, h / 4 + 20, text);
	_stprintf(text, _T("#3:%d"), historyscore[0]);
	outtextxy(w / 10, h / 4 + 40, text);

	outtextxy(w / 10, h / 20, _T("Press any key to start the game"));
	_getch(); // 等待用戶按下任意鍵
	cleardevice(); // 清除畫面
}

void ReadHistoryScore() {
	FILE* savefile;
	savefile = fopen("scoresave.txt", "r+");
	if (savefile == NULL) {
		printf("Error opening file.\n");
		return;
	}

	for (int i = 0; i < 3; i++) {
		if (fscanf(savefile, " %d", &historyscore[i]) != 1) {
			// 處理讀取失敗的情況
			printf("Error reading from file.\n");
			break;
		}
	}

	fclose(savefile);
}

void CkeckAndUpdateHistoryScore() {

	if ((score > historyscore[0]) && (score != historyscore[0]) && (score != historyscore[1]) && (score != historyscore[2])) {
		historyscore[0] = score;
		qsort(historyscore, 3, sizeof(int), compare);
	}
	else
	{
		return;
	}
	FILE* savefile;
	savefile = fopen("scoresave.txt", "w");
	if (savefile == NULL) {
		printf("Error opening file.\n");
		return;
	}

	for (int i = 0; i < 3; i++) {
		fprintf(savefile, "%d ", historyscore[i]);
	}

	fclose(savefile);
}

int compare(const void *a, const void *b) {
	return (*(int*)a - *(int*)b);
}
