#include "ShootDamageObject.h"
#include "Character.h"
#include "ShottenObject.h"

#include "TiledObjectMnager.h"
#include "TileField.h"
#include "Enemy.h"


ShootDamageObject::ShootDamageObject(double power, int cost, double startRatio, double speed, int range, Character& chara, bool isPhysical)
    : CharactersSkill(cost, startRatio)
    , _character(chara)
    , _power(power)
    , _range(range)
    , _speed(speed)
    , _isPhysical(isPhysical)
{
    _image = IMAGE_RESOURCE_TABLE->Create(Main::RESOURCEFILENAME + "resource/graph/effect/fire_ball.png");
}


ShootDamageObject::~ShootDamageObject()
{
}


bool ShootDamageObject::ShouldActivate()
{
	if (_mp < _mpCost)
		return false;

    auto tilePos = _character.GetTilePos();
    auto type = _character.GetType();
    auto opponentType = (type == TiledObject::Type::ENEMY) ? TiledObject::Type::MONSTER : TiledObject::Type::ENEMY;
    auto direction = _character.GetDirection();

    //方向に対応したベクトルを算出して
    TiledVector directionVec(0, 0);
	TiledVector relativeLeftDirectionVec(0, 0);
	TiledVector relativeRightDirectionVec(0, 0);

    switch (direction)
    {
    case TiledVector::Direction::FORWARD:
        directionVec = TiledVector::up;
		relativeLeftDirectionVec = TiledVector::right;
		relativeRightDirectionVec = TiledVector::left;
        break;

    case TiledVector::Direction::BACK:
        directionVec = TiledVector::down;
		relativeLeftDirectionVec = TiledVector::left;
		relativeRightDirectionVec = TiledVector::right;
        break;

    case TiledVector::Direction::LEFT:
        directionVec = TiledVector::left;
		relativeLeftDirectionVec = TiledVector::up;
		relativeRightDirectionVec = TiledVector::down;
        break;

    case TiledVector::Direction::RIGHT:
        directionVec = TiledVector::right;
		relativeLeftDirectionVec = TiledVector::down;
		relativeRightDirectionVec = TiledVector::up;
        break;
    }

    //射程距離だけ調べる
    std::vector<TiledVector> searchTilePoses(_range + 1);
    for (int i = 1; i <= _range; ++i)
    {
        searchTilePoses[i] = tilePos + directionVec * i;
        auto objects = FIELD->GetTiledObjects(searchTilePoses[i]);
        for (auto obj : objects)
        {
            if (!obj->IsEnable())
                continue;

			if (obj->GetType() == TiledObject::Type::BLOCK) {
				break;
			}

            if (obj->GetType() == opponentType
                || obj->GetType() == TiledObject::Type::BATTLE)
            {
                return true;
            }
        }
    }

	

	if (_character.GetEnemyAIType() == Enemy::EnemyAIType::SkillMain) {
		//キャラクターから見て左右方向だけは視界を用いて調べる
		std::vector<TiledVector> searchTilePoses(2 + 1);
		for (int i = 1; i <= 2; ++i)
		{
			searchTilePoses[i] = tilePos + relativeLeftDirectionVec * i;
			auto objects = FIELD->GetTiledObjects(searchTilePoses[i]);
			for (auto obj : objects)
			{
				if (!obj->IsEnable())
					continue;

				if (obj->GetType() == TiledObject::Type::BLOCK) {
					break;
				}

				if (obj->GetType() == opponentType
					|| obj->GetType() == TiledObject::Type::BATTLE)
				{
					TiledVector::Direction updatedirection = TiledVector::Direction::LEFT;
					if (relativeLeftDirectionVec == TiledVector::left) {
						updatedirection = TiledVector::Direction::LEFT;
					}
					else if (relativeLeftDirectionVec == TiledVector::right) {
						updatedirection = TiledVector::Direction::RIGHT;
					}
					else if (relativeLeftDirectionVec == TiledVector::up) {
						updatedirection = TiledVector::Direction::FORWARD;
					}
					else {
						updatedirection = TiledVector::Direction::BACK;
					}
					_character.SetDirection(updatedirection);
					return true;
				}
			}
		}
		for (int i = 1; i <= 2; ++i)
		{
			searchTilePoses[i] = tilePos + relativeRightDirectionVec * i;
			auto objects = FIELD->GetTiledObjects(searchTilePoses[i]);
			for (auto obj : objects)
			{
				if (!obj->IsEnable())
					continue;

				if (obj->GetType() == TiledObject::Type::BLOCK) {
					break;
				}

				if (obj->GetType() == opponentType
					|| obj->GetType() == TiledObject::Type::BATTLE)
				{
					TiledVector::Direction updatedirection = TiledVector::Direction::LEFT;
					if (relativeRightDirectionVec == TiledVector::left) {
						updatedirection = TiledVector::Direction::LEFT;
					}
					else if (relativeRightDirectionVec == TiledVector::right) {
						updatedirection = TiledVector::Direction::RIGHT;
					}
					else if (relativeRightDirectionVec == TiledVector::up) {
						updatedirection = TiledVector::Direction::FORWARD;
					}
					else {
						updatedirection = TiledVector::Direction::BACK;
					}
					_character.SetDirection(updatedirection);
					return true;
				}
			}
		}
	}

    return false;
}


void ShootDamageObject::Activate()
{
    if (_mp < _mpCost)
        return;

    auto tilePos = _character.GetTilePos();
    auto type = _character.GetType();
    auto direction = _character.GetDirection();
    auto param = _character.GetAffectedParameter();

    if (_isPhysical && type == TiledObject::Type::ENEMY)
    {
        switch (direction)
        {
        case TiledVector::Direction::FORWARD:
            _image = IMAGE_RESOURCE_TABLE->Create(Main::RESOURCEFILENAME + "resource/graph/trap/arrow_down.png");
            break;

        case TiledVector::Direction::BACK:
            _image = IMAGE_RESOURCE_TABLE->Create(Main::RESOURCEFILENAME + "resource/graph/trap/arrow_up.png");
            break;

        case TiledVector::Direction::LEFT:
            _image = IMAGE_RESOURCE_TABLE->Create(Main::RESOURCEFILENAME + "resource/graph/trap/arrow_left.png");
            break;

        case TiledVector::Direction::RIGHT:
            _image = IMAGE_RESOURCE_TABLE->Create(Main::RESOURCEFILENAME + "resource/graph/trap/arrow_right.png");
            break;
        }
    }

    int attack = (_isPhysical) ? param._attack : param._magicAttack;
    OBJECT_MGR->Add(std::make_shared<ShottenObject>(_power, attack, _range, _speed, tilePos, direction, type, _image, _isPhysical));

    _mp = 0;
}