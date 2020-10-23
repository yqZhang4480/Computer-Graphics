#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_
#include <graphics.h>
#endif // !_GRAPHICS_H_

#ifndef _CONIO_H_
#define _CONIO_H_
#include <conio.h>
#endif // !_CONIO_H_

#include "gemobj.h"

/**
 * 4    ����׷��
 * 4-3  Blinn-Phong ģ��
 *
 * ʵ��Blinn-Phong ����ģ�͡�
 *
 * ���л�����Windows��Visual Studio 2019
 * EasyX ͼ�ο�汾��20200520��beta��
 *
 * ****  �����������Ӧ���볢�������豸��  ****
 * ****  �����ֱ��������ȷ������ȷ��װEasyXͼ�ο⡣  ****
 *
 * Last Edit: 2020/07/01
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

/*
 getInput  �û�����

 ***  ��Щ������ͼ��ѧ�޹�  ***
 */
bool getInput(Vector3& light, Vector3& e) {
    char c = _getch();
    if (c == 'w') {
        light.z += 20;
    }
    else if (c == 'x') {
        light.z -= 20;
    }
    else if (c == 'a') {
        light.y -= 20;
    }
    else if (c == 'd') {
        light.y += 20;
    }
    else if (c == 'e') {
        light.x -= 20;
    }
    else if (c == 'z') {
        light.x += 20;
    }
    else if (c == 'h') {
        e.y += 20;
    }
    else if (c == 'f') {
        e.y -= 20;
    }
    else if (c == 'v') {
        e.x += 20;
    }
    else if (c == 'y') {
        e.x -= 20;
    }
    else if (c == 't') {
        e.z += 20;
    }
    else if (c == 'b') {
        e.z -= 20;
    }
    else {
        return false;
    }
    return true;
}

int main() {
    initFullscreen();
    BeginBatchDraw();
    Vector3 o(0, 0, 0),
        /*a(100, 0, 0),
        b(0, 100, 0),
        c(0, 0, 100),
        diffuse(0.5, 1, 0.5),
        specular(1, 1, 1);*/
        a(0, 50, 0),
        b(100, 50, 0),
        c(100, 0, 0),
        o_(10, 10, 30),
        a_(10, 40, 30),
        b_(90, 40, 30),
        c_(90, 10, 30),
        diffuse(0.949, 0.753, 0.337),
        specular(0.949, 0.753, 0.337);
    Light light = {
        Vector3(100, -40, 100), Vector3(1, 1, 1)
    };
    /*Surface* s[4] = {
        new Triangle(o, b, a, diffuse, specular, 10),
        new Triangle(o, c, b, diffuse, specular, 10),
        new Triangle(a, c, o, diffuse, specular, 10),
        new Triangle(a, b, c, diffuse, specular, 10)
    };*/
    Surface* s[8] = {
        new Triangle(o_, c_, a_, diffuse, specular, 5),
        new Triangle(a_, c_, b_, diffuse, specular, 5),
        new Triangle(o_, o, c, diffuse, specular, 5),
        new Triangle(c, c_, o_, diffuse, specular, 5),
        new Triangle(b_, c_, c, diffuse, specular, 5),
        new Triangle(c, b, b_, diffuse, specular, 5),
        new Triangle(a, a_, b_, diffuse, specular, 5),
        new Triangle(b_, b, a, diffuse, specular, 5)
    };
    // ** Ҫע����˳����Ӱ�취�����ķ��� **

    Vector3 e(175, 10, 35);
    Vector3 e_to_ScreenCenter(-165, 0, 0);  // �������ָ����Ļ���ĵ�����������Ļ��ֱ��
    DWORD* pImg = GetImageBuffer(0); // EasyX�⺯������ȡ�����Դ�ָ��
    int rW = GetSystemMetrics(SM_CXSCREEN); // ��Ļ��� ����
    int rH = GetSystemMetrics(SM_CYSCREEN); // ��Ļ�߶� ����

    // ������Ļ����ϵ�Ļ�
    Vector3 z(0, 0, 1);
    Vector3 w_axis, h_axis;
    Vector3 n = e_to_ScreenCenter;
    n.normalize();
    if (n == z || n == -z) {
        w_axis = Vector3(1, 0, 0);
    }
    else {
        w_axis = v3Cross(n, z);
    }
    h_axis = v3Cross(n, w_axis);
    do {
        for (int h = 0; h < rH; h++) {
            for (int w = 0; w < rW; w++) {
                int index = w + h * rW;

                // ��һ�������ɹ��ߡ�
                // �ؼ�����������ߵķ���d��
                // d == ����λ�� - e
                // |����λ�� == ��Ļ���� + ƫ��
                // ||��Ļ���� == e + e_to_ScreenCenter
                // ||ƫ�� == ��Ļ���� * ��Ļ����ϵ�Ļ�
                // ���ϣ�d == e_to_ScreenCenter + ƫ��
                Vector3 bia = w_axis * (w - rW / 2)
                    + h_axis * (h - rH / 2);
                Vector3 d = e_to_ScreenCenter + bia;
                d.normalize();
                if (bia.length() == 0.0) {
                    int i = 0;
                }

                // �ڶ������жϹ��߻��еļ��ζ���
                float t = 100000;
                float temp;
                Surface* hitObj = nullptr;
                for (Surface* o : s) {
                    if (o->hit(e, d, 0, t, temp)) {
                        if (t > temp) {
                            t = temp;
                            hitObj = o;
                        }
                    }
                }

                // ���������ڸ�������ʾ���ζ������ɫ��
                Vector3 p = e + d * t; // ���λ��
                Vector3 l = light.position - p;
                l.normalize();
                Vector3 h = l - d;
                h.normalize();
                if (hitObj != nullptr) {
                    Vector3 n = hitObj->getNormal(p);
                    float Lambertian = max(0, n * l);
                    float Specular = pow(max(0, n * h), hitObj->phongExponent);
                    Vector3 color =
                        v3ElementwiseProduct(hitObj->diffuse, light.intensity) * Lambertian * 255 +
                        v3ElementwiseProduct(hitObj->specular, light.intensity) * Specular * 255;
                    int r = color.x,
                        g = color.y,
                        b = color.z;
                    assert(r >= 0 && g >= 0 && b >= 0);
                    if (r > 255) {
                        r = 255;
                    }
                    if (g > 255) {
                        g = 255;
                    }
                    if (b > 255) {
                        b = 255;
                    }
                    pImg[index] = RGB(b, g, r);
                }
                else {
                    pImg[index] = RGB(0, 0, 0);
                }
            }
        }
        wchar_t echo[40];
        wsprintf(echo, L"LIGHT( %d, %d, %d )",
            (int)light.position.x,
            (int)light.position.y,
            (int)light.position.z
        );
        settextstyle(20, 0, L"Arial");
        outtextxy(200, 600, echo);
        wsprintf(echo, L"EYE  ( %d, %d, %d )",
            (int)e.x,
            (int)e.y,
            (int)e.z
        );
        settextstyle(20, 0, L"Arial");
        outtextxy(200, 620, echo);
        FlushBatchDraw();
    } while (getInput(light.position, e));

    for (Surface* o : s) {
        delete o;
        o = nullptr;
    }
    return 0;
}