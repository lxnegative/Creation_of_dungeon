#pragma once
#include "PanelBase.h"

class PanelTrashObject :
	public PanelBase
{
public:
	PanelTrashObject();
	PanelTrashObject(PanelContent _panelContent);
	~PanelTrashObject();

	void Update() override;
	void Draw() override;

	void Init(PanelContent _panelContent);

	void Select();
	void NotSelect();

	void DrawDebugPrint() override;

	bool IsClicked();

	bool GetSelect();	//ëIëèÛë‘Ç©

private:
	PanelContent panel;

	bool isSelect;

};

#pragma once
