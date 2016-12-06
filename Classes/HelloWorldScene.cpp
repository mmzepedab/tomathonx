#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#include "ui/CocosGUI.h"

#include <vector>

#include <sstream>
#include <string>
#include <iostream>

USING_NS_CC;

using namespace std;

int _i;

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
    //////////////////////////////
    // 1. super init first
   /* if ( !Layer::init() )
    {
        return false;
    }*/
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 0) )) {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto s_centre = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
    
    Device::setAccelerometerEnabled(true);
    auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(HelloWorld::OnAcceleration, this));
    
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sounds/cheerful_annoyance.ogg");
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/cheerful_annoyance.ogg", true);
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/pop.mp3");
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/Female/10.ogg");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/Female/9.ogg");

    
   
    _i = 0;
    _score = 0;
    _timeLeft = 60;
    
    addBackground();
    
    this->schedule(schedule_selector(HelloWorld::updateTimer),1.0f);
    this->schedule(schedule_selector(HelloWorld::spawnEnemies),0.5f);

    
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
    _label = Label::createWithBMFont("fonts/clashOfClansFont-ipadhd.fnt", StringUtils::format("%d", _score));
    _label->setPosition(Vec2(origin.x + visibleSize.width/2,
                             origin.y + visibleSize.height - _label->getContentSize().height));
    this->addChild(_label, 1);
    
    _timeLeftLabel = Label::createWithBMFont("fonts/clashOfClansFont-ipadhd.fnt", StringUtils::format("Tiempo: %d", _timeLeft));
    _timeLeftLabel->setPosition(Vec2(origin.x + visibleSize.width/2,
                             origin.y + _label->getContentSize().height));
    this->addChild(_timeLeftLabel);
    
    //_label = Label::createWithTTF(StringUtils::format("%d", _score), "fonts/clashOfClansFont-ipadhd.fnt");
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
    
    
    auto edgeBody = PhysicsBody::createEdgeBox( visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3 );
    
    auto edgeNode = Node::create();
    edgeNode ->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y ) );
    edgeNode->setPhysicsBody( edgeBody );
    
    this->addChild( edgeNode );
    
    
    //this->getPhysicsWorld->setGravity(Vec2(0, -10.0f));
    //add five dynamic body
    
    
    
    

    
    //this->runAction(HelloWorld::createExplotion());
    //HelloWorld::createExplotion();

    //this->schedule(schedule_selector(HelloWorld::spawnEnemies));
    //this->scheduleOnce(schedule_selector(HelloWorld::spawnEnemies), 2.0f);
    
    return true;
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
    addChild(backgroundSprite);
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
        
    
    
        auto button = cocos2d::ui::Button::create(enemyFile + "/frame-1.png", enemyFile +"/frame-4.png", enemyFile +"/frame-4.png");
        
        //Sprite *sprite1 = Sprite::create("11_leaf_head/frame-1.png");
        //sprite1->setPosition(Vec2(s_centre.x + cocos2d::random(-300,300), s_centre.y + cocos2d::random(-300,300)));
    
        button->setScale(0.3);
    button->setPosition(Vec2(s_centre.x , origin.y + button->getBoundingBox().size.height / 2  ));
    
    //auto physicsBody = PhysicsBody::createBox(Size(button->getContentSize().width, button->getContentSize().height), PhysicsMaterial(0, 0.8f, 0));
    
    auto physicsBody = PhysicsBody::createCircle(button->getContentSize().height / 2, PhysicsMaterial(0, 0.8f, 0.5f));
    
    physicsBody->applyImpulse(Vect( cocos2d::random(-300,300), cocos2d::random(600,1000) ));
    physicsBody->setGravityEnable(true);
    physicsBody->setDynamic(true);
    physicsBody->setTag(1);
        button->setPhysicsBody(physicsBody);
        
        button->setTag(_i);
    _i++;
    
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
        //button->getPhysicsBody()->applyImpulse(Vec2(1000, 200));
        addChild(button);
    
        //physicsBody->applyForce(Vec2(-5, -5));
    //}
}

void postData(){
    
}

void HelloWorld::OnAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event)
{
    CCLOG("%f", acc->z);
    
    
    
    Director::getInstance()->getRunningScene()->getPhysicsWorld()->setGravity(Vec2((acc->x)*200, (acc->y)*200));

    //_label->setString(StringUtils::format("%f",acc->x));
}

void* HelloWorld::createExplotion(cocos2d::Vec2 position){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto s_centre = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
    
    SpriteBatchNode* spritebatch = SpriteBatchNode::create("explosion.png");
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("explosion.plist");
    Sprite* Sprite1 = Sprite::createWithSpriteFrameName("frame-1.png");
    Sprite1->setPosition(position);
    Sprite1->setScale(0.3);
    spritebatch->addChild(Sprite1);
    addChild(spritebatch);
    Vector<SpriteFrame*> animFrames(6);
    char str[100] = {0};
    for(int i = 1; i < 6; i++)
    {
        sprintf(str, "frame-%d.png", i);
        SpriteFrame* frame = cache->getSpriteFrameByName( str );
        //animFrames->addObject(frame);
        animFrames.pushBack(frame);
    }
    Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.05f);
    //auto spri = Sprite::createWithSpriteFrame(animFrames.front());
    //Sprite1->runAction( RepeatForever::create( Animate::create(animation) ) );
    auto sequence = Sequence::create(Animate::create(animation), RemoveSelf::create() , NULL);
    Sprite1->runAction(sequence);
    
    return sequence;

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
    if(_timeLeft > 0){
        _timeLeft--;
        
        if(_timeLeft <= 10){
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(StringUtils::format("sounds/Female/%d.ogg", _timeLeft).c_str());
        }
        
        //_label->setString(std::to_string(_score));
        //StringUtils::format("Tiempo: %d", _timeLeft)
        //StringUtils::format("sounds/Female/%d.ogg", _timeLeft)
        _timeLeftLabel->setString(StringUtils::format("Tiempo: %d", _timeLeft));
    }else{
        MessageBox("Congrats you lose", "Time's up");
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/Female/time_over.ogg");
        this->unschedule(schedule_selector(HelloWorld::updateTimer));
    }
}
