#pragma once
#include "PanelBase.h"
#include "EditObject.h"

class PanelAffectObjects :
    public PanelBase
{
    //�J�e�S���I��
public:
    PanelAffectObjects();
    PanelAffectObjects(PanelContent _panelContent);
    ~PanelAffectObjects();

    void Update();
    void Draw();

    void Init(PanelContent _panelContent);

    std::string GetCategoryName();

    void SetSettingObject(std::vector<std::shared_ptr<PanelBase>> _tps, EditObject editObject);

    void DrawDebugPrint() override;

    bool IsClicked();


	PanelContent GetPanelContent() {

		return panel;

	}

private:
    PanelContent panel;
};

