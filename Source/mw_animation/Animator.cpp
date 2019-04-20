#include "Animator.h"


Animator::Animator()
{
}


Animator::~Animator()
{
}

//�Đ�����A�j���\�V�����؂�ւ�
void Animator::Switch(std::string animationName)
{
    auto beforeAnimation = _animation;
    if (beforeAnimation != nullptr)
        beforeAnimation->GetGraphPtr()->SetDisplayMode(false);

    _animation = _animations[animationName];
    if (_animation != nullptr)
        _animation->GetGraphPtr()->SetDisplayMode(true);

    _currentName = animationName;
}

//�A�j����؂�ւ��A���O�̃A�j�����ŏ��̏�Ԃ�
void Animator::SwitchWithReset(std::string animationName)
{
    auto temp = _animation;

    //���d�Đ��h�~
    if (temp == _animations[animationName])
        return;
    else
    {
        //���O�̃A�j���[�V�������X�^�[�g�ʒu��
        temp->SetCurrentTime(0);
    }

    Switch(animationName);
}

void Animator::AddAnimation(std::string name, std::shared_ptr<GraphArray> animation)
{
    bool isFirstAdd = (_animations.size() == 0);
    _animations.insert(std::make_pair(name, animation));

    if (animation != nullptr)
        animation->GetGraphPtr()->SetDisplayMode(false);

    if (isFirstAdd)
        Switch(name);
}

void Animator::RemoveAnimation(std::string animationName)
{
    auto removeAnimation = _animations[animationName];

    //�w��̃A�j����������ΏI��
    if (removeAnimation == nullptr)
        return;

    //���ݍĐ����̃A�j���[�V�����Ɠ������ƁA�I��
    if (_animations[animationName] == _animation)
        return;

	_animations.erase(animationName);
}

void Animator::Update()
{
    if (_animation == nullptr)
        return;

    _animation->Update();
}

/*
void Animator::Draw(const Vector2D vec, bool flip)
{
    _animation->_graphPtr->Draw(vec, flip);
}
*/