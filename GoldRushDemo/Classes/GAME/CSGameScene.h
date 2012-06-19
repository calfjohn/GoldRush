#ifndef __CSGAMESCENE_H__
#define __CSGAMESCENE_H__

#include "data.h"

class CSGameScene:public CCScene
{
public:
	CSGameScene(){};
	~CSGameScene(){};
	LAYER_NODE_FUNC(CSGameScene);
	bool init();

	void UpdateScene();
};

#endif  // __CSGAMESCENE_H__