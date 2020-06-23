#pragma once

#ifndef _MATH_H_
    #define _MATH_H_
    #include <math.h>
#endif // !_MATH_H_

#ifndef _ASSERT_H_
    #define _ASSERT_H_
    #include <assert.h>
#endif // !_ASSERT_H_

/*
 Vector3  三维向量类
 
 以三个浮点数保存三个维度的数值，
 实现了向量的基本操作。
 */
class Vector3 {
public:
    float x, y, z;
    Vector3() {
        this->all(0.0);
    }
    Vector3(float x, float y, float z)
        : x(x), y(y), z(z) {}
    ~Vector3() {}

    // 将向量设为指定值
    void all(float d) {
        x = d;
        y = d;
        z = d;
    }

    Vector3& operator = (const Vector3& v) {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

    bool operator == (const Vector3& v) const {
        return x == v.x && y == v.y && z == v.z;
    }
    bool operator != (const Vector3& v) const {
        return !(*this == v);
    }

    // 取负数
    Vector3 operator - () const {
        return Vector3(-x, -y, -z);
    }

    Vector3 operator + (const Vector3& v) const {
        return Vector3(
            x + v.x,
            y + v.y,
            z + v.z
        );
    }
    Vector3& operator += (const Vector3& v) {
        *this = *this + v;
        return *this;
    }

    Vector3 operator - (const Vector3& v) const {
        return Vector3(-v + *this);
    }
    Vector3& operator -= (const Vector3& v) {
        *this = *this - v;
        return *this;
    }

    // 向量 * 数
    Vector3 operator * (float d) const {
        return Vector3(x * d, y * d, z * d);
    }
    Vector3& operator *= (float d) {
        *this = *this * d;
        return *this;
    }
    // 向量 / 数
    Vector3 operator / (float d) const {
        // if (d == 0.0) d = 1;
        float di = 1.0 / d;
        return *this * di;
    }
    Vector3& operator /= (float d) {
        *this = *this / d;
        return *this;
    }

    // 向量点乘
    float operator * (const Vector3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    // 求模
    float length() {
        return sqrt(x * x + y * y + z * z);
    }

    // 标准化向量
    void normalize() {
        float l = this->length();
        assert(l != 0.0);
        *this /= this->length();
    }
    
};

// 向量叉乘
Vector3 v3Cross(const Vector3& v1, const Vector3& v2) {
    return Vector3(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    );
}

// 向量距离
float v3Distance(const Vector3& v1, const Vector3& v2) {
    float dx = v1.x - v2.x,
          dy = v1.y - v2.y,
          dz = v1.z - v2.z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}