Assignment 1a
=============

Student: *Fahed Nasser*


The Code for Bresenham Algorithm
================================

    void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
    {
        double a, e, DeltaX, DeltaY,Temp;
        int x= p1.x, y=p1.y, Loop=p2.x;
        DeltaY = (double)p2.y - p1.y;
        DeltaX = (double)p2.x - p1.x;
        a = DeltaY / DeltaX;
        if (DeltaX < 0 && DeltaY < 0) //Points are Flipped - a>0 
        {

            DrawLine(p2, p1, color);

        }
        if (DeltaY == 0)
        {
            if (p2.x > p1.x)
            {
                x = p1.x;
                while (x < p2.x)
                {
                    PutPixel(x, p1.y, color);
                    x += 1;
                }
            }
            else
            {
                x = p1.x;
                while (x > p2.x)
                {
                    PutPixel(x, p2.y, color);
                    x -= 1;
                }
            }

        }

        else if (a > 0)

        {
            
            if (a > 1) //Switch a>1
            {

                x = p1.y;
                y = p1.x;
                Temp = DeltaX;
                DeltaX = DeltaY;
                DeltaY = Temp;
                e = -DeltaX;
                Loop = p2.y;
                while (x <= Loop)
                {
                    if (e > 0)
                    {
                        y += 1;
                        e -= (2 * DeltaX);
                    }
                    PutPixel(y, x, color);
                    x += 1;
                    e += (2 * DeltaY);
                }

                
            }

            else   //0<a<1
            { 
                e = -DeltaX;

                while (x <= Loop)
                {
                    if (e > 0)
                    {
                        y += 1;
                        e -= (2 * DeltaX);
                    }
                    PutPixel(x, y, color);
                    x += 1;
                    e += (2 * DeltaY);
                }
            }
        }
        else if (a < 0)
        {
            
            if (a < -1) //Switch&Reflect
            {
                x = p1.y;
                y = p1.x;
                e = -DeltaX;
                Temp = DeltaX;
                DeltaX = DeltaY;
                DeltaY = Temp;
                Loop = p2.y;
                if (DeltaY < 0)
                {
                    DeltaX *= (-1);
                    while (x <= Loop)
                    {
                        if (e < 0)
                        {
                            y -= 1;
                            e -= (2 * DeltaX);
                        }
                        PutPixel(y, x, color);
                        x += 1;
                        e += (2 * DeltaY);
                    }
                }
                else
                {
                    DrawLine(p2, p1, color);
                }
                
                
            }
            else
            {
                if (DeltaY < 0)
                {
                    DeltaY *= (-1);
                    e = -DeltaX;
                    while (x <= Loop)
                    {
                        if (e > 0)
                        {
                            y -= 1;
                            e -= (2 * DeltaX);
                        }
                        PutPixel(x, y, color);
                        x += 1;
                        e += (2 * DeltaY);
                    }
                }
                else
                {
                    DeltaX *= (-1);
                    e = -DeltaX;
                    while (x >= Loop)
                    {
                        if (e > 0)
                        {
                            y += 1;
                            e -= (2 * DeltaX);
                        }
                        PutPixel(x, y, color);
                        x -= 1;
                        e += (2 * DeltaY);
                    }
                }
            }
        
        }

    }

A short explaination about my implementation
============================================

Basically, I examined 5 main situations:

i. a &gt;1

ii. 0 &lt; a &lt;1

iii. -1 &gt; a &gt; 0

iv. a &lt;-1

v. DeltaY=0

For each and every situation, I implemented the corresponding equations and algorithm that we learned in the lecture.
For some situations, I was stuck with lighting the correct px and not entering the while loop. So I had to check and change the +- for the x and y, and Delta X and Delta Y.

2 Extraordinary situtaions were when we are given an X coordinate of P2 that is smaller then the X coordinate of P1. So the Delta X &lt; 0 and it was a bit problematic, so I flipped the points and continued with the same division of situations.
If you take a closer look at the code, you can see that it is all based on the same mindset and algorithm given in the lecture.

The fifth situation I handled with a straight forward and easy loop, where I only increased the X values.

Sanity Check Result
===================

![Sanity Check Result- Circle](https://github.com/HaifaGraphicsCourses/computergraphics2021-fahed-nasser/blob/main/Pictures/Circle.jpg)

Creativity at its best
======================

![Creativity](https://github.com/HaifaGraphicsCourses/computergraphics2021-fahed-nasser/blob/main/Pictures/Creative.jpg)
