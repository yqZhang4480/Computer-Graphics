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
 * 8    ͼ����ˮ��
 * 8-1  ��άͼԪ�Ĺ�դ������
 *
 * ʹ�ù�դ����������ֱ�ߡ������εȡ�
 *
 * ���л�����Windows��Visual Studio 2019
 * EasyX ͼ�ο�汾��20200520��beta��
 *
 * ****  �����������Ӧ���볢�������豸��  ****
 * ****  �����ֱ��������ȷ������ȷ��װEasyXͼ�ο⡣  ****
 *
 * Last Edit: 2020/07/12
 */

/*
 initFullscreen  ��ʼ��ȫ����ͼ����

 ***  ��Щ������ͼ��ѧ�޹�  ***
 */
void initFullscreen() {
    int rW = GetSystemMetrics(SM_CXSCREEN); // ��Ļ��� ����
    int rH = GetSystemMetrics(SM_CYSCREEN); // ��Ļ�߶� ����
    initgraph(rW, rH);
    HWND hwnd = GetHWnd();// EasyX�⺯������ȡ���ھ��
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