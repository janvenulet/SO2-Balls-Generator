#pragma once 
class Ball 
{
    private:
        int X;
        int Y; 
        double speedX;
        double speedY;
    public:
        Ball(int,int);
        ~Ball();
        void move();
        void bounce();
        int getX();
        int getY();
};