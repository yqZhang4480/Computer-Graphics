#pragma once
#ifndef _graphics_H_
#define _graphics_H_
#include <graphics.h>
#endif // !_graphic_H_

#ifndef _canvas_H_
#define _canvas_H_
#include "canvas.h"
#endif // !_canvas_H_

#ifndef eigen
#include "Eigen/Dense"
using namespace eigen;
#define eigen
#endif // !eigen

namespace mouse
{
    MOUSEMSG message;
    namespace WorldPosition
    {
        float x = 0, y = 0, z = 0;
    };
    float grid = 10;
    
    void snapToGrid()
    {
        if (grid <= 0)
        {
            return;
        }
        else
        {
            WorldPosition::x = (int)((WorldPosition::x + 0.5) / grid) * grid;
            WorldPosition::y = (int)((WorldPosition::y + 0.5) / grid) * grid;
            WorldPosition::z = (int)((WorldPosition::z + 0.5) / grid) * grid;
        }
    }

    void getMouseMessage()
    {
        message = GetMouseMsg();
    }

    void inputXZ(vector<Vector3f> identity, float y = WorldPosition::y)
    {
        WorldPosition::y = y;
        Vector3f i = identity[1] - identity[0];
        Vector3f j = identity[2] - identity[0];
        Vector3f k = identity[3] - identity[0];
        Vector3f o = {
            identity[0].x() + y * j.x(),
            identity[0].y() + y * j.y(), 0 };
        Vector3f p = Vector3f{ (float)message.x, (float)message.y, 0 } - o;

        float M = i.x() * k.y() - k.x() * i.y();
        WorldPosition::x = (p.x() * k.y() - k.x() * p.y()) / M;
        WorldPosition::z = (i.x() * p.y() - i.y() * p.x()) / M;
        snapToGrid();
    }
    void inputHeight(vector<Vector3f> identity, float x = WorldPosition::x, float z = WorldPosition::z)
    {
        WorldPosition::x = x;
        WorldPosition::z = z;
        Vector3f i = identity[1] - identity[0];
        Vector3f j = identity[2] - identity[0];
        Vector3f k = identity[3] - identity[0];
        Vector3f o = { 
            identity[0].x() + x * i.x() + z * k.x(),
            identity[0].y() + x * i.z() + z * k.z(), 0 };
        Vector3f p = Vector3f{ (float)message.x, (float)message.y, 0 } - o;

        WorldPosition::y = (p.y() * j.y() + p.x() * j.x()) /
            (j.x() * j.x() + j.y() * j.y());
        snapToGrid();
    }
};
