#pragma once
#include "BattleParameter.h"
#include "ParameterEffecter.h"

class ParameterMultiplier : public ParameterEffecter
{
public:

    //�S�����Ŏw��
    ParameterMultiplier(BattleParameter multiplyPercent, int count, bool isPositive);
    ~ParameterMultiplier();

    //�ΏۂɌ��ʂ�t�^
    void AffectParameter(BattleParameter& param) override;

    // MEMO : �o�t�̕\�����@�����܂�܂ł̃f�o�b�O�\��
    void DrawEffecter(Vector2D anchorPos) override;

private:

    BattleParameter _multiplyPercent;
};

