#include "Character.h"
#include "TiledObjectMnager.h"
#include "ColleagueNotifyer.h"
#include "TileField.h"
#include "BattlingTile.h"
#include "../DebugDraw.h"
#include "Enemy.h"


Character::Character(TiledVector startPos, const BattleParameter param, ColleagueNotifyer& notifyer, std::string name, TiledObject::Type type, std::string resourceName, std::string displayName, bool isBoss)
: TiledObject(startPos)
, _direction(TiledVector::Direction::FORWARD)
, _battleParameter(param)
, _notifyer(notifyer)
, _afterBattleTimer(1, false, false)
, _name(name)
, _isBattling(false)
, _hasAppeared(false)
, _hasArrived(false)
, _target(nullptr)
, _battle(nullptr)
, _defeatSE(Main::RESOURCEFILENAME + "resource/sound/enemy_fall2.wav")
, _infoIcon(_position, _effecters)
, _actCounter(1, true, true)
{
	_type = type;

	int _graphSize = 32;
	int _endTime = 24;

	if(isBoss){	//ボスかどうか
		_graphSize = 64;
		_endTime = 90;
	}
	
	_notifyer.AddColleague(*this);
	
    _battleParameter._speed = fmin(100, fmax(_battleParameter._speed, 0));
    double speedRatio = static_cast<double>(100 - _battleParameter._speed + 15) / 100;
    _actCounter.Reset(30 * speedRatio, true, true);

    std::string fileName = Main::RESOURCEFILENAME + "resource/graph/tiledObject/";
    fileName += resourceName;
    _animator.AddAnimation("front", std::make_shared<GraphArray>(fileName + "_front.png", _graphSize, _endTime));
    _animator.AddAnimation("right", std::make_shared<GraphArray>(fileName + "_right.png", _graphSize, _endTime));
    _animator.AddAnimation("left", std::make_shared<GraphArray>(fileName + "_left.png", _graphSize, _endTime));
    _animator.AddAnimation("back", std::make_shared<GraphArray>(fileName + "_back.png", _graphSize, _endTime));

	if (CheckFileExistence(fileName + "_attack_front.png")) {
		_animator.AddAnimation("skill_front", std::make_shared<GraphArray>(fileName + "_attack_front.png", _graphSize, _actCounter.GetInterval()));
		ExistSkillAnimation_f = true;
	}
	else {
		ExistSkillAnimation_f = false;
	}
	if (CheckFileExistence(fileName + "_attack_right.png")) {
		_animator.AddAnimation("skill_right", std::make_shared<GraphArray>(fileName + "_attack_right.png", _graphSize, _actCounter.GetInterval()));
		ExistSkillAnimation_r = true;
	}
	else {
		ExistSkillAnimation_r = false;
	}
	if (CheckFileExistence(fileName + "_attack_left.png")) {
		_animator.AddAnimation("skill_left", std::make_shared<GraphArray>(fileName + "_attack_left.png", _graphSize, _actCounter.GetInterval()));
		ExistSkillAnimation_l = true;
	}
	else {
		ExistSkillAnimation_l = false;
	}
	if (CheckFileExistence(fileName + "_attack_back.png")) {
		_animator.AddAnimation("skill_back", std::make_shared<GraphArray>(fileName + "_attack_back.png", _graphSize, _actCounter.GetInterval()));
		ExistSkillAnimation_b = true;
	}
	else {
		ExistSkillAnimation_b = false;
	}

	_resourceName = fileName;

	_displayName = displayName;

    auto currentGraph = _animator.GetCurrentGraph();
    currentGraph->SetDisplayMode(false);
    _animator.Transform([&](GraphArray* animation)
    {
        animation->GetGraphPtr()->SetPosition(_position);
        animation->GetGraphPtr()->SetScale(Vector2D(TILE_SCALE, TILE_SCALE));
        animation->GetGraphPtr()->SetRenderType(Texture2D::RenderType::UI);
    });

    _healGraph.Load(Main::RESOURCEFILENAME + "resource/graph/effect/heal.png");
    _healGraph.SetScale(Vector2D(TILE_SCALE, TILE_SCALE));
    _healGraph.SetDisplayMode(false);
	_healGraph.SetRenderType(Texture2D::RenderType::UI);
	_healGraph.SetPriority(static_cast<int>(Sprite::Priority::UI) - 1);

    auto size = _healGraph.GetSize();
    int healdivNum = size._x / size._y;
    _healAnimation.Set(&_healGraph, 32, 32, healdivNum, healdivNum * 6);
    _healAnimation._isLoop = false;
    _healAnimation._isPlaying = false;

    if (_type == Type::ENEMY)
        _afterBattleTimer.Reset(60, false, false);
    else
        _afterBattleTimer.Reset(0, false, false);

    _position = startPos.GetWorldPos();
    _beforeTilePos = GetTilePos();

	Level = -1;

	isSkillActivate = false;

}

Character::Character(TiledVector startPos, const BattleParameter param, ColleagueNotifyer& notifyer, std::string name, TiledObject::Type type, bool isBoss)
	: TiledObject(startPos)
	, _direction(TiledVector::Direction::FORWARD)
	, _battleParameter(param)
	, _notifyer(notifyer)
	, _afterBattleTimer(1, false, false)
	, _name(name)
	, _isBattling(false)
	, _hasAppeared(false)
	, _hasArrived(false)
	, _target(nullptr)
	, _battle(nullptr)
	, _defeatSE(Main::RESOURCEFILENAME + "resource/sound/enemy_fall2.wav")
	, _infoIcon(_position, _effecters)
	, _actCounter(1, true, true)
{
	_type = type;

	int _graphSize = 32;
	int _endTime = 24;

	if (isBoss) {	//ボスかどうか
		_graphSize = 64;
		_endTime = 6;
	}

	_notifyer.AddColleague(*this);

	_battleParameter._speed = fmin(100, fmax(_battleParameter._speed, 0));
	double speedRatio = static_cast<double>(100 - _battleParameter._speed + 15) / 100;
	_actCounter.Reset(30 * speedRatio, true, true);

	std::string fileName = Main::RESOURCEFILENAME + "resource/graph/tiledObject/";
	fileName += _name;
	_animator.AddAnimation("front", std::make_shared<GraphArray>(fileName + "_front.png", _graphSize, _endTime));
	_animator.AddAnimation("right", std::make_shared<GraphArray>(fileName + "_right.png", _graphSize, _endTime));
	_animator.AddAnimation("left", std::make_shared<GraphArray>(fileName + "_left.png", _graphSize, _endTime));
	_animator.AddAnimation("back", std::make_shared<GraphArray>(fileName + "_back.png", _graphSize, _endTime));

	if (CheckFileExistence(fileName + "_attack_front.png")) {
		_animator.AddAnimation("skill_front", std::make_shared<GraphArray>(fileName + "_attack_front.png", _graphSize, _actCounter.GetInterval()));
		ExistSkillAnimation_f = true;
	}
	else {
		ExistSkillAnimation_f = false;
	}
	if (CheckFileExistence(fileName + "_attack_right.png")) {
		_animator.AddAnimation("skill_right", std::make_shared<GraphArray>(fileName + "_attack_right.png", _graphSize, _actCounter.GetInterval()));
		ExistSkillAnimation_r = true;
	}
	else {
		ExistSkillAnimation_r = false;
	}
	if (CheckFileExistence(fileName + "_attack_left.png")) {
		_animator.AddAnimation("skill_left", std::make_shared<GraphArray>(fileName + "_attack_left.png", _graphSize, _actCounter.GetInterval()));
		ExistSkillAnimation_l = true;
	}
	else {
		ExistSkillAnimation_l = false;
	}
	if (CheckFileExistence(fileName + "_attack_back.png")) {
		_animator.AddAnimation("skill_back", std::make_shared<GraphArray>(fileName + "_attack_back.png", _graphSize, _actCounter.GetInterval()));
		ExistSkillAnimation_b = true;
	}
	else {
		ExistSkillAnimation_b = false;
	}


	_resourceName = fileName;

	_displayName = name;

	auto currentGraph = _animator.GetCurrentGraph();
	currentGraph->SetDisplayMode(false);
	_animator.Transform([&](GraphArray* animation)
	{
		animation->GetGraphPtr()->SetPosition(_position);
		animation->GetGraphPtr()->SetScale(Vector2D(TILE_SCALE, TILE_SCALE));
		animation->GetGraphPtr()->SetRenderType(Texture2D::RenderType::UI);
	});

	_healGraph.Load(Main::RESOURCEFILENAME + "resource/graph/effect/heal.png");
	_healGraph.SetScale(Vector2D(TILE_SCALE, TILE_SCALE));
	_healGraph.SetDisplayMode(false);
	_healGraph.SetPriority(Sprite::Priority::UI);

	auto size = _healGraph.GetSize();
	int healdivNum = size._x / size._y;
	_healAnimation.Set(&_healGraph, 32, 32, healdivNum, healdivNum * 6);
	_healAnimation._isLoop = false;
	_healAnimation._isPlaying = false;

	if (_type == Type::ENEMY)
		_afterBattleTimer.Reset(60, false, false);
	else
		_afterBattleTimer.Reset(0, false, false);

	_position = startPos.GetWorldPos();
	_beforeTilePos = GetTilePos();

	Level = -1;

	isSkillActivate = false;

}


Character::~Character()
{
	_notifyer.RemoveColleague(*this);
}


void Character::Update()
{
    GraphicalObject::Update();

    if (!IsEnable())
        return;

    //姿勢情報更新
    _animator.Transform([&](GraphArray* animation)
    {
        animation->GetGraphPtr()->SetPosition(_position);
    });

    //バフ更新
    std::vector<std::unique_ptr<ParameterEffecter>> copy;
    for (size_t i = 0; i<_effecters.size(); ++i)
    {
        if (_effecters[i]->IsEnable())
        {
            _effecters[i]->UpdateCounter();
            copy.push_back(std::move(_effecters[i]));
        }
    }
    _effecters = std::move(copy);

    //スキル更新
    if (_skill.get() != nullptr)
        _skill->Update();

    //回復エフェクト
    if (_healAnimation.HasEndedUp())
    {
        _healAnimation.SetIndex(0);
        _healGraph.SetDisplayMode(false);
    }
    else
    {
        _healAnimation.Update();
        _healAnimation.GetGraphPtr()->SetPosition(_position);
    }
}


//意思決定→行動の順序に基づいた操作
void Character::Action()
{
    _actCounter.Update();
    if (_actCounter.CountOnEnd())
    {
		isSkillActivate = false;
        //指示がなければ終了
        if (_ai == nullptr)
            return;

        //視界
        _sight = FIELD->GetParabolicMovableCell(GetTilePos(), 4, _direction);

        //意思決定
        Think();

        //移動先との差分から向きを更新
        UpdateAttitude();

        //意思遂行
        Act();
    }

    auto vec = (GetTilePos() - _beforeTilePos).GetWorldPos() - Vector2D(FIELD_OFFSET_X, FIELD_OFFSET_Y);
    _position += vec * (1.0 / (_actCounter.GetInterval() + 1));
}


void Character::Draw()
{
    auto currentGraph = _animator.GetCurrentGraph();
    currentGraph->SetDisplayMode(_hasAppeared && !_isBattling);

    if (!_hasAppeared || _isBattling)
        return;

    switch (_direction)
    {
    case TiledVector::Direction::FORWARD:
		if (isSkillActivate && ExistSkillAnimation_f) {
			_animator.SwitchWithReset("skill_front");
		}
		else {
			_animator.SwitchWithReset("front");
		}
        
        break;

    case TiledVector::Direction::LEFT:
		if (isSkillActivate && ExistSkillAnimation_l) {
			_animator.SwitchWithReset("skill_left");
		}
		else {
			_animator.SwitchWithReset("left");
		}
        
        break;

    case TiledVector::Direction::RIGHT:
		if (isSkillActivate && ExistSkillAnimation_r) {
			_animator.SwitchWithReset("skill_right");
		}
		else {
			_animator.SwitchWithReset("right");
		}
        
        break;

    case TiledVector::Direction::BACK:
		if (isSkillActivate && ExistSkillAnimation_b) {
			_animator.SwitchWithReset("skill_back");
		}
		else {
			_animator.SwitchWithReset("back");
		}
        
        break;
    }

    if (_position != GetTilePos().GetWorldPos())
    {
        _animator.Update();
    }

    //AIのデバッグ情報
    if (_ai != nullptr)
        _ai->Draw();

    //パラメータ情報のデバッグ
    _infoIcon.Update();
}


bool Character::Contain(Vector2D pos) const
{
    auto size = _animator.GetCurrentAnimation()->GetSingleSize();
    
    if (pos._x < _position._x)
        return false;
    if (pos._y < _position._y)
        return false;
    if (_position._x + size._x * TILE_SCALE < pos._x)
        return false;
    if (_position._y + size._y * TILE_SCALE  < pos._y)
        return false;

    return true;
}

void Character::UpdateAttitude()
{
    TiledVector pos = GetTilePos();

    //移動先との差分から向きを更新
    TiledVector dir;
    if (_pathToTarget.size() == 0)
    {
        if (_target != nullptr)
            dir = _target->GetTilePos() - pos;
    }
    else
    {
        dir = _pathToTarget[0] - pos;
    }

    if (dir == TiledVector::up)
        _direction = TiledVector::Direction::FORWARD;
    else if (dir == TiledVector::down)
        _direction = TiledVector::Direction::BACK;
    else if (dir == TiledVector::left)
        _direction = TiledVector::Direction::LEFT;
    else if (dir == TiledVector::right)
        _direction = TiledVector::Direction::RIGHT;

    _beforeTilePos = pos;
}


void Character::OnOccuredBattle(BattlingTile* battle)
{
    _beforeTilePos = GetTilePos();
    _position = GetTilePos().GetWorldPos();
    _battle = battle;
    _isBattling = true;

    ResetTarget();
}


void Character::OnFinishBattle(BattlingTile* battle)
{
    if (battle == _battle)
        _battle = nullptr;

    _isBattling = false;
    _afterBattleTimer.Play();
}


void Character::ResetTarget()
{
    _target = nullptr;
    _pathToTarget.clear();
    _pathToTarget.resize(0);
}

bool Character::CheckActable()
{
    //出現してなければ行動できない
    if (!_hasAppeared)
        return false;

    //バトル中なら行動できない
    if (_isBattling)
        return false;

    //バトル直後は動けない(連戦防止)
    _afterBattleTimer.Update();
    return !_afterBattleTimer.IsCounting();
}


BattleParameter Character::GetAffectedParameter()
{
    BattleParameter param = _battleParameter;
    for (size_t i=0; i<_effecters.size(); ++i)
    {
        if (_effecters[i]->IsEnable())
            _effecters[i]->AffectParameter(param);
    }

    return param;
}


void Character::Damaged(int damage)
{
    if (IsEnable())
    {
        _battleParameter._hp -= damage;
        _battleParameter._hp = min(_battleParameter._maxHP, max(_battleParameter._hp, 0));

        if (damage < 0)
        {
            _healAnimation._isPlaying = true;
            _healGraph.SetDisplayMode(true);
        }
    }

    //戦闘で倒されたかを確認して
    if (!IsAlive())
    {
        OnDie();

        //情報共有網からも除外
        _notifyer.NotifyRemoveTarget(*this);
        ResetTarget();
        OnWin();
    }
}


void Character::OnDie()
{    
    _afterBattleTimer.ResetCount();
    _actCounter.ResetCount();
    _hasAppeared = false;
}


void Character::OnWin()
{
    ResetTarget();
    _defeatSE.Play();
}


void Character::Appear()
{
    _hasAppeared = true;
    _appearSE.Play();
}


int Character::GetLevel() {
	return Level;
}

void Character::SetLevel(int lv) {
	Level = lv;
}



bool Character::CheckFileExistence(const std::string& str)
{
	int a = FileRead_open(str.c_str());
	if (a != 0) {
		return true;
	}

	return false;
}


