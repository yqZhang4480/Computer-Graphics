#pragma region HEADER
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_
#include <graphics.h>
#endif // !_GRAPHICS_H_

#ifndef _CONIO_H_
#define _CONIO_H_
#include <conio.h>
#endif // !_CONIO_H_

#include "../canvas.h"  
#pragma endregion

/**
 * 8    图形流水线
 * 8-2  MVP 矩阵变换
 *
 * 尝试对三维三角形进行 MVP 变换。
 * // 没好好写，能跑就行 (￣▽￣)"
 *
 * 运行环境：Windows，Visual Studio 2019
 * EasyX 图形库版本：20200520（beta）
 *
 * ****  若编译后无响应，请尝试重启设备。  ****
 * ****  若出现编译错误，请确保已正确安装EasyX图形库。  ****
 *
 * Last Edit: 2020/10/23
 */

 /*
  initFullscreen  初始化全屏绘图窗口

  ***  这些代码与图形学无关  ***
  */
void initFullscreen()
{
    int rW = GetSystemMetrics(SM_CXSCREEN); // 屏幕宽度 像素
    int rH = GetSystemMetrics(SM_CYSCREEN); // 屏幕高度 像素
    initgraph(rW, rH);
    HWND hwnd = GetHWnd();// EasyX库函数，获取窗口句柄
    LONG l_WinStyle = GetWindowLong(hwnd, GWL_STYLE);
    SetWindowLong(hwnd, GWL_STYLE,
                  (l_WinStyle | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);
    SetWindowPos(hwnd, HWND_TOP, 0, 0, rW, rH, 0);
}


class TriangleTEST : public Triangle3D
{
public:

    TriangleTEST(const Triangle3D& t)
    {
        this->v[0] = t.v[0];
        this->v[1] = t.v[1];
        this->v[2] = t.v[2];
    }

    /*
     判断光线与三角形是否相交。

     @param //光线可表示为ve+t*vd
     @param Vector3& ve 摄像机位置 0，0，0
     @param float t1, t2 t的范围
     @param float& rec 用此变量保存相交时t的值

     @return bool 是否相交
     */
    bool hit(const Vector3f& ve,
             const Vector3f& vd,
             float t1,
             float t2,
             float& rec)
    {
        //使用光线与三角形求交公式判断(参见虎书第四版79页)；
        //为了提高效率使用了几个中间变量。
        assert(t2 > t1);
        float a = v[0].position.x() - v[1].position.x(),
            b = v[0].position.y() - v[1].position.y(),
            c = v[0].position.z() - v[1].position.z(),
            d = v[0].position.x() - v[2].position.x(),
            e = v[0].position.y() - v[2].position.y(),
            f = v[0].position.z() - v[2].position.z(),
            g = vd.x(),
            h = vd.y(),
            i = vd.z(),
            j = v[0].position.x() - ve.x(),
            k = v[0].position.y() - ve.y(),
            l = v[0].position.z() - ve.z();
        float ei_hf = e * i - h * f,
            gf_di = g * f - d * i,
            dh_eg = d * h - e * g,
            ak_jb = a * k - j * b,
            jc_al = j * c - a * l,
            bl_kc = b * l - k * c;
        float M = a * ei_hf + b * gf_di + c * dh_eg;

        rec = (f * ak_jb + e * jc_al + d * bl_kc) / M;
        if (rec > t2 || rec < t1)
        {
            return false;
        }
        float beta = (j * ei_hf + k * gf_di + l * dh_eg) / M;
        if (beta < 0 || beta > 1)
        {
            return false;
        }
        float gamma = (i * ak_jb + h * jc_al + g * bl_kc) / M;
        if (gamma < 0 || gamma > 1 - beta)
        {
            return false;
        }
        return true;
    }
};

int main()
{
    /*
     * 本节仅进行 MVP 变换操作，仍使用类光线追踪算法渲染。
     */
    int rW = GetSystemMetrics(SM_CXSCREEN); // 屏幕宽度 像素
    int rH = GetSystemMetrics(SM_CYSCREEN); // 屏幕高度 像素
    initFullscreen();
    #pragma region 几何体位置
    Vertex3D PointO(0, 0, 0, 0.0, 1.0, 0.0, 1),
        PointA(0, 0, 100, 1.0, 0.0, 0.0, 1),
        PointB(100, 0, 0, 0.0, 0.0, 0.1, 1),
        PointC(0, 100, 0, 0, 0, 0, 1);
    TriangleTEST s[4] = {
        TriangleTEST(Triangle3D(PointO, PointA, PointB)),
        TriangleTEST(Triangle3D(PointB, PointC, PointO)),
        TriangleTEST(Triangle3D(PointC, PointA, PointO)),
        TriangleTEST(Triangle3D(PointA, PointB, PointC))
    };
    Vector3f e(-35, 35, 175);
    #pragma endregion
    Vector3f u_axis;
    Vector3f v_axis;
    #pragma region 变换
    Vector3f e_to_ScreenCenter(0, 0, -165);  // 从摄像机指向屏幕中心的向量，与屏幕垂直。
    //if (viewTransform)
    //{
    #pragma region View Transform
    Vector3f g = e_to_ScreenCenter.normalized();
    Vector3f t(0, 1, 0);
    Matrix4f T_view, R_view;
    T_view << 1, 0, 0, -e.x(),
        0, 1, 0, -e.y(),
        0, 0, 1, -e.z(),
        0, 0, 0, 1;
    Vector3f u = t.cross(-g);
    Vector3f v = t;
    Vector3f w = -g;

    R_view << u.x(), u.y(), u.z(), 0,
        v.x(), v.y(), v.z(), 0,
        w.x(), w.y(), w.z(), 0,
        0, 0, 0, 1;

    Vector4f temp = Vector4f(e.x(), e.y(), e.z(), 1);
    temp = R_view * (T_view * temp);
    e = Vector3f(temp.x(), temp.y(), temp.z());
    temp = Vector4f(e_to_ScreenCenter.x(), e_to_ScreenCenter.y(), e_to_ScreenCenter.z(), 1);
    temp = R_view * (T_view * temp);
    e_to_ScreenCenter = Vector3f(temp.x(), temp.y(), temp.z());
    for (int i = 0; i < 4; i++)
    {
        s[i].premultiMatrix(T_view);
        s[i].premultiMatrix(R_view);
    }
    #pragma endregion
    #pragma region projection transform
    Matrix4f M_pres, M_orth;
    int n = -163,
        f = -1000;
    M_pres << 
        n, 0, 0, 0,
        0, n, 0, 0,
        0, 0, n + f, -n * f,
        0, 0, 1, 0;
    int rp = 320;  // 随便选的，16：9就行。
    int lp = -320;
    int tp = 180;
    int bp = -180;
    M_orth << 
        2.0f / (rp - lp), 0, 0, 0,
        0, 2.0f / (tp - bp), 0, 0,
        0, 0, 2.0f / (n - f), -(n + f) * 1.0f / (n - f),
        0, 0, 0, 1;

    for (int i = 0; i < 4; i++)
    {
        // z = 0 没有特别处理。
        s[i].premultiMatrix(M_pres);
        s[i].premultiMatrix(M_orth);
    }
    #pragma endregion

    /*}*/
    /*else
    {
        Vector3f t(0, 1, 0);
        Vector3f g = e_to_ScreenCenter;
        g.normalize();
        u_axis = g.cross(t);
        v_axis = -t;
    }*/
    #pragma endregion

    BeginBatchDraw();
    DWORD* pImg = GetImageBuffer(0); // EasyX库函数，获取窗口显存指针
    u_axis = u;
    v_axis = -v;
    while (true)
    {
        #pragma region 光线追踪
        for (int index_h = 0; index_h < rH; index_h++)
        {
            for (int index_w = 0; index_w < rW; index_w++)
            {
                int index = index_w + index_h * rW;

                // 第一步，生成光线。
                // 关键在于算出光线的方向d。
                /// 在标准视积空间只需进行正投影。
                Vector3f bia = u_axis * 2.0f * (index_w - rW / 2) / rW
                    + v_axis * 2.0f * (index_h - rH / 2) / rH;
                Vector3f d = Vector3f(0, 0, 1);

                // 第二步，判断光线击中的几何对象。
                float t = 10000000000;
                float temp;
                TriangleTEST* hitObj = nullptr;
                for (int i = 0; i < 4; i++)
                {
                    if (s[i].hit(d+bia, d, 0, t, temp))
                    {
                        if (t > temp)
                        {
                            t = temp;
                            hitObj = s + i;
                        }
                    }
                }

                // 第三步，在该像素显示几何对象的颜色。
                if (hitObj != nullptr)
                {
                    pImg[index] = RGB(
                        (int)(hitObj->v[0].color.z() * 255),
                        (int)(hitObj->v[0].color.y() * 255),
                        (int)(hitObj->v[0].color.x() * 255)
                    );

                }
                else
                {
                    pImg[index] = RGB(64, 64, 64);
                }
            }
        }
        #pragma endregion

        

        FlushBatchDraw();
        _getch();

        Matrix4f xr;
        xr << 1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;
        for (int i = 0; i < 4; i++)
        {
            s[i].premultiMatrix(xr);
        }
    }

    //Canvas canvas;
    srand(0);
}