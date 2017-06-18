#pragma once

#include "cocos2d.h"

class YellowTank : public cocos2d::Sprite
{
public:
    //���Ժ���һ�㳡�������ı���
    int hp;

public:
    YellowTank();
    ~YellowTank();
    static YellowTank* create();

    // ��ʼ������������������Ϣ
    void initOptions();

    //��������¼�
    void addEvents();

    //��Ӽ����ʹ����¼�
    void addTouchListener();
    void touchEvent(cocos2d::Touch* touch);

    //���Ⱥ���
    //�������ȵĺ���
    void updatePowerbar(float ft);

    //������Ϊ
    void runAttack();


private:
    // const ��ʾ�þ���Ĺ�������
    // ���µ�ʱ����������������
    const float m_pressv = 0.5;
    const int m_basic_hurt = 10;
    // ���������������m_basic_hurtһ����Ϊ���ٶȵĲο�ֵ����������������
    float m_power;

    // ����ϵͳ���������
    bool m_istouch;
    // ����UI
    cocos2d::Label* powerbar;
};