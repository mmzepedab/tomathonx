#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "network/HttpClient.h"
#include "PluginFacebook/PluginFacebook.h"


class HelloWorld : public cocos2d::LayerColor, sdkbox::FacebookListener
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    void onHttpRequestCompleted( cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response );
    void onRequestImgCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void updateTimer(float dt);
    
    void addBackground();
    void addHUD();
    
    void spawnEnemies(float dt);
    void createExplotion(cocos2d::Vec2 position);
    
    void OnAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event);
    
    void applyImpulse(float dt);
    
    
protected:
    
private:
    //Facebook callback
    void onLogin(bool isLogin, const std::string& msg);
    void onSharedSuccess(const std::string& message);
    void onSharedFailed(const std::string& message);
    void onSharedCancel();
    void onAPI(const std::string& key, const std::string& jsonData);
    void onPermission(bool isLogin, const std::string& msg);
    void onFetchFriends(bool ok, const std::string& msg);
    void onRequestInvitableFriends( const sdkbox::FBInvitableFriendsInfo& friends );
    void onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg );
    void onInviteFriendsResult( bool result, const std::string& msg );
    void onGetUserInfo( const sdkbox::FBGraphUser& userInfo );
    
    
    
};





#endif // __HELLOWORLD_SCENE_H__
