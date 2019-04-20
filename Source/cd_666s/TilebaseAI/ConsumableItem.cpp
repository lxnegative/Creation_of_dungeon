#include "ConsumableItem.h"


ConsumableItem::ConsumableItem(std::string ItemName, int RecoveryLife, int goldValue, std::string imageFileName)
    : TreasureContents(ItemName, goldValue, imageFileName)
{
	recoveryLife = RecoveryLife;
}


ConsumableItem::~ConsumableItem()
{
}


int ConsumableItem::GetRecoveryLife() {
	return recoveryLife;
}

