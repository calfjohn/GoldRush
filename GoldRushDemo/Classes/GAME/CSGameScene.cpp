#include "CSGameScene.h"
#include "CSGameLayer.h"
#include "CSPlayerControlLayer.h"
#include "CSController.h"

bool CSGameScene::init()
{
	CCLOGINFO("CSGameScene::init()");
	if(!CCScene::init())
		return false;

	CSController::sharedController()->GameStart();

	CSGameLayer *p1 = CSGameLayer::node();
	addChild(p1, ZORDER_DEFAULT, GAMELAYER);

	CSPlayerControlLayer *p2 = CSPlayerControlLayer::node();
	addChild(p2, ZORDER_DEFAULT, CONTROLLAYER);

	return true;
}

void CSGameScene::UpdateScene()
{
	CSGameLayer *p1 = (CSGameLayer *)getChildByTag(GAMELAYER);
	p1->UpdateLayer();

	CSPlayerControlLayer *p2 = (CSPlayerControlLayer *)getChildByTag(CONTROLLAYER);
	p2->UpdateLayer();
}