#include "BossBattle.h"
#include "cd_666s\TilebaseAI\TiledObjectMnager.h"
#include "cd_666s/Resources/AllResourceManager.h"
#include "cd_666s/Utility/CSVReader.h"

#include "BossBattleObject.h"
#include "cd_666s\TilebaseAI\ColleagueNotifyer.h"
#include <array>

BossBattle::BossBattle()
	: _battleTime(32)
	, _readyTime(100)
	, _diedTime(100)
	, _waitTime(100)
	, _messageUI(Main::RESOURCEFILENAME + "resource/graph/ui/BossMessageWindow.png", Vector2D(20, 520))
	, _bossFace(Main::RESOURCEFILENAME + "resource/graph/devilGirlUsual.png", Vector2D(40, 545))
	, _enemyFace(Main::RESOURCEFILENAME + "resource/graph/devilGirlUsual.png", Vector2D(750, 545))
{
	_sequenceTimer = 0;
	now_situation = BattleSequence::Ready;
	_wasBossKilled = false;


	_bossFace.SetScale(Vector2D(2, 2));
	_bossFace.SetPriority(static_cast<int>(Sprite::Priority::UI) + 3);
	_enemyFace.SetScale(Vector2D(2, 2));
	_enemyFace.SetPriority(static_cast<int>(Sprite::Priority::UI) + 3);
	_messageUI.SetPriority(static_cast<int>(Sprite::Priority::UI) + 2);

	_bossFace.SetDisplayMode(false);
	_enemyFace.SetDisplayMode(false);
	_messageUI.SetDisplayMode(false);

	_enemySkills.AddAnimation("stop", std::make_shared<GraphArray>(Main::RESOURCEFILENAME + "resource/graph/item/blade.png", 32, 24));
	_enemySkills.AddAnimation("skill", std::make_shared<GraphArray>(Main::RESOURCEFILENAME + "resource/graph/item/blade.png", 32, 24));
	_enemySkills.GetCurrentGraph()->SetDisplayMode(false);
}


BossBattle::~BossBattle()
{
}

void BossBattle::Init(std::string areaName, std::string stageNum, std::vector<std::shared_ptr<TiledObject>>& objects, ColleagueNotifyer &notifyer)
{
	std::string fileName = Main::RESOURCEFILENAME + "csv/boss/boss";
	fileName += areaName + "-" + stageNum + ".csv";

	std::vector<std::string> bossArray;
	CSVReader reader;
	reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, bossArray, 1);

	std::array<int, 6> param_i;

	for (size_t i = 0; i < param_i.size();i++) {
		param_i[i] = std::stoi(bossArray[i]);
	}

	BattleParameter param = { param_i[0], param_i[1], param_i[2], param_i[3], param_i[4], param_i[5] };
	auto _pos = Vector2D(std::stod(bossArray[6]), std::stod(bossArray[7]));
	std::string name = bossArray[8];

	_boss = std::move(std::make_shared<BossBattleObject>(_pos, param, notifyer, name, true));

	objects.push_back(_boss);

	_battleTime = 32 + (600 - _pos._x) / 6;

	fileName = Main::RESOURCEFILENAME + "csv/boss/boss_face.csv";
	reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, bossArray, 1);

	for (int i = 0; i < bossArray.size(); i += 2) {

		if (bossArray[i] == name) {
			_bossFace.Load(Main::RESOURCEFILENAME + "resource/graph/" + bossArray[i + 1] + ".png");
		}

	}

}

void BossBattle::Update()
{
	if(_intruders.size() < 1){
		_boss->SetInRoom(false);
		_boss->SwitchAnime(false);

		_bossFace.SetDisplayMode(false);
		_enemyFace.SetDisplayMode(false);
		_messageUI.SetDisplayMode(false);

		return ;
	}

	_messageUI.SetDisplayMode(true);
	_bossFace.SetDisplayMode(true);
	_enemyFace.SetDisplayMode(true);

	_enemySkills.Update();

	_boss->SetInRoom(true);
	_intruders.front()->SetInRoom(true);
	
	switch (now_situation)
	{
	case BattleSequence::None:
		break;
	case BattleSequence::Ready:
		Ready();
		break;
	case BattleSequence::EnemyAttack:
		EnemyAttack();
		break;
	case BattleSequence::BossAttack:
		BossAttack();
		break;
	case BattleSequence::BossDied:
		BossDied();
		break;
	default:
		break;
	}
}

void BossBattle::SetBattleObject(std::shared_ptr<TiledObject>& intruder, ColleagueNotifyer& notifyer)
{
	auto param = intruder->GetRawParameter();
	std::string name = intruder->GetName();

	std::shared_ptr<Character> characterIntruder = std::dynamic_pointer_cast<Character>(intruder);

	std::string resourceName = "";
	std::string displayName = "";

	if (characterIntruder) {

		resourceName = characterIntruder->GetResourceName();

		resourceName = resourceName.substr(resourceName.rfind("/") + 1);

		displayName = characterIntruder->GetDisplayName();
	}

	auto _pos = Vector2D(600, 600); 

	std::shared_ptr<BossBattleObject> _intruder;

	if (characterIntruder) {
		_intruder = std::make_shared<BossBattleObject>(_pos, param, notifyer, name, resourceName, displayName, false);
	}
	else {
		_intruder = std::make_shared<BossBattleObject>(_pos, param, notifyer, name, false);
	}

	

	if (_intruders.empty()) {

		std::vector<std::string> faceArray;
		CSVReader reader;

		std::string fileName = Main::RESOURCEFILENAME + "csv/boss/enemy_face.csv";
		reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, faceArray, 1);

		for (int i = 0; i < faceArray.size(); i += 2) {

			if (_intruder->GetResourceName().size() >= faceArray[i].size() && _intruder->GetResourceName().find(faceArray[i], _intruder->GetResourceName().size() - faceArray[i].size()) != std::string::npos) {
				_enemyFace.Load(Main::RESOURCEFILENAME + "resource/graph/" + faceArray[i + 1] + ".png");

			}

		}

		std::vector<std::string> skillArray;
		fileName = Main::RESOURCEFILENAME + "csv/boss/enemy_attackSkill.csv";
		reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, skillArray, 1);

		for (int i = 0; i < skillArray.size(); i += 3) {

			if (_intruder->GetResourceName().size() >= skillArray[i].size() && _intruder->GetResourceName().find(skillArray[i], _intruder->GetResourceName().size() - skillArray[i].size()) != std::string::npos) {
				_enemySkills.SwitchWithReset("stop");
				_enemySkills.RemoveAnimation("skill");
				_enemySkills.AddAnimation("skill", std::make_shared<GraphArray>(Main::RESOURCEFILENAME + "resource/graph/" + skillArray[i + 1] + ".png", 32, 24));
				_enemySkills.SwitchWithReset("skill");
				_enemySkills.GetCurrentGraph()->SetDisplayMode(false);
				_enemySkills.GetCurrentGraph()->SetPosition(_pos);
				_enemySkills.GetCurrentGraph()->SetRenderType(Texture2D::RenderType::UI);
				_enemySkills.GetCurrentGraph()->SetPriority(static_cast<int>(Sprite::Priority::UI) + 3);
				_enemySkills.GetCurrentGraph()->Rotate(std::stod(skillArray[i + 2]));
			}

		}


	}


	//_intruder->SetInRoom(true);
	_intruders.push_back(_intruder);



}

bool BossBattle::HasKilled(BattleParameter& atk, BattleParameter& dff)
{
	int dmg = 0;
	if (atk._attack > atk._magicAttack) {

		double atck = atk._attack;
		double def = dff._defence;
		double damage = 0;

		if (atck <= 346)
			damage = 5 + std::pow(atck, 2.0) * std::pow(def, -0.84) * 100 * 0.01;
		else
			damage = 5 + std::pow(atck, 0.7) * std::pow(def, -0.84) * 100 * 20;

		dmg = damage;

	}
	else {

		double magicAtk = atk._magicAttack;
		double magicDef = dff._magicDefence;
		double damage = 0;

		if (magicAtk <= 742)
		{
			damage = 5 + std::pow(magicAtk, 1.85) * std::pow(magicDef, -0.84) * 100 * 0.01;
		}
		else
		{
			damage = 5 + std::pow(magicAtk, 0.7) * std::pow(magicDef, -0.84) * 100 * 20;
		}

		dmg = damage;
	}

	dff._hp -= dmg;

	if (dff._hp <= 0) {
		return true;
	}
	return false;
}

void BossBattle::GenerateObject(std::string typame)
{
}

void BossBattle::Ready()
{
	_boss->SwitchAnime(false);
	if(_sequenceTimer >= _readyTime){
		_sequenceTimer = 0;
		now_situation = BattleSequence::EnemyAttack;

	}

	++_sequenceTimer;
}

void BossBattle::EnemyAttack()
{
	auto enemy = _intruders.front();
	enemy->SetIsSkillActivate(true);
	if (_enemySkills.GetCurrentGraph()->GetPosition()._x >= _boss->GetPosition()._x - 32 && _enemySkills.GetCurrentGraph()->GetPosition()._x <= _boss->GetPosition()._x + 32) {
		_enemySkills.GetCurrentGraph()->SetDisplayMode(false);
		_enemySkills.SwitchWithReset("skill");
		_boss->SwitchAnime(true);
	}
	else {
		_enemySkills.GetCurrentGraph()->SetPosition(enemy->GetPosition() + Vector2D(TILE_SIZE / 2, TILE_SIZE / 2) - Vector2D(_sequenceTimer * 6, 0));
		_enemySkills.GetCurrentGraph()->SetDisplayMode(true);
		_enemySkills.SwitchWithReset("skill");
		_boss->SwitchAnime(false);
	}
	
	

	if (_sequenceTimer >= _battleTime) {
		auto& e = enemy->GetRawParameter();
		auto& b = _boss->_param;
		
 		if (HasKilled(e, b)) {
			now_situation = BattleSequence::BossDied;
			_boss->SwitchAnime(false);
			return;
		}
		_sequenceTimer = 0;
		
		now_situation = BattleSequence::BossAttack;

		enemy->SetIsSkillActivate(false);

		return;
	}
	++_sequenceTimer;
}

void BossBattle::BossAttack()
{
	_boss->SwitchAnime(false);
	if (_sequenceTimer >= _waitTime) {
		_sequenceTimer = 0;
		now_situation = BattleSequence::Ready;
		_intruders.front()->OutBossRoom();
		_intruders.pop_front();	//çUåÇÇèIÇ¶ÇΩìGÇÕîzóÒÇ©ÇÁèúãé

		if (!_intruders.empty()) {

			std::vector<std::string> faceArray;
			CSVReader reader;

			std::string fileName = Main::RESOURCEFILENAME + "csv/boss/enemy_face.csv";
			reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, faceArray, 1);

			for (int i = 0; i < faceArray.size(); i += 2) {

				if (_intruders.front()->GetResourceName().find(faceArray[i]) != std::string::npos) {
					_enemyFace.Load(Main::RESOURCEFILENAME + "resource/graph/" + faceArray[i + 1] + ".png");
				}

			}

			auto _pos = Vector2D(600, 600);
			std::vector<std::string> skillArray;
			fileName = Main::RESOURCEFILENAME + "csv/boss/enemy_attackSkill.csv";
			reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, skillArray, 1);

			for (int i = 0; i < skillArray.size(); i += 3) {

				if (_intruders.front()->GetResourceName().size() >= skillArray[i].size() && _intruders.front()->GetResourceName().find(skillArray[i], _intruders.front()->GetResourceName().size() - skillArray[i].size()) != std::string::npos) {
					_enemySkills.SwitchWithReset("stop");
					_enemySkills.RemoveAnimation("skill");
					_enemySkills.AddAnimation("skill", std::make_shared<GraphArray>(Main::RESOURCEFILENAME + "resource/graph/" + skillArray[i + 1] + ".png", 32, 24));
					_enemySkills.SwitchWithReset("skill");
					_enemySkills.GetCurrentGraph()->SetDisplayMode(false);
					_enemySkills.GetCurrentGraph()->SetPosition(_pos);
					_enemySkills.GetCurrentGraph()->SetRenderType(Texture2D::RenderType::UI);
					_enemySkills.GetCurrentGraph()->SetPriority(static_cast<int>(Sprite::Priority::UI) + 3);
					_enemySkills.GetCurrentGraph()->Rotate(std::stod(skillArray[i + 2]));
				}

			}

		}

	}

	++_sequenceTimer;
}

void BossBattle::BossDied()
{
	if (_sequenceTimer >= _diedTime) {
		_wasBossKilled = true;
		_boss->Clear();
		return;
	}

	++_sequenceTimer;
}
