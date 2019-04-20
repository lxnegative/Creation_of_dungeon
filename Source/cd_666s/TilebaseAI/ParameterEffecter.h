#pragma once
#include "../../Vector2D.h"

class BattleParameter;

class ParameterEffecter
{
public:
    ParameterEffecter(int continuousTime, bool isPositive);
    ~ParameterEffecter();

    //���Ԍo��
    void UpdateCounter();

    //�ǉ����ʂ��L�����ǂ���
    bool IsEnable() const { return _time < _affectionTime; }

    bool IsPositive() const { return _isPositive; }

    //�ΏۂɌ��ʂ�t�^
    virtual void AffectParameter(BattleParameter& param) = 0 {};

    // MEMO : �o�t�̕\�����@�����܂�܂ł̃f�o�b�O�\��
    virtual void DrawEffecter(Vector2D anchorPos) = 0 {};

private:

    int _time;
    int _affectionTime;

    //�o�t���f�o�t��
    bool _isPositive;

    //���ʂ�\���A�C�R��
    //Sprite _icon;
};

