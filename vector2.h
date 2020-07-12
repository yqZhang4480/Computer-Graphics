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
 Vector2  二维向量类

 以两个浮点数保存两个维度的数值，
 实现了向量的基本操作。
 */
class Vector2 {
public:
    float x, y;
    Vector2() : x(0.0), y(0.0) {}
    Vector2(float x, float y)
        : x(x), y(y) {}
    Vector2(float xy)
        : x(xy), y(xy) {}
    ~Vector2() {}

    // 将向量设为指定值
    void all(float d) {
        x = d;
        y = d;
    }

    Vector2& operator = (const Vector2& v) {
        x = v.x;
        y = v.y;
        return *this;
    }

    bool operator == (const Vector2& v) const {
        return x == v.x && y == v.y;
    }
    bool operator != (const Vector2& v) const {
        return !(*this == v);
    }

    // 取负数
    Vector2 operator - () const {
        return Vector2(-x, -y);
    }

    Vector2 operator + (const Vector2& v) const {
        return Vector2(
            x + v.x,
            y + v.y
        );
    }
    Vector2& operator += (const Vector2& v) {
        *this = *this + v;
        return *this;
    }

    Vector2 operator - (const Vector2& v) const {
        return Vector2(-v + *this);
    }
    Vector2& operator -= (const Vector2& v) {
        *this = *this - v;
        return *this;
    }

    // 向量 * 数
    Vector2 operator * (float d) const {
        return Vector2(x * d, y * d);
    }
    Vector2& operator *= (float d) {
        *this = *this * d;
        return *this;
    }
    // 向量 / 数
    Vector2 operator / (float d) const {
        // if (d == 0.0) d = 1;
        float di = 1.0 / d;
        return *this * di;
    }
    Vector2& operator /= (float d) {
        *this = *this / d;
        return *this;
    }

    // 向量点乘
    float operator * (const Vector2& v) const {
        return x * v.x + y * v.y;
    }

    // 求模
    float length() {
        return sqrt(x * x + y * y);
    }

    // 标准化向量
    void normalize() {
        float l = this->length();
        assert(l != 0.0);
        *this /= this->length();
    }

};

// 向量叉乘
float v2Cross(const Vector2& v1, const Vector2& v2) {
    return v1.x * v2.y - v2.x *v1.y;
}

// 按位乘法
Vector2 v2ElementwiseProduct(const Vector2& v1, const Vector2& v2) {
    return Vector2(v1.x * v2.x, v1.y * v2.y);
}

// 向量距离
float v2Distance(const Vector2& v1, const Vector2& v2) {
    float dx = v1.x - v2.x,
        dy = v1.y - v2.y;
    return sqrt(dx * dx + dy * dy);
}