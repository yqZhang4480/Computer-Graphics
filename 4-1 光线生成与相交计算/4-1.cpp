#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_
#include <graphics.h>
#endif // !_GRAPHICS_H_

#ifndef _CONIO_H_
#define _CONIO_H_
#include <conio.h>
#endif // !_CONIO_H_

#include "../vector3.h"

#include<stdio.h>

/*
 * 4    光线追踪
 * 4-1  光线生成与相交计算
 *
 * 演示光线追踪的基本原理，
 * 包括按像素顺序生成光线、光线与几何图形相交的判断。
 *
 * 运行环境：Windows，Visual Studio 2019
 * EasyX 图形库版本：20200520（beta）
 *
 * ****  若编译后无响应，请尝试重启设备。  ****
 * ****  若出现编译错误，请确保已于Github下载了完整的解决方案，并已正确安装EasyX图形库。  ****
 *
 * Last Edit: 2020/06/22
 */

//几何对象的父类
class Surface {
public:
    Surface() {}
    ~Surface() {}


    virtual bool hit(const Vector3& ve,
        const Vector3& vd,
        float t1,
        float t2,
        float& rec) = 0;
};

// 考虑实现球和三角形两个子类；其他几何对象一般可看作若干三角形的拼接。

//漫反射材质的三角形。
class Triangle : public Surface {
public:
    //三角形的三个顶点
    Vector3 p1, p2, p3;
    //材质漫反射系数，向量的三个值分别对应RGB通道
    Vector3 diffuse;

    Triangle() {}
    Triangle(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 diffuse) 
    : p1(p1), p2(p2), p3(p3), diffuse(diffuse) {}

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
        //使用光线与三角形求交公式判断(参见虎书第四版79页)；
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
        
        float beta = (j * ei_hf + k * gf_di + l * dh_eg) / M,
             gamma = (i * ak_jb + h * jc_al + g * bl_kc) / M;
        rec       = (f * ak_jb + e * jc_al + d * bl_kc) / M;
        
        return (rec < t2 && rec > t1 
            && beta > 0 && gamma > 0 
            && beta + gamma < 1);
    }
};

/*
 init_fullscreen  初始化设备屏幕
 
 通过一系列操作创建全屏绘图窗口。
 */
void init_fullscreen() {
    int rW = GetSystemMetrics(SM_CXSCREEN); // 屏幕宽度 像素
    int rH = GetSystemMetrics(SM_CYSCREEN); // 屏幕高度 像素
    initgraph(rW, rH);
    HWND hwnd = GetHWnd();//获取窗口句柄 以easyx为例
    LONG l_WinStyle = GetWindowLong(hwnd, GWL_STYLE); // 获取窗口信息
    // 设置窗口信息 最大化 取消标题栏及边框
    SetWindowLong(hwnd, GWL_STYLE,
        (l_WinStyle | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);
    //直接修改窗口样式
    SetWindowPos(hwnd, HWND_TOP, 0, 0, rW, rH, 0);
}

int main() {
    init_fullscreen();

    _getch();
}