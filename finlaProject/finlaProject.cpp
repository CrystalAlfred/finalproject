// finlaProject.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。

#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include"game.h"


int main()
{	int paused = 0;
	ReadHistoryScore();
	StartScene();
	while (true)
	{
		GameRunning();
		CkeckAndUpdateHistoryScore();
		GameOver();
	}


	return 0;
}
