#pragma once
#include <type_traits>
#include "Enemy.h"
#include "TileField.h"
#include "TiledObjectMnager.h"

template<typename T>
using SearchExpression = std::function<bool(const T* target, const TiledVector& pos)>;

template<typename T>
using SearchExpressions = std::vector<std::function<bool(const T* target, const TiledVector& pos)>>;


template<typename T>
static T* SearchTargetInSight(
    const TiledVector& searcherPos,
    const std::vector<TiledVector>& sight,
    SearchExpressions<T>& expressions,
    bool findNearest)
{
    std::vector<T*> objects;
    for (size_t i = 0; i< sight.size(); ++i)
    {
        auto foundObjects = FIELD->GetTiledObjects<T>(sight[i]);
        for (auto obj : foundObjects)
        {
            if (obj == nullptr)
                continue;

            //�Ώۂ�����(���݂��Ȃ��Ȃ�)�Ȃ疳��
            if (!obj->IsEnable())
                continue;

            //�ǉ��̏������ł͂�����邩���ׂ�
            bool result = true;
            for (auto& expression : expressions)
            {
                result &= expression(obj, sight[i]);
                if (!result)
                    break;
            }
            //�ǂꂩ�̏�����ʂ�Ȃ�������I��
            if (!result)
                continue;

            //��ԋ߂����̂������Ȃ��Ȃ炱���ŏI��
            if (!findNearest)
                return obj;
            else
                objects.push_back(obj);
        }
    }

    //����������Ȃ���ΏI��
    if (objects.size() == 0)
        return nullptr;

    return GetPrimaryTarget<T>(objects, searcherPos);
}


template<typename T>
static T* SearchTargetInSight(
    const TiledVector& searcherPos, 
    std::vector<TiledVector>& sight, 
    bool findNearest)
{
    std::vector<T*> objects;
    for (size_t i = 0; i< sight.size(); ++i)
    {
        auto foundObjects = FIELD->GetTiledObjects<T>(sight[i]);
        for (auto obj : foundObjects)
        {
            if (obj == nullptr)
                continue;

            //�Ώۂ�����(���݂��Ȃ��Ȃ�)�Ȃ疳��
            if (!obj->IsEnable())
                continue;

            //��ԋ߂����̂������Ȃ��Ȃ炱���ŏI��
            if (!findNearest)
                return obj;
            else
                objects.push_back(obj);
        }
    }

    //����������Ȃ���ΏI��
    if (objects.size() == 0)
        return nullptr;

    return GetPrimaryTarget<T>(objects, searcherPos);

}


template<typename T>
static T* SearchTargetInField(
    const TiledVector& searcherPos,
    SearchExpressions<T>& expressions,
    bool findNearest)
{
    std::vector<T*> objects;
    auto foundObjects = OBJECT_MGR->GetSharedObjects<T>();
    for (size_t i = 0; i < foundObjects.size(); ++i)
    {
        auto& obj = foundObjects[i];
        if (obj == nullptr)
            continue;

        //�Ώۂ�����(���݂��Ȃ��Ȃ�)�Ȃ疳��
        if (!obj->IsEnable())
            continue;

        //�ǉ��̏������ł͂�����邩���ׂ�
        bool result = true;
        for (auto& expression : expressions)
        {
            result &= expression(obj.get(), searcherPos);
            if (!result)
                break;
        }
        //�ǂꂩ�̏�����ʂ�Ȃ�������I��
        if (!result)
            continue;

        objects.push_back(obj.get());
    }

    //����������Ȃ���ΏI��
    if (objects.size() == 0)
        return nullptr;

    return GetPrimaryTarget<T>(objects, searcherPos);
}


template<typename T>
static T* SearchTargetInField(
    const TiledVector& searcherPos, 
    bool findNearest)
{
    std::vector<T*> objects;
    auto foundObjects = OBJECT_MGR->GetSharedObjects<T>();
    for (size_t i = 0; i< foundObjects.size(); ++i)
    {
        auto& obj = foundObjects[i];
        if (obj == nullptr)
            continue;

        //�Ώۂ�����(���݂��Ȃ��Ȃ�)�Ȃ疳��
        if (!obj->IsEnable())
            continue;

        //��ԋ߂����̂������Ȃ��Ȃ炱���ŏI��
        if (!findNearest)
            return obj.get();
        else
            objects.push_back(obj.get());
    }

    //����������Ȃ���ΏI��
    if (objects.size() == 0)
        return nullptr;

    return GetPrimaryTarget<T>(objects, searcherPos);
}

template <typename T>
static T* GetPrimaryTarget(const std::vector<T*>& targets, const TiledVector& pos)
{
    T* result = nullptr;
    int minOffset = 999;
    for (auto target : targets)
    {
        if (target == nullptr)
            continue;

        //�����܂ŗ�����߂��ق���D�悷��悤�ɂ���
        int offset = (target->GetTilePos() - pos).GetBresenhamLength(false);
        if (minOffset <= offset)
            continue;

        minOffset = offset;
        result = target;
    }

    return result;
}


template <typename T>
static T* GetPrimaryTarget(const std::vector<std::shared_ptr<T>>& targets, const TiledVector& pos)
{
    T* result = nullptr;
    int minOffset = 999;
    for (size_t i = 0; i< targets.size(); ++i)
    {
        auto& target = objects[i];
        if (target == nullptr)
            continue;

        //�����܂ŗ�����߂��ق���D�悷��悤�ɂ���
        int offset = (target->GetTilePos() - pos).GetBresenhamLength(false);
        if (minOffset <= offset)
            continue;

        minOffset = offset;
        result = target;
    }

    return result;
}