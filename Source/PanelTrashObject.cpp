#include "PanelTrashObject.h"

#include "PanelSettingObject.h"
#include "PanelDisplayer.h"
#include "cd_666s/Utility/CSVReader.h"
#include "cd_666s/Resources/AllResourceManager.h"


PanelTrashObject::PanelTrashObject()
	: panel(PanelContent())
{
}

PanelTrashObject::PanelTrashObject(PanelContent _panelContent)
	: panel(std::move(_panelContent))
{
	std::string filename = Main::RESOURCEFILENAME + "resource/graph/ui/GarbageBox.png";


	auto pos = panel._pos;

	_position.Set(pos._x, pos._y);

	_graph.Load(filename);

	_graph.SetPosition(_position);

	_graph.SetPriority(Sprite::Priority::UI);
	//_graph.SetScale(Vector2D(TILE_SIZE / 32.0, TILE_SIZE / 32.0));

	//_graph.SetDisplayMode(true);

}


PanelTrashObject::~PanelTrashObject()
{
}


void PanelTrashObject::Init(PanelContent _panelContent)
{
	panel = _panelContent;
}

void PanelTrashObject::Update()
{
	GraphicalObject::Update();

}

void PanelTrashObject::Draw()
{
	GraphicalObject::Draw();

	//DrawCircle(panel._pos._x, panel._pos._y, 5, GetColor(255, 0, 0));

}

void PanelTrashObject::Select()
{
	isSelect = true;
	_graph.Load(Main::RESOURCEFILENAME + "resource/graph/ui/SelectGarbageBox.png");
}

void PanelTrashObject::NotSelect()
{
	isSelect = false;
	_graph.Load(Main::RESOURCEFILENAME + "resource/graph/ui/GarbageBox.png");
}

void PanelTrashObject::DrawDebugPrint()
{
	printfDx("PanelPageChangeObject:\n");
	printfDx("name:(%s)  ", panel._name.c_str());
	printfDx("(X:%7.1f, Y:%7.1f)\n", panel._pos._x, panel._pos._y);
	printfDx("-------------------------------\n");

	std::ofstream writing_file;
	writing_file.open(RESOURCE_TABLE->GetFolderPath() + "test.csv", std::ios::app);

	writing_file << "name:" << panel._name << " (X,Y):" << panel._pos._x << "," << panel._pos._y << std::endl;
	writing_file.close();
}

bool PanelTrashObject::IsClicked()
{
	return GetIsClicked(panel);
}

bool PanelTrashObject::GetSelect() {

	return isSelect;

}

