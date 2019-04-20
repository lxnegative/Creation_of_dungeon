#include "cd_666s/Render/Sprite.h"
#include "RobTresureItem.h"

#pragma once
class GameResult
{
public:
	GameResult();
	~GameResult();

	void Init(std::string areaNum, std::string stagenum, bool isBoss, int PartNum, std::string pointNumber);

	void Calculation();

	void Update();
	void Draw();

	bool GetIsStop() { return IsStop; };
	int GetMoney() { return money; };

private:

	std::string _areaNumber;

	std::string _stageNum;

	std::string _pointNum;

	int _partNum;

	int _count;

	int _magnification;
	int _clearBonus;

	int _defeatedNum;

	bool isBoss;

	std::shared_ptr<Sprite> _StageClearGr;
	std::shared_ptr<Sprite> _BossClearGr;
	std::shared_ptr<Sprite> _UnderLineGr;

	bool IsStop;

	struct ResultItem {
		std::shared_ptr<Sprite> _graph;
		RobTresureItem robTresureItem;
		int num;

	};

	std::vector<ResultItem> resultItem;

	std::vector<std::weak_ptr<Sprite>> displayObject;

	int money;
	

};

