#pragma region HEADER
#pragma once

#include"Eigen/Dense"
using namespace Eigen;

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_
#include <graphics.h>
#endif // !_GRAPHICS_H_  

class Canvas;
#pragma endregion

#pragma region 2D 图元与画布
class Vertex2D
{
public:

    #pragma region 构造/析构方法
    Vertex2D() {}
    Vertex2D(float x, float y, float r = 0, float g = 0, float b = 0, float a = 0)
    {
        position = Vector2f(x, y);
        color = Vector4f(r, g, b, a);
    }
    Vertex2D(const Vector2f& position, const Vector4f& color = Vector4f(0, 0, 0, 0)) :
        position(position), color(color)
    {}
    ~Vertex2D()
    {}
    #pragma endregion

    Vector2f position;
    Vector4f color;
};
class Gemobj2D
{
public:
    virtual void draw(const Canvas& c) const = 0;
};
class Canvas
{
public:
    #pragma region 构造/析构方法
    Canvas() :
        pImg(GetImageBuffer()),
        width(GetSystemMetrics(SM_CXSCREEN)),
        height(GetSystemMetrics(SM_CYSCREEN))
    {}
    Canvas(DWORD* pImg, int width, int height) :
        pImg(pImg), width(width), height(height)
    {}
    ~Canvas() {}
    #pragma endregion

    #pragma region 重写
    void draw(const Gemobj2D& g) const
    {
        g.draw(*this);
    }
    #pragma endregion

    // 帧缓存区指针
    DWORD* pImg = nullptr;
    int width = -1;
    int height = -1;
};
class Line2D : Gemobj2D
{
public:

    #pragma region 构造/析构方法
    Line2D()
    {}
    Line2D(float x1, float y1, float x2, float y2,
           float color1r = 0, float color1g = 0, float color1b = 0, float color1a = 0,
           float color2r = 0, float color2g = 0, float color2b = 0, float color2a = 0)
    {
        *this = Line2D(Vertex2D(x1, y1, color1r, color1g, color1b, color1a),
                       Vertex2D(x2, y2, color2r, color2g, color2b, color2a));
    }
    Line2D(const Vertex2D& v0, const Vertex2D& v1)
    {
        Vertex2D v0_, v1_;
        if (v0.position.x() <= v1.position.x())
        {
            v0_ = v0;
            v1_ = v1;
        }
        else
        {
            v0_ = v1;
            v1_ = v0;
        }
        this->v[0] = v0_;
        this->v[1] = v1_;
    }
    ~Line2D()
    {}
    #pragma endregion

    #pragma region 重写
    virtual void draw(const Canvas& c) const
    {
        Line2D l = *this;
        int x0 = v[0].position.x() + .5,
            y0 = v[0].position.y() + .5,
            x1 = v[1].position.x() + .5,
            y1 = v[1].position.y() + .5,
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
                    evaluateColor(x, x0, y, y0, x1_x0, y1_y0, c);
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
                    evaluateColor(x, x0, y, y0, x1_x0, y1_y0, c);
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
                    evaluateColor(x, x0, y, y0, x1_x0, y1_y0, c);
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
                    evaluateColor(x, x0, y, y0, x1_x0, y1_y0, c);
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
    #pragma endregion

private:
    #pragma region 重构提取的函数
    inline void evaluateColor(int x, int x0,
                              int y, int y0,
                              int x1_x0, int y1_y0,
                              const Canvas& c) const
    {
        float t = sqrt((float)((x - x0) * (x - x0) + (y - y0) * (y - y0)) /
                       (x1_x0 * x1_x0 + y1_y0 * y1_y0));
        Vector4f color = v[0].color * (1 - t) + v[1].color * t;
        int index = y * c.width + x;
        DWORD color0RGB = c.pImg[index];
        Vector4f color0(GetRValue(color0RGB),
                        GetGValue(color0RGB),
                        GetBValue(color0RGB), 1);

        color = color0 * (1 - color.w()) + color * color.w();

        c.pImg[index] = RGB((int)(color.z() * 255),
                            (int)(color.y() * 255),
                            (int)(color.x() * 255));
    }
    inline float function(float x, float y) const
    {
        float x1 = v[0].position.x();
        float x2 = v[1].position.x();
        float y1 = v[0].position.y();
        float y2 = v[1].position.y();

        return (x2 - x1) * y - (y2 - y1) * x - x2 * y1 + x1 * y2;
    }
    #pragma endregion

    Vertex2D v[2];
};
class Triangle2D : Gemobj2D
{
public:
    #pragma region 构造/析构方法
    Triangle2D()
    {}
    Triangle2D(const Vertex2D& v0, const Vertex2D& v1, const Vertex2D& v2)
    {
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
    }
    Triangle2D(float x1, float y1,
               float x2, float y2,
               float x3, float y3,
               float color1r = 0, float color1g = 0, float color1b = 0, float color1a = 0,
               float color2r = 0, float color2g = 0, float color2b = 0, float color2a = 0,
               float color3r = 0, float color3g = 0, float color3b = 0, float color3a = 0)
    {
        *this = Triangle2D(
            Vertex2D(x1, y1, color1r, color1g, color1b, color1a),
            Vertex2D(x2, y2, color2r, color2g, color2b, color2a),
            Vertex2D(x3, y3, color3r, color3g, color3b, color3a)
        );
    }
    ~Triangle2D()
    {}
    #pragma endregion

    #pragma region 重写
    virtual void draw(const Canvas& C) const
    {
        int x0 = v[0].position.x(),
            x1 = v[1].position.x(),
            x2 = v[2].position.x(),
            y0 = v[0].position.y(),
            y1 = v[1].position.y(),
            y2 = v[2].position.y();
        int x_min = min(min(x0, x1), x2),
            x_max = max(max(x0, x1), x2),
            y_min = min(min(y0, y1), y2),
            y_max = max(max(y0, y1), y2);
        for (int x = x_min; x <= x_max; x++)
        {
            for (int y = 0; y <= y_max; y++)
            {
                if ((unsigned)y >= C.height || (unsigned)x >= C.width)
                {
                    continue;
                }
                float a = x1 - x0,
                    b = x2 - x0,
                    c = y1 - y0,
                    d = y2 - y0,
                    e = x - x0,
                    f = y - y0;
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
                evaluateColor(x, y, beta, gamma, C);
            }
        }
    }
    #pragma endregion

private:
    inline void evaluateColor(int x, int y, float& beta, float& gamma, const Canvas& C) const
    {
        int index = y * C.width + x;
        Vector4f color = v[0].color * (1 - beta - gamma) +
            v[1].color * beta +
            v[2].color * gamma;

        DWORD color0RGB = C.pImg[index];
        Vector4f color0(GetRValue(color0RGB),
                        GetGValue(color0RGB),
                        GetBValue(color0RGB), 1);

        color = color0 * (1 - color.w()) + color * color.w();
        C.pImg[index] = RGB((int)(color.z() * 255),
                            (int)(color.y() * 255),
                            (int)(color.x() * 255));
    }
    Vertex2D v[3];
};
#pragma endregion
#pragma region 3D 图元
class Vertex3D
{
public:

    #pragma region 构造/析构方法
    Vertex3D() {}
    Vertex3D(float x, float y, float z, float r = 0, float g = 0, float b = 0, float a = 0)
    {
        position = Vector3f(x, y, z);
        color = Vector4f(r, g, b, a);
    }
    Vertex3D(const Vector3f& position, const Vector4f& color = Vector4f(0, 0, 0, 0)) :
        position(position), color(color)
    {}
    ~Vertex3D()
    {}
    #pragma endregion

    Vertex3D& premultiMatrix(const Matrix4f& transformMatrix)
    {
        Vector4f homogeneous(this->position.x(),
                             this->position.y(),
                             this->position.z(), 1);
        homogeneous = transformMatrix * homogeneous;
        float w = homogeneous.w();
        this->position = Vector3f(homogeneous.x() / w,
                                  homogeneous.y() / w,
                                  homogeneous.z() / w);
        return *this;
    }

    Vector3f position;
    Vector4f color;
};
class Gemobj3D
{
public:
    virtual Gemobj3D& premultiMatrix(const Matrix4f&) = 0;
};
class Triangle3D : Gemobj3D
{
public:
    #pragma region 构造/析构方法
    Triangle3D()
    {}
    Triangle3D(const Vertex3D& v0, const Vertex3D& v1, const Vertex3D& v2)
    {
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
    }

    Triangle3D(float x1, float y1, float z1,
               float x2, float y2, float z2,
               float x3, float y3, float z3,
               float color1r = 0, float color1g = 0, float color1b = 0, float color1a = 0,
               float color2r = 0, float color2g = 0, float color2b = 0, float color2a = 0,
               float color3r = 0, float color3g = 0, float color3b = 0, float color3a = 0)
    {
        *this = Triangle3D(
            Vertex3D(x1, y1, z1, color1r, color1g, color1b, color1a),
            Vertex3D(x2, y2, z2, color2r, color2g, color2b, color2a),
            Vertex3D(x3, y3, z3, color3r, color3g, color3b, color3a)
        );
    }
    ~Triangle3D()
    {}
    #pragma endregion

    #pragma region 重写
    virtual Triangle3D& premultiMatrix(const Matrix4f& t)
    {
        v[0].premultiMatrix(t);
        v[1].premultiMatrix(t);
        v[2].premultiMatrix(t);
        return *this;
    }
    #pragma endregion

    Vertex3D v[3];
};
#pragma endregion

class Camara
{
public:
    #pragma region 构造/析构方法
    Camara() {}
    Camara(const Vector3f& eyePosition,
           const Vector3f& topDirection,
           const Vector3f& gazeDirection) :
        eyePosition(eyePosition),
        topDirection(topDirection.normalized()),
        gazeDirection(gazeDirection.normalized())
    {}
    Camara(float ex, float ey, float ez,
           float tx, float ty, float tz,
           float gx, float gy, float gz)
    {
        *this = Camara(Vector3f(ex, ey, ez), Vector3f(tx, ty, tz), Vector3f(gx, gy, gz));
    }
    ~Camara() {}
    #pragma endregion

    Vector3f eyePosition;
    Vector3f topDirection;
    Vector3f gazeDirection;
};
