#include "MineBomb.h"
#include "MapTile.h"
#include "Enemy.h"
#include "BattleCaliculate.h"
#include "../DebugDraw.h"
#include "TiledObjectMnager.h"

MineBomb::MineBomb(TiledVector trapPos, int cost, int range, int power, int attack, int stuckedTime, bool mine, std::string graphName)
    : Trap(trapPos, cost, 5)
    , _range(range)
    , _power(power)
    , _attack(attack)
    , _targetCache(nullptr)
    , _stuckedTime(stuckedTime)
    , _isMine(mine)
{
    if (_isMine)
    {
		if (!graphName.empty()) {
			_graph.Load(Main::RESOURCEFILENAME + "resource/graph/trap/" + graphName + ".png");
		}
		else {
			_graph.Load(Main::RESOURCEFILENAME + "resource/graph/trap/mine.png");
		}
        
        _explodeSounde.Load(Main::RESOURCEFILENAME + "resource/sound/flame.wav");
    }
    else
    {
		if (!graphName.empty()) {
			_graph.Load(Main::RESOURCEFILENAME + "resource/graph/trap/" + graphName + ".png");
		}
		else {
			_graph.Load(Main::RESOURCEFILENAME + "resource/graph/trap/bareTrap.png");
		}
        
        _explodeSounde.Load(Main::RESOURCEFILENAME + "resource/sound/enemy_fall2.wav");
    }

    _graph.SetPriority(200);
    _graph.SetPosition(_position);

    int divNum = _graph.GetSize()._x / _graph.GetSize()._y;
    _graphArray.Set(&_graph, 32, 32, divNum, divNum * 4);

    _graphArray._isLoop = false;
    _graphArray._isPlaying = false;
}


MineBomb::~MineBomb()
{
}


void MineBomb::Draw()
{
    _graphArray.Update();

    bool display = (!_graphArray.HasEndedUp() || IsEnable());

    //�������ɂ��邩�ǂ���
    float alpha = (display) ? 1.0f : 0.35f;
    _graph.GetTexturePtr().lock()->SetBaseColor(Color4(1, 1, 1, alpha));

    //�ϋv�̓Q�[�W��\�����邩
    if (!display)
    {
        double ratio = static_cast<double>(_duravity) / static_cast<double>(_cost);
        Debug::DrawRectWithSize(_position + Vector2D(-16, 0), Vector2D(64, 8), ColorPalette::BLACK4, false);
        Debug::DrawRectWithSize(_position + Vector2D(-16, 0), Vector2D(64 * ratio, 8), ColorPalette::BLUE4, true);
    }

    if (IsEnable())
        _graphArray.SetIndex(0);
}


void MineBomb::Activate()
{
    if (_targetCache == nullptr)
        return;

    Trap::Activate();
    _graphArray._isPlaying = true;

    auto param = _targetCache->GetAffectedParameter();
    _targetCache->Damaged(Battle::GetPhysicalAttackDamage(_power, _attack, param._defence));
    _targetCache->StuckOn(_stuckedTime);

    _explodeSounde.Play();
}


bool MineBomb::IsActivatable() const
{
    if (!Trap::IsActivatable())
        return false;

    bool isOverrided = false;

    //�G�������̃}�X�ɂ��邩�ǂ���
    Enemy* target = GetTile().lock()->GetTiledObject<Enemy>();
    if (target != nullptr)
    {
        //��苗���܂ŋ߂Â����甭��
        auto distance = (target->GetPosition() - _position).GetLength();
        if (distance < TILE_SIZE / 4)
        {
            isOverrided = true;
            _targetCache = target;
        }
    }

    return isOverrided;
}