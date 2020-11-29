#ifndef _models_H_
#define _models_H_
#include "models.h"
#endif // !_models_H_

enum class DisplayMode
{
    LINES,
    SURFACES
};

#pragma region 光照
class Light
{
public:
    Light() {}
    ~Light() {}

    Vector3f intensity = Vector3f(0, 0, 0);
};
class PointLight : public Vertex3D, public Light
{
public:
    PointLight(const Vector3f& position, const Vector3f& intensity)
    {
        this->position = position;
        this->intensity = intensity;
    }
    PointLight(float x, float y, float z, float r, float g, float b)
    {
        position = Vector3f(x, y, z);
        intensity = Vector3f(r, g, b);
    }
    ~PointLight() {}
};
class AreaLight : public Model, public Light
{
public:
    AreaLight(const Vector3f& intensity, const list<Gemobj3D>& triangles, const Vector3f& position, float rotation[], float scale[])
    {
        this->intensity = intensity;
    }
    AreaLight(float r, float g, float b)
    {
        intensity = Vector3f(r, g, b);
    }
    ~AreaLight() {}
};
#pragma endregion

class World_Shader
{
public:
    World_Shader(const Canvas& canvas, const Camera& camera) :
        canvas(canvas), camera(camera)
    {
        displayMode = DisplayMode::LINES;
        viewMode = ViewMode::PERSPECTIVE;
        rplain = canvas.width;
        tplain = canvas.height;

        depthBuffer = new float[canvas.width * canvas.height];
        constDepthBuffer = new float[canvas.width * canvas.height];
        refBuffer = new const Gemobj3D*[canvas.width * canvas.height];
        constRefBuffer = new const Gemobj3D*[canvas.width * canvas.height];
        for (int x = 0; x < canvas.width; x++)
        {
            for (int y = 0; y < canvas.height; y++)
            {
                constDepthBuffer[y * canvas.width + x] = -1;
                constRefBuffer[y * canvas.width + x] = nullptr;
            }
        }
    }
    ~World_Shader() {}

    Canvas canvas;
    Camera camera;
    DisplayMode displayMode;
    ViewMode viewMode;
    float rplain, tplain;
    constexpr static const float fplain = -5000;

    float* depthBuffer;
    float* constDepthBuffer;
    const Gemobj3D** refBuffer;
    const Gemobj3D** constRefBuffer;

    void addModel(Model* model)
    {
        models.push_back(model);
    }
    void removeModel(Model* model)
    {
        for (int i = 0; i < models.size(); i++)
        {
            vector<Model*>::iterator it;
            for (it = models.end() - 1; it != models.begin(); it--)
            {
                if (*it == model)
                {
                    models.erase(it);
                }
            }
            if (*it == model)
            {
                models.erase(it);
            }
        }
    }
    void addPointLight(PointLight* pl)
    {
        pointLights.push_back(pl);
    }
    void removePointLight(PointLight* pl)
    {
        for (int i = 0; i < models.size(); i++)
        {
            vector<PointLight*>::iterator it;
            for (it = pointLights.end() - 1; it != pointLights.begin(); it--)
            {
                if (*it == pl)
                {
                    pointLights.erase(it);
                }
            }
            if (*it == pl)
            {
                pointLights.erase(it);
            }
        }
    }

    vector<Vector3f> getIndentityPositionInViewport() const
    {
        float w;
        Model model;
        model.vertexs = { Vertex3D(0,0,0), Vertex3D(1,0,0), Vertex3D(0,1,0), Vertex3D(0,0,1) };
        auto vertexs = model.getVertexsCopy();

        canvas.viewportTransform(
            camera.projectionTransform(
                camera.viewTransform(
                    model.modelTransform(vertexs)
                ), rplain, tplain, fplain, w, viewMode)
        );

        assert(vertexs.size() == 4);
        return { vertexs[0].position, vertexs[1].position, vertexs[2].position, vertexs[3].position };
    }

    void vertexAmbientShader() const
    {
        for (auto& model : models)
        {
            for (auto& v : model->vertexs)
            {
                v.color = model->ambient;
            }
        }
    }
    void vertexDiffuseShader() const
    {
        for (auto& model : models)
        {
            for (auto& v : model->vertexs)
            {
                for (auto& pl : pointLights)
                {
                    Vector3f l = pl->position - v.position;
                    l.normalize();
                    // float diffuse = max(0, v.evaluateNormal().dot(l)) / ((l - v.position).norm() * (l - v.position).norm());
                    float diffuse = max(0, -v.evaluateNormal().dot(l));
                    v.color += Vector3f(1 * pl->intensity.x() * diffuse,
                                        1 * pl->intensity.y() * diffuse,
                                        1 * pl->intensity.z() * diffuse);
                    if (v.color.x() > 1)
                    {
                        v.color.x() = 1;
                    }
                    if (v.color.y() > 1)
                    {
                        v.color.y() = 1;
                    }
                    if (v.color.z() > 1)
                    {
                        v.color.z() = 1;
                    }
                    // 此处不能用顶点的原始颜色做系数，否则会多次迭代出bug
                }
            }
        }
    }
    void pixelSpecularShader(const Canvas& c) const
    {
        for (int x = 0; x < c.width; x++)
        {
            for (int y = 0; y < c.height; y++)
            {
                int index = y * c.width + x;
            }
        }
    }

    void draw(bool forcedDrawBordlines = false) const
    {
        cleardevice();
        memcpy(depthBuffer, constDepthBuffer, sizeof(float) * canvas.width * canvas.height);
        memcpy(refBuffer, constRefBuffer, sizeof(Gemobj3D*) * canvas.width * canvas.height);
        float w;
        vector<set<Triangle3D*>> deleteList;
        for (auto& model : models)
        {
            deleteList.push_back(model->prepareAndGetTriangles());
        }
        vertexAmbientShader();
        vertexDiffuseShader();
        for (auto& model : models)
        {
            auto vertexs = model->getVertexsCopy();

            canvas.viewportTransform(
                camera.projectionTransform(
                    camera.viewTransform(
                        model->modelTransform(vertexs)
                    ), rplain, tplain, fplain, w, viewMode)
            );
            if (forcedDrawBordlines || displayMode == DisplayMode::LINES)
            {
                model->drawBorderlines(vertexs, canvas, refBuffer);
            }
            if (displayMode == DisplayMode::SURFACES)
            {
                model->drawTriangles(vertexs, canvas, depthBuffer, refBuffer);
            }
        }
        for (auto& d : deleteList)
        {
            for (auto& t : d)
            {
                delete t;
            }
        }
    }

//private:
    vector<Model*> models;
    vector<PointLight*> pointLights;
    vector<AreaLight*> areaLights;
};
