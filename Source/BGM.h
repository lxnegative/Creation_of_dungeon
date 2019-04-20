#pragma once
#include <string>


class BGM {

private:

    static const int BGM_NUM_MAX = 10;
    static const int SE_NUM_MAX = 20;

    int bgmHandle[BGM_NUM_MAX];
    int seHandle[SE_NUM_MAX];

public:
    BGM();
    BGM(std::string class_name);
    ~BGM();

    void BGMPlay();//bgmHandle[0]���Đ�����B���ɍĐ�����Ă���ꍇ�A�������Ȃ��B�Đ�����ہA�S�Ă�BGM���~����
    void BGMPlay(int bgmNum);//bgmHandle[bgmNum]���Đ�����B���ɍĐ�����Ă���ꍇ�A�������Ȃ��B�Đ�����ہA�S�Ă�BGM���~����
    void BGMStop();//�S�Ă�BGM���~����

    void SEPlay(int seNum);//seHandle[bgmNum]���Đ�����B
    void SEStop(int seNum);//seHandle[bgmNum]���~����B

};