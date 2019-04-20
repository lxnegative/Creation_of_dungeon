#include <array>
#include "../Utility/CSVReader.h"
#include "../Resources/AllResourceManager.h"

#include "Dungeon.h"
#include "StartPoint.h"
#include "Goal.h"
#include "Enemy.h"
#include "Monster.h"

#include "ShootDamageObject.h"
#include "MagicAttackAround.h"
#include "RiseParameter.h"
#include "MagicHeal.h"
#include "MagicHealAround.h"

#include "MineBomb.h"
#include "CurseArea.h"
#include "MagicBomb.h"
#include "Sanctuary.h"
#include "Emplacement.h"
#include "../../DungeonInfomation.h"


//���x������蕥���A���x���㑱�̒l��ǂݎ��
std::string LoadLabeledElem(const std::string& label, const std::string& rawData)
{
    size_t it = rawData.find(label);
    std::string cutData = rawData.substr(it + label.length());

    size_t dataEnd = cutData.find("&");
    cutData = cutData.substr(0, dataEnd);
    return cutData;
}

template<class T>
std::string LoadLabeledElemIfFind(const std::string& label, const std::string& rawData, T fallback)
{
    size_t it = rawData.find(label);
    std::string cutData = rawData.substr(it + label.length());
    if (it == std::string::npos)
        return std::to_string(fallback);

    size_t dataEnd = cutData.find("&");
    cutData = cutData.substr(0, dataEnd);
    return cutData;
}

void Enemy::LoadEnemys(std::vector<std::shared_ptr<TiledObject>>& objects, StartPoint& start, Goal& goal, ColleagueNotifyer& notifyer, std::string fileName)
{
    _defeatedNum = 0;
    _enemysNum = 0;
	_robTresureItem.clear();

    std::vector<std::string> dataArray;
    CSVReader reader;
    reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, dataArray, 2);

    const int parameterNum = 12;
    std::array<int, parameterNum> params = { 0, 0, 0, 0, 0, 0, 0 };
    std::string name = "";
    std::string skillData = "";

	std::string ResourceName = "";
	std::string DisplayName = "";

	EnemyAIType aiType = EnemyAIType::Normal;

    int idx = 0;
    int count = 0;

	bool isTemplate = false;

	int Level = -1;

    for (auto data : dataArray)
    {
        if (data.empty()) {
            break;
        }

		//�`���҂Ƀe���v���[�g��ǉ�
		if (count == 0) {
			if (data == "template") {
				isTemplate = true;
				count++;
				continue;
			}
		}

		if (isTemplate) {
			if (count == 1) {
				name = data;
			}
			else if (count == 2) {
				params[6] = std::stoi(data);
			}
			else if (count == 3) {
				const int paramNum = 11;
				std::string TemplateFilename = Main::RESOURCEFILENAME + "csv/StageData/EnemyData/" + name + ".csv";
				std::vector<std::string> TemplateDataArray;
				reader.Read(RESOURCE_TABLE->GetFolderPath() + TemplateFilename, TemplateDataArray, 1);

				ResourceName = TemplateDataArray[(paramNum * (std::stoi(data) - 1)) + 1];
				DisplayName = TemplateDataArray[(paramNum * (std::stoi(data) - 1)) + 2];

				for (int i = 3; i < paramNum - 2; i++) {
					params[i - 3] = std::stoi(TemplateDataArray[(paramNum * (std::stoi(data) - 1)) + i]);
				}

				if (TemplateDataArray[(paramNum * (std::stoi(data) - 1)) + paramNum - 2] == "Normal") {
					aiType = EnemyAIType::Normal;
				}
				else if (TemplateDataArray[(paramNum * (std::stoi(data) - 1)) + paramNum - 2] == "SkillMain") {
					aiType = EnemyAIType::SkillMain;
				}
				else if (TemplateDataArray[(paramNum * (std::stoi(data) - 1)) + paramNum - 2] == "Hunter") {
					aiType = EnemyAIType::Hunter;
				}
				else if (TemplateDataArray[(paramNum * (std::stoi(data) - 1)) + paramNum - 2] == "Healer") {
					aiType = EnemyAIType::Healer;
				}
				else if(TemplateDataArray[(paramNum * (std::stoi(data) - 1)) + paramNum - 2] == "Default"){
					if (name == "blaver" || name == "fighter") {
						aiType = EnemyAIType::Normal;
					}
					if (name == "magician") {
						aiType = EnemyAIType::SkillMain;
					}
					if (name == "thief") {
						aiType = EnemyAIType::Hunter;
					}
					if (name == "healer") {
						aiType = EnemyAIType::Healer;
					}
				}

				
				skillData = TemplateDataArray[(paramNum * (std::stoi(data) - 1)) + paramNum - 1];

				count = parameterNum - 1;
				Level = std::stoi(data);
				isTemplate = false;
			}

			if (count != parameterNum - 1) {
				count++;
				continue;
			}
			//�`���҂Ƀe���v���[�g�����܂�
		}
		else {
			// MEMO : �Ōゾ���̓t�@�C���������̂܂܎g��
			if (count < parameterNum - 5)
			{
				params[count] = std::stoi(data);
			}
			else if (count == parameterNum - 5)
			{
				name = data;
			}
			else if (count == parameterNum - 4) {

				ResourceName = data;

			}
			else if (count == parameterNum - 3) {
				DisplayName = data;
			}
			else if (count == parameterNum - 2) {
				if (data == "Normal") {
					aiType = EnemyAIType::Normal;
				}
				else if (data == "SkillMain") {
					aiType = EnemyAIType::SkillMain;
				}
				else if (data == "Hunter") {
					aiType = EnemyAIType::Hunter;
				}
				else if (data == "Healer") {
					aiType = EnemyAIType::Healer;
				}
				else if (data == "Default") {
					if (name == "blaver" || name == "fighter") {
						aiType = EnemyAIType::Normal;
					}
					if (name == "magician") {
						aiType = EnemyAIType::SkillMain;
					}
					if (name == "thief") {
						aiType = EnemyAIType::Hunter;
					}
					if (name == "healer") {
						aiType = EnemyAIType::Healer;
					}
				}
			}
			else
			{
				skillData = data;
			}
		}
        count++;

        if (count == parameterNum)
        {
            //�퓬�f�[�^�ݒ�
            //hp, skillCost, atk, def, matk, mdef, speed
            BattleParameter param = { params[0], params[1], params[2], params[3], params[4], params[5]};

            auto enemy = std::make_shared<Enemy>(start.GetTilePos(), param, goal, notifyer, name, ResourceName, DisplayName, aiType);
			enemy->SetLevel(Level);

            //�X�L������
            std::unique_ptr<CharactersSkill> skill(CharactersSkill::CreateSkill(skillData, *enemy));
            enemy->_skill = std::move(skill);

            objects.push_back(enemy);
            //�o�����Ԃ�b�P�ʂɕϊ����ē���҃��X�g�ɒǉ�
            start.AddToAppearList(enemy, params[6] * 60);

            //���̃L������
            count = 0;
            idx++;
			Level = -1;

            _enemysNum++;
        }
    }
}


void Dungeon::LoadTileSize(std::string stageName, std::vector<std::string>& rawData)
{
    auto stageNum = std::stoi(stageName);
    auto tileSize = rawData[stageNum * 2 - 1];

    if (tileSize == "large")
        TILE_SIZE = 48;
    else if (tileSize == "midium")
        TILE_SIZE = 40;
    else
        TILE_SIZE = 32;
}

void Monster::LoadMonsters_DungeonInfomation(std::vector<std::shared_ptr<TiledObject>>& objects, ColleagueNotifyer& notifyer)
{
	std::vector<std::string> dataArray = DungeonInfomation::GetInstance()->GetEditMonsterDataList();

	const int parameterNum = 12;
	std::array<int, parameterNum> params = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int idx = 0;
	int count = 0;
	std::string name;
	std::string skill;
	std::string ResourceName;
	std::string DisplayName;
	for (auto data : dataArray)
	{
		if (data.empty()) {
			break;
		}

		// MEMO : �Ōゾ���̓t�@�C���������̂܂܎g��
		if (count < parameterNum - 4)
			params[count] = std::stoi(data);
		else if (count == parameterNum - 4)
			name = data;
		else if (count == parameterNum - 3)
			ResourceName = data;
		else if (count == parameterNum - 2)
			DisplayName = data;
		else if (count == parameterNum - 1)
			skill = data;

		count++;

		if (count == parameterNum)
		{
			BattleParameter param = { params[0], params[1], params[2], params[3], params[4], params[5] };
			TiledVector startPos(params[6], params[7]);

			auto monster = std::make_shared<Monster>(startPos, param, nullptr, notifyer, name, skill, ResourceName, DisplayName);
			objects.push_back(monster);

			auto magicSquare = std::make_shared<MagicSquare>(startPos, *monster);
			monster->_home = magicSquare.get();
			objects.push_back(magicSquare);

			//���̃L������
			count = 0;
			idx++;
		}
	}
}

void Monster::LoadMonsters(std::vector<std::shared_ptr<TiledObject>>& objects, ColleagueNotifyer& notifyer, std::string fileName)
{
    std::vector<std::string> dataArray;
    CSVReader reader;
    reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, dataArray, 1);

    const int parameterNum = 12;
    std::array<int, parameterNum> params = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int idx = 0;
    int count = 0;
    std::string name;
    std::string skill;
	std::string ResourceName;
	std::string DisplayName;
    for (auto data : dataArray)
    {
        if (data.empty()) {
            break;
        }

        // MEMO : �Ōゾ���̓t�@�C���������̂܂܎g��
		if (count < parameterNum - 4)
			params[count] = std::stoi(data);
		else if (count == parameterNum - 4)
			name = data;
		else if (count == parameterNum - 3)
			ResourceName = data;
		else if (count == parameterNum - 2)
			DisplayName = data;
        else if (count == parameterNum - 1)
            skill = data;

        count++;

        if (count == parameterNum)
        {
            BattleParameter param = { params[0], params[1], params[2], params[3], params[4], params[5] };
            TiledVector startPos(params[6], params[7]);

            auto monster = std::make_shared<Monster>(startPos, param, nullptr, notifyer, name, skill, ResourceName, DisplayName);
            objects.push_back(monster);

            auto magicSquare = std::make_shared<MagicSquare>(startPos, *monster);
            monster->_home = magicSquare.get();
            objects.push_back(magicSquare);

            //���̃L������
            count = 0;
            idx++;
        }
    }
}


std::unique_ptr<CharactersSkill> Monster::CreateSkillFromName(std::string name, std::string skillData)
{
    if (skillData == "")
        return nullptr;
    if (skillData == "null")
        return nullptr;

    if (name == "slime")
    {
        auto cost = std::stoi(LoadLabeledElem("cost:", skillData));
        auto paramValue = std::stoi(LoadLabeledElem("param:", skillData));
        auto timeSec = std::stod(LoadLabeledElem("time:", skillData));
        auto time = static_cast<int>(timeSec * 60);

        //�S�����ɕϊ�
        double startMPRatio = std::stoi(LoadLabeledElemIfFind("mpRatio:", skillData, 0.0));
        startMPRatio /= 100;

        BattleParameter percentParam(100, 100, paramValue, 100, paramValue, 100);
        ParameterMultiplier param(percentParam, time, true);
        return std::make_unique<RiseParameter>(std::move(param), cost, startMPRatio, *this);
    }

    if (name == "minotaur")
    {
        auto cost = std::stoi(LoadLabeledElem("cost:", skillData));
        auto paramValue = std::stoi(LoadLabeledElem("param:", skillData));
        auto timeSec = std::stod(LoadLabeledElem("time:", skillData));
        auto time = static_cast<int>(timeSec * 60);

        //�S�����ɕϊ�
        double startMPRatio = std::stoi(LoadLabeledElemIfFind("mpRatio:", skillData, 0.0));
        startMPRatio /= 100;

        BattleParameter percentParam(100, paramValue, 100, 100, 100, 100);
        ParameterMultiplier param(percentParam, time, true);
        return std::make_unique<RiseParameter>(std::move(param), cost, startMPRatio, *this);
    }
    
    if (name == "bone")
    {
        auto skill = ShootDamageObject::Create(skillData, *this, true);

        skill->SetImage(IMAGE_RESOURCE_TABLE->Create(Main::RESOURCEFILENAME + "resource/graph/effect/throwing_bone.png"));

        return skill;
    }

	if (name == "ghost")
	{
		auto skill = ShootDamageObject::Create(skillData, *this, false);

		skill->SetImage(IMAGE_RESOURCE_TABLE->Create(Main::RESOURCEFILENAME + "resource/graph/effect/fire_B.png"));

		return skill;
	}

    return nullptr;
}


std::unique_ptr<CharactersSkill> CharactersSkill::CreateSkill(std::string skillData, Character& chara)
{
    if (skillData == "")
        return nullptr;

    if (skillData == "null")
        return nullptr;

    if (skillData.find("magic_shoot") != std::string::npos)
        return ShootDamageObject::Create(skillData, chara, false);

    if (skillData.find("shoot") != std::string::npos)
        return ShootDamageObject::Create(skillData, chara, true);

    if (skillData.find("magic_explode") != std::string::npos)
        return MagicAttackAround::Create(skillData, chara);

	if (skillData.find("heal_around") != std::string::npos)
		return MagicHealAround::Create(skillData, chara);

    if (skillData.find("heal") != std::string::npos)
        return MagicHeal::Create(skillData, chara);

    if (skillData.find("param_rise") != std::string::npos)
        return RiseParameter::Create(skillData, chara);

    return nullptr;
}


std::unique_ptr<ShootDamageObject> ShootDamageObject::Create(std::string data, Character& chara, bool isPhysical)
{
    int cost = std::stoi(LoadLabeledElem("cost:", data));
    int range = std::stoi(LoadLabeledElem("range:", data));
    double power = std::stod(LoadLabeledElem("power:", data));
    double speed = std::stod(LoadLabeledElem("speed:", data));

    //�S�����ɕϊ�
    double startMPRatio = std::stoi(LoadLabeledElemIfFind("mpRatio:", data, 0.0));
    startMPRatio /= 100;

    return std::make_unique<ShootDamageObject>(power, cost, startMPRatio, speed, range, chara, isPhysical);
}


std::unique_ptr<MagicAttackAround> MagicAttackAround::Create(std::string data, Character& chara)
{
    int cost = std::stoi(LoadLabeledElem("cost:", data));
    int range = std::stoi(LoadLabeledElem("range:", data));
    double power = std::stod(LoadLabeledElem("power:", data));

    //�S�����ɕϊ�
    double startMPRatio = std::stoi(LoadLabeledElemIfFind("mpRatio:", data, 0.0));
    startMPRatio /= 100;

    return std::make_unique<MagicAttackAround>(power, cost, startMPRatio, range, chara);
}


std::unique_ptr<MagicHeal> MagicHeal::Create(std::string data, Character& chara)
{
    int cost = std::stoi(LoadLabeledElem("cost:", data));
    int range = std::stoi(LoadLabeledElem("range:", data));
    double power = std::stod(LoadLabeledElem("power:", data));

    //�S�����ɕϊ�
    double startMPRatio = std::stoi(LoadLabeledElemIfFind("mpRatio:", data, 0.0));
    startMPRatio /= 100;

    return std::make_unique<MagicHeal>(power, cost, startMPRatio, range, chara);
}


std::unique_ptr<MagicHealAround> MagicHealAround::Create(std::string data, Character& chara)
{
    int cost = std::stoi(LoadLabeledElem("cost:", data));
    int range = std::stoi(LoadLabeledElem("range:", data));
    double power = std::stod(LoadLabeledElem("power:", data));

    //�S�����ɕϊ�
    double startMPRatio = std::stoi(LoadLabeledElemIfFind("mpRatio:", data, 0.0));
    startMPRatio /= 100;

    return std::make_unique<MagicHealAround>(power, cost, startMPRatio, range, chara);
}


std::unique_ptr<RiseParameter> RiseParameter::Create(std::string data, Character& chara)
{
    auto cost = std::stoi(LoadLabeledElem("cost:", data));
    auto timeSec = std::stod(LoadLabeledElem("time:", data));
    auto time = static_cast<int>(timeSec * 60);

    int hp = std::stod(LoadLabeledElem("hp:", data));
    int atk = std::stod(LoadLabeledElem("atk:", data));
    int def = std::stod(LoadLabeledElem("def:", data));
    int matk = std::stod(LoadLabeledElem("matk:", data));
    int mdef = std::stod(LoadLabeledElem("mdef:", data));
    int spd = std::stoi(LoadLabeledElem("spd:", data));
    BattleParameter percentParam(hp, atk, def, matk, mdef, spd);

    //�S�����ɕϊ�
    double startMPRatio = std::stoi(LoadLabeledElemIfFind("mpRatio:", data, 0.0));
    startMPRatio /= 100;

    ParameterMultiplier param(percentParam, time, true);
    return std::make_unique<RiseParameter>(std::move(param), cost, startMPRatio, chara);
}


void Trap::CreateTrap(std::string typeName, int countX, int countY, std::vector<std::shared_ptr<TiledObject>>& objects)
{
    auto tilePos = TiledVector(countX, countY);

    if (typeName.find("name:mine") != std::string::npos
        || typeName.find("name:bare_trap") != std::string::npos)
    {
        objects.push_back(MineBomb::Create(typeName, tilePos));
        return;
    }
    if (typeName.find("name:magic_bomb") != std::string::npos)
    {
        objects.push_back(MagicBomb::Create(typeName, tilePos));
        return;
    }
    if (typeName.find("name:curse") != std::string::npos)
    {
        objects.push_back(CurseArea::Create(typeName, tilePos));
        return;
    }
    if (typeName.find("name:sanctuary") != std::string::npos)
    {
        objects.push_back(Sanctuary::Create(typeName, tilePos));
        return;
    }
    if (typeName.find("name:emplacement") != std::string::npos)
    {
        objects.push_back(Emplacement::Create(typeName, tilePos));
        return;
    }
}


std::shared_ptr<MineBomb> MineBomb::Create(std::string data, TiledVector pos)
{
    std::string name = LoadLabeledElem("name:", data);
    int cost = std::stoi(LoadLabeledElem("cost:", data));
    int range = std::stoi(LoadLabeledElem("range:", data));
    int power = std::stoi(LoadLabeledElem("power:", data));
    int attack = std::stoi(LoadLabeledElem("attack:", data));
    double stuckTimeSec = std::stod(LoadLabeledElem("stuck:", data)); // MEMO : �b�P�ʂŐݒ肳���邽��
    int stuckTime = static_cast<int>(stuckTimeSec * 60.0);

	std::string graphName = LoadLabeledElemIfFind("graph:", data, -1);
	if (graphName == "-1") graphName = "";

    if (name == "mine")
        return std::make_shared<MineBomb>(pos, cost, range, power, attack, stuckTime, true, graphName);
    else
        return std::make_shared<MineBomb>(pos, cost, range, power, attack, stuckTime, false, graphName);
}


std::shared_ptr<MagicBomb> MagicBomb::Create(std::string data, TiledVector pos)
{
    int cost = std::stoi(LoadLabeledElem("cost:", data));
    int range = std::stoi(LoadLabeledElem("range:", data));
    int power = std::stoi(LoadLabeledElem("power:", data));
    int attack = std::stoi(LoadLabeledElem("attack:", data));

	std::string graphName = LoadLabeledElemIfFind("graph:", data, -1);
	if (graphName == "-1") graphName = "";

    return std::make_shared<MagicBomb>(pos, cost, range, power, attack, graphName);
}


std::shared_ptr<CurseArea> CurseArea::Create(std::string data, TiledVector pos)
{
    int cost = std::stoi(LoadLabeledElem("cost:", data));
    int hp = std::stoi(LoadLabeledElem("hp:", data));
    int atk = std::stoi(LoadLabeledElem("atk:", data));
    int def = std::stoi(LoadLabeledElem("def:", data));
    int matk = std::stoi(LoadLabeledElem("matk:", data));
    int mdef = std::stoi(LoadLabeledElem("mdef:", data));
    int spd = std::stoi(LoadLabeledElem("spd:", data));

    double continuousTimeSec = std::stod(LoadLabeledElem("time:", data));
    int continuousTime = static_cast<int>(continuousTimeSec * 60.0);

	std::string graphName = LoadLabeledElemIfFind("graph:", data, -1);
	if (graphName == "-1") graphName = "";

    BattleParameter param(hp, atk, def, matk, mdef, spd);
    ParameterMultiplier multiplier(std::move(param), continuousTime, false);
    return std::make_shared<CurseArea>(pos, cost, std::move(multiplier), graphName);
}


std::shared_ptr<Sanctuary> Sanctuary::Create(std::string data, TiledVector pos)
{
    int cost = std::stoi(LoadLabeledElem("cost:", data));
    int range = std::stoi(LoadLabeledElem("range:", data));

    int hp = std::stoi(LoadLabeledElem("hp:", data));
    int atk = std::stoi(LoadLabeledElem("atk:", data));
    int def = std::stoi(LoadLabeledElem("def:", data));
    int matk = std::stoi(LoadLabeledElem("matk:", data));
    int mdef = std::stoi(LoadLabeledElem("mdef:", data));
    int spd = std::stoi(LoadLabeledElem("spd:", data));

    double continuousTimeSec = std::stod(LoadLabeledElem("time:", data));
    int continuousTime = static_cast<int>(continuousTimeSec * 60.0);

	std::string graphName = LoadLabeledElemIfFind("graph:", data, -1);
	if (graphName == "-1") graphName = "";

    BattleParameter param(hp, atk, def, matk, mdef, spd);
    ParameterMultiplier multiplier(std::move(param), continuousTime, true);

    return std::make_shared<Sanctuary>(pos, range, cost, std::move(multiplier), graphName);
}


std::shared_ptr<Emplacement> Emplacement::Create(std::string data, TiledVector pos)
{
    int cost = std::stoi(LoadLabeledElem("cost:", data));
    int power = std::stoi(LoadLabeledElem("power:", data));
    int attack = std::stoi(LoadLabeledElem("attack:", data));

    auto directionStr = LoadLabeledElem("direction:", data);
    auto direction = TiledVector::BACK;
    if (directionStr == "left")
        direction = TiledVector::LEFT;
    else if (directionStr == "right")
        direction = TiledVector::RIGHT;
    else if (directionStr == "down")
        direction = TiledVector::FORWARD;

	std::string graphName = LoadLabeledElemIfFind("graph:", data, -1);
	if (graphName == "-1") graphName = "";

	std::string arrowGraphName = LoadLabeledElemIfFind("arrowgraph:", data, -1);
	if (arrowGraphName == "-1") arrowGraphName = "";

	std::string isPhysicalstr = LoadLabeledElemIfFind("isPhysical:", data, -1);
	if (isPhysicalstr == "-1") isPhysicalstr = "true";

	bool isPhysical = true;

	if (isPhysicalstr == "false") {
		isPhysical = false;
	}

    return std::make_shared<Emplacement>(pos, cost, power, attack, direction, graphName, arrowGraphName, isPhysical);
}



//�G���ォ��ǉ��Ő�������
std::vector<TiledObject*> Monster::GenerateMonster(std::vector<std::shared_ptr<TiledObject>>& objects, ColleagueNotifyer& notifyer, std::string fileName, TiledVector startPos,std::string* GenerateText, int level)
{

    *GenerateText = "";

    std::vector<TiledObject*> monsterObjects;

    std::vector<std::string> dataArray;
    CSVReader reader;
    reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, dataArray, 1);

    const int parameterNum = 11;

	if (level > 1) {
		dataArray.erase(dataArray.begin(), dataArray.begin() + ((parameterNum * (level - 1))));
	}

    std::array<int, parameterNum> params = { 0, 0, 0, 0, 0, 0, 0 };
    int idx = 0;
    int count = 0;
    std::string name;
    std::string skill;
	std::string ResourceName = "";
	std::string DisplayName = "";
    for (auto data : dataArray)
    {
		
		if (count == 0) {
			count++;
			continue;
		}
		

        // MEMO : �Ōゾ���̓t�@�C���������̂܂܎g��
        if (count < parameterNum - 4)
            params[count - 1] = std::stoi(data);
        else if (count == parameterNum - 4) {
            *GenerateText += std::to_string(startPos._x) + "," + std::to_string(startPos._y) + ",";
            name = data;
		}
		else if (count == parameterNum - 3) {
			ResourceName = data;
		}
		else if (count == parameterNum - 2) {
			DisplayName = data;
		}
        else if (count == parameterNum - 1)
            skill = data;

        count++;

        *GenerateText += data;

        if (count == parameterNum)
        {
            BattleParameter param = { params[0], params[1], params[2], params[3], params[4], params[5] };
            //TiledVector startPos(params[6], params[7]);

            auto monster = std::make_shared<Monster>(startPos, param, nullptr, notifyer, name, skill, ResourceName, DisplayName);
            objects.push_back(monster);

			monster->SetLevel(level);

            auto magicSquare = std::make_shared<MagicSquare>(startPos, *monster);
            monster->_home = magicSquare.get();
            objects.push_back(magicSquare);

            monsterObjects.push_back(monster.get());
            monsterObjects.push_back(magicSquare.get());

            //���̃L������
            count = 0;
            idx++;
			break;
        }else{
            *GenerateText += ",";
        }
    }

    return monsterObjects;

}












