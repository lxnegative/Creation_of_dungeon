#pragma once
#include <memory>
#include "TreasureContents.h"
#include "../Resources/ImageResource.h"

class ConsumableItem : public TreasureContents
{
public:
    ConsumableItem(std::string ItemName,int RecoveryLife,int goldValue, std::string imageFileName);
    ~ConsumableItem();

	int GetRecoveryLife();

private:

	int recoveryLife;

};

