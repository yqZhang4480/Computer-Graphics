#pragma region HEAD
#include"../shader.h"
#include"../mouse.h"
using namespace mouse;
using namespace mouse::WorldPosition;

#ifndef _graphics_H_
#define _graphics_H_
#include <graphics.h>
#endif // !_graphics_H_

#ifndef _time_H_
#define _time_H_
#include <time.h>
#endif // !_time_H_
#pragma endregion

/**
 * 8    图形流水线
 * 8-3  三维图元的光栅化绘制
 *
 * 绘制三维三角形。
 *
 * 运行环境：Windows，Visual Studio 2019
 * EasyX 图形库版本：20200520（beta）
 *
 * ****  若编译后无响应，请尝试重启设备。  ****
 * ****  若出现编译错误，请确保已正确安装EasyX图形库。  ****
 *
 * Last Edit: 2020/10/27
 */

wchar_t bottomstr[300] = {};
Canvas initFullscreen()
{
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    initgraph(width, height);
    HWND hwnd = GetHWnd();// EasyX库函数，获取窗口句柄
    LONG l_WinStyle = GetWindowLong(hwnd, GWL_STYLE);
    SetWindowLong(hwnd, GWL_STYLE,
                  (l_WinStyle | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);
    SetWindowPos(hwnd, HWND_TOP, 0, 0, width, height, 0);
    DWORD* pImg = GetImageBuffer();
    return Canvas(pImg, width, height);
}

void buildCuboid(World_Shader& ws, Model* ui)
{
    #pragma region 初始化变量
    ui = new Model();
    float x0 = x, height0 = y = 0, z0 = z;
    float x1 = x, z1 = z;
    float height1 = y;
    #pragma endregion

    #pragma region 1/4 底面坐标1
    bool firstLoop = true;
    while (true)
    {
        bool flag = false;
        message = GetMouseMsg();
        switch (message.uMsg)
        {
            case WM_MOUSEMOVE:
                inputXZ(ws.getIndentityPositionInViewport(),0);
                x0 = x; z0 = z;
                break;
            case WM_LBUTTONDOWN:
                inputXZ(ws.getIndentityPositionInViewport());
                x0 = x; z0 = z;
                flag = true;
                break;
            default:
                break;
        }
        delete ui;
        ui = new Lines({ { 0,y,z0 }, { x0,y,z0 },
                         { x0,y,0 }, { x0,y,z0 },
                         { 0,y,0 }, { x0,y,z0 } });
        if (firstLoop)
        {
            ws.models.push_back(ui);
            firstLoop = false;
        }
        ws.models.pop_back();
        ws.models.push_back(ui);
        ws.draw(true);
        if (flag)
        {
            break;
        }
        wsprintf(bottomstr,
                 L"长方体（1/4）：指定底面的第一个点的底面坐标：%d，%d",
                 (int)x0, (int)z0);
        outtextxy(3, ws.canvas.height - 20, bottomstr);
        FlushBatchDraw();
    }
    #pragma endregion
    #pragma region 2/4 底面坐标2
    while (true)
    {
        bool flag = false;
        message = GetMouseMsg();
        switch (message.uMsg)
        {
            case WM_MOUSEMOVE:
                inputXZ(ws.getIndentityPositionInViewport());
                x1 = x; z1 = z;
                break;
            case WM_LBUTTONDOWN:
                inputXZ(ws.getIndentityPositionInViewport());
                x1 = x; z1 = z;
                flag = true;
                break;
            default:
                break;
        }
        delete ui;
        ui = new Rectangle_xz({ x0,height0,z0 }, { x1-x0,0,z1-z0 });
        ws.models.pop_back();
        ws.models.push_back(ui);
        if (flag)
        {
            break;
        }
        ws.draw(true);
        wsprintf(bottomstr,
                 L"长方体（2/4）：指定底面的第二个点的底面坐标：%d，%d",
                 (int)x1, (int)z1);
        outtextxy(3, ws.canvas.height - 20, bottomstr);
        FlushBatchDraw();
    }
    #pragma endregion
    #pragma region 3/4 底面高度
    while (true)
    {
        bool flag = false;
        message = GetMouseMsg();
        switch (message.uMsg)
        {
            case WM_MOUSEMOVE:
                inputHeight(ws.getIndentityPositionInViewport());
                height0 = y;
                break;
            case WM_LBUTTONDOWN:
                inputHeight(ws.getIndentityPositionInViewport());
                height0 = y;
                flag = true;
                break;
            default:
                break;
        }
        delete ui;
        ui = new Rectangle_xz({ x0,height0,z0 }, { x1-x0,0,z1-z0 });
        ws.models.pop_back();
        ws.models.push_back(ui);
        ws.draw(true);
        if (flag)
        {
            break;
        }
        wsprintf(bottomstr, L"长方体（3/4）：指定底面的高：%d", (int)height0);
        outtextxy(3, ws.canvas.height - 20, bottomstr);
        FlushBatchDraw();
    }
    #pragma endregion
    #pragma region 4/4 顶面高度
    while (true)
    {
        bool flag = false;
        message = GetMouseMsg();
        switch (message.uMsg)
        {
            case WM_MOUSEMOVE:
                inputHeight(ws.getIndentityPositionInViewport());
                height1 = y;
                break;
            case WM_LBUTTONDOWN:
                inputHeight(ws.getIndentityPositionInViewport());
                height1 = y;
                flag = true;
                break;
            default:
                break;
        }
        delete ui;
        ui = new Cuboid(x0, z0, x1, z1, height0, height1);
        ws.models.pop_back();
        ws.models.push_back(ui);

        if (flag)
        {
            break;
        }
        ws.draw(true);
        wsprintf(bottomstr,
                 L"长方体（4/4）：指定顶面的高度：%d，%d，%d",
                 (int)x1, (int)z1, (int)height1);
        outtextxy(3, ws.canvas.height - 20, bottomstr);
        FlushBatchDraw();
    }
    #pragma endregion

    wsprintf(bottomstr, L"长方体（4/4）");
    outtextxy(3, ws.canvas.height - 20, bottomstr);
    FlushBatchDraw();
}

int main(int argc, char* argv[])
{
    #pragma region 初始化视图
    Canvas canvas = initFullscreen();
    
    Camera camera(350, 70, 20,
                  0, 1, 0,
                  -175, -35, -10,
                  directionFromDegree[78]);
    World_Shader ws(canvas, camera);
    Cuboid cuboid =  Cuboid(0,  0,  140, 140, 0,   100);
    Cuboid linker =  Cuboid(10, 10, 130, 130, 100, 10);
    Cuboid cuboid1 = Cuboid(0,  0,  140, 140, 110, 100);
    Cuboid linker1 = Cuboid(10, 10, 130, 130, 210, 10);
    Cuboid cuboid2 = Cuboid(0,  0,  140, 140, 220, 100);
    Cuboid linker2 = Cuboid(10, 10, 130, 130, 320, 10);
    Cuboid cuboid3 = Cuboid(0,  0,  140, 140, 330, 100);
    Cuboid linker3 = Cuboid(10, 10, 130, 130, 430, 10);
    Cuboid cuboid4 = Cuboid(0,  0,  140, 140, 440, 100);
    Cuboid linker4 = Cuboid(10, 10, 130, 130, 540, 10);
    Cuboid cuboid5 = Cuboid(0,  0,  140, 140, 550, 100);
    Cuboid linker5 = Cuboid(10, 10, 130, 130, 650, 10);
    Cuboid cuboid6 = Cuboid(0,  0,  140, 140, 660, 100);
    //cuboid.ambient = Vector3f(0.2,0.4,0.2);
    PointLight p = PointLight(400, 800, 400, 0.3, 0.3, 0.3);
    PointLight p1 = PointLight(-400, 800, -400, 0.4, 0.4, 0.3);
    ws.addModel(&cuboid);
    ws.addModel(&linker);
    ws.addModel(&cuboid1);
    ws.addModel(&linker1);
    ws.addModel(&cuboid2);
    ws.addModel(&linker2);
    ws.addModel(&cuboid3);
    ws.addModel(&linker3);
    ws.addModel(&cuboid4);
    ws.addModel(&linker4);
    ws.addModel(&cuboid5);
    ws.addModel(&linker5);
    ws.addModel(&cuboid6);
    ws.addPointLight(&p);
    ws.addPointLight(&p1);
    #pragma endregion

    #pragma region 初始化交互
    ws.displayMode = DisplayMode::LINES;
    DisplayMode userDisplayMode = ws.displayMode;
    EditMode editMode = EditMode::CUBOID;
    message = GetMouseMsg();
    int mousePosX = message.x, d_x = 0,
        mousePosY = message.y, d_y = 0;
    bool rotating = false;
    bool moving = false;
    bool drawDepth = false;
    bool D = false;
    bool V = false;

    LOGFONT f;
    gettextstyle(&f);                     
    f.lfHeight = 17;                      
    wsprintf(f.lfFaceName, L"Segoe UI");
    //f.lfWeight = FW_SEMIBOLD;
    f.lfQuality = ANTIALIASED_QUALITY;
    settextstyle(&f);                     
    #pragma endregion

    #pragma region 绘图循环
    BeginBatchDraw();
    while (true)
    {
        auto startTime = clock();
        list<Triangle3D> t;
        ws.draw();

        #pragma region 回显
        auto endTime = clock();
        wchar_t fpsstr[30] = L"FPS: 999";
        float time = (float)(endTime - startTime) / CLOCKS_PER_SEC;
        if (time > 0)
        {
            wsprintf(fpsstr, L"FPS: %3d", (int)(1 / time));
        }
        outtextxy(0, 0, fpsstr);
        
        outtextxy(3, ws.canvas.height - 20, bottomstr);
        FlushBatchDraw();
        #pragma endregion
        #pragma region 键盘操作

        if (!D && GetAsyncKeyState('D'))
        {
            D = true;
        }
        if (D && !GetAsyncKeyState('D'))
        {
            D = false;
            switch (ws.displayMode)
            {
                case DisplayMode::LINES:
                    ws.displayMode = DisplayMode::SURFACES;
                    userDisplayMode = ws.displayMode;
                    break;
                case DisplayMode::SURFACES:
                    ws.displayMode = DisplayMode::LINES;
                    userDisplayMode = ws.displayMode;
                    break;
            }
            continue;
        }

        if (!V && GetAsyncKeyState('V'))
        {
            V = true;
        }
        if (V && !GetAsyncKeyState('V'))
        {
            V = false;
            switch (ws.viewMode)
            {
                case ViewMode::PERSPECTIVE:
                    ws.viewMode = ViewMode::ORTHOGRAPHIC;
                    ws.displayMode = DisplayMode::LINES;
                    userDisplayMode = ws.displayMode;
                    break;
                case ViewMode::ORTHOGRAPHIC:
                    ws.viewMode = ViewMode::PERSPECTIVE;
                    break;
            }
            continue;
        }
        #pragma endregion
        #pragma region 鼠标操作
        Model* newModel = nullptr;
        if (ws.displayMode == DisplayMode::SURFACES || MouseHit())
        {
            message = GetMouseMsg();
            switch (message.uMsg)
            {
                case WM_MOUSEWHEEL:
                    ws.camera.fov -= message.wheel * 3;
                    if (ws.camera.fov < directionFromDegree[20])
                    {
                        ws.camera.fov = directionFromDegree[20];
                    }
                    if (ws.camera.fov > directionFromDegree[140])
                    {
                        ws.camera.fov = directionFromDegree[140];
                    }
                    break;
                case WM_LBUTTONDOWN:
                    switch (editMode)
                    {
                        case EditMode::SELECT_MODEL:
                            break;
                        case EditMode::SELECT_TRIANGLE:
                            break;
                        case EditMode::CUBOID:
                            ws.displayMode = DisplayMode::LINES;
                            ws.viewMode = ViewMode::PERSPECTIVE;
                            buildCuboid(ws, newModel);
                            break;
                        default:
                            break;
                    }
                    break;
                case WM_MBUTTONDOWN:
                    rotating = true;
                    ws.displayMode = DisplayMode::LINES;
                    break;
                case WM_MBUTTONUP:
                    rotating = false;
                    ws.displayMode = userDisplayMode;
                    break;
                case WM_RBUTTONDOWN:
                    moving = true;
                    ws.displayMode = DisplayMode::LINES;
                    break;
                case WM_RBUTTONUP:
                    moving = false;
                    ws.displayMode = userDisplayMode;
                    break;
                case WM_MOUSEMOVE:
                    d_x = message.x - mousePosX;
                    d_y = message.y - mousePosY;
                    mousePosX = message.x;
                    mousePosY = message.y;
                    break;
            }
            if (rotating)
            {
                int rotateAngley = -d_x * 65536 / canvas.width;
                int rotateAnglex = -d_y * 65536 / canvas.height;
                Vector3f rotateAxis = ws.camera.gazeDirection.cross(ws.camera.topDirection);
                Matrix3f I, crossItemMatrix;
                crossItemMatrix <<
                    0, -rotateAxis.z(), rotateAxis.y(),
                    rotateAxis.z(), 0, -rotateAxis.x(),
                    -rotateAxis.y(), rotateAxis.x(), 0;
                I <<
                    1, 0, 0,
                    0, 1, 0,
                    0, 0, 1;
                Matrix3f rotatex = I * qcos(rotateAnglex) +
                    rotateAxis * rotateAxis.transpose() * (1 - qcos(rotateAnglex)) +
                    crossItemMatrix * qsin(rotateAnglex),
                    rotatey;
                rotatey <<
                    qcos(rotateAngley), 0, qsin(rotateAngley),
                    0, 1, 0,
                    -qsin(rotateAngley), 0, qcos(rotateAngley);

                ws.camera.eyePosition = rotatey * ws.camera.eyePosition;
                ws.camera.gazeDirection = rotatey * ws.camera.gazeDirection;
                ws.camera.topDirection = rotatey * ws.camera.topDirection;
            }
            if (moving)
            {
                Vector3f xAxis = ws.camera.gazeDirection.cross(ws.camera.topDirection);
                ws.camera.eyePosition += ws.camera.topDirection * d_y + xAxis * -d_x;
            }
            // FlushMouseMsgBuffer();
        }
        #pragma endregion
    }
    #pragma endregion

    return 0;
}