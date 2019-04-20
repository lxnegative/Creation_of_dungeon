#pragma once

#include <memory>
#include <utility>
#include <string>

#include "cd_666s/Resources//ResourceManager.h"
#include "Vector2D.h"

/*
�ȈՓI�ɗ��p�\�ȉ摜�N���X
*/

class Graph
{
    friend class GraphArray;

public:

    //���ׂď����l�ɐݒ肵�ĉ������Ȃ�
    Graph();

    //�����ŉ摜��ǂݍ���
    Graph(std::string fileName);

    //�����ŉ摜��ǂݍ��݁A�`��ʒu�ƈʒu�w���
    Graph(std::string fileName, Vector2D point, Vector2D scale = Vector2D(1, 1));

    ~Graph();

    //�t�@�C���������Ƃɉ摜��ǂݍ���
    void Load(std::string fileName);

    //�摜�̊g�嗦��ݒ�
    void SetGUIScale(double argWidth, double argHeight);
    void SetGUIScale(Vector2D scale) { _scale = scale; };

    //�`��̊�ʒu��ݒ�
    void SetPosition(Vector2D pos) { _anchor = pos; }

    void Draw(bool flip = false);
 
    //�摜�̊g�嗦���擾
    Vector2D GetGUIScale() const { return _scale; }

    //���摜�̃s�N�Z���T�C�Y���擾
    Vector2D GetGUISize() const { return _size; }

    //���摜�̃s�N�Z���T�C�Y * �g�嗦�@�ŎZ�o�����
    //�摜���g�債���ۂ̃T�C�Y���擾
    Vector2D GetSize() const
    {
        return Vector2D(_size._x * _scale._x, _size._y * _scale._y);
    }

private:

    //�摜�̊�{�T�C�Y
    Vector2D _size;

    //�g�嗦
    Vector2D _scale;

    //�I�t�Z�b�g�l(0, 0�ŃI�u�W�F�N�g�̍�����W)
    Vector2D offset;

    //�`��̊�ʒu
    Vector2D _anchor;

    std::shared_ptr<ImageResource> _imageResource;

    //�����x
    int _alpha;

    double _angle;
};
