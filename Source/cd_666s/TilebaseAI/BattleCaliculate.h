#pragma once

class Character;

namespace Battle
{
    double GetDamage(int power, int attack, bool isPhysical, Character& opponent);
    //�����U���p
    double GetPhysicalAttackDamage(int power, int attack, int defence);
    //���@�U���p
    double GetMagicalAttackDamage(int power, int attack, int defence);
    //������_���[�W�p
    double GetPhysicalDefencedDamage(int power, int attack, int defence);
    //���@��_���[�W�p
    double GetMagicalDefencedDamage(int power, int attack, int defence);
};
