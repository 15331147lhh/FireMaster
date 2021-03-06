﻿#include "BlueTank.h"
#include "Bullet.h"
#include "Progress.h"
#include "FireMaster.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace CocosDenshion;
using namespace cocos2d;

BlueTank::BlueTank() {}

BlueTank::~BlueTank() {}

BlueTank* BlueTank::create()
{
    BlueTank* pSprite = new BlueTank();

    if (pSprite->initWithSpriteFrameName("blueAttack1.png"))
    {
        pSprite->autorelease();
        pSprite->initOptions();
        pSprite->addEvents();
        return pSprite;
    }

    CC_SAFE_DELETE(pSprite);
    return nullptr;
}

void BlueTank::initOptions()
{
    // do things here like setTag(), setPosition(), any custom logic.
    auto tankScaleSize = 0.7;

    this->setAnchorPoint(Point(0.5, 0.5));
    this->setPosition(100, 100);
    this->setScale(tankScaleSize);

    this->setDefaultProperty();
    isInTurn = false;
}

void BlueTank::addEvents() {
    this->addTouchListener();
}

void BlueTank::addTouchListener()
{
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    // 开始按下的回调函数
    listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        cocos2d::Vec2 p = touch->getLocation();
        cocos2d::Rect rect = this->getBoundingBox();

        if (rect.containsPoint(p))
        {
            if (Global::turn % 2 == 1 && Global::bullets.empty() && !isInTurn) {
                m_power = 0;
                m_istouch = true;//按钮按下
                isInTurn = true;
                this->schedule(schedule_selector(BlueTank::updatePowerbar), 0.1);//蓄力时间判断，每隔0.1秒调度一次

				//按下取消等待计时器
				auto clockUI = this->getParent()->getChildByName("waitClock");
				if (clockUI != NULL) {
					clockUI->setVisible(false);
				}

                // 按下的时候添加力度进度条到场景里面去
				// 创建蓄力条，先用label代替
                powerbar = Progress::create("progressBg.png", "blood.png");
                powerbar->setScaleX(3);
                powerbar->setScaleY(1.5);
                powerbar->setRotation(180);
                powerbar->setProgress(0);
                // 相对于坦克来设置对应的powerbar位置
                powerbar->setPosition(this->getPosition().x, this->getPosition().y + 100);
                this->getParent()->addChild(this->powerbar);

                return true; // to indicate that we have consumed it.
            }
        }
        return false; // we did not consume this event, pass thru.
    };

    // 结束按下的回调函数
    listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        if (Global::turn % 2 == 1) {
            // 把蓄力条给去掉
            this->unschedule(schedule_selector(BlueTank::updatePowerbar));
            this->getParent()->removeChild(powerbar);

			//发射音效
			SimpleAudioEngine::getInstance()->playEffect("music/attack.wav");

            CCLOG("launch _power:%f", m_power);
            BlueTank::touchEvent(touch);
            m_istouch = false;
        }
    };

    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 30);
}

void BlueTank::touchEvent(cocos2d::Touch* touch)
{
    this->runAttack();
    CCLOG("touched BlueTank");
}

void BlueTank::updatePowerbar(float ft)
{
    if (m_istouch)
    {
        // 按照增长速度来蓄力，并且反馈在UI上面
		m_power += m_pressv;
		powerbar->setProgress(5 * m_power);
    }
}

void BlueTank::runAttack()
{
    auto fireAnimate = CallFunc::create([&]() {
        // 在这里添加那个发射的boom的动画，设置好位置，添加，运行动画，移除
		auto boom = Sprite::createWithSpriteFrameName("fire1.png");
		boom->setPosition(this->getPosition().x - 30, this->getPosition().y + 40);

		this->getParent()->addChild(boom, 2, "boom"); // 设置一个名字，便于追踪

													  // 去掉爆炸后的效果
        auto s = Sequence::create(Animate::create(AnimationCache::getInstance()->getAnimation("fireAnimation")),
            CallFunc::create(
                [&]() {
            this->getParent()->getChildByName("boom")->removeAllChildrenWithCleanup(true);
            this->getParent()->getChildByName("boom")->removeFromParentAndCleanup(true);
            //this->getParent()->getChildByName("boom")->removeFromParent();
        }
        ), nullptr);
        boom->runAction(s);
    });

    auto launch = CallFunc::create([&]() {
        // 在这里添加子弹生成,同时设置好物理的刚体属性，旋转发射角度，水平和垂直初速度等等，
        // 利用m_power设置好子弹的杀伤力。在FireMaster场景类里面进行调度检测碰撞。
        auto curr_bullet_hurt = m_basic_hurt;
        if (this->curr_bullet_name == "tank_bullet4.png") {
            curr_bullet_hurt = Global::bigBullet;
        }
        auto b = Bullet::create(this->curr_bullet_name, curr_bullet_hurt);
        // 子弹相关属性设置，还需要调整
        b->setPosition(this->getPosition().x + 20, this->getPosition().y + 40);
        b->setRotation(-50.0f);
        b->getPhysicsBody()->setVelocity(Vec2(m_power * 25, m_power * 20));

        Global::bullets.push_back(b);
        this->getParent()->addChild(b, 1);
    });

    auto setDefaultLamda = CallFunc::create([&]() {
        this->setDefaultProperty();
    });
    auto attackAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("blueTankAttackAnimation"));
	auto afterAttackAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("blueTankAfterAttackAnimation"));

    if (bullet_count == 1) {
        auto s = Sequence::create(attackAnimate, fireAnimate, launch, DelayTime::create(0.5f),
            afterAttackAnimate, setDefaultLamda, nullptr);
        this->runAction(s);
    }
    else if (bullet_count == 3) {
        auto s = Sequence::create(attackAnimate, fireAnimate, launch,
            DelayTime::create(0.5f), fireAnimate, launch, DelayTime::create(0.5f),
            fireAnimate, launch, DelayTime::create(0.5f), afterAttackAnimate, setDefaultLamda, nullptr);
        this->runAction(s);
    }

}

void BlueTank::setDefaultProperty()
{
	curr_bullet_name = default_bullet_name;
	bullet_count = 1;
}
