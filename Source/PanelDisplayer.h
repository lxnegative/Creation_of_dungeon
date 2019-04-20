#pragma once
#include "PanelBase.h"
class PanelDisplayer :
    public PanelBase
{
    //�P���ɒl��s�x�󂯎��\�����邾��
public:
    PanelDisplayer();
    PanelDisplayer(PanelContent _panelContent);
    ~PanelDisplayer();

    void Update() override;
    void Draw() override;

    void Init(PanelContent _panelContent);

    bool IsClicked();

    void DrawDebugPrint() override;

private:
    PanelContent panel;
};

