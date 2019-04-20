#include "ParameterRiser.h"
#include "../DebugDraw.h"

ParameterRiser::ParameterRiser(BattleParameter addedParameter, int count, bool isPositive)
    : ParameterEffecter(count, isPositive)
    , _addedParameter(std::move(addedParameter))
{
}


ParameterRiser::~ParameterRiser()
{
}


//�ΏۂɌ��ʂ�t�^
void ParameterRiser::AffectParameter(BattleParameter& param)
{
    param += _addedParameter;
}


// MEMO : �o�t�̕\�����@�����܂�܂ł̃f�o�b�O�\��
void ParameterRiser::DrawEffecter(Vector2D anchorPos)
{
	/*
    Debug::DrawCircle(anchorPos, 16, ColorPalette::BLACK4, false);
    if (IsPositive())
        Debug::DrawString(anchorPos - Vector2D(8, 8), "P+");
    else
        Debug::DrawString(anchorPos - Vector2D(8, 8), "P-");
	*/
}