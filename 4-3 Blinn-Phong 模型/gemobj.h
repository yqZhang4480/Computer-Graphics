#pragma once
#include "../vector3.h"

/**
 * GEMOBJ.H
 *
 * 定义几何对象。
 * Last Edit: 2020/07/01
 */

/**
 * Surface  几何对象的基类
 *
 * 实现了Blinn-Phong 模型要求的表面漫反射与镜面反射。
 */
class Surface {
public:
    // 材质漫反射颜色，向量的三个值分别对应RGB通道
    Vector3 diffuse;
    // 材质镜面反射颜色，向量的三个值分别对应RGB通道
    Vector3 specular;
    // Phong 指数，表征材质光泽度
    float phongExponent;

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

/**
 * Triangle  三角形面
 *
 * 继承自Surface 类。
 */
class Triangle : public Surface {
public:
    //三角形的三个顶点
    Vector3 p1, p2, p3;
    //三角形面的法向量
    Vector3 normal;

    Triangle() {}
    /**
     * 创建一个带有漫反射与镜面反射的三角形。
     * 其法向量的方向与点的顺序有关。具体来说，
     * 右手四指弯曲依次通过p1, p2, p3，大拇指所指方向即为法向量方向。
     * 
     * @param p1,p2,p3 三角形的三个顶点
     * @param diffuse 漫反射颜色，向量的三个值分别对应RGB通道
     * @param specular 镜面反射颜色，向量的三个值分别对应RGB通道
     * @param phongExponent 材质光泽度
     */
    Triangle(const Vector3& p1,
             const Vector3& p2,
             const Vector3& p3,
             const Vector3& diffuse,
             const Vector3& specular,
             float          phongExponent
        ) : p1(p1), p2(p2), p3(p3) {
        this->diffuse = diffuse;
        this->specular = specular;
        this->phongExponent = phongExponent;
        this->normal = v3Cross(p2 - p1, p3 - p1);
        this->normal.normalize();
    }

    /**
     * 求三角形的法向量。
     * 法向量的方向与点的顺序有关。具体来说，
     * 右手四指弯曲依次通过p1, p2, p3，大拇指所指方向即为法向量方向。
     *
     * @param p 任意一点，默认为空指针
     */
    virtual Vector3 getNormal(const Vector3) {
        return normal;
    }

    /**
     * 判断光线与三角形是否相交。
     *
     * @param      //交点可表示为ve+t*vd
     * @param[in]  ve 摄像机位置
     * @param[in]  vd 光线方向，由交点指向摄像机
     * @param[in]  t1,t2 t的范围，t1<t2
     * @param[out] rec 此变量保存相交时t的值
     *
     * @return bool 是否相交
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

        rec = -(f * ak_jb + e * jc_al + d * bl_kc) / M;
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

/**
 * Sphere  球面
 *
 * 继承自Surface 类。
 */
class Sphere : public Surface {
public:
    //球心
    Vector3 center;
    //半径
    float radius = 0;

    Sphere() {}
    /**
     * 创建一个带有漫反射与镜面反射的球面。
     *
     * @param center 球心
     * @param radius 半径
     * @param diffuse 漫反射颜色，向量的三个值分别对应RGB通道
     * @param specular 镜面反射颜色，向量的三个值分别对应RGB通道
     * @param phongExponent 材质光泽度
     */
    Sphere(const Vector3& center,
        float radius,
        const Vector3& diffuse,
        const Vector3& specular,
        float          phongExponent
    ) : center(center), radius(radius) {
        this->diffuse = diffuse;
        this->specular = specular;
        this->phongExponent = phongExponent;
    }

    /**
     * 求球面的法向量。
     * 法向量的方向由球心指向球面。
     *
     * @param p 球面上一点
     */
    virtual Vector3 getNormal(const Vector3 p) {
        Vector3 normal = p - center;
        normal.normalize();
        return normal;
    }

    /**
     * 判断光线与球面是否相交。
     *
     * @param      //交点可表示为ve+t*vd
     * @param[in]  ve 摄像机位置
     * @param[in]  vd 光线方向，由交点指向摄像机
     * @param[in]  t1,t2 t的范围，t1<t2
     * @param[out] rec 此变量保存相交时t的值
     *
     * @return bool 是否相交
     */
    virtual bool hit(const Vector3& ve,
        const Vector3& vd,
        float t1,
        float t2,
        float& rec) {
        assert(t2 > t1);
        //使用光线与球面求交公式判断，
        //为了提高效率使用了几个中间变量。
        Vector3 e_c = ve - center;
        float d_e_c = vd * e_c;
        float d_d = vd * vd;
        float delta = d_e_c * d_e_c - d_d * (e_c * e_c - radius * radius);
        if (delta < 0) {
            return false;
        }
        rec = (-d_e_c - sqrt(delta)) / d_d;
        if (rec > t2 || rec < t1) {
            return false;
        }
        return true;
    }
};

/**
 * Light  点光源
 */
struct Light {
    // 位置坐标
    Vector3 position;
    // 光强，向量的三个值分别对应RGB通道；
    // 光强大小与距离无关。
    Vector3 intensity;
};