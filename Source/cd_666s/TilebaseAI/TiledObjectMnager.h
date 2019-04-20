#ifndef TiledObjectMnager_h
#define TiledObjectMnager_h

#include "../Utility/Singleton.h"
#include "../Utility/PoolingSystem.h"
#include "TiledObject.h"
#include <memory>

class TiledObjectManager : public Singleton<TiledObjectManager>
{
    friend class Dungeon;
    friend class MakeDungeon;
	friend class BossBattle;
public:
    TiledObjectManager();
    ~TiledObjectManager();
    
    void Add(std::shared_ptr<TiledObject> obj);
    //void Remove(std::weak_ptr<TiledObject> obj);
    void Remove(const TiledObject* obj);
    
    void Clear();
    void Refresh();

    //����̃I�u�W�F�N�g���擾
    template<class T>
    T* GetContainedObject(Vector2D vec)
    {
        for (size_t i = 0; i < _objects.size(); ++i)
        {
            if (_objects[i] == nullptr)
                continue;

            //�Y���^�C�v���Ȃ���ΏI��
            if (typeid(T) != typeid(*_objects[i]))
                continue;

            //���W���ӂ���łȂ���ΏI��
            if (!_objects[i]->Contain(vec))
                continue;

            //�ϊ��\�Ȃ�ǉ�
            std::shared_ptr<T> objPtr = std::dynamic_pointer_cast<T>(_objects[i]);
            if (objPtr.get() == nullptr)
                continue;

            return objPtr.get();
        }

        //std::weak_ptr<T> empty;
        return nullptr;
    }

    //����̃I�u�W�F�N�g���擾
    template<class T>
    std::vector<T*> GetContainedObjects(Vector2D vec)
    {
        std::vector<T*> results;
        results.reserve(16);

        for (size_t i = 0; i < _objects.size(); ++i)
        {
            if (_objects[i] == nullptr)
                continue;

            //�Y���^�C�v���Ȃ���ΏI��
            if (typeid(T) != typeid(*_objects[i]))
                continue;

            //�^�C�����ɑ��݂��Ȃ���ΏI��
            if (!_objects[i]->Contain(vec))
                continue;

            //�ϊ��\�Ȃ�ǉ�
            std::shared_ptr<T> objPtr = std::dynamic_pointer_cast<T>(_objects[i]);
            if (objPtr.get() == nullptr)
                continue;

            results.push_back(objPtr.get());
        }

        return results;
    }

    template<>
    std::vector<TiledObject*> GetContainedObjects(Vector2D vec)
    {
        std::vector<TiledObject*> results;
        results.reserve(16);

        for (size_t i = 0; i < _objects.size(); ++i)
        {
            if (_objects[i] == nullptr)
                continue;

            //�^�C�����ɑ��݂��Ȃ���ΏI��
            if (!_objects[i]->Contain(vec))
                continue;

            results.push_back(_objects[i].get());
        }

        return results;
    }

    //����̃I�u�W�F�N�g���擾
    template<class T>
    std::shared_ptr<T> GetSharedObject(TiledObject* rawPtr)
    {
        for (size_t i = 0; i < _objects.size(); ++i)
        {
            if (_objects[i] == nullptr)
                continue;

            //�Y���^�C�v���Ȃ���ΏI��
            if (typeid(T) != typeid(*_objects[i]))
                continue;

            if (rawPtr == _objects[i].get())
                return std::dynamic_pointer_cast<T>(_objects[i]);
        }

        return nullptr;
    }

    //����̌^�̃I�u�W�F�N�g���擾(�d���̂Ŏg�p�̍ۂɒ���)
    template<class T>
    std::shared_ptr<T> GetSharedObject()
    {
        for (size_t i = 0; i < _objects.size(); ++i)
        {
            if (_objects[i] == nullptr)
                continue;

            //�Y���^�C�v���Ȃ���ΏI��
            if (typeid(T) != typeid(*_objects[i]))
                continue;

            return std::dynamic_pointer_cast<T>(_objects[i]);
        }

        return nullptr;
    }

    //����̌^�̃I�u�W�F�N�g���擾(�d���̂Ŏg�p�̍ۂɒ���)
    template<class T> 
    std::vector<std::shared_ptr<T>> GetSharedObjects()
    {
        std::vector<std::shared_ptr<T>> results;
        results.reserve(16);

        for (size_t i = 0; i < _objects.size(); ++i)
        {
            if (_objects[i] == nullptr)
                continue;

            //�Y���^�C�v���Ȃ���ΏI��
            if (typeid(T) != typeid(*_objects[i]))
                continue;

            results.push_back(std::dynamic_pointer_cast<T>(_objects[i]));
        }

        return results;
    }

private:
    
    std::vector<std::shared_ptr<TiledObject>> _objects;
    
private:

    bool _needRefresh;
    std::vector<const TiledObject*> _deleteOrders;
    std::vector<std::shared_ptr<TiledObject>> _addOrders;
};

#define OBJECT_MGR TiledObjectManager::GetInstance()

#endif /* TiledObjectMnager_h */
