#ifndef PoolingSystem_h
#define PoolingSystem_h

#include <vector>
#include <memory>
#include "Singleton.h"

/*
 �v�f������ʂ̊K�w�ŕێ���
 ��̗v�f�������I�ɐ؂�l�߂�z��g�����W���[��
 */

template <class T>
class PoolingSystem
{
public:
    PoolingSystem();
    ~PoolingSystem();
    
    //�Ǘ��z��ɒǉ�
    void Add(std::weak_ptr<T> addOrder);
    //�Ǘ��z�񂩂珜�O
    void Remove(std::weak_ptr<T> deleteOrder);

    void Clear();
    void Update();
    
    //�z��{��
    std::vector<std::weak_ptr<T>> _objects;
    
protected:
    
    bool _needRefresh;
    void Refresh();
    
    std::vector<std::weak_ptr<T>> _deleteOrders;
    std::vector<std::weak_ptr<T>> _addOrders;
};


#include "PoolingSystem_Private.h"

#endif /* PoolingSystem_h */
