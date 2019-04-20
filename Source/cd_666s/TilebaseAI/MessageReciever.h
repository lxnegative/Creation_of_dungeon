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

	void DrawFalse();	//メッセージウインドウを描画しない
	void DrawTrue();	//メッセージウインドウを描画する

    MessageManager _processer;

private:

    
    //displayer
    Sprite _face, _messageUI;
    std::vector<std::vector<std::shared_ptr<ImageResource>>> _faces;
};

