#pragma once
#include "Vector2D.h"
#include <algorithm>
#include <math.h>

//�x�N�g���̊e������value�{�ɂ���
void Vector2D::Magnify(double value)
{
    _x *= value;
    _y *= value;
}

void Vector2D::Normalize()
{
    double length = GetLength();
    
    if (length <= 0.0001)
    {
        _x = 0;
        _y = 0;
        return;
    }
    
    _x /= length;
    _y /= length;
}

//�x�N�g�����w��̊p�x������]������
void Vector2D::Rotate(double theta)
{
    double length = GetLength();
    
    if (length <= 0.0001)
    {
        _x = 0;
        _y = 0;
        return;
    }
    
    //���݂�x��(1, 0)�Ƃ̊p�x���Z�o
    //b = (1, 0)
    //cos = (a.x * b.x + a.y * b.y) / (|a| * |b|)
    // = a.x / a.length
    double rad = acos(_x / length);
    
    //acos�l�����ł͎��v�E�����v�܂�肪���ʂł��Ȃ��̂�
    //asin�l���Ƃ��Ċm�F
    //sin = a.y / a.length
    if (asin(_y / length) < 0)
        rad *= -1.0;
    
    //���݂̊p�x���炳��ɉ�]������
    //theta += rad;
    _x = length * cos(rad + theta * (3.14159265359 / 180.0));
    _y = length * sin(rad + theta * (3.14159265359 / 180.0));
}

void Vector2D::Swap()
{
    std::swap(_x, _y);
}

void Vector2D::Set(const Vector2D &start, const Vector2D &end)
{
    _x = end._x - start._x;
    _y = end._y - start._y;
}

void Vector2D::Set(double elemX, double elemY)
{
    _x = elemX;
    _y = elemY;
}

void Vector2D::Set(double startX, double startY, double endX, double endY)
{
    _x = endX - startX;
    _y = endY - startY;
}

double Vector2D::GetLength() const
{
    return sqrt(_x * _x + _y * _y);
}

double Vector2D::GetSqLength() const
{
    return (_x * _x + _y * _y);
}

//���ϒl���Z�o
double Vector2D::Dot(const Vector2D &vecA, const Vector2D &vecB)
{
    return (vecA._x * vecB._x + vecA._y * vecB._y);
}

//�N���X�ς��o��
double Vector2D::Cross(const Vector2D &vecA, const Vector2D &vecB)
{
    return (vecA._x * vecB._y - vecA._y * vecB._x);
}

//Z�����̃x�N�g��(0, 0, 1)��3������ŊO�όv�Z���A�Z�o���ꂽ�x�N�g�����擾
Vector2D Vector2D::Cross(const Vector2D &vecA, bool useRightHandSystem)
{
    //Vector3 c = (0, 0, 1) �ƌ�������
    double z = (useRightHandSystem) ? 1.0 : -1.0;
    
    //bx = ay * cz - cy * az = ay * cz;
    //by = az * cy - cz * ax = -cz * ax;
    //bz = ay * cx - cy * ax = 0;
    
    return std::move(Vector2D(vecA._y * z, -vecA._x * z));
}

Vector2D Vector2D::GetNormalized() const
{
    Vector2D temp(_x, _y);
    temp.Normalize();
    
    return std::move(temp);
}

Vector2D Vector2D::GetRotated(double theta) const
{
    Vector2D temp(_x, _y);
    temp.Rotate(theta);
    
    return std::move(temp);
}