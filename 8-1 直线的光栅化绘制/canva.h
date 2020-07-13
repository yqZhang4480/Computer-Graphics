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
    Point2D(const Vector2& position, const Vector3& color = (0,0,0)) :
        position(position), color(color) {}
    Point2D(float x, float y, const Vector3& color = (0, 0, 0))
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

    Line2D() {}
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
    Line2D(float a,
        float b,
        float c,
        float d,
        const Vector3& color1 = (1,1,1),
        const Vector3& color2 = (1,1,1) )
    {
        *this = Line2D(Point2D(a, b, color1), Point2D(c, d, color2));
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
                if ((unsigned)y < c.height && (unsigned)x < c.width)
                {
                    float t = sqrt((float)((x - x0) * (x - x0) + (y - y0) * (y - y0)) /
                        (x1_x0 * x1_x0 + y1_y0 * y1_y0));
                    Vector3 color = v[0].color * (1 - t) + v[1].color * t;
                    int index = y * c.width + x;
                    c.pImg[index] = RGB((int)(color.z * 255),
                                        (int)(color.y * 255),
                                        (int)(color.x * 255));
                }
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
                if ((unsigned)y < c.height && (unsigned)x < c.width)
                {
                    float t = sqrt((float)((x - x0) * (x - x0) + (y - y0) * (y - y0)) /
                        (x1_x0 * x1_x0 + y1_y0 * y1_y0));
                    int index = y * c.width + x;
                    Vector3 color = v[0].color * (1 - t) + v[1].color * t;
                    c.pImg[index] = RGB((int)(color.z * 255),
                                        (int)(color.y * 255),
                                        (int)(color.x * 255));
                }
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
                if ((unsigned)y < c.height && (unsigned)x < c.width)
                {
                    float t = sqrt((float)((x - x0) * (x - x0) + (y - y0) * (y - y0)) /
                        (x1_x0 * x1_x0 + y1_y0 * y1_y0));
                    int index = y * c.width + x;
                    Vector3 color = v[0].color * (1 - t) + v[1].color * t;
                    c.pImg[index] = RGB((int)(color.z * 255),
                                        (int)(color.y * 255),
                                        (int)(color.x * 255));
                }
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
                
                if ((unsigned)y < c.height && (unsigned)x < c.width)
                {
                    float t = sqrt((float)((x - x0) * (x - x0) + (y - y0) * (y - y0)) /
                        (x1_x0 * x1_x0 + y1_y0 * y1_y0));
                    int index = y * c.width + x;
                    Vector3 color = v[0].color * (1 - t) + v[1].color * t;
                    c.pImg[index] = RGB((int)(color.z * 255),
                                        (int)(color.y * 255),
                                        (int)(color.x * 255));
                }
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

class Triangle2D : public Gemobj
{
public:
    Point2D p[3];

    Triangle2D() {}
    Triangle2D(const Point2D& p0, const Point2D& p1, const Point2D& p2)
    {
        p[0] = p0;
        p[1] = p1;
        p[2] = p2;
    }
    Triangle2D(float x1,
        float y1,
        float x2,
        float y2,
        float x3,
        float y3,
        const Vector3& color1 = (1, 1, 1),
        const Vector3& color2 = (1, 1, 1),
        const Vector3& color3 = (1, 1, 1))
    {
        *this = Triangle2D(
            Point2D(x1, y1, color1),
            Point2D(x2, y2, color2),
            Point2D(x3, y3, color3)
        );
    }

    virtual void draw(const Canva& C) const
    {
        int x_min = min(min(p[0].position.x, p[1].position.x), p[2].position.x),
            x_max = max(max(p[0].position.x, p[1].position.x), p[2].position.x),
            y_min = min(min(p[0].position.y, p[1].position.y), p[2].position.y),
            y_max = max(max(p[0].position.y, p[1].position.y), p[2].position.y);
        for (int x = x_min; x <= x_max; x++)
        {
            for (int y = 0; y <= y_max; y++)
            {
                if ((unsigned)y >= C.height || (unsigned)x >= C.width)
                {
                    continue;
                }
                float a = p[1].position.x - p[0].position.x,
                      b = p[2].position.x - p[0].position.x,
                      c = p[1].position.y - p[0].position.y,
                      d = p[2].position.y - p[0].position.y,
                      e = x - p[0].position.x,
                      f = y - p[0].position.y;
                float M = a * d - b * c;
                float beta = (e * d - b * f) / M;
                if (beta < 0 || beta > 1)
                {
                    continue;
                }
                float gamma = (a * f - c * e) / M;
                if (gamma < 0 || gamma > 1 || beta + gamma > 1)
                {
                    continue;
                }
                int index = y * C.width + x;
                Vector3 color = p[0].color * (1 - beta - gamma) +
                                p[1].color * beta +
                                p[2].color * gamma;
                C.pImg[index] = RGB((int)(color.z * 255),
                                    (int)(color.y * 255),
                                    (int)(color.x * 255));
            }
        }
    }
};