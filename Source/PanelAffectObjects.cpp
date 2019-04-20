#include "PanelAffectObjects.h"

#include "PanelSettingObject.h"
#include "PanelDisplayer.h"
#include "cd_666s/Utility/CSVReader.h"
#include "cd_666s/Resources/AllResourceManager.h"

PanelAffectObjects::PanelAffectObjects()
    : panel(PanelContent())
{
}

PanelAffectObjects::PanelAffectObjects(PanelContent _panelContent)
    : panel(std::move(_panelContent))
{
    std::string filename = Main::RESOURCEFILENAME + "resource/graph/ui/" + panel._name + ".png";


    auto pos = panel._pos;

    _position.Set(pos._x, pos._y);

    _graph.Load(filename);

    _graph.SetPosition(_position);

    _graph.SetPriority(Sprite::Priority::UI);
    //_graph.SetScale(Vector2D(TILE_SIZE / 32.0, TILE_SIZE / 32.0));

    //_graph.SetDisplayMode(true);

}

PanelAffectObjects::~PanelAffectObjects()
{
}


void PanelAffectObjects::Init(PanelContent _panelContent)
{
    panel = _panelContent;
}

void PanelAffectObjects::Update()
{
    GraphicalObject::Update();
}

void PanelAffectObjects::Draw()
{
    GraphicalObject::Draw();

    //DrawCircle(panel._pos._x, panel._pos._y, 5, GetColor(255, 0, 0));

}

std::string PanelAffectObjects::GetCategoryName()
{
    if (panel._name.find("Monster") != std::string::npos) {
        return "MONSTER";
    }
    else if (panel._name.find("Trap") != std::string::npos) {
        return "TRAP";
    }
    else if (panel._name.find("Block") != std::string::npos) {
        return "BLOCK";
    }
    else {
        return "";
    }
}


void PanelAffectObjects::SetSettingObject(std::vector<std::shared_ptr<PanelBase>> _tps, EditObject editObject)
{

    std::vector<std::string> _array;

	_array = editObject.getEditOblectList(GetCategoryName());

	int DataNum = 0;

	std::string fileName = "";

	if (GetCategoryName() == "MONSTER") {
		DataNum = 6;
		
	}
	else if (GetCategoryName() == "TRAP") {
		DataNum = 5;
	}
	else if (GetCategoryName() == "BLOCK") {
		DataNum = 5;
	}
	else {
		return;
	}


    for (int i = 0; i < _tps.size(); i++) {

		std::shared_ptr<PanelSettingObject> _ps = dynamic_pointer_cast<PanelSettingObject>(_tps[i]);

        if (!_array.empty()) {
            if (_array.size() >= DataNum) {
				
				if (GetCategoryName() == "MONSTER") {
					fileName = Main::RESOURCEFILENAME + "csv/Edit/MONSTER_DATA/" + _array[0] + "/" + _array[0] + ".csv";

					std::vector<std::string> dataArray;
					CSVReader reader;
					reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, dataArray, 1);

					const int parameterNum = 11;

					if (std::stoi(_array[4]) > 1) {
						dataArray.erase(dataArray.begin(), dataArray.begin() + ((parameterNum * (std::stoi(_array[4]) - 1))));
					}

					_ps->PanelSettingObject_SettingPanel(_array[0], _array[3], GetCategoryName(), dataArray[parameterNum - 3], std::stoi(_array[4]), std::stoi(_array[5]));

				}
				else {
					_ps->PanelSettingObject_SettingPanel(_array[0], _array[2], GetCategoryName(), _array[1], std::stoi(_array[3]), std::stoi(_array[4]));
				}
				
                //_tps[i] = PanelSettingObject(_array[i]);
				_array.erase(_array.begin(), _array.begin() + DataNum);
            }
            else {
				_array.clear();
				_ps->PanelSettingObject_SettingPanel("", "", GetCategoryName(), "", -1, -1);
                //_tps[i] = std::make_shared<PanelSettingObject>(nullptr);
            }
            
        }
        else {
			_ps->PanelSettingObject_SettingPanel("", "", GetCategoryName(), "", -1, -1);
            //_tps[i] = std::make_shared<PanelSettingObject>(nullptr);
        }
    }
}

void PanelAffectObjects::DrawDebugPrint()
{
    printfDx("PanelAffectObjects:\n");
    printfDx("name:(%s)  ", panel._name.c_str());
    printfDx("(X:%7.1f, Y:%7.1f)\n", panel._pos._x, panel._pos._y);
    printfDx("-------------------------------\n");

    std::ofstream writing_file;
    writing_file.open(RESOURCE_TABLE->GetFolderPath() + "test.csv", std::ios::app);

    writing_file << "name:" << panel._name << " (X,Y):" << panel._pos._x << "," << panel._pos._y << std::endl;
    writing_file.close();
}

bool PanelAffectObjects::IsClicked()
{
    return GetIsClicked(panel);
}
