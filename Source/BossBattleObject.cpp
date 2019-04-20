#include "BossBattleObject.h"
#include "cd_666s/TilebaseAI/TiledObjectMnager.h"
#include "cd_666s/TilebaseAI/TiledVector.h"
#include "cd_666s/TilebaseAI/ColleagueNotifyer.h"

BossBattleObject::BossBattleObject(Vector2D startPos, BattleParameter param, ColleagueNotifyer& notifyer, std::string name, std::string resourceName, std::string displayName, bool _isBoss)
	: Character(TiledVector::ConvertToTiledPos(startPos), param, notifyer , name, TiledObject::Type::BOSS_SCENE, resourceName, displayName, _isBoss)
	, _isInRoom(false)
	, move_sum(0.0)
{
	_position = startPos;
	_appearSE.Load(Main::RESOURCEFILENAME + "resource/sound/blockSelect.wav");
	
	_direction = TiledVector::Direction::LEFT;
	_animator.SwitchWithReset("left");

	_param = param;

	auto currentGraph = _animator.GetCurrentGraph();
	currentGraph->SetDisplayMode(_isInRoom);

#if 0
	std::string fileName = Main::RESOURCEFILENAME + "resource/graph/tiledObject/";

	fileName += name;

	int _graphSize = 64;
	if (name.find("boss") == std::string::npos) {
		_objType = ObjectType::ENEMY;
		fileName += "_left";
		_graphSize = 32;
	}
	else {
		fileName += "_right";
	}

	_animator.AddAnimation("", std::make_shared<GraphArray>(fileName + ".png", _graphSize, 24));

	auto currentGraph = _animator.GetCurrentGraph();
	currentGraph->SetDisplayMode(false);
	_animator.Transform([&](GraphArray* animation)
	{
		animation->GetGraphPtr()->SetPosition(_position);
		//animation->GetGraphPtr()->SetScale(Vector2D(TILE_SCALE, TILE_SCALE));
		animation->GetGraphPtr()->SetRenderType(Texture2D::RenderType::UI);
	});
#endif
}

BossBattleObject::BossBattleObject(Vector2D startPos, BattleParameter param, ColleagueNotifyer& notifyer, std::string name, bool _isBoss)
	: Character(TiledVector::ConvertToTiledPos(startPos), param, notifyer, name, TiledObject::Type::BOSS_SCENE, _isBoss)
	, _isInRoom(false)
	, move_sum(0.0)
{
	_position = startPos;
	_appearSE.Load(Main::RESOURCEFILENAME + "resource/sound/blockSelect.wav");

	_direction = TiledVector::Direction::LEFT;
	_animator.SwitchWithReset("left");

	_param = param;

	auto currentGraph = _animator.GetCurrentGraph();
	currentGraph->SetDisplayMode(_isInRoom);

}


BossBattleObject::~BossBattleObject()
{
}

void BossBattleObject::Init()
{
}

void BossBattleObject::Update()
{

	GraphicalObject::Update();

	if (!_isInRoom)
		return;


	//姿勢情報更新
	_animator.Transform([&](GraphArray* animation)
	{
		animation->GetGraphPtr()->SetPosition(_position);
	});
}

void BossBattleObject::Draw()
{
	auto currentGraph = _animator.GetCurrentGraph();
	currentGraph->SetDisplayMode(_isInRoom);

	if (!_isInRoom)
		return;

	switch (_direction)
	{
	case TiledVector::Direction::FORWARD:
		if (isSkillActivate && ExistSkillAnimation_f) {
			_animator.GetCurrentAnimation()->_isLoop = false;
			_animator.SwitchWithReset("skill_front");

		}
		else {
			_animator.GetCurrentAnimation()->_isLoop = true;
			_animator.SwitchWithReset("front");
		}

		break;

	case TiledVector::Direction::LEFT:
		if (isSkillActivate && ExistSkillAnimation_l) {
			_animator.GetCurrentAnimation()->_isLoop = false;
			_animator.SwitchWithReset("skill_left");
		}
		else {
			_animator.GetCurrentAnimation()->_isLoop = true;
			_animator.SwitchWithReset("left");
		}

		break;

	case TiledVector::Direction::RIGHT:
		if (isSkillActivate && ExistSkillAnimation_r) {
			_animator.GetCurrentAnimation()->_isLoop = false;
			_animator.SwitchWithReset("skill_right");
		}
		else {
			_animator.GetCurrentAnimation()->_isLoop = true;
			_animator.SwitchWithReset("right");
		}

		break;

	case TiledVector::Direction::BACK:
		if (isSkillActivate && ExistSkillAnimation_b) {
			_animator.GetCurrentAnimation()->_isLoop = false;
			_animator.SwitchWithReset("skill_back");
		}
		else {
			_animator.GetCurrentAnimation()->_isLoop = true;
			_animator.SwitchWithReset("back");
		}

		break;
	}

	_animator.Update();

	/*以下、ボスのHP表示(Debug仕様)*/
	if (Character::GetName().find("boss") == std::string::npos)
		return;


	return;

	const int height = 20;
	auto _pos = Vector2D(_position._x, _position._y - 30);
	auto _wh1 = Vector2D(_param._hp, height);
	auto _wh2 = Vector2D(_param._maxHP, height);

	Debug::DrawRectWithSize_TopPriority(_pos, _wh1, ColorPalette::RED4, true);
	Debug::DrawRectWithSize_TopPriority(_pos, _wh2, ColorPalette::RED4, false);
}

void BossBattleObject::Clear()
{
	_notifyer.NotifyRemoveTarget(*this);
}

void BossBattleObject::SwitchAnime(bool _isAnimate)
{
	_animator.GetCurrentAnimation()->_isLoop = _isAnimate;
	if (!_isAnimate) {
		_animator.GetCurrentAnimation()->SetIndex(0);
	}
}

void BossBattleObject::OutBossRoom()
{
	_isInRoom = false;
	SwitchAnime(false);
}
