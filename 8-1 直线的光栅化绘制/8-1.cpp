#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_
#include <graphics.h>
#endif // !_GRAPHICS_H_

#ifndef _CONIO_H_
#define _CONIO_H_
#include <conio.h>
#endif // !_CONIO_H_

#include "canva.h"

/**
 * 8    图形流水线
 * 8-1  二维图元的光栅化绘制
 *
 * 使用光栅化方法绘制直线、三角形等。
 *
 * 运行环境：Windows，Visual Studio 2019
 * EasyX 图形库版本：20200520（beta）
 *
 * ****  若编译后无响应，请尝试重启设备。  ****
 * ****  若出现编译错误，请确保已正确安装EasyX图形库。  ****
 *
 * Last Edit: 2020/07/12
 */

/*
 initFullscreen  初始化全屏绘图窗口

 ***  这些代码与图形学无关  ***
 */
void initFullscreen() {
    int rW = GetSystemMetrics(SM_CXSCREEN); // 屏幕宽度 像素
    int rH = GetSystemMetrics(SM_CYSCREEN); // 屏幕高度 像素
    initgraph(rW, rH);
    HWND hwnd = GetHWnd();// EasyX库函数，获取窗口句柄
    LONG l_WinStyle = GetWindowLong(hwnd, GWL_STYLE);
    SetWindowLong(hwnd, GWL_STYLE,
        (l_WinStyle | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);
    SetWindowPos(hwnd, HWND_TOP, 0, 0, rW, rH, 0);
}

int main()
{
    initFullscreen();
    Canva c;
    srand(0);
    for (int i = 0; i < 10000; i++)
    {
        int x = rand() % c.width,
            y = rand() % c.height;
        if (x + 200 >= c.width || y - 124 < 0)
        {
            continue;
        }
        Gemobj&& g = Line2D(
            Point2D(x, y, Vector3(0)),
            Point2D(x + 200,
                    y - 124,
                    Vector3(rand() / 32768.0,
                            rand() / 32768.0,
                            rand() / 32768.0))
        );
        c.draw(g);
    }
    saveimage(L"line10000.png");
    _getch();
    cleardevice();
    srand(0);
    for (int i = 0; i < 1000; i++)
    {
        Gemobj&& g = Triangle2D(
            Point2D(rand()%c.width,
                    rand()%c.height,
                    Vector3(rand() / 32768.0,
                            rand() / 32768.0,
                            rand() / 32768.0)),
            Point2D(rand()%c.width,
                    rand()%c.height,
                    Vector3(rand() / 32768.0,
                            rand() / 32768.0,
                            rand() / 32768.0)),
            Point2D(rand()%c.width,
                    rand()%c.height,
                    Vector3(rand() / 32768.0,
                            rand() / 32768.0,
                            rand() / 32768.0))
        );
        c.draw(g);
    }
    saveimage(L"triangle1000.png");
    _getch();
}