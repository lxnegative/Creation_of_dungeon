#include "BreadcrumbChaser.h"
#include "Breadcrumb.h"
#include "../Character.h"
#include "../TileField.h"
#include <algorithm>

BreadcrumbChaser::BreadcrumbChaser(std::vector<TiledVector> &pathRef
                                   , Character &owner
                                   , int searchRange
                                   , bool diagonal)
: PathFindingAIBase(owner, pathRef)
, _searchRange(searchRange)
, _diagonal(diagonal)
, _currentID(0)
{
}


BreadcrumbChaser::~BreadcrumbChaser()
{
}


void BreadcrumbChaser::Reset()
{
    _currentID = 0;
}


void BreadcrumbChaser::Update()
{
    SearchBreadcrumbs();
}


void BreadcrumbChaser::SearchBreadcrumbs()
{
    TiledVector ownerPos = _owner.GetTilePos();
    std::vector<const Breadcrumb *> breadcrumbs;
    
    //���͂�8�}�X��Breadcrumb�����邩���ׂ�
    if (_diagonal)
    {
        for (size_t i=0; i<8; ++i)
        {
            TiledVector pos = ownerPos + TiledVector::eightDirecton[i];
            auto breadcrumb = FIELD->GetBreadcrumb(pos);
            
            if ( breadcrumb != nullptr )
                breadcrumbs.push_back(breadcrumb);
        }
    }
    //���͂�4�}�X��Breadcrumb�����邩���ׂ�
    else
    {
        for (size_t i=0; i<4; ++i)
        {
            TiledVector pos = ownerPos + TiledVector::fourDirecton[i];
            auto breadcrumb = FIELD->GetBreadcrumb(pos);
            
            if ( breadcrumb != nullptr )
                breadcrumbs.push_back(breadcrumb);
        }
    }
    
    //�����Ȃ���ΏI��
    if (breadcrumbs.size() == 0)
        return;
    
    //�擾����breadcrumb��ID�Ń\�[�g
    sort(breadcrumbs.begin(), breadcrumbs.end(),
         [](const Breadcrumb* lhs, const Breadcrumb* rhs)
         {
             return (rhs->GetID() < lhs->GetID());
         });
    
    auto breadcrumb = breadcrumbs[0];
    
    //�����鎩���̃m�[�h��ID�̕����傫����ΏI��
    if (breadcrumb->GetID() <= _currentID)
        return;
    
    //�ŏI�I�Ɍ��肵��breadcrumb�̈ʒu��ړI�n�Ƃ���
    _pathRef.push_back( breadcrumb->GetTilePos() );
    _currentID = breadcrumb->GetID();
}