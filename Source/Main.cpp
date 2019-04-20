
#include "DxLib.h"
#include "Main.h"
#include "MainLoop.h"
#include "resource.h"
#include "cd_666s/Resources/AllResourceManager.h"
#include "cd_666s/InputManager/InputManager.h"
#include "cd_666s/Render/RenderManager.h"
#include <Windows.h>
#include <direct.h>

bool Main::DEBUGFLAG = false;
std::string Main::RESOURCEFILENAME = "resource/";

Main::Main()
{
}


Main::~Main()
{
}

char JPEG[] =
{
	'C' - 21,
	'S' - 21,
	0
};

//���[�v�̑�{
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{



	SetOutApplicationLogValidFlag(FALSE);

	Main::DEBUGFLAG = false;

	int Argc;        // �R�}���h���C�������̌�
	LPWSTR *Argv;    // �R�}���h���C�������̃��X�g

	Argv = CommandLineToArgvW(GetCommandLineW(), &Argc);

	if (Argc >= 2) {
		size_t num;
		std::string str = "";
		char c[256];
		wcstombs_s(&num,c,Argv[1],sizeof(Argv[1]));

		str = std::string(c);

		if (str == "Debug" || str == "debug") {
			Main::DEBUGFLAG = true;
		}
	}

	ChangeWindowMode(TRUE);
	SetGraphMode(1280, 720, 32);

	SetMainWindowText("CREATION OF DUNGEON");
	SetWindowIconID(IDI_ICON1);

	//********** �t�H���g�̃��[�h **********

	HRSRC  hFontResource = FindResource(NULL, MAKEINTRESOURCE(IDR_STRINGFONT1), _TEXT("STRINGFONT"));
	HGLOBAL LoadRes = LoadResource(NULL, hFontResource);
	LPVOID lpFontImage = LockResource(LoadRes);
	DWORD  dwFontImageSize = SizeofResource(NULL, hFontResource);
	DWORD  dwFontCount = 0;
	HANDLE hFontResourceHandle = AddFontMemResourceEx(lpFontImage, dwFontImageSize, NULL, &dwFontCount);

	

	for (int i = 0; JPEG[i] != 0; i++)
	{
		JPEG[i] += 21;
	}

	SetUseDXArchiveFlag(true);
	SetDXArchiveKeyString(JPEG);
	SetDXArchiveExtension("data");


	if (DxLib_Init() == -1)     // �c�w���C�u��������������
	{
		return -1;          // �G���[���N�����璼���ɏI��
	}

	// ********** �t�H���g�̃��[�h **********
	/*
	LPCSTR font_path = "JF-Dot-K14.ttf"; // �ǂݍ��ރt�H���g�t�@�C���̃p�X
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// �t�H���g�Ǎ��G���[����
		MessageBox(NULL, "�t�H���g�Ǎ����s", "", MB_OK);
	}
	*/

	ChangeFont("JF�h�b�gK14", DX_CHARSET_DEFAULT);

	//���\�[�X�Ǘ����W���[��������
	char cwd[512];
	memset(cwd, 0, 512);
	_getcwd(cwd, 512);
	std::string temp(cwd, 512);
	auto it = temp.find('\0');
	temp.resize(it);
	temp.append("/");
	RESOURCE_TABLE->SetAllTypesFolderPath(temp);
	RENDER_MGR->Init();

	MainLoop main_loop;

	main_loop.GameLoop();

	if (RemoveFontMemResourceEx(hFontResourceHandle)) {
	}
	else {
		MessageBox(NULL, "remove failure", "", MB_OK);
	}

	DxLib_End();                // �c�w���C�u�����g�p�̏I������

	return 0;               // �\�t�g�̏I�� 

}