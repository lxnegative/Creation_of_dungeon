#pragma once
#include "../Utility/Singleton.h"
#include "AI/Breadcrumb.h"
#include "MapTile.h"
#include "FieldType.h"
#include <vector>
#include <memory>

class TileField : public Singleton <TileField>
{
    friend class Singleton<TileField>;
    typedef std::vector<std::vector<std::weak_ptr<MapTile>>> Field;
    typedef std::vector<std::vector<int>> StepTable;
    
public:
    TileField();
    ~TileField();
    
    void Clear();
    void Init(int width, int height);
    void Setup();
    void Draw();
    
    //�^�C���ɂ�����I�u�W�F�N�g���w��̃^�C���ɓo�^
    void RegistObject(TiledObject &obj, TiledVector pos);
    //�^�C���ɂ�����I�u�W�F�N�g�����O
    void RemoveObject(TiledObject &obj);
    
    //�^�C���ɂ�����I�u�W�F�N�g���w��̃^�C���ɓo�^
    void RegistBreadcrumb(Breadcrumb *crumb, TiledVector pos);
    //�^�C���ɂ�����I�u�W�F�N�g�����O
    void RemoveBreadcrumb(Breadcrumb *crumb);
    
    
    //���O���ēo�^���鏈������C�ɍs��
    void MoveObject(TiledObject &obj, TiledVector pos);
    
    template<class T = TiledObject>
    T* GetTiledObject(const TiledVector &pos)
    {
        //�͈͊O���Q�Ƃ��悤�Ƃ�����null��Ԃ�
        if (!IsInside(pos))
            return nullptr;

        return _field[pos._y][pos._x].lock()->GetTiledObject<T>();
    }

    template<class T = TiledObject>
    std::vector<T*> GetTiledObjects(const TiledVector &pos)
    {
        //�͈͊O���Q�Ƃ��悤�Ƃ�����null��Ԃ�
        if (!IsInside(pos))
        {
            std::vector<T*> empty;
            return std::move(empty);
        }

        return std::move(_field[pos._y][pos._x].lock()->GetTiledObjects<T>());
    }

    int GetRawNumber(const TiledVector &pos) const;
    void SetRawNumber(const TiledVector &pos, int number);
    FieldType GetFieldType(const TiledVector &pos) const;
    void SetFieldType(const TiledVector &pos, std::string data);

    FieldType GetDefaultFieldType() const { return _defaultFieldType; };
    void SetDefaultFieldType(std::string data);

    const Field &GetFieldRef() const { return _field; }
    
    //�t�B�[���h�͈͓̔��ɂ��邩�ǂ���
    const bool IsInside(const TiledVector &pos) const;
    
    //���̃}�X�Ɉړ��ł��邩�ǂ���
    const bool IsMovableThere(TiledVector pos, TiledObject& moveObj) const
    {
        return (     IsInside(pos)
                &&  _field[pos._y][pos._x].lock()->IsRegistable(moveObj));
    }
    
    const Breadcrumb* GetBreadcrumb(TiledVector pos) const
    {
        if (!IsInside(pos))
            return nullptr;
        
        return _field[pos._y][pos._x].lock()->GetBreadcrumb();
    }
    
    const TiledVector GetFieldSize() const { return _fieldSize; }
    
    //Astar��DFS�Ōv�Z�����ړ��\�}�X��\��
    std::vector<TiledVector> GetMovableCell(TiledVector pos, int movableRange);
    std::vector<TiledVector> GetParabolicMovableCell(TiledVector pos, int movableRange, TiledVector::Direction directon);
    
private:
    
    //DFS�Ŏ��L�������ړ��ł���}�X���v�Z
    void CalcMovableCell(const TiledVector &pos, StepTable& stepTable, int step, int degree, TiledVector::Direction);
    void CalcParabolicMovableCell(TiledVector pos, const TiledVector &basePos, StepTable& stepTable, int range, TiledVector::Direction dir, TiledVector::Direction baseDir);
    
    FieldType _defaultFieldType;
    TiledVector _fieldSize;
    Field _field;
    std::vector<std::shared_ptr<MapTile>> _gobjs;
};

#define FIELD TileField::GetInstance()