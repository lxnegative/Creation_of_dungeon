#pragma once
#include "MessageManager.h"
#include "../Render/Sprite.h"

class MessageReciever
{
public:
    MessageReciever();
    ~MessageReciever();

    void Recieve(const TalkDatabase& senderData);

    void Init();
    void Update();
    void Draw();

	void DrawFalse();	//���b�Z�[�W�E�C���h�E��`�悵�Ȃ�
	void DrawTrue();	//���b�Z�[�W�E�C���h�E��`�悷��

    MessageManager _processer;

private:

    
    //displayer
    Sprite _face, _messageUI;
    std::vector<std::vector<std::shared_ptr<ImageResource>>> _faces;
};

