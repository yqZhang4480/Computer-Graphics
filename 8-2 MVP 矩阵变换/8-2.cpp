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
 * 8    ͼ����ˮ��
 * 8-2  MVP ����任
 *
 * ���Զ���ά�����ν��� MVP �任��
 * // û�ú�д�����ܾ��� (������)"
 *
 * ���л�����Windows��Visual Studio 2019
 * EasyX ͼ�ο�汾��20200520��beta��
 *
 * ****  �����������Ӧ���볢�������豸��  ****
 * ****  �����ֱ��������ȷ������ȷ��װEasyXͼ�ο⡣  ****
 *
 * Last Edit: 2020/10/23
 */

 /*
  initFullscreen  ��ʼ��ȫ����ͼ����

  ***  ��Щ������ͼ��ѧ�޹�  ***
  */
void initFullscreen()
{
    int rW = GetSystemMetrics(SM_CXSCREEN); // ��Ļ��� ����
    int rH = GetSystemMetrics(SM_CYSCREEN); // ��Ļ�߶� ����
    initgraph(rW, rH);
    HWND hwnd = GetHWnd();// EasyX�⺯������ȡ���ھ��
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
     �жϹ������������Ƿ��ཻ��

     @param //���߿ɱ�ʾΪve+t*vd
     @param Vector3& ve �����λ�� 0��0��0
     @param float t1, t2 t�ķ�Χ
     @param float& rec �ô˱��������ཻʱt��ֵ

     @return bool �Ƿ��ཻ
     */
    bool hit(const Vector3f& ve,
             const Vector3f& vd,
             float t1,
             float t2,
             float& rec)
    {
        //ʹ�ù������������󽻹�ʽ�ж�(�μ�������İ�79ҳ)��
        //Ϊ�����Ч��ʹ���˼����м������
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
     * ���ڽ����� MVP �任��������ʹ�������׷���㷨��Ⱦ��
     */
    int rW = GetSystemMetrics(SM_CXSCREEN); // ��Ļ��� ����
    int rH = GetSystemMetrics(SM_CYSCREEN); // ��Ļ�߶� ����
    initFullscreen();
    #pragma region ������λ��
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
    #pragma region �任
    Vector3f e_to_ScreenCenter(0, 0, -165);  // �������ָ����Ļ���ĵ�����������Ļ��ֱ��
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
    int rp = 320;  // ���ѡ�ģ�16��9���С�
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
        // z = 0 û���ر���
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
    DWORD* pImg = GetImageBuffer(0); // EasyX�⺯������ȡ�����Դ�ָ��
    u_axis = u;
    v_axis = -v;
    while (true)
    {
        #pragma region ����׷��
        for (int index_h = 0; index_h < rH; index_h++)
        {
            for (int index_w = 0; index_w < rW; index_w++)
            {
                int index = index_w + index_h * rW;

                // ��һ�������ɹ��ߡ�
                // �ؼ�����������ߵķ���d��
                /// �ڱ�׼�ӻ��ռ�ֻ�������ͶӰ��
                Vector3f bia = u_axis * 2.0f * (index_w - rW / 2) / rW
                    + v_axis * 2.0f * (index_h - rH / 2) / rH;
                Vector3f d = Vector3f(0, 0, 1);

                // �ڶ������жϹ��߻��еļ��ζ���
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

                // ���������ڸ�������ʾ���ζ������ɫ��
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