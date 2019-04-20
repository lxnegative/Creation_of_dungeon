#include "Trap.h"
#include "../DebugDraw.h"
#include "../../mw_animation/GraphArray.h"

Trap::Trap(TiledVector startPos, int duravity, int level)
    : TiledObject(startPos)
    , _duravity(duravity)
    , _cost(duravity)
    , _level(level)
{
    _type = TiledObject::Type::TRAP;
    _position = startPos.GetWorldPos();
    _activateSound.Load(Main::RESOURCEFILENAME + "resource/sound/activateTrap.wav");
}


Trap::~Trap()
{
}


void Trap::Update()
{
    _duravity++;
    _duravity = min(_cost, max(_duravity, 0));

    if (IsActivatable())
        Activate();
}


void Trap::Draw()
{
    GraphicalObject::Draw();

    bool display = Trap::IsActivatable();

    //�������ɂ��邩�ǂ���
    float alpha = (display) ? 1.0f : 0.35f;
    auto tex = _graph.GetTexturePtr();
    if (!tex.expired())
        tex.lock()->SetBaseColor(Color4(1, 1, 1, alpha));

    //�ϋv�̓Q�[�W��\�����邩
    if (!display)
    {
        double ratio = static_cast<double>(_duravity) / static_cast<double>(_cost);
        Debug::DrawRectWithSize(_position + Vector2D(-16, 0), Vector2D(64, 8), ColorPalette::BLACK4, false);
        Debug::DrawRectWithSize(_position + Vector2D(-16, 0), Vector2D(64 * ratio, 8), ColorPalette::BLUE4, true);
    }
}

//�g���b�v���N���ł��邩�ǂ���
bool Trap::IsActivatable() const
{
    return IsEnable();
}

//�g���b�v���N��
void Trap::Activate()
{
    _duravity -= _cost;
    _activateSound.Play();
}


bool Trap::Contained(const GraphArray& animation, const Vector2D& pos)
{
    auto size = animation.GetSingleSize();
    auto position = animation.GetGraphPtr()->GetPosition();

    if (pos._x < position._x)
        return false;
    if (pos._y < position._y)
        return false;
    if (position._x + size._x * TILE_SCALE < pos._x)
        return false;
    if (position._y + size._y * TILE_SCALE  < pos._y)
        return false;

    return true;
}