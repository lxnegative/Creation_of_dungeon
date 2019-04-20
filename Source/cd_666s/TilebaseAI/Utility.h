#pragma once
#include <vector>
#include "TiledVector.h"

void Bresenham(TiledVector target, TiledVector pos, std::vector<TiledVector> &path, bool diagonal = false, int length = 0)
{
    int deltaX = target._x - pos._x;
    int deltaY = target._y - pos._y;
    
    //�����Ɏw�肪�Ȃ���΍��������擾
    if (length == 0)
    {
        TiledVector delta(deltaX, deltaY);
        length = delta.GetBresenhamLength(diagonal);
    }
    
    //
    int count = 0;
    //
    int minX = (pos._x < target._x) ? pos._x : target._x;
    int minY = (pos._y < target._y) ? pos._y : target._y;
    int maxX = (pos._x < target._x) ? target._x : pos._x;
    int maxY = (pos._y < target._y) ? target._y : pos._y;
    
    //������0���Z�ɂȂ�̂Ŏ蓮�Œl��Ԃ�
    int countX = pos._x;
    int countY = pos._y;
    if (deltaX == 0)
    {
        for (; minY <= maxY; )
        {
            (deltaY < 0) ? countY-- : countY++;
            path.push_back(TiledVector(countX, countY));
            
            count++;
            if (length == count)
                return;
        }
        return;
    }
    
    //
    double d = 0;
    double delta = abs(deltaY / deltaX);
    
    //�ЂƂׂɂ������Ƃ�
    for ( ; minX <= maxX; )
    {
        //�X����������
        d += delta;
        
        bool flag = false;
        
        //�}�X�̉��������z������
        if (0.5 <= d)
        {
            if (!diagonal)
                flag = true;
            
            //�����1���炷
            (deltaY < 0) ? countY-- : countY++;
            
            //�}�X�����̊�l��1��ɖ߂�
            d = d - 1.0;
        }
        
        if (!flag)
            (deltaX < 0) ? countX-- : countX++;
        
        path.push_back(TiledVector(countX, countY));
        
        count++;
        if (length <= count)
            return;
    }
}