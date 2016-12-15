#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#include "ui/CocosGUI.h"

#include <vector>

#include <sstream>
#include <string>
#include <iostream>

#include "Enemy.h"
#include "EnemyAnimation.hpp"

#include "PluginFacebook/PluginFacebook.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"



USING_NS_CC;

using namespace std;
using namespace rapidjson;

int _i;
Vec2 _origin;
Size _visibleSize;
Vec2 _s_centre;


cocos2d::Label* _label;
cocos2d::Label* _timeLeftLabel;
int _score;
int _timeLeft;
cocos2d::Label* _bigCountDownLabel;

cocos2d::PhysicsBody *_physicsBody;
bool isScreenBeingTouched = false;

bool _isGameActive = false;

EnemyAnimation *_enemy;

cocos2d::Label* _userNameLabel;

Sprite* _imageFrame;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //Log Resouces searchpath dependeding on resolution
    //std::vector<std::string> v{"Hello, ", " Cruel ", "World!"};
    std::string s;
    s = accumulate(begin(FileUtils::getInstance()->getSearchPaths()), end(FileUtils::getInstance()->getSearchPaths()), s);
    CCLOG("Search path: %s", s.c_str());
   
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bird.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("explosion.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("cat.plist");
    
    //SpriteFrameCache::getInstance()->
    
    TextureCache::sharedTextureCache()->addImage("buttonGreen.png");
    TextureCache::sharedTextureCache()->addImage("buttonYellow.png");
    
    // background
    //auto background = Sprite::createWithSpriteFrameName("frame-5.png");
    //background->setPosition(origin.x + visibleSize.width / 2,origin.y + visibleSize.height/2);
    //this->addChild(background, 5);
    
    
    
    //////////////////////////////
    // 1. super init first
   /* if ( !Layer::init() )
    {
        return false;
    }*/
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 0) )) {
        return false;
    }
    _origin = Director::getInstance()->getVisibleOrigin();
    _visibleSize = Director::getInstance()->getVisibleSize();
    auto _s_centre = Vec2(_visibleSize.width/2 + _origin.x, _visibleSize.height/2 + _origin.y);
    
    
    //Sprite *explosionSprite = Sprite::create("explosion/explosion-1.png");
    auto explosionSprite = Sprite::createWithSpriteFrameName("bird-7.png");
    explosionSprite->setPosition(_s_centre);
    
    //addChild(explosionSprite, 100);
    
    
    //createExplotion(s_centre);
    Device::setAccelerometerEnabled(true);
    Device::setAccelerometerInterval(1.0f/60.f);
    auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(HelloWorld::OnAcceleration, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sounds/backgroundMusic.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/backgroundMusic.mp3", true);
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/pop.mp3");    
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/countdown.mp3");

    
   
    _i = 0;
    _score = 0;
    _timeLeft = 60;
    _isGameActive = true;
    _bigCountDownLabel = cocos2d::Label::createWithTTF("10", "fonts/supercell.ttf", 30);
    _bigCountDownLabel->setColor(Color3B(0,179,211));
    _bigCountDownLabel->setOpacity(0);
    _bigCountDownLabel->setPosition(Point( _visibleSize.width / 2 + _origin.x, _visibleSize.height / 2 + _origin.y ));
    //Shadow effect
    _bigCountDownLabel->enableOutline(Color4B(0,0,0,255), 1);
    _bigCountDownLabel->enableShadow(Color4B(0,0,0,255),Size(0,-1),0);
    
    this->addChild(_bigCountDownLabel, 200);
    
    
    
    addBackground();
    addHUD();
    
    auto edgeBody = PhysicsBody::createEdgeBox( _visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3 );
    
    auto edgeNode = Node::create();
    edgeNode ->setPosition( Point( _visibleSize.width / 2 + _origin.x, _visibleSize.height / 2 + _origin.y ) );
    edgeNode->setPhysicsBody( edgeBody );
    
    this->addChild( edgeNode );
    
    sdkbox::PluginFacebook::setListener(this);
    
    #ifdef SDKBOX_ENABLED
    sdkbox::PluginFacebook::init();
    //MessageBox("SDKBOX!", "Funcionando");
    #endif

    //sdkbox::PluginFacebook::requestReadPermissions({sdkbox::FB_PERM_READ_PUBLIC_PROFILE, sdkbox::FB_PERM_READ_EMAIL, sdkbox::FB_PERM_READ_USER_FRIENDS});
    //sdkbox::PluginFacebook::requestPublishPermissions({sdkbox::FB_PERM_PUBLISH_POST});

    
    
    if (sdkbox::PluginFacebook::isLoggedIn())
    {
        CCLOG("______________LOGGED IN______________");
        sdkbox::FBAPIParam params;
        params["fields"] = "name,email,first_name,last_name,id";
        sdkbox::PluginFacebook::api("me", "GET", params, "me");
        
        
    }else{
        sdkbox::PluginFacebook::login();
    }
    
    
    
    
    
    
    _imageFrame = Sprite::create("imageFrame.png");
    
    // position the sprite on the center of the screen
    _imageFrame->setPosition(Vec2(_origin.x + _imageFrame->getContentSize().width / 2, _visibleSize.height + _origin.y - _imageFrame->getContentSize().height / 2));
    //_imageFrame->setAnchorPoint(Vec2(0, 1));
    // add the sprite as a child to this layer
    this->addChild(_imageFrame, 100);
    
    //s.SetInt(s.GetInt() + 1);
    
    // 3. Stringify the DOM
    //StringBuffer buffer;
    //Writer<StringBuffer> writer(buffer);
    //d.Accept(writer);
    
    // Output {"project":"rapidjson","stars":11}
    //std::cout << buffer.GetString() << std::endl;
    
    
    /*Working: Partilcles
    ParticleSystemQuad *p = ParticleSystemQuad::create("particle_texture.plist");
    p->setPosition(Point( _visibleSize.width / 2 + _origin.x, p->getContentSize().height/ 2 + _origin.y ));
    p->setScale(0.5f);
    Color4F startColor(255, 0.5f, 0.5f, 1.0f);
    p->setStartColor(startColor);
    
    Color4F startColorVar(255, 0.5f, 0.5f, 1.0f);
    p->setStartColorVar(startColorVar);
    
    Color4F endColor(255, 0.1f, 0.1f, 0.2f);
    p->setEndColor(endColor);
    
    Color4F endColorVar(255, 0.1f, 0.1f, 0.2f);
    p->setEndColorVar(endColorVar);
    p->setBlendAdditive(true);
    //p->setBlendFunc(BlendFunc::ADDITIVE);
    this->addChild(p);
     */
    
    auto button = cocos2d::ui::Button::create("buttonGreen.png", "buttonYellow.png", "buttonYellow.png");
    
    //button->setTitleText("Jugar");
    button->setScale(_visibleSize.width / button->getContentSize().width * 0.5);
    button->setPosition(Point( _visibleSize.width / 2 + _origin.x, _visibleSize.height / 2 + _origin.y ));
    
    //_label = Label::createWithBMFont("fonts/clashOfClansFont-ipadhd.fnt", "Jugar");
    cocos2d::Label *buttonLabel = Label::createWithTTF("Share", "fonts/supercell.ttf", 17);
    //Shadow effect
    buttonLabel->enableOutline(Color4B(0,0,0,150), 1);
    buttonLabel->enableShadow(Color4B(0,0,0,100),Size(0,-1),0);
    buttonLabel->setPosition(Vec2(button->getContentSize().width / 2, button->getContentSize().height / 1.8));
    //_label->setAnchorPoint(Vec2(0.5f, 0.5f));
    
    button->addChild(buttonLabel);
    //addChild(button, 500);
    
    //auto button = cocos2d::ui::Button::create("Button.png", "ButtonYellow.png", "ButtonYellow.png");
    
    //button->setTitleText("Jugar");
    //button->setScale(_visibleSize.width / button->getContentSize().width * 0.5);
    //button->setPosition(Vec2(_origin.x + _visibleSize.width/2, _origin.y + _visibleSize.height/2 ));
    
    button->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                printf("Login to Facebook");
                MessageBox("Login to Facebook!", "Alert");
                sdkbox::PluginFacebook::login();
                
                
                //auto myScene = HelloWorld::createScene();
                //Director::getInstance()->replaceScene(myScene);
                //Director::getInstance()->replaceScene(TransitionSlideInT::create(1, myScene) );
                
                
                break;
        }
    });
    
    //this->addChild(button, 6);
    
    
    
    
    
    
    
    
    
    //WORKING: Enemy floating with Impulse
    /*
    _enemy = EnemyAnimation::create();
    _enemy->setPosition(_origin.x + _enemy->getContentSize().width / 2 ,_origin.y + _visibleSize.height/2);
    _enemy->setScale(_visibleSize.width / _enemy->getContentSize().width * 0.2);
    _physicsBody = PhysicsBody::createCircle(_enemy->getContentSize().height / 2, PhysicsMaterial(0.0f, 1.0f, 0.1f));
    
    _physicsBody->applyImpulse(Vect( 0, 30));
    _physicsBody->setGravityEnable(true);
    _physicsBody->setDynamic(true);
    _physicsBody->setTag(1);
    _physicsBody->setVelocity(Vec2(0, -100));
    _enemy->setPhysicsBody(_physicsBody);
    //enemy->setTag(_i);
    addChild(_enemy);
    
    
    auto listener1 = EventListenerTouchOneByOne::create();
    
    
    // trigger when you push down
    listener1->onTouchBegan = [](Touch* touch, Event* event){
        // your code
        CCLOG("Tocado");
        isScreenBeingTouched = true;
        
        return true; // if you are consuming it
    };
    
    // trigger when moving touch
    listener1->onTouchMoved = [](Touch* touch, Event* event){
        // your code
    };
    
    // trigger when you let up
    listener1->onTouchEnded = [=](Touch* touch, Event* event){
        // your code
        //physicsBody->applyImpulse(Vect( 0, 2000));
        isScreenBeingTouched = false;
    };
    
    // Add listener
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
    */
    //WORKING: Enemy floating with impulse
    
    //this->schedule(schedule_selector(HelloWorld::applyImpulse),0.1f);
    this->schedule(schedule_selector(HelloWorld::updateTimer),1.0f);
    this->schedule(schedule_selector(HelloWorld::spawnEnemies),1.0f);

    
    /*
    this->schedule(schedule_selector(HelloWorld::updateTimer),1.0f);
    
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto s_centre = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
    */
    //_score = 60;
    
    
    //_label = Label::createWithTTF(StringUtils::format("%d", _score), "fonts/clashOfClansFont-ipadhd.fnt", 25);
    //Shadow effect
    //_label->enableShadow(Color4B(0,0,0,255),Size(1,-1),0);
     
    //_label->setString("No");
    // position the label on the center of the screen

    // add the label as a child to this layer

    /*
    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    //this->addChild(sprite, 0);
    
    auto button = cocos2d::ui::Button::create("frame-1.png", "frame-4.png", "frame-4.png");
    
    //button->setTitleText("Salta");
    button->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height/2 ));
    
    button->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                printf("Button 1 clicked");
                _score++;
                
                //_label->setString(std::to_string(_score));
                //_label->setString("Nel");
                
                
                //[label setString: [NSString stringWithFormat:@"%d",score]];
                //MessageBox("Congrats on completing the game!", "Victory");
                break;
            default:
                break;
        }
    });
    
    this->addChild(button);
    
    
    cocos2d::network::HttpRequest *request = new cocos2d::network::HttpRequest( );
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json; charset=utf-8");
    request->setHeaders(headers);
    
    request->setUrl( "https://tomathonweb.appspot.com/api/rank/" );
    request->setRequestType( cocos2d::network::HttpRequest::Type::POST );
    request->setResponseCallback( CC_CALLBACK_2( HelloWorld::onHttpRequestCompleted, this ) );
    
    std::string postData = "{\"username\": \"mmzepedab2\", \"first_name\": \"mmzepedab\", \"last_name\": \"mmzepedab\", \"highest_score\": \"mmzepedab\", \"points\": \"mmzepedab\", \"email\": \"mmzepedab\", \"facebook_id\": \"mmzepedab\"}";
    request->setRequestData( postData.c_str(), postData.length() );
    //cocos2d::network::HttpClient::getInstance()->send( request );
    request->release( );
    */
    
    //auto physicsBody = PhysicsBody::createBox(Size(visibleSize.width, 81.0f), PhysicsMaterial(0.1f, 1.0f, 0.0f));
    //physicsBody->setDynamic(false);
    
    //create a sprite
    //auto sprite1 = Sprite::create("frame-1.png");
    //sprite1->setPosition(Vec2(s_centre.x, origin.y));
    //sprite1->setScale(0.20);
    //addChild(sprite1);
    
    
    
    //apply physicsBody to the sprite
    //sprite1->setPhysicsBody(physicsBody);
    
    
    
    
    
    //this->getPhysicsWorld->setGravity(Vec2(0, -10.0f));
    //add five dynamic body
    
    
    
    

    
    //this->runAction(HelloWorld::createExplotion());
    //HelloWorld::createExplotion();

    //this->schedule(schedule_selector(HelloWorld::spawnEnemies));
    //this->scheduleOnce(schedule_selector(HelloWorld::spawnEnemies), 2.0f);
    
    return true;
}


                   
void HelloWorld::applyImpulse(float dt){
    if(isScreenBeingTouched == true){
        _physicsBody->applyImpulse(Vect( 0, 30));
    }
}


void HelloWorld::addBackground(){
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto s_centre = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
    
    //create a sprite
    auto backgroundSprite = Sprite::create("uncolored_castle.png");
    backgroundSprite->setPosition(Vec2(s_centre.x, s_centre.y));
    //Sacale Type 1
    //backgroundSprite->setScale(visibleSize.width / backgroundSprite->getContentSize().width, visibleSize.height / backgroundSprite->getContentSize().height);
    
    //Sacale Type 2
    float scale = MAX(visibleSize.width / backgroundSprite->getContentSize().width, visibleSize.height / backgroundSprite->getContentSize().height);
    backgroundSprite->setScale(scale);
    
    //Sacale Type 3
    //float scale = MIN(visibleSize.width / backgroundSprite->getContentSize().width, visibleSize.height / bg->getContentSize().height);
    //backgroundSprite->setScale(scale);
    
    //sprite1->setScale(0.20);
    addChild(backgroundSprite, -1000);
}

void HelloWorld::addHUD(){
    _label = Label::createWithBMFont("fonts/clashOfClansFont-ipadhd.fnt", StringUtils::format("%d", _score));
    _label->setPosition(Vec2(_origin.x + _visibleSize.width/2,
                             _origin.y + _visibleSize.height - _label->getContentSize().height));
    this->addChild(_label, 2);
    
    auto timerBGSprite = Sprite::create("timerBG.png");
    
    _timeLeftLabel = Label::createWithBMFont("fonts/clashOfClansFont-ipadhd.fnt", StringUtils::format("Tiempo: %d", _timeLeft));
    _timeLeftLabel->setPosition(Vec2(_origin.x + _visibleSize.width/2,
                                     _origin.y + _label->getContentSize().height));
    
    timerBGSprite->setPosition(_timeLeftLabel->getPosition());
    //this->addChild(timerBGSprite, 3);
    this->addChild(_timeLeftLabel, 4);

}

void HelloWorld::spawnEnemies(float dt){
    
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto s_centre = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
    
    
    //physicsBody->setDynamic(false);
    
    
    
    //for (int i = 0; i < 2; ++i)
    //{
        
        int randomValue = cocos2d::random(1,10);
        string enemyFile;
        
        switch (randomValue) {
            case 1:
                enemyFile = "01_Pretty_Bird";
                break;
            case 2:
                enemyFile = "02_green_cat";
                break;
            case 3:
                enemyFile = "03_Biting_Bee";
                break;
            case 4:
                enemyFile = "04_angry_egg";
                break;
            case 5:
                enemyFile = "05_earphone_bug";
                break;
            case 6:
                enemyFile = "06_little_bat";
                break;
            case 7:
                enemyFile = "07_flying_pumpkin";
                break;
            case 8:
                enemyFile = "08_bat_monster";
                break;
            case 9:
                enemyFile = "09_blue_bear";
                break;
            case 10:
                enemyFile = "10_pink_hairy_bug";
                break;
                
            default:
                break;
        }
        
        //physicsBody = PhysicsBody::createBox(Size(65.0f, 81.0f), PhysicsMaterial(0.1f, 1.0f, 0.0f));
        
        //set the body isn't affected by the physics world's gravitational force
    
    
        //set initial velocity of physicsBody
        //physicsBody->setVelocity(Vec2(cocos2d::random(-5,5), cocos2d::random(-5,5)));
        //physicsBody->setVelocity(Vec2(cocos2d::random(-5,5), 500));
        
    Enemy *enemy = Enemy::create();
    //Scale the enemy to be 20% of the width of the screen
    enemy->setScale(visibleSize.width / enemy->getContentSize().width * 0.2);
    enemy->setPosition(Vec2(s_centre.x , origin.y + enemy->getBoundingBox().size.height / 2  ));
    
    //auto button = cocos2d::ui::Button::create(enemyFile + "/frame-1.png", enemyFile +"/frame-4.png", enemyFile +"/frame-4.png");
        //auto button = cocos2d::ui::Button::create("frame-1.png", "frame-5.png", "frame-5.png", cocos2d::ui::TextureResType::PLIST);
        
        //Sprite *sprite1 = Sprite::create("11_leaf_head/frame-1.png");
        //sprite1->setPosition(Vec2(s_centre.x + cocos2d::random(-300,300), s_centre.y + cocos2d::random(-300,300)));
    
    //button->setScale(0.3);
    //button->setPosition(Vec2(s_centre.x , origin.y + button->getBoundingBox().size.height / 2  ));
    
    //auto physicsBody = PhysicsBody::createBox(Size(button->getContentSize().width, button->getContentSize().height), PhysicsMaterial(0, 0.8f, 0));
    
    auto physicsBody = PhysicsBody::createCircle(enemy->getContentSize().height / 2, PhysicsMaterial(0, 0.8f, 0.5f));
    
    physicsBody->applyImpulse(Vect( cocos2d::random(-300,300), cocos2d::random(600,1000) ));
    physicsBody->setGravityEnable(true);
    physicsBody->setDynamic(true);
    physicsBody->setTag(1);
    enemy->setPhysicsBody(physicsBody);
    enemy->setTag(_i);
    addChild(enemy);
        //button->setPhysicsBody(physicsBody);
        
        //button->setTag(_i);
    _i++;
    
    /*
        button->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type){
            switch (type)
            {
                case ui::Widget::TouchEventType::BEGAN:
                    break;
                case ui::Widget::TouchEventType::ENDED:{
                    printf("Button 1 clicked");
                    _score++;
                    //_label = Label::createWithTTF(StringUtils::format("%d", _score), "fonts/Marker Felt.ttf", 24);
                    _label->setString(StringUtils::format("%d", _score));
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/pop.mp3");
                    //_label->setString(std::to_string(_score));
                    //_label->setString("Nel");
                    //printf("%s",((cocos2d::ui::Button*) sender)->getPosition());
                    HelloWorld::createExplotion(((cocos2d::ui::Button*) sender)->getPosition());
                    this->removeChildByTag(((cocos2d::ui::Button*) sender)->getTag());
                    
                    //this->removeChild();
                    //[label setString: [NSString stringWithFormat:@"%d",score]];
                    //MessageBox("Congrats on completing the game!", "Victory");
                    
                }
                    break;
                default:
                    break;
            }
        });
     
     */
        //button->getPhysicsBody()->applyImpulse(Vec2(1000, 200));
        //addChild(button);
    
        //physicsBody->applyForce(Vec2(-5, -5));
    //}
}

void postData(){
    
}

void HelloWorld::OnAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event)
{
    CCLOG("%f", acc->z);
    
    
    
    Director::getInstance()->getRunningScene()->getPhysicsWorld()->setGravity(Vec2((acc->x)*200, (acc->y)*200));

    /* WORKING: Rotate sprite with Accelerometer
    float angleRadians = atanf((float)acc->x / (float)acc->y);
    float angleDegrees = -CC_RADIANS_TO_DEGREES(angleRadians);
    float cocosAngle = 1 * angleDegrees;
    _enemy->setRotation(cocosAngle);
     */
    
    //_label->setString(StringUtils::format("%f",acc->x));
}

void HelloWorld::createExplotion(cocos2d::Vec2 position){
    
    if(_isGameActive){
        _score++;
    }
    _label->setString(StringUtils::format("%d", _score));
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/pop.mp3");
    
    
    //-- Add Sprite With Animation Sample - What's added to the screen is the spriteBatch
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto s_centre = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
    //Add Sprite With Animation Sample - What's added to the screen is the spriteBatch
    SpriteBatchNode* spritebatch = SpriteBatchNode::create("explosion.png");
    Sprite *Sprite1 = Sprite::createWithSpriteFrameName("frame-1.png");
    spritebatch->setPosition(position);
    spritebatch->addChild(Sprite1);
    spritebatch->setScale(0.3);
    addChild(spritebatch);
    
    Vector<SpriteFrame*> animFrames(6);
    
    char str[100] = {0};
    for(int i = 1; i < 7; i++)
    {
        sprintf(str, "frame-%d.png", i);
        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName( str );
        animFrames.pushBack(frame);
    }
    
    Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    //Sprite1->runAction( RepeatForever::create( Animate::create(animation) ) );
    
    auto sequence = Sequence::create(Animate::create(animation), RemoveSelf::create() , NULL);
    Sprite1->runAction(sequence);
    
    //-- Add Sprite With Animation Sample - What's added to the screen is the spriteBatch

}

void HelloWorld::onHttpRequestCompleted( cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response )
{
    std::vector<char> *buffer = response->getResponseData( );
    
    printf( "Get data: ");
    for ( unsigned int i = 0; i < buffer->size( ); i++ )
    {
        printf( "%c", ( *buffer )[i] );
    }
    printf( "\n" );
    
    if ( 200 != response->getResponseCode( ) )
    {
        printf( "Succeeded" );
        return;
    }
    else
    {
        printf( "Failed" );
    }
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}

void HelloWorld::updateTimer(float dt)
{
    _timeLeft--;
    _timeLeftLabel->setString(StringUtils::format("Tiempo: %d", _timeLeft));
    if(_timeLeft > 0){
        
        
        if (_timeLeft == 30) {
            this->unschedule(schedule_selector(HelloWorld::spawnEnemies));
            this->schedule(schedule_selector(HelloWorld::spawnEnemies),0.5f);
        }
        
        if (_timeLeft == 10) {
            this->unschedule(schedule_selector(HelloWorld::spawnEnemies));
            this->schedule(schedule_selector(HelloWorld::spawnEnemies),0.2f);
        }
        
        if(_timeLeft == 10){
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/countdown.mp3");
        }
        
        if(_timeLeft < 11){
            _bigCountDownLabel->setString(StringUtils::format("%d", _timeLeft));
            auto fadeInFadeOutSequence = Sequence::create(FadeIn::create(0.5), FadeOut::create(0.5), NULL);
            _bigCountDownLabel->runAction(fadeInFadeOutSequence);
        }
        
        //_label->setString(std::to_string(_score));
        //StringUtils::format("Tiempo: %d", _timeLeft)
        //StringUtils::format("sounds/Female/%d.ogg", _timeLeft)
        
    }else{
        _isGameActive = false;
        
        //MessageBox("Congrats you lose", "Time's up");
        this->unschedule(schedule_selector(HelloWorld::updateTimer));
        this->unschedule(schedule_selector(HelloWorld::spawnEnemies));
        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        
        auto gameOverBGSprite = Sprite::create("gameOverBg.png");
        gameOverBGSprite->setPosition(Vec2(_origin.x + _visibleSize.width/2, _origin.y + _visibleSize.height/2 ));
        addChild(gameOverBGSprite);
        
        
        
        cocos2d::Label* finalScoreLabel = Label::createWithTTF("0", "fonts/supercell.ttf", 30);
        finalScoreLabel->setString(StringUtils::format("%d", _score));
        finalScoreLabel->setPosition(Vec2(gameOverBGSprite->getContentSize().width / 2.0, gameOverBGSprite->getContentSize().height / 2.5 ));
        finalScoreLabel->enableOutline(Color4B(0,0,0,255), 1);
        finalScoreLabel->enableShadow(Color4B(0,0,0,255),Size(0,-1),0);
        gameOverBGSprite->addChild(finalScoreLabel);
        
        //Action *zoomIN = ScaleBy::create(0.5f, 0.7f);
        //Action *zoomOut = zoomIN->reverse();
        Sequence *zoomSeq = Sequence::create(ScaleTo::create(0.5f, 0.7f), ScaleTo::create(0.5f, 1.0f), NULL);
        //finalScoreLabel->runAction(zoomSeq);
        
        finalScoreLabel->runAction( RepeatForever::create( zoomSeq ) );
        
        cocos2d::Label* finalScoreTitleLabel = Label::createWithTTF("Score:", "fonts/supercell.ttf", 12);
        finalScoreTitleLabel->setPosition(Vec2(finalScoreLabel->getPosition().x, finalScoreLabel->getPosition().y + finalScoreLabel->getContentSize().height / 2 + finalScoreTitleLabel->getContentSize().height / 2));
        finalScoreTitleLabel->enableOutline(Color4B(0,0,0,255), 1);
        finalScoreTitleLabel->enableShadow(Color4B(0,0,0,255),Size(0,-1),0);
        gameOverBGSprite->addChild(finalScoreTitleLabel);
        
        auto button = cocos2d::ui::Button::create("buttonGreen.png", "buttonYellow.png", "buttonYellow.png");
        
        //button->setTitleText("Jugar");
        button->setScale(_visibleSize.width / button->getContentSize().width * 0.5);
        button->setPosition(Vec2(gameOverBGSprite->getPosition().x, gameOverBGSprite->getPosition().y - gameOverBGSprite->getContentSize().height / 2 ));
        
        //_label = Label::createWithBMFont("fonts/clashOfClansFont-ipadhd.fnt", "Jugar");
        cocos2d::Label *buttonLabel = Label::createWithTTF("Jugar", "fonts/supercell.ttf", 17);
        //Shadow effect
        buttonLabel->enableOutline(Color4B(0,0,0,150), 1);
        buttonLabel->enableShadow(Color4B(0,0,0,100),Size(0,-1),0);
        buttonLabel->setPosition(Vec2(button->getContentSize().width / 2, button->getContentSize().height / 1.8));
        //_label->setAnchorPoint(Vec2(0.5f, 0.5f));
        
        button->addChild(buttonLabel);
        //addChild(button, 500);
        
        //auto button = cocos2d::ui::Button::create("Button.png", "ButtonYellow.png", "ButtonYellow.png");
        
        //button->setTitleText("Jugar");
        //button->setScale(_visibleSize.width / button->getContentSize().width * 0.5);
        //button->setPosition(Vec2(_origin.x + _visibleSize.width/2, _origin.y + _visibleSize.height/2 ));
        
        button->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type){
            switch (type)
            {
                case ui::Widget::TouchEventType::BEGAN:
                    break;
                case ui::Widget::TouchEventType::ENDED:
                    printf("Button 1 clicked");
                    
                    
                    auto myScene = HelloWorld::createScene();
                    //Director::getInstance()->replaceScene(myScene);
                    Director::getInstance()->replaceScene(TransitionSlideInT::create(1, myScene) );
                    
                    
                    break;
            }
        });
        
        this->addChild(button, 6);
        
    }
}


/*********************
 * Facebook callbacks
 *********************/
void HelloWorld::onLogin(bool isLogin, const std::string& error)
{
    //MessageBox("Alert", "Facebook Login Call back!");
    //MessageBox("Error", error.c_str());
    
    CCLOG("##FB isLogin: %d, error: %s", isLogin, error.c_str());
    
    if (isLogin)
    {
        CCLOG("______________LOGGED IN______________");
        sdkbox::FBAPIParam params;
        params["fields"] = "name,email,first_name,last_name";
        sdkbox::PluginFacebook::api("me", "GET", params, "me");
        
    }
    
    std::string title = "login ";
    title.append((isLogin ? "success" : "failed"));
    //    MessageBox(error.c_str(), title.c_str());
}


void HelloWorld::onAPI(const std::string& tag, const std::string& jsonData)
{
    CCLOG("##FB onAPI: tag -> %s, json -> %s", tag.c_str(), jsonData.c_str());
    //MessageBox(jsonData.c_str(), "Alert");
    //const char* jsonbuf = "{\"name\":\"Marito Zepeda\",\"email\":\"bzmm5@hotmail.com\"}";
    rapidjson::Document                 JSON;
    //rapidjson::Value::MemberIterator    M;
    //const char                          *key,*value;
    
    JSON.Parse<0>(jsonData.c_str());
    
    if (JSON.HasParseError())
    {
        CCLOG("Json has errors!!!");
    }
    
    rapidjson::Value& idJSON = JSON["id"];
    rapidjson::Value& firstNameJSON = JSON["first_name"];
    rapidjson::Value& lastNameJSON = JSON["last_name"];
    CCLOG("%s = %s", firstNameJSON.GetString(),lastNameJSON.GetString());
    //MessageBox(stringJSON.GetString(), "Json");
    
    _userNameLabel = Label::createWithBMFont("fonts/clashOfClansFont-ipadhd.fnt", StringUtils::format("%s %s", firstNameJSON.GetString(), lastNameJSON.GetString()));
    //_userNameLabel = cocos2d::Label::createWithTTF("Mario Zepeda", "fonts/supercell.ttf", 6);
    //_userNameLabel->setColor(Color3B(255,224,144));
    //_userNameLabel->enableOutline(Color4B(0,0,0,255), 1);
    //_userNameLabel->enableShadow(Color4B(0,0,0,255),Size(0.5,-0.5),0);
    
    _userNameLabel->setAnchorPoint(Vec2(0, 0.5));
    _userNameLabel->setScale(_visibleSize.width / _userNameLabel->getContentSize().width * 0.3);
    _userNameLabel->setPosition(Vec2(_imageFrame->getPosition().x + _imageFrame->getContentSize().width/2, _imageFrame->getPosition().y));
    addChild(_userNameLabel, 201);
    //_userNameLabel->setString(StringUtils::format("%s %s", firstNameJSON.GetString(), lastNameJSON.GetString()));
    //std::string url=StringUtils::format("https://graph.facebook.com/%s/picture?width=200&height=200",userInfo.getUserId().c_str());
    
    string _id = idJSON.GetString(); // id require to whome you want to fectch photo
    cocos2d::network::HttpRequest* request = new (std::nothrow) cocos2d::network::HttpRequest();
    string url = "https://graph.facebook.com/"+_id+"/picture?height=120&width=120";
    request->setUrl(url.c_str());
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(HelloWorld::onRequestImgCompleted, this));
    request->setTag("GetImage");
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
    
}

void HelloWorld::onSharedSuccess(const std::string& message)
{
    CCLOG("##FB onSharedSuccess:%s", message.c_str());
    
    //    MessageBox(message.c_str(), "share success");
}

void HelloWorld::onSharedFailed(const std::string& message)
{
    CCLOG("##FB onSharedFailed:%s", message.c_str());
    
    //    MessageBox(message.c_str(), "share failed");
}

void HelloWorld::onSharedCancel()
{
    CCLOG("##FB onSharedCancel");
    
    //    MessageBox("", "share cancel");
}

void HelloWorld::onPermission(bool isLogin, const std::string& error)
{
    CCLOG("##FB onPermission: %d, error: %s", isLogin, error.c_str());
    
    std::string title = "permission ";
    title.append((isLogin ? "success" : "failed"));
    //    MessageBox(error.c_str(), title.c_str());
}

void HelloWorld::onFetchFriends(bool ok, const std::string& msg){}
void HelloWorld::onRequestInvitableFriends( const sdkbox::FBInvitableFriendsInfo& friends ){}
void HelloWorld::onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg ){}
void HelloWorld::onInviteFriendsResult( bool result, const std::string& msg ){}

void HelloWorld::onGetUserInfo( const sdkbox::FBGraphUser& userInfo )
{
    CCLOG("Facebook id:'%s' name:'%s' last_name:'%s' first_name:'%s' email:'%s' installed:'%d'",
          userInfo.getUserId().c_str(),
          userInfo.getName().c_str(),
          userInfo.getFirstName().c_str(),
          userInfo.getLastName().c_str(),
          userInfo.getEmail().c_str(),
          userInfo.isInstalled ? 1 : 0
          );
    /*
    MessageBox("User ID", userInfo.getUserId().c_str() );
    MessageBox("User Name", userInfo.getName().c_str() );
    MessageBox("User First Name", userInfo.getFirstName().c_str() );
    MessageBox("User Last Name", userInfo.getLastName().c_str() );
    MessageBox("User Email", userInfo.getEmail().c_str() );
     */
    
    //MessageBox("User First Name", userInfo.getFirstName().c_str() );

    
    
    
    
}

void HelloWorld::onRequestImgCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    
    log("AppDelegate::onHttpRequestCompleted - onHttpRequestCompleted BEGIN");
    if (!response)
    {
        log("onHttpRequestCompleted - No Response");
        return;
    }
    
    log("onHttpRequestCompleted - Response code: %lu", response->getResponseCode());
    
    if (!response->isSucceed())
    {
        log("onHttpRequestCompleted - Response failed");
        log("onHttpRequestCompleted - Error buffer: %s", response->getErrorBuffer());
        return;
    }
    log("onHttpRequestCompleted - Response code: %s", response->getResponseDataString());
    
    std::vector<char> *buffer = response->getResponseData();
    
    Image * image = new  Image ();
    image-> initWithImageData ( reinterpret_cast<const unsigned char*>(&(buffer->front())), buffer->size());
    Texture2D * texture = new  Texture2D ();
    texture-> initWithImage (image);
    Sprite* sp = Sprite::createWithTexture(texture);
    sp->setScale(0.5);
    //sp->setAnchorPoint(Vec2(0, 1));
    addChild(sp, 250);
    sp->setPosition(_imageFrame->getPosition());
    //if(sp){
    //_userNameLabel->setPosition(Vec2(sp->getPosition().x + sp->getContentSize().width, _visibleSize.height + _origin.y - _userNameLabel->getContentSize().height));
    //}
}


