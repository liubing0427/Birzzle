#include "AtlasLoader.h"


AtlasLoader* AtlasLoader::sharedAtlasLoader = nullptr;

AtlasLoader* AtlasLoader::getInstance(){
	if(sharedAtlasLoader == NULL) {
		sharedAtlasLoader = new AtlasLoader();
		if(!sharedAtlasLoader->init()){
			delete sharedAtlasLoader;
			sharedAtlasLoader = NULL;
			CCLOG("ERROR: Could not init sharedAtlasLoader");
		}
	}
	return sharedAtlasLoader;
}


void AtlasLoader::destroyInstance()
{
	CC_SAFE_DELETE(sharedAtlasLoader);
}

AtlasLoader::AtlasLoader(){}


bool AtlasLoader::init(){
	return true;
}

void AtlasLoader::loadAtlas(string filename){

	SpriteFrameCache* frame = SpriteFrameCache::sharedSpriteFrameCache();
	frame->addSpriteFramesWithFile(filename);
}

Sprite* AtlasLoader::getSpriteByName(string name, bool isscale){
	auto sprite = Sprite::create(name);
	if(isscale)
	{
		sprite->setScaleY(1136.0f/960);
		//sprite->setScaleX(0.5f);
	}
	return sprite;
}
