#pragma once
#include "BattleParameter.h"
#include "ParameterEffecter.h"

class ParameterRiser : public ParameterEffecter
{
public:
    //���Z�����l���w��
    ParameterRiser(BattleParameter addedParameter, int count, bool isPositive);
    ~ParameterRiser();

    //�ΏۂɌ��ʂ�t�^
    void AffectParameter(BattleParameter& param) override;

    // MEMO : �o�t�̕\�����@�����܂�܂ł̃f�o�b�O�\��
    void DrawEffecter(Vector2D anchorPos) override;

private:

    BattleParameter _addedParameter;
};

