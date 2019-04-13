#include "Ball.h"
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <iostream>


double gravity = 5.0;
double deltaTime = 0.2;


Ball::Ball(int x, int y)
{
    this->X = x;
    this->Y = y;
    int tmp = 0;
    tmp = rand() % 9 - 5;
    this->speedX = tmp;
    tmp = rand() % 10 - 12;
    this->speedY = tmp;
}


void Ball::move()
{
    int screenX, screenY; 
    getmaxyx(stdscr, screenY, screenX);
    screenY--;
    
    if (this->X + speedX <= 0)
    {
        if (this->X + speedX == 0) 
                this->X=0;
        else 
                this->X = abs(this->X + speedX); // speed higher than discance
        speedX *= -1; 
    }
    else if (this->X + speedX >= screenX)
    {
        if (this->X + speedX == screenX)
                this->X=screenX-1;
        speedX *= -1; 
    }
    else
    {
        this->X += speedX; 
    }

    if (this->Y <= 0 )
    {
        if (this->Y + speedY == 0) 
                this->Y=0;
        else 
                this->Y = abs(this->Y + speedY); // speed higher than discance
        speedY *= -1;
    }
    else if ( this->Y == screenY) //surface friction
    {
        speedX *= 0.9;
        if (abs(speedX) < 0.4)
                speedX = 0.0;
    }

    if (this -> Y  > screenY - 1 ) 
    {
        this -> Y = screenY - 1;
        speedY *=  0.50;
        speedY *= -1;
        if (abs(speedY) < 4.70) 
            this->Y = screenY; 
    }
    else 
    {
        speedY +=  gravity*deltaTime;
        this->Y +=  deltaTime*speedY + 0.5 * gravity* deltaTime*deltaTime;
    }
    
    // if (this->getY() + speedY <= 0)
    // {
    //     if (this->getY() + speedY == 0) 
    //             this->Y=0;
    //     else 
    //             this->Y = abs(this->Y + speedY);
    //     speedY *= -1; 
    // }
    // else if (this->getY() + speedY >= screenY)
    // {
    //     if (this->getY() + speedY == 0)
    //             this->Y=0;
    //     else 
    //             this->Y= screenY - abs(screenY - (this->Y + speedY));
    //     speedY *= -1; 
    // }
    // else 
    // {
    //     this->Y += speedY;
    // }
    return;
}

int Ball::getX()
{
    return this->X;
}

int Ball::getY()
{
    return this->Y;
}

Ball::~Ball()
{}
