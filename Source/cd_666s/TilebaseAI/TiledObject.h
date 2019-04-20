#pragma once
#include "GraphicalObject.h"
#include "TiledVector.h"
#include "BattleParameter.h"
#include "../Utility/PoolingSystem.h"
#include "BattleParameter.h"

class MapTile;
class Character;

class TiledObject : public GraphicalObject
{
public:
    TiledObject(TiledVector pos);
    virtual ~TiledObject();
    
    enum class Type
    {
        DEFAULT,
        MONSTER,
        ENEMY,
        ITEM,
        BLOCK,
        START,
        GOAL,
        MAGIC_SQUARE,
        BATTLE, 
        TRAP,
		ATTACK,
		BOSS_SCENE
    };
    
    //���[�v�����̑O��1�x�����Ă΂�鏈��
    virtual void Init();

    //�\������ł���I�u�W�F�N�g����Ăяo�����֐�
    virtual void Interact(Character& character);
    
    //���̃I�u�W�F�N�g�̏�ɕʂ̃I�u�W�F�N�g�����邩�ǂ���
    //overwriter : ��ɏ�낤�Ƃ���I�u�W�F�N�g
    virtual bool IsOverwritable(TiledObject* overwriter) = 0;
    
    virtual void OnClicked();
    
    //�A�C�e���擾�ł̏����⏢�����ꂽ���ǂ����ȂǁA
    //�I�u�W�F�N�g�͐������Ă�����ɗp���邩�ǂ����𔻒f����̂ɗp����.
    //�ڍׂȏ����͔h����ɂ���ĈقȂ�.
    virtual bool IsEnable() const { return true; }
    
    //�I�u�W�F�N�g���|���ꂽ���Ƃ������Ƃ��Ɏg�p
    virtual int GetDefeatedNum() const { return 0; }

    //�I�u�W�F�N�g���ړI�n�ɂ��ǂ蒅�������ǂ�����Ԃ�
    virtual bool HasArrived() const { return false; }

	virtual BattleParameter GetRawParameter() const { return BattleParameter(); }

	virtual std::string GetName() const { return ""; }

    void SetTile(std::weak_ptr<MapTile> tile) { _tile = tile; }
    void ExitTile() { _tile.reset(); }
    
    Type GetType() const { return _type; }
    std::weak_ptr<MapTile> GetTile() const { return _tile; }
    TiledVector GetTilePos() const;

protected:
    
    void CheckClicked();
    
    std::string _name;
    Type _type;
    
private:
    std::weak_ptr<MapTile> _tile;
};

