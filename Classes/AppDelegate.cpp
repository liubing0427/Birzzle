#include "AppDelegate.h"
#include "LoadingScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
	srand(time(0)+rand()); 
    // initialize director
    auto director = Director::getInstance();
    auto eglView = EGLView::getInstance();

	director->setOpenGLView(eglView);
	eglView->setDesignResolutionSize(640,960, ResolutionPolicy::SHOW_ALL);

	// set the resource directory
	this->setResourceSearchResolution();

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = LoadingScene::create();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void AppDelegate::setResourceSearchResolution()
{
	std::vector<std::string> paths;
	paths.push_back("fonts");
	paths.push_back("image");
	paths.push_back("sounds");
	FileUtils::getInstance()->setSearchResolutionsOrder(paths);
}