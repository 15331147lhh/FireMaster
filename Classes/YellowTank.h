#pragma once
#include "Progress.h"
#include "cocos2d.h"
#include "Global.h"
#include "Bullet.h"
#include <string>

class YellowTank : public cocos2d::Sprite
{
public:
    //���Ժ���һ�㳡�������ı���
    bool isInTurn;

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
    void setDefaultProperty();
    void setCurrBulletName(const std::string & t_name) {
        curr_bullet_name = t_name;
    }
    void setBulletCount(const int n) {
        bullet_count = n;
    }

private:
    // const ��ʾ�þ���Ĺ�������
    // ���µ�ʱ����������������
    const std::string default_bullet_name = "bullet1.png";
	const float m_pressv = Global::pressv;
	const int m_basic_hurt = Global::smallBullet;

    // ���������������m_basic_hurtһ����Ϊ���ٶȵĲο�ֵ����������������
    float m_power;

    // ����ϵͳ���������
    bool m_istouch;
    // ����UI
    Progress* powerbar;

    std::string curr_bullet_name;
    int bullet_count;
};