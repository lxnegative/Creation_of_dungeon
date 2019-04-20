#include "GameResult.h"
#include "cd_666s/Utility/CSVReader.h"
#include "cd_666s/Resources/AllResourceManager.h"
#include "cd_666s/TilebaseAI//Enemy.h"
#include "cd_666s/DebugDraw.h"
#include "Information_SaveData.h"

GameResult::GameResult()
{

}


GameResult::~GameResult()
{
}

void GameResult::Init(std::string areaNum, std::string stagenum, bool isBoss, int PartNum, std::string pointNumber)
{

	_areaNumber = areaNum;

	this->isBoss = isBoss;
	_stageNum = stagenum;

	_pointNum = pointNumber;

	_partNum = PartNum;

	CSVReader reader;

	std::string fileName = Main::RESOURCEFILENAME + "csv/StageData/MapData/Area" + _areaNumber + "/StageResult_DefeateBonus.csv";

	std::vector<std::string> dataArray;
	reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, dataArray, 1);

	_magnification = 0;

	for (int i = 0; i < dataArray.size(); i += 2) {
		if (dataArray[i] == _stageNum) {
			_magnification = std::stoi(dataArray[i + 1]);
		}
	}

	_StageClearGr = std::make_shared<Sprite>();
	if (PartNum == 0) {
		_StageClearGr->Load(Main::RESOURCEFILENAME + "resource/graph/ui/result/ResultOverUI_Part.png");
	}
	else {
		_StageClearGr->Load(Main::RESOURCEFILENAME + "resource/graph/ui/result/ResultOverUI_Stage.png");
	}
	_StageClearGr->SetRenderType(Texture2D::RenderType::UI);
	_StageClearGr->SetPriority(static_cast<int>(Sprite::Priority::UI));
	_StageClearGr->SetPosition(Vector2D(440, 60));
	_StageClearGr->SetDisplayMode(false);

	_BossClearGr = std::make_shared<Sprite>();
	_BossClearGr->Load(Main::RESOURCEFILENAME + "resource/graph/ui/result/ResultOverUI_Boss.png");
	_BossClearGr->SetRenderType(Texture2D::RenderType::UI);
	_BossClearGr->SetPriority(static_cast<int>(Sprite::Priority::UI));
	_BossClearGr->SetPosition(Vector2D(440, 60));
	_BossClearGr->SetDisplayMode(false);

	_UnderLineGr = std::make_shared<Sprite>();
	_UnderLineGr->Load(Main::RESOURCEFILENAME + "resource/graph/ui/result/ResultUnderUI.png");
	_UnderLineGr->SetRenderType(Texture2D::RenderType::UI);
	_UnderLineGr->SetPriority(static_cast<int>(Sprite::Priority::UI));
	_UnderLineGr->SetPosition(Vector2D(440, 600));
	_UnderLineGr->SetDisplayMode(false);

	IsStop = false;

	resultItem.clear();

	displayObject.clear();

	if (!Information_SaveData::GetInstance()->getIsClear(std::stoi(_areaNumber), std::stoi(_pointNum)) && _partNum == 1) {
		fileName = Main::RESOURCEFILENAME + "csv/StageData/MapData/Area" + _areaNumber + "/StageResult_FirstClearBonus.csv";

		std::vector<std::string> dataArray;
		reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, dataArray, 1);

		_clearBonus = 0;

		for (int i = 0; i < dataArray.size(); i += 2) {
			if (dataArray[i] == _stageNum) {
				_clearBonus = std::stoi(dataArray[i + 1]);
			}
		}
	}
	

}

void GameResult::Calculation()
{
	resultItem.clear();
	displayObject.clear();

	_defeatedNum = Enemy::GetDefeatedNum_Static();

	std::vector<RobTresureItem> temp = Enemy::GetRobTresureItem();

	for (int i = 0; i < temp.size(); i++) {
		if (resultItem.empty()) {
			ResultItem rt;
			rt.robTresureItem = temp[i];
			rt.num = 1;
			rt._graph = std::make_shared<Sprite>();
			rt._graph->Load(rt.robTresureItem.ItemGraphPath);
			rt._graph->SetDisplayMode(false);
			rt._graph->SetRenderType(Texture2D::RenderType::UI);
			rt._graph->SetPriority(static_cast<int>(Sprite::Priority::UI) + 2001);
			rt._graph->SetScale(Vector2D(64.0 / rt._graph->GetSize()._x, 64.0 / rt._graph->GetSize()._y));
			resultItem.push_back(rt);
		}
		else {
			bool look = false;
			for (int j = 0; j < resultItem.size(); j++) {
				if (resultItem[j].robTresureItem.ItemName == temp[i].ItemName) {
					look = true;
					resultItem[j].num++;
				}
			}
			if (!look) {
				ResultItem rt;
				rt.robTresureItem = temp[i];
				rt.num = 1;
				rt._graph = std::make_shared<Sprite>();
				rt._graph->Load(rt.robTresureItem.ItemGraphPath);
				rt._graph->SetDisplayMode(false);
				rt._graph->SetRenderType(Texture2D::RenderType::UI);
				rt._graph->SetPriority(static_cast<int>(Sprite::Priority::UI) + 2001);
				rt._graph->SetScale(Vector2D(64.0 / rt._graph->GetSize()._x, 64.0 / rt._graph->GetSize()._y));
				resultItem.push_back(rt);
			}
		}
	}

	if (isBoss) {
		displayObject.push_back(_BossClearGr);
	}
	else {
		displayObject.push_back(_StageClearGr);
	}


	for (int i = 0; i < resultItem.size(); i++) {

		resultItem[i]._graph->SetPosition(Vector2D(480, 170 + 70 * i));
		displayObject.push_back(resultItem[i]._graph);


		//Debug::DrawString(Vector2D(540, 170 + 36 * i + 20), "Å~" + std::to_string(resultItem[i].num), Color4(1, 1, 1, 1), 20);

	}


	displayObject.push_back(_UnderLineGr);


}

void GameResult::Update()
{



	_count++;

}

void GameResult::Draw()
{

	money = 0;

	for (int i = 0; i < displayObject.size(); i++) {

		if (i < 1 + resultItem.size()) {
			if (_count > (i * 50)) {
				displayObject[i].lock()->SetDisplayMode(true);
				if (i >= 1) {
					Debug::DrawString_TopPriority(Vector2D(550, 170 + 68 * (i - 1) + 20), "Å~" + std::to_string(resultItem[i - 1].num), Color4(1, 1, 1, 1), 30);

					Debug::DrawString_TopPriority(Vector2D(800, 170 + 68 * (i - 1) + 20), std::to_string(resultItem[i - 1].robTresureItem.goldValue * resultItem[i - 1].num) + "Çl", Color4(1, 1, 1, 1), 30);
					money += resultItem[i - 1].robTresureItem.goldValue * resultItem[i - 1].num;
				}
			}
		}
		else if(Information_SaveData::GetInstance()->getIsClear(std::stoi(_areaNumber), std::stoi(_pointNum)) || (_partNum != 1 && !isBoss)){
			if (_count > ((i) * 50)) {
				Debug::DrawString_TopPriority(Vector2D(440, 170 + 68 * (i - 1) + 20), "ì|ÇµÇΩñ`åØé“ÇÃêîÅF" + std::to_string(_defeatedNum), Color4(1, 1, 1, 1), 30);
				Debug::DrawString_TopPriority(Vector2D(800, 170 + 68 * (i - 1) + 20), std::to_string(_defeatedNum * _magnification) + "Çl", Color4(1, 1, 1, 1), 30);
				money += _defeatedNum * _magnification;
			}
			if (_count > ((i + 1) * 50)) {
				displayObject[i].lock()->SetDisplayMode(true);
			}
			if (_count > ((i + 2) * 50)) {
				Debug::DrawString_TopPriority(Vector2D(540, 650), "åvÅ@Å@" + std::to_string(money) + "Çl", Color4(1, 1, 1, 1), 30);
				IsStop = true;
			}
			break;
		}
		else if (!Information_SaveData::GetInstance()->getIsClear(std::stoi(_areaNumber), std::stoi(_pointNum)) && (_partNum == 1 || isBoss)) {
			if(_count > ((i) * 50)) {
				Debug::DrawString_TopPriority(Vector2D(440, 170 + 68 * (i - 1) + 20), "ì|ÇµÇΩñ`åØé“ÇÃêîÅF" + std::to_string(_defeatedNum), Color4(1, 1, 1, 1), 30);
				Debug::DrawString_TopPriority(Vector2D(800, 170 + 68 * (i - 1) + 20), std::to_string(_defeatedNum * _magnification) + "Çl", Color4(1, 1, 1, 1), 30);
				money += _defeatedNum * _magnification;
			}
			if (_count > ((i + 1) * 50)) {
				Debug::DrawString_TopPriority(Vector2D(440, 170 + 68 * (i) + 20), "èââÒÉNÉäÉAÉ{Å[ÉiÉXÅF", Color4(1, 1, 0, 1), 30);
				Debug::DrawString_TopPriority(Vector2D(800, 170 + 68 * (i) + 20), std::to_string(_clearBonus) + "Çl", Color4(1, 1, 0, 1), 30);
				money += _clearBonus;
			}
			if (_count > ((i + 2) * 50)) {
				displayObject[i].lock()->SetDisplayMode(true);
			}
			if (_count > ((i + 3) * 50)) {
				Debug::DrawString_TopPriority(Vector2D(540, 650), "åvÅ@Å@" + std::to_string(money) + "Çl", Color4(1, 1, 1, 1), 30);
				IsStop = true;
			}
			break;
		}



	}




}
