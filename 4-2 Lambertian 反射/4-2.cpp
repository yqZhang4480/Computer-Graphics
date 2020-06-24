#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_
#include <graphics.h>
#endif // !_GRAPHICS_H_

#ifndef _CONIO_H_
#define _CONIO_H_
#include <conio.h>
#endif // !_CONIO_H_

#include "../vector3.h"

/*
 * 4    光线追踪
 * 4-2  Lambertian 反射
 *
 * 演示 Lambertian 反射，
 * 在4-1的基础上进行了修改，主要集中于第252行之后。
 *
 * 运行环境：Windows，Visual Studio 2019
 * EasyX 图形库版本：20200520（beta）
 *
 * ****  若编译后无响应，请尝试重启设备。  ****
 * ****  若出现编译错误，请确保已于Github下载了完整的解决方案，并已正确安装EasyX图形库。  ****
 *
 * Last Edit: 2020/06/24
 */

 //几何对象的父类
class Surface {
public:
    //材质漫反射系数，向量的三个值分别对应RGB通道
    Vector3 diffuse;

    Surface() {}
    ~Surface() {}

    virtual bool hit(const Vector3& ve,
        const Vector3& vd,
        float t1,
        float t2,
        float& rec) = 0;

    virtual Vector3 getNormal(Vector3) = 0;
};

// 考虑实现球和三角形两个子类；其他几何对象一般可看作若干三角形的拼接。

//三角形类
class Triangle : public Surface {
public:
    //三角形的三个顶点
    Vector3 p1, p2, p3;
    Vector3 normal;

    Triangle() {}
    Triangle(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 diffuse)
        : p1(p1), p2(p2), p3(p3) {
        this->diffuse = diffuse;
        normal = v3Cross(p2 - p1, p3 - p1);
        normal.normalize();
    }

    //求三角形的法向量
    //右手四指弯曲依次通过p1, p2, p3，大拇指所指方向即为法向量方向。
    virtual Vector3 getNormal(Vector3) {
        return normal;
    }
    /*
     判断光线与三角形是否相交。

     @param //光线可表示为ve+t*vd
     @param Vector3& ve 摄像机位置
     @param Vector3& vd 光线方向，由交点指向摄像机
     @param float t1, t2 t的范围
     @param float& rec 用此变量保存相交时t的值

     @return bool 是否相交
     */
    virtual bool hit(const Vector3& ve,
        const Vector3& vd,
        float t1,
        float t2,
        float& rec) {
        //使用光线与三角形求交公式判断，
        //为了提高效率使用了几个中间变量。
        assert(t2 > t1);
        float a = p1.x - p2.x,
            b = p1.y - p2.y,
            c = p1.z - p2.z,
            d = p1.x - p3.x,
            e = p1.y - p3.y,
            f = p1.z - p3.z,
            g = vd.x,
            h = vd.y,
            i = vd.z,
            j = p1.x - ve.x,
            k = p1.y - ve.y,
            l = p1.z - ve.z;
        float ei_hf = e * i - h * f,
            gf_di = g * f - d * i,
            dh_eg = d * h - e * g,
            ak_jb = a * k - j * b,
            jc_al = j * c - a * l,
            bl_kc = b * l - k * c;
        float M = a * ei_hf + b * gf_di + c * dh_eg;

        rec = (f * ak_jb + e * jc_al + d * bl_kc) / M;
        if (rec > t2 || rec < t1) {
            return false;
        }
        float beta = (j * ei_hf + k * gf_di + l * dh_eg) / M;
        if (beta < 0 || beta > 1) {
            return false;
        }
        float gamma = (i * ak_jb + h * jc_al + g * bl_kc) / M;
        if (gamma < 0 || gamma > 1 - beta) {
            return false;
        }
        return true;
    }
};

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
        color(0.5, 1, 0.5);
    Vector3 light(100, -40, 100);
    Triangle* s[4] = {
        new Triangle(o, b, a, color),
        new Triangle(o, c, b, color),
        new Triangle(a, c, o, color),
        new Triangle(a, b, c, color)
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
                Vector3 d = -(e_to_ScreenCenter + bia);
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
                Vector3 p = -d * t; // 点的位置
                Vector3 l = light - p;
                l.normalize();
                if (hitObj != nullptr) {
                    Vector3 n = hitObj->getNormal(p);
                    float Lambertian = max(0, n * l);
                    Vector3 rgb = hitObj->diffuse * 255 * Lambertian;
                    pImg[index] = RGB(
                        (int)rgb.x,
                        (int)rgb.y,
                        (int)rgb.z);
                }
                else {
                    pImg[index] = RGB(0, 0, 0);
                }
            }
        }
        wchar_t echo[40];
        wsprintf(echo, L"LIGHT( %d, %d, %d )",
            (int)light.x, (int)light.y, (int)light.z);
        settextstyle(20, 0, L"Arial");
        outtextxy(200, 200, echo);
        FlushBatchDraw();
        
    } while (getInput(light, e));
    delete s[0], s[1], s[2], s[3];
    s[0] = s[1] = s[2] = s[3] = nullptr;
    return 0;
}