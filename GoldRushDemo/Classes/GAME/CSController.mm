#define  MIN_MARGIN 10
#include "CSController.h"
#include "../GAME/CSGameScene.h"
#include "../GAME/CSGameLayer.h"
#include "../GAME/CSPlayerControlLayer.h"
#include "../GAME/CSTransactionLayer.h"

CSController* CSController::m_pInstance = NULL;
CSController::CFRelease CSController::Garbo;


string CSController::BundlePath(string sPath)
{
	std::string dPath;
	
	dPath = [[[NSBundle mainBundle] bundlePath] UTF8String];
	dPath = dPath + "/" +sPath;
	return dPath;
	
}

string CSController::TmpPath(string sPath)
{
	std::string dPath;
	
	dPath = [[[NSBundle mainBundle] bundlePath] UTF8String];
	dPath = dPath + "/tmp/" +sPath;
	return dPath;
	
}


bool CSController::init()
{
	return true;
}


bool CSController::GameStart()
{
	//读取关卡初始化数据
	
	//初始化游戏轮次
	m_nGameRound = DataManager::sharedDataManager()->xmlGamegound();
	
	//初始化玩家信息
	int nCount = DataManager::sharedDataManager()->m_mapPlayer.size();
	for (int i = 1; i <= nCount; i++) 
	{		
		USER *pUser = &DataManager::sharedDataManager()->m_mapPlayer[i];
		pUser->bPlay = DataManager::sharedDataManager()->xmlPlayerJoin(pUser->uid);	
		if(!pUser->bPlay)
		{
			DataManager::sharedDataManager()->m_mapPlayer.erase(i);
			continue;
		}
		pUser->bAI = DataManager::sharedDataManager()->xmlPlayerAI(pUser->uid);
		
		pUser->beenz = DataManager::sharedDataManager()->xmlPlayerBeenz(pUser->uid);
		
		pUser->pEmployee->count = DataManager::sharedDataManager()->xmlPlayerEmployeeCount(pUser->uid);

		pUser->bMaster = DataManager::sharedDataManager()->xmlPlayerMaster(pUser->uid);
	}
	
	//初始化船位置、船上货物
	if(DataManager::sharedDataManager()->xmlAutoLoadCargo())
	{
		int nIndex = 1;
		map<int, CARGO>::iterator iterCargo;
		for (iterCargo = DataManager::sharedDataManager()->m_mapCargo.begin(); iterCargo != DataManager::sharedDataManager()->m_mapCargo.end() && nIndex <= 3; iterCargo++)
		{
			SEACRAFT *pSeaCraft = &DataManager::sharedDataManager()->m_mapSeacraft[nIndex];
			pSeaCraft->position = DataManager::sharedDataManager()->xmlSeacraftPos(pSeaCraft->id);
			
			(*iterCargo).second.pStock->bInvest = DataManager::sharedDataManager()->xmlCargoInvest((*iterCargo).first);
			
			if((*iterCargo).second.pStock->bInvest)
			{
				pSeaCraft->pCargo = &(*iterCargo).second;
				nIndex++;
			}
			else
			{
				DecideCargo((*iterCargo).first);
			}
		}
	}
	
	//删除运送的货物
	DataManager::sharedDataManager()->m_mapCargo.erase(DataManager::sharedDataManager()->getNoInvestStockCode());
	
	InitUserIndex();
	
	//初始化所有位置上的船员
	{
		//港口
		map<int, PORTS>::iterator iterPorts;
		for (iterPorts = DataManager::sharedDataManager()->m_mapPorts.begin(); iterPorts != DataManager::sharedDataManager()->m_mapPorts.end(); iterPorts++)
		{
			(*iterPorts).second.spot.uid = DataManager::sharedDataManager()->xmlPortsUserId((*iterPorts).second.id);
		}
		
		//修船厂
		map<int, SHIPYARD>::iterator iterYard;
		for (iterYard = DataManager::sharedDataManager()->m_mapShipyard.begin(); iterYard != DataManager::sharedDataManager()->m_mapShipyard.end(); iterYard++)
		{
			(*iterYard).second.spot.uid = DataManager::sharedDataManager()->xmlShipyardUserId((*iterYard).second.id);
		}
		
		
		//货物
		map<int, CARGO>::iterator iterCargo;
		for (iterCargo = DataManager::sharedDataManager()->m_mapCargo.begin(); iterCargo != DataManager::sharedDataManager()->m_mapCargo.end(); iterCargo++)
		{
			int nIndex = 1;
			vector<GAMESPOT>::iterator iterTemp;
			for(iterTemp = (*iterCargo).second.spotList.begin();  iterTemp != (*iterCargo).second.spotList.end(); ++iterTemp, nIndex++)
			{
				(*iterTemp).uid = DataManager::sharedDataManager()->xmlCargoUserId((*iterCargo).first, nIndex);
			}		
		}
		
		//保险
		DataManager::sharedDataManager()->m_insurance.uid = DataManager::sharedDataManager()->xmlInsuranceUserId();
		
		//领航员
		map<int, NAVIGATOR>::iterator iterNav;
		for (iterNav = DataManager::sharedDataManager()->m_mapNavigator.begin(); iterNav != DataManager::sharedDataManager()->m_mapNavigator.end(); iterNav++)
		{
			(*iterNav).second.spot.uid = DataManager::sharedDataManager()->xmlNavigatorUserId((*iterNav).first);
		}
		
		//海盗
		map<int, GAMESPOT>::iterator iterPri;
		for (iterPri = DataManager::sharedDataManager()->m_mapPirate.begin(); iterPri != DataManager::sharedDataManager()->m_mapPirate.end(); iterPri++)
		{
			(*iterPri).second.uid = DataManager::sharedDataManager()->xmlPirateUserId((*iterPri).first);
		}
	}
	
	DataManager::sharedDataManager()->setCurrentUserId(1);
	
	return true;
}

bool CSController::GameEnd()
{
	ClearAccounts();

	return true;
}

void CSController::NextRound()
{
 	m_nGameRound--;
 	if (m_nGameRound < 0)
 	{
 		return;
 	}

	//扔三个骰子、更新船位置

	int i;
	map<int, SEACRAFT>::iterator itercraft;

	for (i=1, itercraft = DataManager::sharedDataManager()->m_mapSeacraft.begin(); itercraft != DataManager::sharedDataManager()->m_mapSeacraft.end(); itercraft++,i++)
	{
		int nTemp = DiceGo();
		DataManager::sharedDataManager()->m_mapDice[i] = nTemp;
		(*itercraft).second.position +=	nTemp;
	}
}

void CSController::NextTips()
{
	CSGameScene *pScene = (CSGameScene *)CCDirector::sharedDirector()->getRunningScene();
	CSPlayerControlLayer *pPlayerControllerLayer = (CSPlayerControlLayer *)pScene->getChildByTag(CONTROLLAYER);
	pPlayerControllerLayer->NextTips(NULL);
}

void CSController::ToSail()
{
	CSGameScene *pScene = (CSGameScene *)CCDirector::sharedDirector()->getRunningScene();
	CSGameLayer *pGameLayer = (CSGameLayer *)pScene->getChildByTag(GAMELAYER);
	pGameLayer->UpdateLayer();
}

void CSController::SailTo()
{
	if( m_nGameRound == 0)
	{
		GameEnd();
		UpdatePlayerInfo();
	}
	else
	{
		NextTips();
	}
}

void CSController::restartLevel()
{
	DataManager::sharedDataManager()->init();
	CCDirector::sharedDirector()->replaceScene(CSGameScene::node());
}

bool CSController::DecideGamespot(int userId, int nGamespotID)
{
	USER *pUser = DataManager::sharedDataManager()->FindUserById(userId);
	GAMESPOT *pSpot = DataManager::sharedDataManager()->FindGamespot(nGamespotID);
	if (pSpot && pUser) 
	{
		if (pSpot->uid == NO_USER && pUser->pEmployee->count > 0) 
		{
			//保险的特殊性质，先付费给玩家
			if(DataManager::sharedDataManager()->m_insurance.index == nGamespotID)
			{
				CaculatorMoney(pUser, pSpot->sell);
				pSpot->uid = pUser->uid;
				pUser->pEmployee->count--;

				return true;
			}	
			
			if (CaculatorMoney(pUser, -pSpot->buy))
			{
				pSpot->uid = pUser->uid;
				pUser->pEmployee->count--;
				return true;
			}
		}
		else 
		{
			if (pSpot->uid == userId && CaculatorMoney(pUser, pSpot->buy)) 
			{
				pSpot->uid = NO_USER;
				pUser->pEmployee->count++;
				return true;
			}
		}
		
		return true;
	}	

	return false;
}

void CSController::UpdatePlayerInfo()
{
	CSGameScene *pScene = (CSGameScene *)CCDirector::sharedDirector()->getRunningScene();
	CSPlayerControlLayer *pPlayerControllerLayer = (CSPlayerControlLayer *)pScene->getChildByTag(CONTROLLAYER);
	pPlayerControllerLayer->UpdateLayer();
}

bool CSController::CheckMargin()
{
	map<int ,USER>::iterator iter;

	//	检查保证金、冻结保证金、初始化玩家比索
	for (iter = DataManager::sharedDataManager()->m_mapPlayer.begin(); iter != DataManager::sharedDataManager()->m_mapPlayer.end(); iter++)
	{
		if ( (*iter).second.accountBalance  < MIN_MARGIN)
		{
			return false;
		}
		(*iter).second.accountBalance -= MIN_MARGIN;
		(*iter).second.margin = MIN_MARGIN;
		(*iter).second.beenz = 30;
	}

	return true;
}

bool CSController::CaculatorMoney(USER *pUser, int beenz)
{
	if (!pUser || (beenz < 0 && pUser->beenz + beenz < 0) )
	{
		return false;
	}

	pUser->beenz += beenz;
	if (beenz>=0)
	{
		printf("%s增加比索%d,当前比索%d\n", pUser->nickName.c_str(), beenz, pUser->beenz);		
	}
	else
	{
		printf("%s扣除比索%d,当前比索%d\n", pUser->nickName.c_str(), -beenz, pUser->beenz);		
	}

	return true;
}

//竞标港务局长
void CSController::DecideMaster(int id, int nBeenz)
{
	USER *pUser = DataManager::sharedDataManager()->FindUserById(id);
	if (!pUser && !CaculatorMoney(pUser, -nBeenz))
	{
		CCLOGINFO("查无此人,ID号：%d", id);
	}

	pUser->bMaster = true;
}

void CSController::DistributionStock()
{
	//分配玩家股票
	map<int ,USER>::iterator iter;

	for (iter = DataManager::sharedDataManager()->m_mapPlayer.begin(); iter != DataManager::sharedDataManager()->m_mapPlayer.end(); iter++)
	{
		int index = 2;//分配2张股票
		while(index > 0)
		{
			map<int ,STOCK>::iterator iter2;
			float fRandom = CCRANDOM_0_1();		
			if (fRandom < 0.25)
			{
				iter2 = DataManager::sharedDataManager()->m_mapStockMarket.find(1);
			}
			else if (fRandom < 0.50)
			{
				iter2 = DataManager::sharedDataManager()->m_mapStockMarket.find(2);
			}
			else if (fRandom < 0.75)
			{
				iter2 = DataManager::sharedDataManager()->m_mapStockMarket.find(3);
			}
			else
			{
				iter2 = DataManager::sharedDataManager()->m_mapStockMarket.find(4);
			}

			if (iter2->second.count <= 0)
			{
				bool bHasStock = false;//still have stock?
				map<int, STOCK>::iterator iterTemp;
				for(iterTemp = DataManager::sharedDataManager()->m_mapStockMarket.begin(); iterTemp !=  DataManager::sharedDataManager()->m_mapStockMarket.end(); ++iterTemp)
				{
					if((*iterTemp).second.count > 0)
					{
						bHasStock = true;
					}
				}

				if (!bHasStock)
				{
					return;
				}
				
				continue;
			}

			vector<USERSTOCK>::iterator iter3;
			for (iter3 = (*iter).second.stockList.begin(); iter3 != (*iter).second.stockList.end(); iter3++)
			{
//				if (iter3->pStock->code == iter2->second.code)
				{
					iter2->second.count--;
					iter3->count++;
					break;
				}
			}	

			index--;
		}
	}

}

int CSController::DiceGo()
{
	float fRandom = CCRANDOM_0_1();
	if (fRandom < 0.16)
	{
		return 1;
	}
	else if (fRandom < 0.33)
	{
		return 2;
	}
	else if (fRandom < 0.5)
	{
		return 3;
	}
	else if (fRandom < 0.67)
	{
		return 4;
	}
	else if (fRandom < 0.83)
	{
		return 5;
	}
	else
	{
		return 6;
	}

	return 0;
}

//计算玩家的股票
bool CSController::caculatorStock(USER *pUser, int nCode)
{
	map<int, STOCK>::iterator iter;

	iter = DataManager::sharedDataManager()->m_mapStockMarket.find(nCode);
	if (iter != DataManager::sharedDataManager()->m_mapStockMarket.end()
		&& (*iter).second.count >= 0)
	{

		vector<USERSTOCK>::iterator iter3;
		for (iter3 = pUser->stockList.begin(); iter3 != pUser->stockList.end(); iter3++)
		{
//			if (iter3->pStock->code == strCode)
			{
				(*iter).second.count--;
				iter3->count++;
				CaculatorMoney(pUser, -(*iter).second.price);
				return true;
			}
		}
	}

	return false;
}

//登船
bool CSController::BoardSeaCraft(USER *pUser, int id)
{
	SEACRAFT *pSeacraft = DataManager::sharedDataManager()->FindSeaCraft(id);

	if (!pUser || pUser->pEmployee->count <= 0 || !pSeacraft)
	{
		CCLOGINFO("信息错误:没有船员了或者船不存在，不能登船\n");
		return false;
	}	

	vector<GAMESPOT>::iterator iterTemp;
	for(iterTemp = pSeacraft->pCargo->spotList.begin();  iterTemp != pSeacraft->pCargo->spotList.end(); ++iterTemp)
	{
		if ((*iterTemp).uid == NO_USER)
		{
			if (CSController::CaculatorMoney(pUser, -(*iterTemp).buy))
			{
				(*iterTemp).uid  = pUser->pEmployee->id;
				pUser->pEmployee->count--;
				return true;
			}
		}
	}

	return false;
}

//玩家占据游戏位nGameSpotId，扣除对应的费用
bool CSController::CaculatorGamespot(int userID, int nGameSpotId)
{
	USER *pUser = DataManager::sharedDataManager()->FindUserById(userID);
	if (!pUser || nGameSpotId < 0 || nGameSpotId > 'z')
	{
		printf("此位置不存在\n");
		return false;
	}

	GAMESPOT *pGameSpot = NULL;

	//港口
	map<int, PORTS>::iterator iterPorts;
	for (iterPorts = DataManager::sharedDataManager()->m_mapPorts.begin(); iterPorts != DataManager::sharedDataManager()->m_mapPorts.end() && !pGameSpot; iterPorts++)
	{
//		if((*iterPorts).second.spot.code == code)
		{
			if ((*iterPorts).second.spot.uid != NO_USER)
			{
				printf("已经有人占据了\n");
				return false;
			}
			pGameSpot = &(*iterPorts).second.spot;
			break;
		}
	}

	//修船厂
	map<int, SHIPYARD>::iterator iterYard;
	for (iterYard = DataManager::sharedDataManager()->m_mapShipyard.begin(); iterYard != DataManager::sharedDataManager()->m_mapShipyard.end() && !pGameSpot; iterYard++)
	{
//		if((*iterYard).second.spot.code == code)
		{
			if ((*iterYard).second.spot.uid != NO_USER)
			{
				printf("已经有人占据了\n");
				return false;
			}

			pGameSpot = &(*iterYard).second.spot;
			break;
		}
	}


	//货物
	map<int, CARGO>::iterator iterCargo;
	for (iterCargo = DataManager::sharedDataManager()->m_mapCargo.begin(); iterCargo != DataManager::sharedDataManager()->m_mapCargo.end() && !pGameSpot; iterCargo++)
	{
		vector<GAMESPOT>::iterator iterTemp;
		for(iterTemp = (*iterCargo).second.spotList.begin();  iterTemp != (*iterCargo).second.spotList.end(); ++iterTemp)
		{
//			if((*iterTemp).code == code)
			{
//				if ((*iterCargo).second.pStock->code == DataManager::sharedDataManager()->m_strNoUseStockCode)
				{
					printf("此货物没有运输，不能部署\n");
					return false;
				}

				if ((*iterTemp).uid != NO_USER)
				{
					printf("已经有人占据了\n");
					return false;
				}

				pGameSpot = &(*iterTemp);
				break;
			}			
		}	
	}

	//保险
//	if(DataManager::sharedDataManager()->m_insurance.code == code && !pGameSpot)
	{
		if (DataManager::sharedDataManager()->m_insurance.uid != NO_USER)
		{
			printf("已经有人占据了\n");
			return false;
		}

		pGameSpot = &DataManager::sharedDataManager()->m_insurance;
		//保险的特殊性质，先付费给玩家
		CaculatorMoney(pUser, pGameSpot->sell);
	}

	//领航员
	map<int, NAVIGATOR>::iterator iterNav;
	for (iterNav = DataManager::sharedDataManager()->m_mapNavigator.begin(); iterNav != DataManager::sharedDataManager()->m_mapNavigator.end() && !pGameSpot; iterNav++)
	{
//		if((*iterNav).second.spot.code == code)
		{
			if ((*iterNav).second.spot.uid != NO_USER)
			{
				printf("已经有人占据了\n");
				return false;
			}

			pGameSpot = &(*iterNav).second.spot;
			break;
		}
	}

	//海盗
	map<int, GAMESPOT>::iterator iterPri;
	for (iterPri = DataManager::sharedDataManager()->m_mapPirate.begin(); iterPri != DataManager::sharedDataManager()->m_mapPirate.end() && !pGameSpot; iterPri++)
	{
//		if((*iterPri).second.code == code)
		{
			if ((*iterPri).second.uid != NO_USER)
			{
				printf("已经有人占据了\n");
				return false;
			}

			pGameSpot = &(*iterPri).second;
			break;
		}
	}

	if (pGameSpot && CaculatorMoney(pUser, -pGameSpot->buy))
	{
		pGameSpot->uid = pUser->pEmployee->id;

		return true;
	}

	if (!pGameSpot)
	{
		printf("此位置不存在\n");
	}
	else
	{
		printf("钱不够啊\n");
	}

	return false;
}

//决定不运上船的货物
bool CSController::DecideCargo(int nCode)
{
	map<int, STOCK>::iterator iter;

	iter = DataManager::sharedDataManager()->m_mapStockMarket.find(nCode);
	if (iter != DataManager::sharedDataManager()->m_mapStockMarket.end())
	{
		DataManager::sharedDataManager()->setNoInvestStockCode(nCode);
		return true;
	}

	return false;
}

//决定船运输的货物
bool CSController::DecideSeaCraft(SEACRAFT *pSeacraft, int nCode)
{
	if (DataManager::sharedDataManager()->getNoInvestStockCode() == nCode)
	{
		printf("	此货物不能运输\n");
		return false;
	}

	map<int, CARGO>::iterator iter;
	for (iter = DataManager::sharedDataManager()->m_mapCargo.begin(); iter != DataManager::sharedDataManager()->m_mapCargo.end(); iter++)
	{
//		if ( (*iter).second.pStock->code == strCode)
		{
			if ((*iter).second.pStock->bInvest)
			{
				printf("	此货物已运输\n");
				return false;
			}

			pSeacraft->pCargo = &(*iter).second;
			pSeacraft->pCargo->pStock->bInvest = true;
			return true;
		}
	}

	printf("	请输入正确的货物代码\n");
	return false;
}


void CSController::InitUserIndex()
{
	map<int ,USER>::iterator iter;
	for (iter = DataManager::sharedDataManager()->m_mapPlayer.begin(); iter != DataManager::sharedDataManager()->m_mapPlayer.end(); iter++)
	{
		(*iter).second.nIndex = m_nGameRound;
	}
}

void CSController::MasterExcute(USER *pUser)
{
	if (!pUser)
	{
		return;
	}

	printf("%s执行船长权利\n", pUser->nickName.c_str());

	//买股票
	string strCode;
	char chCode[FILENAME_MAX];
	do 
	{
		printf("请选择你要购买的股票(输入0放弃)：");
		char code;
		printf("\n");
		sprintf(chCode, "%c", code);
//		if (code == '0' || caculatorStock(pUser, chCode))
		{
			break;
		}
	} while (true);

	//选择运输的货物
	map<STOCK *, CARGO>::iterator iterCargo;
	do 
	{
		printf("请选择不运上船的货物：");
		char code;
		sprintf(chCode, "%c", code);
//		if (DecideCargo(chCode))
		{
			printf("\n");
			break;
		}
	} while (true);

	//船的起始位置
	map<int, SEACRAFT>::iterator itercraft;
	for (itercraft = DataManager::sharedDataManager()->m_mapSeacraft.begin(); itercraft != DataManager::sharedDataManager()->m_mapSeacraft.end(); itercraft++)
	{
		do 
		{
			printf("请选择%d号船的货物：", (*itercraft).second.id);
			char code;
			sprintf(chCode, "%c", code);

//			DataManager::sharedDataManager()->m_mapStockMarket.find(chCode);
//			if (DecideSeaCraft(&(*itercraft).second, chCode))
			{
				printf("\n");
				break;
			}
		} while (true);

		do 
		{
			printf("请选择%d号船的位置【0-5】：", (*itercraft).second.id);
			char code;

			int nTemp = code - '0';
			if (nTemp >= 0 && nTemp <= 5)
			{
				(*itercraft).second.position = nTemp;
				printf("\n");
				break;
			}

			printf("	请输入正确的船位置\n");
		} while (true);
	}
}

//领航员执行权力
void CSController::NavigatorExcute()
{
	map<int, NAVIGATOR>::iterator iterNav;
	for (iterNav = DataManager::sharedDataManager()->m_mapNavigator.begin(); iterNav != DataManager::sharedDataManager()->m_mapNavigator.end(); iterNav++)
	{
		if ((*iterNav).second.spot.uid != NO_USER)
		{
			do 
			{
				USER *pUser = DataManager::sharedDataManager()->FindUserByEmployee((*iterNav).second.spot.uid);
				if (!pUser)
				{
					break;
				}

				printf("%s请选择领航的船号：", pUser->nickName.c_str());
				char code;
				int nTemp = code - '0';
				if (nTemp < 1 || nTemp > 3)
				{
					printf("	请输入正确的船号\n");
					continue;
				}

				map<int, SEACRAFT>::iterator itercraft;
				itercraft = DataManager::sharedDataManager()->m_mapSeacraft.find(nTemp);
				if (itercraft == DataManager::sharedDataManager()->m_mapSeacraft.end())
				{
					printf("	请输入正确的船号\n");
					continue;
				}

				do 
				{
					printf("\n请选择前景还是后退（前进1 后退0）：");

					nTemp = code - '0';				
					if (nTemp < 0 || nTemp > 1)
					{
						printf("	输入错误\n");
						continue;
					}

					if (nTemp == 1)
					{
						(*itercraft).second.position += (*iterNav).second.step;
						printf("\n%d号船前进%d步，当前位置%d\n", (*itercraft).first, (*iterNav).second.step, (*itercraft).second.position);
					}
					else
					{
						(*itercraft).second.position -= (*iterNav).second.step;
						printf("\n%d号船后退%d步，当前位置%d\n", (*itercraft).first, (*iterNav).second.step, (*itercraft).second.position);
					}				
					break;
				} while (true);

				break;
			} while (true);
		}
	}
}

//清除运输此货物的玩家（赶下船）
void CSController::ClearCargoPlayer(CARGO *pCargo)
{
	if (!pCargo)
	{
		return;
	}

	vector<GAMESPOT>::iterator iter;
	for(iter = pCargo->spotList.begin(); iter != pCargo->spotList.end(); iter++)
	{
		(*iter).uid = NO_USER;
	}
}

//清算海盗收益
bool CSController::CalulatorPirate(CARGO *pCargo)
{
	//海盗
	int nTemp = 0;//计算海盗个数
	map<int, GAMESPOT>::iterator iterPri;
	for (iterPri = DataManager::sharedDataManager()->m_mapPirate.begin(); iterPri != DataManager::sharedDataManager()->m_mapPirate.end(); iterPri++)
	{
		if ((*iterPri).second.uid != NO_USER)
		{
			nTemp++;
		}
	}

	if (nTemp == 0)
	{
		return false;
	}

	for (iterPri = DataManager::sharedDataManager()->m_mapPirate.begin(); iterPri != DataManager::sharedDataManager()->m_mapPirate.end(); iterPri++)
	{
		CaculatorMoney(DataManager::sharedDataManager()->FindUserByEmployee((*iterPri).second.uid), pCargo->pStock->value/nTemp);
	}

	return true;
}

//清算船上玩家收益
void CSController::CalulatorSeacraft(CARGO *pCargo)
{
	if (!pCargo)
	{
		return;
	}

	vector<GAMESPOT>::iterator iter;
	int nTemp = 0;//统计多少玩家在船上
	for(iter = pCargo->spotList.begin(); iter != pCargo->spotList.end(); iter++)
	{
		if ((*iter).uid != NO_USER)
		{
			nTemp++;
		}
	}

	if (nTemp == 0)
	{
		return;
	}

	for(iter = pCargo->spotList.begin(); iter != pCargo->spotList.end(); iter++)
	{
		CaculatorMoney(DataManager::sharedDataManager()->FindUserByEmployee((*iter).uid), pCargo->pStock->value/nTemp);
	}
}

//船到港
void CSController::SeacraftToPorts(SEACRAFT *pSeacraft)
{
	map<int, PORTS>::iterator iterPorts;
	for (iterPorts = DataManager::sharedDataManager()->m_mapPorts.begin(); iterPorts != DataManager::sharedDataManager()->m_mapPorts.end(); iterPorts++)
	{
		if((*iterPorts).second.pSeacraft)
		{
			continue;
		}

		(*iterPorts).second.pSeacraft = pSeacraft;
		break;
	}
}

//船到修船厂
void CSController::SeacraftToShipyard(SEACRAFT *pSeacraft)
{
	map<int, SHIPYARD>::iterator iterYard;
	for (iterYard = DataManager::sharedDataManager()->m_mapShipyard.begin(); iterYard != DataManager::sharedDataManager()->m_mapShipyard.end(); iterYard++)
	{
		if((*iterYard).second.pSeacraft)
		{
			continue;
		}

		(*iterYard).second.pSeacraft = pSeacraft;
		break;
	}
}

void CSController::CaculatorStock(CARGO *pCargo)
{
	if (!pCargo)
	{
		return;
	}

	pCargo->pStock->price = 10;
}

//计算港口玩家收入
void CSController::CalulatorPorts()
{
	map<int, PORTS>::iterator iterPorts;
	for (iterPorts = DataManager::sharedDataManager()->m_mapPorts.begin(); iterPorts != DataManager::sharedDataManager()->m_mapPorts.end(); iterPorts++)
	{
		if ((*iterPorts).second.pSeacraft)
		{
			CaculatorMoney(DataManager::sharedDataManager()->FindUserByEmployee((*iterPorts).second.spot.uid), (*iterPorts).second.spot.sell); 
		}

	}
}

//计算修船厂玩家收入
void CSController::CalulatorShipyard()
{
	int nTemp=0;//统计修船厂赔给玩家的钱
	//清算保险玩家收益
	map<int, SHIPYARD>::iterator iterYard;
	for (iterYard = DataManager::sharedDataManager()->m_mapShipyard.begin(); iterYard != DataManager::sharedDataManager()->m_mapShipyard.end(); iterYard++)
	{
		if ((*iterYard).second.pSeacraft)
		{
			nTemp += (*iterYard).second.spot.sell;//不到港的船，都要赔钱，赔钱给玩家或者银行
			CaculatorMoney(DataManager::sharedDataManager()->FindUserByEmployee((*iterYard).second.spot.uid), (*iterYard).second.spot.sell); 
		}
	}

	//计算保险玩家的收益
	CaculatorMoney(DataManager::sharedDataManager()->FindUserByEmployee(DataManager::sharedDataManager()->m_insurance.uid), -nTemp); 
}


//清算到港、修船厂的船以及船上玩家的收益
void CSController::ClearSeaCraft()
{
	map<int, SEACRAFT>::iterator itercraft;
	for (itercraft = DataManager::sharedDataManager()->m_mapSeacraft.begin(); itercraft != DataManager::sharedDataManager()->m_mapSeacraft.end(); itercraft++)
	{	
		if( (*itercraft).second.position > 16)
		{
			//清算船上玩家收益
			CalulatorSeacraft((*itercraft).second.pCargo);

			//船到港
			SeacraftToPorts(&(*itercraft).second);

			//清算股市股票价格
			CaculatorStock((*itercraft).second.pCargo);
		}
		else
		{
			//船到修船厂
			SeacraftToShipyard(&(*itercraft).second);		
		}
	}
}

void CSController::ClearAccounts()
{
	//TODO 海盗选择上哪一艘船，用AI计算
/*	map<int, SEACRAFT>::iterator itercraft;
	for (itercraft = DataManager::sharedDataManager()->m_mapSeacraft.begin(); itercraft != DataManager::sharedDataManager()->m_mapSeacraft.end(); itercraft++)
	{
		CARGO *pCargo = (*itercraft).second.pCargo;
		if( (*itercraft).second.position != 13)
		{
			continue;
		}

		printf("海盗执行权力\n");

		//清理船上玩家
		ClearCargoPlayer(pCargo);

		//清算海盗收益
		if(!CalulatorPirate(pCargo))
		{
			continue;
		}

		//海盗决定船的到港与否
		char code;
		do 
		{
			printf("请选择%d号船到港还是去修船厂(到港1 去修船厂0)：", (*itercraft).first);
			if (code == '0' || code == '1')
			{
				break;
			}
			printf("	输入不正确\n");
		} while (true);

		if (code == '1')
		{
			(*itercraft).second.position++;
		}
		else
		{
			(*itercraft).second.position--;
		}
	}
	*/
	//清算到港、修船厂的船以及船上玩家的收益
	ClearSeaCraft();

	//清算港口玩家收益
	CalulatorPorts();

	//清算修船厂玩家收益
	CalulatorShipyard();
}

void CSController::transaction()
{
	if(CCDirector::sharedDirector()->getRunningScene())
	{
		CSTransactionLayer *pLayer = CSTransactionLayer::node();
	
		CCDirector::sharedDirector()->getRunningScene()->addChild(pLayer, ZORDER_HIGHEST);
	}
}