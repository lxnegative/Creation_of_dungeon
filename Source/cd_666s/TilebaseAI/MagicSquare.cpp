#include "MagicSquare.h"
#include "TileField.h"
#include "TiledObjectMnager.h"
#include "Monster.h"
#include "../InputManager/InputManager.h"
#include "../DebugDraw.h"
#include "../Resources/ResourceManager.h"

MagicSquare::MagicSquare(TiledVector tilePos, Monster& monster)
: TiledObject(tilePos)
, _monster(monster)
{
    _type = TiledObject::Type::MAGIC_SQUARE;
    _position = GetTilePos().GetWorldPos();

    /*
    _currentGraph = _animation.SetWithCreate(Main::RESOURCEFILENAME + "resource/graph/tiledObject/magic_square.png", 32, 32, 5, 16);
    _currentGraph->SetPosition(_position);
    _currentGraph->SetPriority(-1);
    _currentGraph->SetScale(Vector2D(TILE_SIZE / 32.0, TILE_SIZE / 32.0));
    _currentGraph->SetDisplayMode(false);
    */

    _currentGraph.Load(Main::RESOURCEFILENAME + "resource/graph/tiledObject/magic_square.png");
    _currentGraph.SetPosition(_position);
    _currentGraph.SetPriority(-1);
    _currentGraph.SetScale(Vector2D(TILE_SIZE / 32.0, TILE_SIZE / 32.0));
    _currentGraph.SetDisplayMode(false);

    _graph.Load(Main::RESOURCEFILENAME + "resource/graph/tiledObject/magicSquare_none.png");
    _graph.SetPosition(_position);
    _graph.SetPriority(Sprite::Priority::FIELD_OBJECT);
    _graph.SetScale(Vector2D(TILE_SIZE / 32.0, TILE_SIZE / 32.0));

    int divNum = _currentGraph.GetSize()._x / 32;

    _animation.Set(&_currentGraph, 32, 32, divNum, 56);
    _animation._isLoop = true;

	isFastInteract = false;

}


MagicSquare::~MagicSquare()
{
}


void MagicSquare::Update()
{
    if (MOUSE->DoubleClicked())
    {
        auto tiledCursorPos = TiledVector::ConvertToTiledPos(MOUSE->GetCursorPos());
        if (!FIELD->IsInside(tiledCursorPos))
            return;
        
        //自分の位置がクリックされていなければ終了
        if (tiledCursorPos != GetTilePos())
            return;
        
        BattleParameter _monsterParam = _monster.GetRawParameter();

        //生きていたら
        if (_monster.IsAlive() && _monster.IsEnable() && GetTilePos() != _monster.GetTilePos())
        {
            //送還
            WarpHomeMonster();
            return;
        }
        //召喚
        //ProduceMonster();
        
    }

    if (MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L))
    {
        auto tiledCursorPos = TiledVector::ConvertToTiledPos(MOUSE->GetCursorPos());
        if (!FIELD->IsInside(tiledCursorPos))
            return;

        //自分の位置がクリックされていなければ終了
        if (tiledCursorPos != GetTilePos())
            return;

        //召喚
        ProduceMonster();
    }

	if (_monster.IsEnable()) {
		isFastInteract = false;
	}

}


void MagicSquare::WarpHomeMonster()
{
    //上に何かあるなどでモンスターを移動できないとワープできない
    if (!GetTile().lock()->IsRegistable(_monster))
        return;
     
	isFastInteract = true;

    _monster.WarpToHome(*this);
}


void MagicSquare::ProduceMonster()
{
    //上に何かあるなどでモンスターを移動できないと召喚できない
    if (!GetTile().lock()->IsRegistable(_monster))
        return;
    
    if (_monster.IsReadyToProduce())
        _monster.Appear();
}


void MagicSquare::Draw()
{
    if (_monster.IsReadyToProduce())
    {
		_graph.SetDisplayMode(false);
		_currentGraph.SetDisplayMode(true);
        _animation.Update();
    }
    else
    {
		_graph.SetDisplayMode(true);
		_currentGraph.SetDisplayMode(false);
        _animation.SetIndex(0);

		//耐久力ゲージを表示するか
		if (!_monster._hasAppeared) {
			auto param = _monster.GetAffectedParameter();
			double ratio = static_cast<double>(param._hp) / static_cast<double>(param._maxHP);
			Debug::DrawRectWithSize(_position + Vector2D(-16, 0), Vector2D(64, 8), ColorPalette::BLACK4, false);
			Debug::DrawRectWithSize(_position + Vector2D(-16, 0), Vector2D(64 * ratio, 8), ColorPalette::BLUE4, true);
		}

    }

    //_currentGraph = _animation.GetGraphPtr();
}


bool MagicSquare::IsOverwritable(TiledObject* overwriter)
{
    return true;
}


void MagicSquare::Interact(Character& character)
{
    //フィールドに出現中は回復できない
    if (character.IsEnable())
        return;

    auto param = character.GetAffectedParameter();
    auto hp = param._hp;
    auto maxHP = param._maxHP;
    if (hp < maxHP)
    {
        //character.Damaged(-1);

		int recoveryRate = 600;

		if (isFastInteract) {
			recoveryRate = 400;
		}

        hp += maxHP / recoveryRate + 1;
        hp = max(0, min(hp, maxHP));
        character._battleParameter._hp = hp;
    }
}
