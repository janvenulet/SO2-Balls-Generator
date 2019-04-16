#include<ncurses.h>
#include <vector>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <thread>
#include "Ball.h"
#include <mutex>
#include <condition_variable>

using namespace std;
 
mutex mtx, mtx2;
condition_variable cv; 
vector <Ball*> balls;
vector <thread> ballsThreads;
int numberOfBalls = 30;
int timeInterval = 1000; //miliseconds beetwen balls generation
int windowX, windowY, mutexX, mutexY;
bool runningLoop = true;
int mutexHeight = 8;
bool ballWaitingForMutex = false;
int ballsInMutex=0;

void pressButtonToExit()
{
	while (runningLoop) 
	{
		char key = getch();
		if (key == '\n')
		runningLoop = false;
	}
	//ballWaitingForMutex=false;  tu rozwiazac problem wyjscia
}



bool ballInMutex(int ball)
{
	int ballX = balls[ball]->getX();
	int ballY = balls[ball]->getY();

	if ( (ballX > mutexX && ballX <= mutexX + 10) && (ballY > mutexY && ballY <= mutexY + mutexHeight)) //10 to ilosc znakow z lancuchu ____
		return true;
	else 
		return false;
}

void ballThreadFunction(int ball)
{
	bool ballLeavingMutex = false;
	while(runningLoop)
	{
		balls[ball]->move();
		this_thread::sleep_for(std::chrono::milliseconds(50));
		if (ballInMutex(ball)) 
		{ 
			if (ballLeavingMutex)
				continue;
			if (mtx2.try_lock())
			{
				if (ballsInMutex > 0) 
					ballWaitingForMutex = true;
				ballsInMutex++;
				while (ballsInMutex == 2)
				{
					this_thread::sleep_for(std::chrono::milliseconds(50));
				}
				mtx2.unlock();
			}
			else 
				continue;
			// if (mtx.try_lock())
			// {
			// 	ballWaitingForMutex = false;
			// 	while (!ballWaitingForMutex)
			// 	{
			// 		this_thread::sleep_for(std::chrono::milliseconds(100));
			// 	} 
			// 	mtx.unlock();				
			// }
			// else 
		
			mtx.lock();
			while (!ballWaitingForMutex && runningLoop)
			{
				this_thread::sleep_for(std::chrono::milliseconds(50));
			} 
			ballLeavingMutex = true;
			ballWaitingForMutex = false;
			ballsInMutex--;
			mtx.unlock();
			
							
		}
		else 
			ballLeavingMutex = false;
	}
}

void generateBalls()
{
	getmaxyx(stdscr, windowY, windowX);
	for (int i = 0 ; i < numberOfBalls; i++) 
	{
		balls.push_back(new Ball(windowX/2, windowY/2));
		ballsThreads.push_back(std::thread(ballThreadFunction, i));
		if (!runningLoop) break;
		this_thread::sleep_for(std::chrono::milliseconds(timeInterval));
	}
}

void drawRectangleMutex()
{
	getmaxyx(stdscr, windowY, windowX);	
	mutexX = windowX/4;
	mutexY = windowY/2;
	mvaddstr(mutexY, mutexX, "__________"); //10 "podlog"
	mvaddstr(mutexY + mutexHeight, mutexX, "__________");
	for (int i = 1; i <= mutexHeight ; i++) 
	{
		mvaddch(mutexY + i, mutexX, '|');
		mvaddch(mutexY + i, mutexX +10, '|');
	}
}

void renderScreen()
{
	while (runningLoop)
	{
		clear();
		drawRectangleMutex();

		for (int i = 0; i < balls.size(); i++)
		{
			mvprintw(balls[i]->getY(), balls[i]->getX(), "o");	
		}
		refresh();
		std::this_thread::sleep_for(std::chrono::milliseconds(15));
	}
}

int main(int argc, char const * argv [])
{	

	srand (time(NULL));
	initscr();
	curs_set(0); //Cursor visability
	thread renderScreenThread(renderScreen);
	thread generateBallsThread(generateBalls);
	thread pressButtonToExitThread(pressButtonToExit);
	

	renderScreenThread.join();
	generateBallsThread.join();
	pressButtonToExitThread.join();

	for (int i = 0; i < ballsThreads.size(); i++)
	{
		ballsThreads[i].join();
		
	}
	
	endwin();
	return 0; 
}
