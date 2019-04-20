#ifndef EnemysItem_h
#define EnemysItem_h

#include "TiledObject.h"
#include "../Sound/Sound.h"

class Enemy;

template <class T>
class EnemysItem : public TiledObject
{
public:
    EnemysItem(std::unique_ptr<T> contents, TiledVector tilePos);
    ~EnemysItem();
    
    bool IsOverwritable(TiledObject* overwriter) override;
    void GiveItem(Enemy& character);
    
    void Update() override;
    void Draw() override;
    
    bool IsEnable() const override;
    
    bool IsEmpty() const { return (_contents == nullptr); }

	virtual void Init() override;

private:
    
    Sprite _contentsGraph;
    std::shared_ptr<ImageResource> _openedImage;

    std::unique_ptr<T> _contents;

    Sound _openSound;

    int _countAfterEmpty;
};

#include "EnemysItem_Private.h"

#endif /* EnemysItem_h */
