#pragma once
#ifndef _canvas_H_
#define _canvas_H_
#include "canvas.h"
#endif // !_canvas_H_
enum class EditMode
{
    SELECT_MODEL,
    SELECT_TRIANGLE,
    CUBOID
};

/*
 * models.h
 * 定义各类几何形体。
 */


class Model
{
public:
    #pragma region 构造/析构函数
    Model() {}
    Model(const vector<Vertex3D>& vertexs,
          const vector<int>& triangleReferenceIndex,
          const Vector3f& position,
          const Vector3f& ambient,
          float rotation[],
          float scale[]) :
        vertexs(vertexs), triangleReferenceIndex(triangleReferenceIndex), position(position), ambient(ambient)
    {

        this->rotation[0] = rotation[0];
        this->rotation[1] = rotation[1];
        this->rotation[2] = rotation[2];

        this->scale[0] = scale[0];
        this->scale[1] = scale[1];
        this->scale[2] = scale[2];
    }
    Model(const vector<Vertex3D>& vertexs,
          const vector<int>& triangleReferenceIndex,
          const Vector3f& position = { 0,0,0 },
          const Vector3f& ambient = { 0,0,0 }) :
        vertexs(vertexs), triangleReferenceIndex(triangleReferenceIndex), position(position), ambient(ambient)
    {

        this->rotation[0] = 0;
        this->rotation[1] = 0;
        this->rotation[2] = 0;

        this->scale[0] = 1;
        this->scale[1] = 1;
        this->scale[2] = 1;
    }
    ~Model() {}
    #pragma endregion

    vector<Vertex3D>& modelTransform(vector<Vertex3D>& vertexs) const
    {
        Matrix4f s;
        s <<
            scale[0], 0, 0, 0,
            0, scale[1], 0, 0,
            0, 0, scale[2], 0,
            0, 0, 0, 1;
        Angle x = rotation[0],
            y = rotation[1],
            z = rotation[2];
        Matrix4f rx, ry, rz;
        rx <<
            1, 0, 0, 0,
            0, qcos(x), -qsin(x), 0,
            0, qsin(x), qcos(x), 0,
            0, 0, 0, 1;
        ry <<
            qcos(y), 0, qsin(y), 0,
            0, 1, 0, 0,
            -qsin(y), 0, qcos(y), 0,
            0, 0, 0, 1;
        rz <<
            qcos(z), -qsin(z), 0, 0,
            qsin(z), qcos(z), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;
        Matrix4f r = rz * ry * rx;
        Matrix4f t;
        t <<
            1, 0, 0, position.x(),
            0, 1, 0, position.y(),
            0, 0, 1, position.z(),
            0, 0, 0, 1;
        Matrix4f rs = r * s;
        Matrix4f m = t * rs;
        for (auto& v : vertexs)
        {
            v.premultiMatrix(m);
        }

        return vertexs;
    }
    vector<Vertex3D> getVertexsCopy() const
    {
        return vertexs;
    }
    set<Triangle3D*> prepareAndGetTriangles()
    {
        set<Triangle3D*> triangles;
        for (int i = 0; i + 2 < triangleReferenceIndex.size(); i += 3)
        {

            if (vertexs[triangleReferenceIndex[i]].culled ||
                vertexs[triangleReferenceIndex[i + 1]].culled ||
                vertexs[triangleReferenceIndex[i + 2]].culled)
            {
                continue;
            }
            Triangle3D* item = new Triangle3D(&vertexs[triangleReferenceIndex[i]],
                                            &vertexs[triangleReferenceIndex[i + 1]],
                                            &vertexs[triangleReferenceIndex[i + 2]]);
            pair<set<Triangle3D*>::iterator, bool> p = triangles.insert(item);
            if (p.second == false)
            {
                delete item;
            }
        }
        return triangles;
    }
    set<Triangle3D*> prepareAndGetTriangles(vector<Vertex3D>& vertexs) const
    {
        set<Triangle3D*> triangles;
        for (int i = 0; i + 2 < triangleReferenceIndex.size(); i += 3)
        {

            if (vertexs[triangleReferenceIndex[i]].culled ||
                vertexs[triangleReferenceIndex[i + 1]].culled ||
                vertexs[triangleReferenceIndex[i + 2]].culled)
            {
                continue;
            }
            Triangle3D* item = new Triangle3D(&vertexs[triangleReferenceIndex[i]],
                                              &vertexs[triangleReferenceIndex[i + 1]],
                                              &vertexs[triangleReferenceIndex[i + 2]]);
            pair<set<Triangle3D*>::iterator, bool> p = triangles.insert(item);
            if (p.second == false)
            {
                delete item;
            }
        }
        return triangles;
    }

    void setBorderline(const vector<int>& index)
    {
        borderlineIndex = index;
    }
    void drawTriangles(vector<Vertex3D>& vertexs, const Canvas& c, float* depthBuffer, const Gemobj3D** refBuffer)
    {
        for (auto& t : prepareAndGetTriangles(vertexs))
        {
            c.draw(*t, depthBuffer, refBuffer);
            delete t;
        }
    }
    void drawBorderlines(vector<Vertex3D>& vertexs, const Canvas c, const Gemobj3D** refBuffer)
    {
        for (int i = 0; i + 1 < borderlineIndex.size(); i += 2)
        {
            c.draw(Line3D(&vertexs[borderlineIndex[i]], &vertexs[borderlineIndex[i + 1]]), nullptr, refBuffer);
        }
    }

    Vector3f position;
    Angle rotation[3] = { };
    float scale[3] = { 1,1,1 };
    Vector3f ambient;
    vector<Vertex3D> vertexs;
protected:
    vector<int> triangleReferenceIndex;
    vector<int> borderlineIndex;
};
class Lines : public Model
{
public:
    Lines()
    {
        this->position = { 0,0,0 };
        this->ambient = { 0,0,0 };
    }
    Lines(vector<Vertex3D> vertexs)
    {
        *this = Lines();
        this->vertexs = vertexs;
        for (int i = 0; i < vertexs.size(); i++)
        {
            borderlineIndex.push_back(i);
        }
    }
    void addLine(const Vector3f& pos1, const Vector3f& pos2)
    {
        vertexs.push_back(Vertex3D(pos1));
        vertexs.push_back(Vertex3D(pos2));
        if (borderlineIndex.size() != 0)
        {
            borderlineIndex.push_back(borderlineIndex.back() + 1);
            borderlineIndex.push_back(borderlineIndex.back() + 1);
        }
        else
        {
            borderlineIndex = { 0,1 };
        }
    }
    void popLine_back()
    {
        vertexs.pop_back();
        vertexs.pop_back();
        borderlineIndex.pop_back();
        borderlineIndex.pop_back();
    }
};
class Rectangle_xz : public Model
{
public:
    Rectangle_xz(const Vector3f& position, const Vector3f& bia)
    {
        this->position = position;
        this->ambient = { 0,0,0 };
        Vector3f a = { bia.x(), 0 ,0 };
        Vector3f b = { 0, 0, bia.z() };
        this->vertexs = {
            /*0*/ Vertex3D(0,0,0),
            /*1*/ Vertex3D(a),
            /*2*/ Vertex3D(bia),
            /*3*/ Vertex3D(b) };
        this->triangleReferenceIndex = { 0,1,2, 2,3,0 };
        this->borderlineIndex = { 0,1, 1,2, 2,3, 3,0 };
    }
};
class Cuboid : public Model
{
public:
    Cuboid(float x0,float z0, float x1, float z1, float y0, float height)
    {
        this->position = { x0,y0,z0 };
        this->ambient = { 0,0,0 };
        this->vertexs = {
            /*0*/ Vertex3D(Vector3f(0,0,0)),
            /*1*/ Vertex3D(Vector3f(x1-x0,0,0)),
            /*2*/ Vertex3D(Vector3f(x1-x0,0,z1-z0)),
            /*3*/ Vertex3D(Vector3f(0,0,z1-z0)),
            /*4*/ Vertex3D(0, height, 0),
            /*5*/ Vertex3D(x1-x0, height, 0),
            /*6*/ Vertex3D(x1-x0, height, z1-z0),
            /*7*/ Vertex3D(0, height, z1-z0)
        };
        this->triangleReferenceIndex = {
            0,1,2, 2,3,0, 4,5,6, 6,7,4,
            4,0,1, 1,5,4, 5,1,2, 2,6,5,
            6,2,3, 3,7,6, 7,3,0, 0,4,7
        };
        this->borderlineIndex = { 0,1, 1,2, 2,3, 3,0, 4,5, 5,6, 6,7, 7,4, 0,4, 1,5, 2,6, 3,7 };
    }
    void drawBorderlines(vector<Vertex3D>& vertexs, const Canvas c, const Gemobj3D** refBuffer)
    {
        for (int i = 0; i + 1 < 4; i++)
        {
            c.draw(Line3D(&vertexs[i], &vertexs[i + 1]), nullptr, refBuffer);
        }
        for (int i = 4; i + 1 < 8; i++)
        {
            c.draw(Line3D(&vertexs[i], &vertexs[i + 1]), nullptr, refBuffer);
        }
        for (int i = 0; i + 4 < 8; i++)
        {
            c.draw(Line3D(&vertexs[i], &vertexs[i + 4]), nullptr, refBuffer);
        }
    }
};