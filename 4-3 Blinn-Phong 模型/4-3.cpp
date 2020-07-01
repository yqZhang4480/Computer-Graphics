#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_
#include <graphics.h>
#endif // !_GRAPHICS_H_

#ifndef _CONIO_H_
#define _CONIO_H_
#include <conio.h>
#endif // !_CONIO_H_

#include "../gemobj.h"

/**
 * 4    光线追踪
 * 4-3  Blinn-Phong 模型
 *
 * 实现Blinn-Phong 光照模型。
 *
 * 运行环境：Windows，Visual Studio 2019
 * EasyX 图形库版本：20200520（beta）
 *
 * ****  若编译后无响应，请尝试重启设备。  ****
 * ****  若出现编译错误，请确保已正确安装EasyX图形库。  ****
 *
 * Last Edit: 2020/07/01
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

/*
 getInput  用户输入

 ***  这些代码与图形学无关  ***
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
        a(100, 0, 0),
        b(0, 100, 0),
        c(0, 0, 100),
        diffuse(0.5, 1, 0.5),
        specular(1, 1, 1);
    Light light = {
        Vector3(100, -40, 100), Vector3(1, 1, 1)
    };
    Surface* s[4] = {
        new Triangle(o, b, a, diffuse, specular, 10),
        new Triangle(o, c, b, diffuse, specular, 10),
        new Triangle(a, c, o, diffuse, specular, 10),
        new Triangle(a, b, c, diffuse, specular, 10)
    };
    // ** 要注意点的顺序，这影响法向量的方向 **

    Vector3 e(175, 10, 35);
    Vector3 e_to_ScreenCenter(-165, 0, 0);  // 从摄像机指向屏幕中心的向量，与屏幕垂直。
    DWORD* pImg = GetImageBuffer(0); // EasyX库函数，获取窗口显存指针
    int rW = GetSystemMetrics(SM_CXSCREEN); // 屏幕宽度 像素
    int rH = GetSystemMetrics(SM_CYSCREEN); // 屏幕高度 像素

    // 计算屏幕坐标系的基
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

                // 第一步，生成光线。
                // 关键在于算出光线的方向d。
                // d == 像素位置 - e
                // |像素位置 == 屏幕中心 + 偏移
                // ||屏幕中心 == e + e_to_ScreenCenter
                // ||偏移 == 屏幕坐标 * 屏幕坐标系的基
                // 综上，d == e_to_ScreenCenter + 偏移
                Vector3 bia = w_axis * (w - rW / 2)
                    + h_axis * (h - rH / 2);
                Vector3 d = e_to_ScreenCenter + bia;
                d.normalize();
                if (bia.length() == 0.0) {
                    int i = 0;
                }

                // 第二步，判断光线击中的几何对象。
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

                // 第三步，在该像素显示几何对象的颜色。
                Vector3 p = e + d * t; // 点的位置
                Vector3 l = light.position - p;
                l.normalize();
                Vector3 h = l - d;
                h.normalize();
                if (hitObj != nullptr) {
                    Vector3 n = hitObj->getNormal(p);
                    float Lambertian = max(0, n * l);
                    float Specular = pow(max(0, n * h), hitObj->phongExponent);
                    Vector3 color =
                        v3ElementwiseProduct(hitObj->diffuse, light.intensity) * Lambertian * 255 /*+
                        v3ElementwiseProduct(hitObj->specular, light.intensity) * Specular * 255*/;
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

    delete s[0], s[1], s[2], s[3];
    s[0] = s[1] = s[2] = s[3] = nullptr;
    return 0;
}