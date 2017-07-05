#pragma once

#include "cocos2d.h"
#include <string>

class BlueTank : public cocos2d::Sprite
{
public:
    //���Ժ���һ�㳡�������ı���
    int hp;
    std::string curr_bullet_name;
    int bullet_count;

public:
    BlueTank();
    ~BlueTank();
    static BlueTank* create();

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
    void setDefaultProperty();

private:
    // const ��ʾ�þ���Ĺ�������
    // ���µ�ʱ����������������
    const std::string default_bullet_name = "bullet1.png";
    const float m_pressv = 0.5;
    const int m_basic_hurt = 10;
    // ���������������m_basic_hurtһ����Ϊ���ٶȵĲο�ֵ����������������
    float m_power;

    // ����ϵͳ���������
    bool m_istouch;
    // ����UI
    cocos2d::Label* powerbar;
};