#pragma once
#include "../vector2.h"
#include "../vector3.h"
#ifndef _GRAPHICS_H_
    #define _GRAPHICS_H_
    #include <graphics.h>
#endif // !_GRAPHICS_H_

class Line2D;
class Canva;

class Gemobj
{
public:
    virtual void draw(const Canva& c) const = 0;
};

class Canva
{
public:
    // 帧缓存区指针
    DWORD* pImg = nullptr;
    // 画布宽度
    int width = -1;
    // 画布高度
    int height = -1;

    Canva() :
        pImg(GetImageBuffer()),
        width(GetSystemMetrics(SM_CXSCREEN)),
        height(GetSystemMetrics(SM_CYSCREEN))
    {}
    Canva(DWORD* pImg, int width, int height) :
        pImg(pImg), width(width), height(height) {}
    ~Canva() {}

    void draw(const Gemobj& g)
    {
        g.draw(*this);
    }
};

class Point2D
{
public:
    Vector2 position;
    Vector3 color;
    Point2D() {}
    Point2D(const Vector2& position, const Vector3 color = (0,0,0)) :
        position(position), color(color) {}
    Point2D(float x, float y, const Vector3 color = (0, 0, 0))
    {
        *this = Point2D(Vector2(x, y), color);
    }
    ~Point2D() {}
};

class Line2D : public Gemobj
{
public:
    Point2D v[2];
    /*const static enum type
    {
        SOLID = 0xFFFF,
        DASH = 0xF0F0,
        DASHDOT = 0xE4E4,
        DASHDOTDOT = 0xFF24
    };
    int width = 1;*/

    Line2D(const Point2D& p0, const Point2D& p1)
    {
        Point2D p0_, p1_;
        if (p0.position.x <= p1.position.x)
        {
            p0_ = p0;
            p1_ = p1;
        }
        else
        {
            p0_ = p1;
            p1_ = p0;
        }
        this->v[0] = p0_;
        this->v[1] = p1_;
    }
    Line2D(float a, float b, float c, float d)
    {
        *this = Line2D(Point2D(a, b), Point2D(c, d));
    }
    ~Line2D() {}

    float function(float x, float y) const
    {
        return (v[1].position.x - v[0].position.x) * y -
            (v[1].position.y - v[0].position.y) * x -
            v[1].position.x * v[0].position.y + v[0].position.x * v[1].position.y;
    }

    virtual void draw(const Canva& c) const
    {
        Line2D l = *this;
        int x0 = v[0].position.x + .5,
            y0 = v[0].position.y + .5,
            x1 = v[1].position.x + .5,
            y1 = v[1].position.y + .5,
            x = x0,
            y = y0;
        int y1_y0 = y1 - y0,
            x1_x0 = x1 - x0;
        float k = (float)y1_y0 / x1_x0;
        if (k <= -1)
        {
            float d = function(x + 0.5, y - 1);
            for (; y > y1; y--)
            {
                float t = (float)((x - x0) * (x - x0) + (y - y0) * (y - y0)) /
                    (x1_x0 * x1_x0 + y1_y0 * y1_y0);
                int index = y * c.width + x;
                Vector3 color = v[0].color * (1 - t) + v[1].color * t;
                c.pImg[index] = RGB((int)(color.z * 255),
                                    (int)(color.y * 255),
                                    (int)(color.x * 255));
                if (d < 0)
                {
                    x++;
                    d += -x1_x0 - y1_y0;
                }
                else
                {
                    d += -x1_x0;
                }
            }
        }
        else if (k <= 0)
        {
            float d = function(x + 1, y - 0.5);
            for (; x < x1; x++)
            {
                float t = (float)((x - x0) * (x - x0) + (y - y0) * (y - y0)) /
                    (x1_x0 * x1_x0 + y1_y0 * y1_y0);
                int index = y * c.width + x;
                Vector3 color = v[0].color * (1 - t) + v[1].color * t;
                c.pImg[index] = RGB((int)(color.z * 255),
                                    (int)(color.y * 255),
                                    (int)(color.x * 255));
                if (d > 0)
                {
                    y--;
                    d += -x1_x0 - y1_y0;
                }
                else
                {
                    d += -y1_y0;
                }
            }
        }
        else if (k <= 1)
        {
            float d = function(x + 1, y + 0.5);
            for (; x < x1; x++)
            {
                float t = (float)((x - x0) * (x - x0) + (y - y0) * (y - y0)) /
                    (x1_x0 * x1_x0 + y1_y0 * y1_y0);
                int index = y * c.width + x;
                Vector3 color = v[0].color * (1 - t) + v[1].color * t;
                c.pImg[index] = RGB((int)(color.z * 255),
                                    (int)(color.y * 255),
                                    (int)(color.x * 255));
                if (d < 0)
                {
                    y++;
                    d += x1_x0 - y1_y0;
                }
                else
                {
                    d += -y1_y0;
                }
            }
        }
        else if (k > 1)
        {
            float d = function(x + 0.5, y + 1);
            for (; y < y1; y++)
            {
                float t = (float)((x - x0) * (x - x0) + (y - y0) * (y - y0)) /
                    (x1_x0 * x1_x0 + y1_y0 * y1_y0);
                int index = y * c.width + x;
                Vector3 color = v[0].color * (1 - t) + v[1].color * t;
                c.pImg[index] = RGB((int)(color.z * 255),
                                    (int)(color.y * 255),
                                    (int)(color.x * 255));
                if (d > 0)
                {
                    x++;
                    d += x1_x0 - y1_y0;
                }
                else
                {
                    d += x1_x0;
                }
            }
        }
        else
        {
            return;
        }
    }
};