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

Sprite* AtlasLoader::getSpriteByName(string name){
	return Sprite::createWithSpriteFrameName(name);;
}
