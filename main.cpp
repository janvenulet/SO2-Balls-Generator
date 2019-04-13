#include<ncurses.h>
#include <vector>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <thread>
#include "Ball.h"

using namespace std;

vector <Ball*> balls;
vector <thread> ballsThreads;
int numberOfBalls = 5;
int timeInterval = 1000; //miliseconds beetwen balls generation
int windowX, windowY;
bool runningLoop = true;

void pressButtonToExit()
{
	while (runningLoop) 
	{
		char key = getch();
		if (key == '\n')
		runningLoop = false;
	}
}

void ballThreadFunction(int ball)
{
	while(runningLoop)
	{
		balls[ball]->move();
		this_thread::sleep_for(std::chrono::milliseconds(40));
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

void renderScreen()
{
	while (runningLoop)
	{
		clear();
		for (int i = 0; i < balls.size(); i++)
		{
			mvprintw(balls[i]->getY(), balls[i]->getX(), "o");	
		}
		refresh();
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
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
