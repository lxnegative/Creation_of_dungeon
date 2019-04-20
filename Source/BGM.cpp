#include "BGM.h"
#include "DxLib.h"


BGM::BGM() {

}

BGM::BGM(std::string class_name) {
    //bgmHandle[0]�ɂ悭�g��BGM�����[�h����
    //BGM�͓����Đ��ł��Ȃ��悤�ɂ��Ă���

    if (class_name == "game") {
        //���ݎ�荇�����K���ɏ����Ă���
        bgmHandle[0] = LoadSoundMem("bgm\\sample.ogg");

        seHandle[0] = LoadSoundMem("se\\sample.ogg");

    }
    else if (class_name == "editmap") {


    }

}

BGM::~BGM() {

    for (int i = 0; i < BGM_NUM_MAX; i++) {
        DeleteSoundMem(bgmHandle[i]);
    }

    for (int i = 0; i < SE_NUM_MAX; i++) {
        DeleteSoundMem(seHandle[i]);
    }

}

void BGM::BGMPlay() {

    if (CheckSoundMem(bgmHandle[0])) {
        return;
    }

    for (int i = 0; i < BGM_NUM_MAX; i++) {
        StopSoundMem(bgmHandle[i]);
    }

    PlaySoundMem(bgmHandle[0], DX_PLAYTYPE_LOOP, TRUE);


}

void BGM::BGMPlay(int bgmNum) {

    if (CheckSoundMem(bgmHandle[bgmNum])) {
        return;
    }

    for (int i = 0; i < BGM_NUM_MAX; i++) {
        StopSoundMem(bgmHandle[i]);
    }

    PlaySoundMem(bgmHandle[bgmNum], DX_PLAYTYPE_LOOP, TRUE);

}

void BGM::BGMStop() {

    for (int i = 0; i < BGM_NUM_MAX; i++) {
        StopSoundMem(bgmHandle[i]);
    }

}


void BGM::SEPlay(int seNum) {

    PlaySoundMem(seHandle[seNum], DX_PLAYTYPE_BACK, TRUE);

}

void BGM::SEStop(int seNum) {

    for (int i = 0; i < SE_NUM_MAX; i++) {
        StopSoundMem(seHandle[i]);
    }

}




