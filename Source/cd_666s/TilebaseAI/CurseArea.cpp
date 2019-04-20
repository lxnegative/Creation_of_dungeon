#include "CurseArea.h"
#include "Enemy.h"
#include "TileField.h"
#include "TiledObjectMnager.h"
#include "../Resources/ResourceManager.h"

CurseArea::CurseArea(TiledVector pos, int cost, ParameterMultiplier multiplier, std::string graphName)
    : Trap(pos, cost, 5)
    , _multiplier(std::move(multiplier))
{
	if (!graphName.empty()) {
		_graph.Load(Main::RESOURCEFILENAME + "resource/graph/trap/" + graphName + ".png");
	}
	else {
		_graph.Load(Main::RESOURCEFILENAME + "resource/graph/trap/CurseArea.png");
	}
    
    _sound.Load(Main::RESOURCEFILENAME + "resource/sound/curseArea.wav");
    _sound.SetVolume(200);
}


CurseArea::~CurseArea()
{
}


void CurseArea::Activate()
{
    if (_targetCache == nullptr)
        return;

    Trap::Activate();
    _targetCache->AddParameterEffecter(std::make_unique<ParameterMultiplier>(_multiplier));
    _sound.Play();
}


bool CurseArea::IsActivatable() const
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
