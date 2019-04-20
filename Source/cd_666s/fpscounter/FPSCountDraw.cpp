#include "FPSCountDraw.h"
#include "../DebugDraw.h"
#include "../../Main.h"

FPSDrawer::FPSDrawer(FPSCounter& fpsCounter)
    : _fpsCounter(fpsCounter)
{
}


FPSDrawer::~FPSDrawer()
{
}


void FPSDrawer::Draw()
{
	if (Main::DEBUGFLAG) {
		Debug::DrawString_TopPriority(Vector2D::zero, "FPS : " + std::to_string(_fpsCounter.GetFPS()), Color4(1.0, 0.0, 0.0, 1.0));
		Debug::DrawString_TopPriority(Vector2D(0, 700), std::to_string(_fpsCounter.GetDeltaTime()), Color4(1.0, 0.0, 0.0, 1.0));
	}

}