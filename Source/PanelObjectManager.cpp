#include "PanelObjectManager.h"



PanelObjectManager::PanelObjectManager()
    : _objects(true)
{
}


PanelObjectManager::~PanelObjectManager()
{
    Clear();
}

void PanelObjectManager::Add(std::shared_ptr<PanelBase> obj)
{
    _addOrders.push_back(obj);
}

void PanelObjectManager::Remove(const PanelBase * obj)
{
    if (obj == nullptr)
        return;

    _deleteOrders.push_back(obj);
    _needRefresh = true;
}

void PanelObjectManager::Clear()
{
    _objects.clear();
    _objects.resize(0);

    _addOrders.clear();
    _addOrders.resize(0);

    _deleteOrders.clear();
    _deleteOrders.resize(0);
}

void PanelObjectManager::Refresh()
{
    //�ǉ����ꂽ�I�u�W�F�N�g�𐳋K�Ɏg�p����I�u�W�F�N�g�ɓo�^
    for (auto order : _addOrders)
    {
        _objects.push_back(order);
    }
    //order���N���A
    _addOrders.clear();
    _addOrders.resize(0);

    for (auto order : _deleteOrders)
    {
        for (size_t i = 0; i < _objects.size(); ++i)
        {
            if (_objects[i].get() == nullptr)
                continue;

            //�폜�˗��̂��̂��S���X�g���ɂ�������폜
            if (order != _objects[i].get())
                continue;

            _objects[i] = nullptr;
            _objects[i].reset();
            _needRefresh = true;
            break;
        }
    }

    _deleteOrders.clear();
    _deleteOrders.resize(0);

    //��̔z����l�߂�
    if (_needRefresh)
    {
        //swap�Z�@�p�̔z��
        std::vector<std::shared_ptr<PanelBase>> swaped;

        for (auto obj : _objects)
        {
            if (obj == nullptr)
                continue;

            //empty�ȊO��ǉ�����
            swaped.push_back(obj);
        }

        //�؂�l�߂����X�g�Ɠ���ւ���
        _objects = std::move(swaped);
        _needRefresh = false;
    }
}
