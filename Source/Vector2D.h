#ifndef VECTOR2D_H
#define VECTOR2D_H

class Vector2D
{
public:
    Vector2D() : _x(0), _y(0){};
    Vector2D(double elemX, double elemY);
    Vector2D(double startX, double startY, double endX, double endY);
    Vector2D(const Vector2D &start, const Vector2D &end);
    ~Vector2D();
    
    inline void Set(const Vector2D &start, const Vector2D &end);
    inline void Set(double startX, double startY, double endX, double endY);
    inline void Set(double elemX, double elemY);
    
    const Vector2D operator+(const Vector2D &vec) const;
    const Vector2D operator+=(const Vector2D &vec);
    const Vector2D operator-(const Vector2D &vec) const;
    const Vector2D operator-=(const Vector2D &vec);
    const Vector2D operator*(const Vector2D &vec) const;
    const Vector2D operator*=(const Vector2D &vec);
    const Vector2D operator*(const double scale) const;
    const Vector2D operator*=(const double scale);
    const bool operator==(const Vector2D &vec) const;
    const bool operator!=(const Vector2D &vec) const;

    //�x�N�g���̒�����Ԃ�
    inline double GetLength() const;
    //�x�N�g������2���Ԃ�
    inline double GetSqLength() const;
    
    //�x�N�g���𐳋K���������̂�Ԃ�
    inline Vector2D GetNormalized() const;
    //�x�N�g�����w��̊p�x(�x���w��)��]���������̂�Ԃ�
    inline Vector2D GetRotated(double theta) const;
    
    //�x�N�g���̊e������value�{�ɂ���
    inline void Magnify(double value);
    //�x�N�g���𐳋K������
    inline void Normalize();
    //�x�N�g�������_����_��,Z�������Ɏw��̊p�x(�x���w��)������]������
    inline void Rotate(double theta);
    //x��y�����ւ���
    inline void Swap();
    
    //���ϒl���Z�o
    inline static double Dot(const Vector2D &vecA, const Vector2D &vecB);
    //�N���X�ς��o��
    inline static double Cross(const Vector2D &vecA, const Vector2D &vecB);
    
    //Z�����̃x�N�g��(0, 0, 1)��3������ŊO�όv�Z���A�Z�o���ꂽ�x�N�g�����擾
    //vec : �������Z�o�������x�N�g��
    //useRightHandSystem : �E��n�ł̐���Z���W�̌������g�p���邩�ǂ���
    inline static Vector2D Cross(const Vector2D &vec, bool useRightHandSystem);
    
    //���_
    static const Vector2D zero;
    
    //�J������Up�x�N�g����Ō������ꂼ��̕����x�N�g��
    static const Vector2D up;
    static const Vector2D left;
    static const Vector2D right;
    static const Vector2D down;
    
    double _x, _y;
};

#include "Vector2D_InlineDef.h"

#endif