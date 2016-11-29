#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#include "ui/CocosGUI.h"

#include <vector>

#include <sstream>
#include <string>
#include <iostream>

USING_NS_CC;

using namespace std;



Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
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
    if ( !Layer::init() )
    {
        return false;
    }
    
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
    
    _score = 60;
    _label = Label::createWithTTF(std::to_string(_score), "fonts/Marker Felt.ttf", 24);
    //_label->setString("No");
    // position the label on the center of the screen
    _label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - _label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(_label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    //this->addChild(sprite, 0);
    
    auto button = cocos2d::ui::Button::create("Button.png", "ButtonYellow.png", "ButtonYellow.png");
    
    button->setTitleText("Salta");
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
                
                _label->setString(std::to_string(_score));
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
    
    return true;
}

void postData(){
    
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
    _score--;
    _label->setString(std::to_string(_score));
}
