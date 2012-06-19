
#include <fstream>
#include "DataManager.h"
#include "CSController.h"

#include "platform/third_party/win32/iconv/iconv.h"


DataManager* DataManager::m_pInstance = NULL;
DataManager::CFRelease DataManager::Garbo;

int DataManager::GBKToUTF8(std::string & gbkStr)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	iconv_t iconvH;

	iconvH = iconv_open("utf-8", "gb2312");

	if (iconvH == 0)
	{
		return -1;
	}

	const char* strChar = gbkStr.c_str();
	const char** pin = &strChar;
	size_t strLength = gbkStr.length();
	char* outbuf = (char*) malloc(strLength*4);
	char* pBuff = outbuf;

	memset( outbuf, 0, strLength*4);
	size_t outLength = strLength*4;

	if (-1 == iconv(iconvH, pin, &strLength, &outbuf, &outLength))
	{
		iconv_close(iconvH);
		return -1;
	}

	gbkStr = pBuff;
	iconv_close(iconvH);

#endif

	return 0;
}

DataManager::DataManager() 
{
	m_nIndex = 1;
	m_nCurrentUserId = NO_USER;
	m_nLevel = 1;
}

bool DataManager::init()
{
	srand (time(NULL) );

	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("GoldRush.plist", "GoldRush.png");
	
	InitPosition();
	InitRulerPosition();
	initStockMarket();
	initCargo();
	initSeaCraft();
	initPorts();
	initNavigator();
	initInsurance();
	initPirate();
	initShipyard();
	initEmployee();
	initDice();
	initPlayer();

	initGameSpot();
	initUserDefault(m_nLevel);

	return true;
}

bool DataManager::initDice()
{
	m_mapDice[1] = CSController::DiceGo();
	m_mapDice[2] = CSController::DiceGo();
	m_mapDice[3] = CSController::DiceGo();
	
	return true;
}

bool DataManager::InitPosition()
{
	ifstream is;
	Json::Reader reader;
	Json::Value json_root;
	is.open (CSController::BundlePath("gameSpot.cfg").c_str(), ios::binary );
	if(is.fail() || !reader.parse(is, json_root) || json_root.getMemberNames().size() == 0)
	{
		CCLOG("gameSpot config file cannot be opened\n");
		is.close();
		return false;
	}
	is.close();

	m_mapPosition.clear();
	map<int, CCPoint>::iterator iterTemp;
	Json::Value::Members member = json_root.getMemberNames();
	for(int index = 0; index < (int)member.size(); index++)
	{	
		POSITION posTemp;
		posTemp.x = (float)json_root[member[index]]["X"].asDouble();
		posTemp.y = (float)json_root[member[index]]["Y"].asDouble();
		m_mapPosition[atoi(member[index].c_str())] = posTemp;
	}	

	return true;
}

bool DataManager::InitRulerPosition()
{
	ifstream is;
	Json::Reader reader;
	Json::Value json_root;
	is.open (CSController::BundlePath("rulerSpot.cfg").c_str(), ios::binary );
	if(is.fail() || !reader.parse(is, json_root) || json_root.getMemberNames().size() == 0)
	{
		CCLOG("gameSpot config file cannot be opened\n");
		is.close();
		return false;
	}
	is.close();

	m_mapRulerPosition.clear();
	map<int, CCPoint>::iterator iterTemp;
	Json::Value::Members member = json_root.getMemberNames();
	for(int index = 0; index < (int)member.size(); index++)
	{	
		POSITION posTemp;
		posTemp.x = (float)json_root[member[index]]["X"].asDouble();
		posTemp.y = (float)json_root[member[index]]["Y"].asDouble();
		m_mapRulerPosition[atoi(member[index].c_str())] = posTemp;
	}	

	return true;
}

POSITION *DataManager::FindPostion(int nPos)
{
	map<int, POSITION>::iterator iter;
	iter = m_mapPosition.find(nPos);
	
	if (iter != m_mapPosition.end())
	{
		return &(*iter).second;
	}

	return NULL;
}

POSITION *DataManager::FindRulerPostion(int nPos)
{
	map<int, POSITION>::iterator iter;
	iter = m_mapRulerPosition.find(nPos);

	if (iter != m_mapRulerPosition.end())
	{
		return &(*iter).second;
	}

	return NULL;
}

bool DataManager::initUserDefault(int level)
{
	CCLOG("********************** init value ***********************");
	m_nLevel = level;

	if(CCUserDefault::openXMLFile(CSController::BundlePath("UserDefault.xml")))
	{
		CCUserDefault::sharedUserDefault();
	}

	
/*	char chLevel[32];
	sprintf(chLevel, "Level%d", level);
	
	 
	// set default value
	CCUserDefault::sharedUserDefault()->setBoolForKey("PortsVisible1", true, chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("PortsEnable1", true, chLevel);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("PortsUserID1", NO_USER, chLevel);

	CCUserDefault::sharedUserDefault()->setBoolForKey("PortsVisible2", true, chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("PortsEnable2", true, chLevel);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("PortsUserID2", NO_USER, chLevel);

	CCUserDefault::sharedUserDefault()->setBoolForKey("PortsVisible3", true, chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("PortsEnable3", true, chLevel);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("PortsUserID3", NO_USER, chLevel);

	CCUserDefault::sharedUserDefault()->setBoolForKey("ShipyardVisible1", true, chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("ShipyardEnable1", true, chLevel);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("ShipyardsUserID1", NO_USER, chLevel);
	
	CCUserDefault::sharedUserDefault()->setBoolForKey("ShipyardEnable2", true, chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("ShipyardVisible2", true, chLevel);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("ShipyardsUserID2", NO_USER, chLevel);
	
	CCUserDefault::sharedUserDefault()->setBoolForKey("ShipyardEnable3", true, chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("ShipyardVisible3", true, chLevel);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("ShipyardsUserID3", NO_USER, chLevel);

	CCUserDefault::sharedUserDefault()->setBoolForKey("PirateVisible1", true, chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("PirateEnable1", true, chLevel);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("PirateUserID1", NO_USER, chLevel);

	CCUserDefault::sharedUserDefault()->setBoolForKey("PirateVisible2", true, chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("PirateEnable2", true, chLevel);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("PirateUserID2", NO_USER, chLevel);

	CCUserDefault::sharedUserDefault()->setBoolForKey("NavigatorVisible1", true, chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("NavigatorEnable1", true, chLevel);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("NavigatorUserID1", NO_USER, chLevel);

	CCUserDefault::sharedUserDefault()->setBoolForKey("NavigatorVisible2", true, chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("NavigatorEnable2", true, chLevel);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("NavigatorUserID2", NO_USER, chLevel);
	
	CCUserDefault::sharedUserDefault()->setBoolForKey("InsuranceVisible", true, chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("InsuranceEnable", true, chLevel);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("InsuranceUserID", NO_USER, chLevel);
	
	CCUserDefault::sharedUserDefault()->setBoolForKey("StockMarketVisible", true, chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("StockMarketEnable", true, chLevel);

	CCUserDefault::sharedUserDefault()->setBoolForKey("PlayerJoin1", true, chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("PlayerAI1", false, chLevel);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("PlayerBeenz1", 5, chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("IsMaster1", true, chLevel);

	CCUserDefault::sharedUserDefault()->setBoolForKey("PlayerJoin2", false, chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("PlayerAI2", true, chLevel);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("PlayerBeenz2", 30, chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("IsMaster2", false, chLevel);

	CCUserDefault::sharedUserDefault()->setBoolForKey("PlayerJoin3", false, chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("PlayerAI3", true, chLevel);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("PlayerBeenz3", 30, chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("IsMaster3", false, chLevel);

	CCUserDefault::sharedUserDefault()->setBoolForKey("PlayerJoin4", false, chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("PlayerAI4", true, chLevel);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("PlayerBeenz4", 30, chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("IsMaster4", false, chLevel);
	
	CCUserDefault::sharedUserDefault()->setBoolForKey("PlayerJoin5", false, chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("PlayerAI5", true, chLevel);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("PlayerBeenz5", 30, chLevel);	

	CCUserDefault::sharedUserDefault()->setIntegerForKey("SeacraftPosition1", 11, chLevel);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("SeacraftPosition2", 12, chLevel);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("SeacraftPosition3", 13, chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("AutoLoadCargo", true, chLevel);
	
	CCUserDefault::sharedUserDefault()->setStringForKey("CargoUserID1", "0000", chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("CargoInvest1", true, chLevel);
	
	CCUserDefault::sharedUserDefault()->setStringForKey("CargoUserID2", "0000", chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("CargoInvest2", true, chLevel);
	
	CCUserDefault::sharedUserDefault()->setStringForKey("CargoUserID3", "0000", chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("CargoInvest3", true, chLevel);

	CCUserDefault::sharedUserDefault()->setStringForKey("CargoUserID4", "0000", chLevel);
	CCUserDefault::sharedUserDefault()->setBoolForKey("CargoInvest4", false, chLevel);

	CCUserDefault::sharedUserDefault()->setIntegerForKey("GameRound", 1, chLevel);
*/	 
	return true;
}

bool DataManager::initStockMarket()
{
	ifstream is;
	Json::Reader reader;
	Json::Value json_root;
	is.open (CSController::BundlePath("stock.cfg").c_str(), ios::binary );
	if(is.fail() || !reader.parse(is, json_root) || json_root.getMemberNames().size() == 0)
	{
		CCLOG("stock config file cannot be opened\n");
		is.close();
		return false;
	}
	is.close();


	m_mapStockMarket.clear();

	STOCK tempStock;
	Json::Value::Members member = json_root.getMemberNames();
	for(int index = 0; index < (int)member.size(); index++)
	{	
		tempStock.code=json_root[member[index]]["code"].asInt();
		tempStock.name=json_root[member[index]]["name"].asString();
		tempStock.count=json_root[member[index]]["count"].asInt();
		tempStock.price=json_root[member[index]]["price"].asInt();
		tempStock.value=json_root[member[index]]["value"].asInt();
		tempStock.bInvest = true;
		tempStock.position = ccp(264+index*16, 0);
		tempStock.stockFile = json_root[member[index]]["stockFile"].asString();
		tempStock.seaCraftName = json_root[member[index]]["seaCraftName"].asString();
		tempStock.seaCraftFile = json_root[member[index]]["seaCraftFile"].asString();
		tempStock.seaCraftRulerFile = json_root[member[index]]["seaCraftRulerFile"].asString();
		
		m_mapStockMarket[tempStock.code]=tempStock;
	}	

	m_nNoInvestStockCode = 0;
	return true;
}

bool DataManager::initCargo()
{
	ifstream is;
	Json::Reader reader;
	Json::Value json_root;
	is.open (CSController::BundlePath("cargo.cfg").c_str(), ios::binary );
	if(is.fail() || !reader.parse(is, json_root) || json_root.getMemberNames().size() == 0)
	{
		CCLOG("cargo config file cannot be opened\n");
		is.close();
		return false;
	}
	is.close();

	m_mapCargo.clear();
	map<int, STOCK>::iterator iterTemp;
	for(iterTemp = m_mapStockMarket.begin(); iterTemp !=  m_mapStockMarket.end(); ++iterTemp)
	{
		CARGO tempCargo;
		tempCargo.pStock = &(*iterTemp).second;

		Json::Value tempValue = json_root[ConvertToString((*iterTemp).first)];
		Json::Value::Members member = tempValue.getMemberNames();
		for(int index = 0; index < (int)member.size(); index++)
		{	
			GAMESPOT gameSpot;
			gameSpot.index = m_nIndex++;
			gameSpot.uid = NO_USER;
			gameSpot.buy = tempValue[member[index]]["buy"].asInt();
			gameSpot.sell = tempValue[member[index]]["sell"].asInt();
			gameSpot.position = CCPointZero;
			tempCargo.spotList.push_back(gameSpot);
		}

		m_mapCargo[(*iterTemp).first] = tempCargo;
	}

	return true;
}

bool DataManager::initSeaCraft()
{
	m_mapSeacraft.clear();

	SEACRAFT tempSeacraft;
	tempSeacraft.id = 1;
	tempSeacraft.position = 0;
	tempSeacraft.pCargo = NULL;
	m_mapSeacraft[tempSeacraft.id] = tempSeacraft;
	
	tempSeacraft.id = 2;
	m_mapSeacraft[tempSeacraft.id] = tempSeacraft;
	
	tempSeacraft.id = 3;
	m_mapSeacraft[tempSeacraft.id] = tempSeacraft;
	
	return true;
}

bool DataManager::initNavigator()
{
	m_mapNavigator.clear();

	NAVIGATOR tempNavigator;
	tempNavigator.step = 1;
	tempNavigator.spot.index = m_nIndex++;
	tempNavigator.spot.buy = 2;
	tempNavigator.spot.sell = 0;
	tempNavigator.spot.uid = NO_USER;
	tempNavigator.spot.position = ccp(8, 232);
	m_mapNavigator[1] = tempNavigator;

	tempNavigator.step = 2;
	tempNavigator.spot.index = m_nIndex++;
	tempNavigator.spot.buy = 5;
	tempNavigator.spot.sell = 0;
	tempNavigator.spot.uid = NO_USER;
	tempNavigator.spot.position = ccp(8, 198);
	m_mapNavigator[2] = tempNavigator;

	return true;
}

bool DataManager::initInsurance()
{
	m_insurance.index = m_nIndex++;
	m_insurance.uid = NO_USER;
	m_insurance.buy = 0;
	m_insurance.sell = 10;
	m_insurance.position = ccp(282, 280);
	return true;
}

bool DataManager::initPirate()
{
	m_mapPirate.clear();

	GAMESPOT tempGamespot;
	tempGamespot.index = m_nIndex++;
	tempGamespot.uid = NO_USER;
	tempGamespot.buy = 5;
	tempGamespot.position = ccp(24, 344);
	tempGamespot.sell = 0;
	m_mapPirate[1] = tempGamespot;

	tempGamespot.index = m_nIndex++;
	tempGamespot.position = ccp(8, 344);
	m_mapPirate[2] = tempGamespot;

	return true;
}

bool DataManager::initPorts()
{
	m_mapPorts.clear();

	PORTS tempPorts;
	tempPorts.id = 1;
	tempPorts.pSeacraft = NULL;
	tempPorts.spot.index = m_nIndex++;
	tempPorts.spot.buy = 4;
	tempPorts.spot.sell = 6;
	tempPorts.spot.uid = NO_USER;
	tempPorts.spot.position = ccp(24, 422);
	m_mapPorts[tempPorts.id] = tempPorts;

	tempPorts.id = 2;
	tempPorts.pSeacraft = NULL;
	tempPorts.spot.index = m_nIndex++;
	tempPorts.spot.buy = 3;
	tempPorts.spot.sell = 8;
	tempPorts.spot.uid = NO_USER;
	tempPorts.spot.position = ccp(24, 438);
	m_mapPorts[tempPorts.id] = tempPorts;

	tempPorts.id = 3;
	tempPorts.pSeacraft = NULL;
	tempPorts.spot.index = m_nIndex++;
	tempPorts.spot.buy = 2;
	tempPorts.spot.sell = 15;
	tempPorts.spot.uid = NO_USER;
	tempPorts.spot.position = ccp(24, 456);
	m_mapPorts[tempPorts.id] = tempPorts;

	return true;
}

bool DataManager::initShipyard()
{
	m_mapShipyard.clear();

	SHIPYARD tempShipyard;
	tempShipyard.id = 1;
	tempShipyard.pSeacraft = NULL;
	tempShipyard.spot.index = m_nIndex++;
	tempShipyard.spot.buy = 4;
	tempShipyard.spot.sell = 6;
	tempShipyard.spot.uid = NO_USER;
	tempShipyard.spot.position = ccp(296, 422);
	m_mapShipyard[tempShipyard.id] = tempShipyard;

	tempShipyard.id = 2;
	tempShipyard.pSeacraft = NULL;
	tempShipyard.spot.index = m_nIndex++;
	tempShipyard.spot.buy = 3;
	tempShipyard.spot.sell = 8;
	tempShipyard.spot.uid = NO_USER;
	tempShipyard.spot.position = ccp(296, 438);
	m_mapShipyard[tempShipyard.id] = tempShipyard;

	tempShipyard.id = 3;
	tempShipyard.pSeacraft = NULL;
	tempShipyard.spot.index = m_nIndex++;
	tempShipyard.spot.buy = 2;
	tempShipyard.spot.sell = 15;
	tempShipyard.spot.uid = NO_USER;
	tempShipyard.spot.position = ccp(296, 456);
	m_mapShipyard[tempShipyard.id] = tempShipyard;

	return true;
}

bool DataManager::initEmployee()
{
	m_mapEmployee.clear();

	EMPLOYEE tempEmployee;
	tempEmployee.id = 1;
	tempEmployee.count = 4;
	tempEmployee.employeeFile = "sailor1.png";
	tempEmployee.employeeFile_s = "sailor1s.png";

	m_mapEmployee[tempEmployee.id] = tempEmployee;

	tempEmployee.id = 2;
	tempEmployee.count = 4;
	tempEmployee.employeeFile = "sailor2.png";
	tempEmployee.employeeFile_s = "sailor2s.png";
	m_mapEmployee[tempEmployee.id] = tempEmployee;

	tempEmployee.id = 3;
	tempEmployee.count = 4;
	tempEmployee.employeeFile = "sailor3.png";
	tempEmployee.employeeFile_s = "sailor3s.png";
	m_mapEmployee[tempEmployee.id] = tempEmployee;

	tempEmployee.id = 4;
	tempEmployee.count = 4;
	tempEmployee.employeeFile = "sailor4.png";
	tempEmployee.employeeFile_s = "sailor4s.png";
	m_mapEmployee[tempEmployee.id] = tempEmployee;
	
	tempEmployee.id = 5;
	tempEmployee.count = 4;
	tempEmployee.employeeFile = "sailor4.png";
	tempEmployee.employeeFile_s = "sailor4s.png";
	m_mapEmployee[tempEmployee.id] = tempEmployee;	

	return true;
}

bool DataManager::initUserStock(USER *pUser)
{
	if (!pUser)
	{
		return false;
	}
	
	map<int, STOCK>::iterator iterTemp; 
	for (iterTemp = m_mapStockMarket.begin(); iterTemp != m_mapStockMarket.end(); iterTemp++)
	{
		USERSTOCK tempUserStock;
		tempUserStock.count = 0;
		tempUserStock.pStock =&(*iterTemp).second;
		pUser->stockList.push_back(tempUserStock);
	}

	return  true;
}

bool DataManager::initPlayer()
{
	map<int, EMPLOYEE>::iterator iterTemp; 

	USER player;
	player.uin = "1";
	player.uid = 1;
	player.nickName = "杰克";
	player.margin = 0;
	player.accountBalance = 50;
	player.stockList.clear();
	initUserStock(&player);
	iterTemp = m_mapEmployee.find(1);
	player.pEmployee = &(*iterTemp).second;
	player.beenz = 0;
	player.bMaster = false;
	player.nIndex = 0;
	player.totalBeenz = 0;
	player.postion = ccp(8, 56);
	player.bPlay = false;
	player.bAI = false;
	m_mapPlayer[player.uid] = player;

	player.uin = "2";
	player.uid = 2;
	player.nickName = "奥尔森";
	player.margin = 0;
	player.accountBalance = 50;
	player.stockList.clear();
	initUserStock(&player);
	iterTemp = m_mapEmployee.find(2);
	player.pEmployee = &(*iterTemp).second;
	player.beenz = 0;
	player.bMaster = false;
	player.nIndex = 0;
	player.totalBeenz = 0;
	player.postion = ccp(8, 40);
	player.bPlay = false;
	player.bAI = false;	
	m_mapPlayer[player.uid] = player;

	player.uin = "3";
	player.uid = 3;
	player.nickName = "迈克";
	player.margin = 0;
	player.accountBalance = 50;
	player.stockList.clear();
	initUserStock(&player);
	iterTemp = m_mapEmployee.find(3);
	player.pEmployee = &(*iterTemp).second;
	player.beenz = 0;
	player.bMaster = false;
	player.nIndex = 0;
	player.totalBeenz = 0;
	player.postion = ccp(8, 24);
	player.bPlay = false;
	player.bAI = false;
	m_mapPlayer[player.uid] = player;

	player.uin = "4";
	player.uid = 4;
	player.nickName = "奥黛丽";
	player.margin = 0;
	player.accountBalance = 50;
	player.stockList.clear();
	initUserStock(&player);
	iterTemp = m_mapEmployee.find(4);
	player.pEmployee = &(*iterTemp).second;
	player.beenz = 0;
	player.bMaster = false;
	player.nIndex = 0;
	player.totalBeenz = 0;
	player.postion = ccp(8, 8);
	player.bPlay = false;
	player.bAI = false;
	m_mapPlayer[player.uid] = player;
	
	player.uin = "5";
	player.uid = 5;
	player.nickName = "玛莉莲";
	player.margin = 0;
	player.accountBalance = 50;
	player.stockList.clear();
	initUserStock(&player);
	iterTemp = m_mapEmployee.find(5);
	player.pEmployee = &(*iterTemp).second;
	player.beenz = 0;
	player.bMaster = false;
	player.nIndex = 0;
	player.totalBeenz = 0;
	player.postion = ccp(8, 8);
	player.bPlay = false;
	player.bAI = false;
	m_mapPlayer[player.uid] = player;	

	return true;
}

bool DataManager::initGameSpot()
{
	m_mapGameSpot.clear();
	
	//港口
	map<int, PORTS>::iterator iterPorts;
	for (iterPorts = m_mapPorts.begin(); iterPorts != m_mapPorts.end(); iterPorts++)
	{
		m_mapGameSpot[(*iterPorts).second.spot.index] = &(*iterPorts).second.spot;
	}
	
	//修船厂
	map<int, SHIPYARD>::iterator iterYard;
	for (iterYard = m_mapShipyard.begin(); iterYard != m_mapShipyard.end(); iterYard++)
	{
		m_mapGameSpot[(*iterYard).second.spot.index] = &(*iterYard).second.spot;
	}
	
	//货物
	map<int, CARGO>::iterator iterCargo;
	for (iterCargo = m_mapCargo.begin(); iterCargo != m_mapCargo.end(); iterCargo++)
	{
		vector<GAMESPOT>::iterator iterTemp;
		for(iterTemp = (*iterCargo).second.spotList.begin();  iterTemp != (*iterCargo).second.spotList.end(); ++iterTemp)
		{
			m_mapGameSpot[(*iterTemp).index] = &(*iterTemp);
		}	
	}
	
	//保险
	m_mapGameSpot[m_insurance.index] = &m_insurance;
	
	//领航员
	map<int, NAVIGATOR>::iterator iterNav;
	for (iterNav = m_mapNavigator.begin(); iterNav != m_mapNavigator.end(); iterNav++)
	{
		m_mapGameSpot[(*iterNav).second.spot.index] = &(*iterNav).second.spot;
	}
	
	//海盗
	map<int, GAMESPOT>::iterator iterPri;
	for (iterPri = m_mapPirate.begin(); iterPri != m_mapPirate.end(); iterPri++)
	{
		m_mapGameSpot[(*iterPri).second.index]  = &(*iterPri).second;
	}
	
	return true;
}

USER *DataManager::FindUserById(int id)
{
	map<int ,USER>::iterator iter;
	iter = m_mapPlayer.find(id);
	if (iter != m_mapPlayer.end())
	{
		return &(*iter).second;
	}

	return NULL;
}

USER *DataManager::FindUserByGamespotID(int id)
{
	map<int, GAMESPOT*>::iterator iter;
	iter = m_mapGameSpot.find(id);
	if (iter != m_mapGameSpot.end())
	{
		return FindUserById((*iter).second->uid);
	}
	
	return NULL;
}

USER *DataManager::GetCurrentUser()
{
	return FindUserById(m_nCurrentUserId);
}

GAMESPOT *DataManager::FindGamespot(int id)
{
	map<int, GAMESPOT*>::iterator iter;
	iter = m_mapGameSpot.find(id);
	if (iter != m_mapGameSpot.end())
	{
		return (*iter).second;
	}
	
	return NULL;
}

USER *DataManager::FindUserByEmployee(int nEmployeeID)
{
	map<int ,USER>::iterator iter;
	for (iter = m_mapPlayer.begin(); iter != m_mapPlayer.end(); iter++)
	{
		if ( (*iter).second.pEmployee->id == nEmployeeID)
		{
			return &(*iter).second;
		}
	}

	return NULL;
}

SEACRAFT *DataManager::FindSeaCraft(int id)
{
	map<int, SEACRAFT>::iterator iter;
	iter = m_mapSeacraft.find(id);
	if (iter != m_mapSeacraft.end())
	{
		return &(*iter).second;
	}

	return NULL;
}

bool DataManager::getBoolForKey(const char* pKey, bool defaultValue)
{
	char chLevel[32];
	sprintf(chLevel, "Level%d", m_nLevel);
	
	return CCUserDefault::sharedUserDefault()->getBoolForKey(pKey, defaultValue, chLevel);
}

int	DataManager::getIntegerForKey(const char* pKey, int defaultValue)
{
	char chLevel[32];
	sprintf(chLevel, "Level%d", m_nLevel);
	
	return CCUserDefault::sharedUserDefault()->getIntegerForKey(pKey, defaultValue, chLevel);
}

float DataManager::getFloatForKey(const char* pKey, float defaultValue)
{
	char chLevel[32];
	
	return CCUserDefault::sharedUserDefault()->getFloatForKey(pKey, defaultValue, chLevel);
}
	
double DataManager::getDoubleForKey(const char* pKey, double defaultValue)
{
	char chLevel[32];
	sprintf(chLevel, "Level%d", m_nLevel);
	
	return CCUserDefault::sharedUserDefault()->getDoubleForKey(pKey, defaultValue, chLevel);
}

string DataManager::getStringForKey(const char* pKey, const string & defaultValue)
{
	char chLevel[32];
	sprintf(chLevel, "Level%d", m_nLevel);
	
	return CCUserDefault::sharedUserDefault()->getStringForKey(pKey, defaultValue, chLevel);
}

bool DataManager::xmlPortsVisible(int portsId)
{
	char chKey[32];
	sprintf(chKey, "Ports%d", portsId);
	
	string tempString = getStringForKey(chKey, "000");

	if (tempString.length() ==3) 
	{
		return bool(tempString[0] - '0');
	}
	
	return false;
}

bool DataManager::xmlPortsEnable(int portsId)
{
	char chKey[32];
	sprintf(chKey, "Ports%d", portsId);
	
	string tempString = getStringForKey(chKey, "000");
	
	if (tempString.length() ==3) 
	{
		return bool(tempString[1] - '0');
	}
	
	return false;
}
	
int DataManager::xmlPortsUserId(int portsId)
{
	char chKey[32];
	sprintf(chKey, "Ports%d", portsId);
	
	string tempString = getStringForKey(chKey, "000");
	
	if (tempString.length() ==3) 
	{
		return tempString[2] - '0';
	}
	
	return 0;
}

bool DataManager::xmlShipyardVisible(int shipyardsId)
{
	char chKey[32];
	sprintf(chKey, "Shipyard%d", shipyardsId);
	
	string tempString = getStringForKey(chKey, "000");
	
	if (tempString.length() ==3) 
	{
		return bool(tempString[0] - '0');
	}
	
	return false;
}

bool DataManager::xmlShipyardEnable(int shipyardsId)
{
	char chKey[32];
	sprintf(chKey, "Shipyard%d", shipyardsId);
	
	string tempString = getStringForKey(chKey, "000");
	
	if (tempString.length() ==3) 
	{
		return bool(tempString[1] - '0');
	}
	
	return false;	
}

int DataManager::xmlShipyardUserId(int shipyardsId)
{
	char chKey[32];
	sprintf(chKey, "Shipyard%d", shipyardsId);
	
	string tempString = getStringForKey(chKey, "000");
	
	if (tempString.length() ==3) 
	{
		return tempString[2] - '0';
	}
	
	return 0;
}

bool DataManager::xmlPirateVisible(int priateId)
{
	char chKey[32];
	sprintf(chKey, "Pirate%d", priateId);
	
	string tempString = getStringForKey(chKey, "000");
	
	if (tempString.length() ==3) 
	{
		return bool(tempString[0] - '0');
	}
	
	return false;
}

bool DataManager::xmlPirateEnable(int priateId)
{
	char chKey[32];
	sprintf(chKey, "Pirate%d", priateId);
	
	string tempString = getStringForKey(chKey, "000");
	
	if (tempString.length() ==3) 
	{
		return bool(tempString[1] - '0');
	}
	
	return false;
}

int DataManager::xmlPirateUserId(int priateId)
{
	char chKey[32];
	sprintf(chKey, "Pirate%d", priateId);
	
	string tempString = getStringForKey(chKey, "000");
	
	if (tempString.length() ==3) 
	{
		return tempString[2] - '0';
	}
	
	return 0;
}

bool DataManager::xmlNavigatorVisible(int navigatorId)
{
	char chKey[32];
	sprintf(chKey, "Navigator%d", navigatorId);
	
	string tempString = getStringForKey(chKey, "000");
	
	if (tempString.length() ==3) 
	{
		return bool(tempString[0] - '0');
	}
	
	return false;
}

bool DataManager::xmlNavigatorEnable(int navigatorId)
{
	char chKey[32];
	sprintf(chKey, "Navigator%d", navigatorId);
	
	string tempString = getStringForKey(chKey, "000");
	
	if (tempString.length() ==3) 
	{
		return bool(tempString[1] - '0');
	}
	
	return false;
}

int DataManager::xmlNavigatorUserId(int navigatorId)
{
	char chKey[32];
	sprintf(chKey, "Navigator%d", navigatorId);
	
	string tempString = getStringForKey(chKey, "000");
	
	if (tempString.length() ==3) 
	{
		return tempString[2] - '0';
	}
	
	return 0;
}

bool DataManager::xmlInsuranceVisible()
{
	string tempString = getStringForKey("Insrurance", "000");
	
	if (tempString.length() ==3) 
	{
		return bool(tempString[0] - '0');
	}
	
	return false;
}


bool DataManager::xmlInsuranceEnable()
{
	string tempString = getStringForKey("Insrurance", "000");
	
	if (tempString.length() ==3) 
	{
		return bool(tempString[1] - '0');
	}
	
	return false;
}


int DataManager::xmlInsuranceUserId()
{
	string tempString = getStringForKey("Insrurance", "000");
	
	if (tempString.length() ==3) 
	{
		return tempString[2] - '0';
	}
	
	return 0;
}

bool DataManager::xmlPlayerJoin(int userId)
{
	char chKey[32];
	sprintf(chKey, "Player%d", userId);
	
	string tempString = getStringForKey(chKey, "00000");
	
	if (tempString.length() == 5) 
	{
		return bool(tempString[0] - '0');
	}
	
	return false;
}

bool DataManager::xmlPlayerAI(int userId)
{
	char chKey[32];
	sprintf(chKey, "Player%d", userId);
	
	string tempString = getStringForKey(chKey, "0000");
	
	if (tempString.length() == 5) 
	{
		return bool(tempString[1] - '0');
	}
	
	return false;
}

int DataManager::xmlPlayerBeenz(int userId)
{
	char chKey[32];
	sprintf(chKey, "Player%d", userId);
	
	string tempString = getStringForKey(chKey, "00000");
	
	if (tempString.length() == 5) 
	{
		return tempString[2] - '0';
	}
	
	return 0;
}


int DataManager::xmlPlayerEmployeeCount(int userId)
{
	char chKey[32];
	sprintf(chKey, "Player%d", userId);
	
	string tempString = getStringForKey(chKey, "00000");
	
	if (tempString.length() == 5) 
	{
		return tempString[3] - '0';
	}
	
	return 0;
}

bool DataManager::xmlPlayerMaster(int userId)
{
	char chKey[32];
	sprintf(chKey, "Player%d", userId);
	
	string tempString = getStringForKey(chKey, "00000");
	
	if (tempString.length() == 5) 
	{
		return bool(tempString[4] - '0');
	}
	
	return false;
}

int DataManager::xmlSeacraftPos(int seacraftId)
{
	char chKey[32];
	sprintf(chKey, "SeacraftPosition%d", seacraftId);
	
	return getIntegerForKey(chKey, 0);
}

bool DataManager::xmlAutoLoadCargo()
{
	return getBoolForKey("AutoLoadCargo", false);
}

bool DataManager::xmlStockMarketVisible()
{
	return getBoolForKey("StockMarketVisible", false);
}

bool DataManager::xmlCargoInvest(int cargoId)
{
	char chKey[32];
	sprintf(chKey, "Cargo%d", cargoId);
	
	string tempString = getStringForKey(chKey, "0000000000000");
	
	if (tempString.length() > 1) 
		return bool(tempString[0] - '0');

	return false;
}

bool DataManager::xmlCargoVisible(int cargoId, int index)
{
	char chKey[32];
	sprintf(chKey, "Cargo%d", cargoId);
	
	string tempString = getStringForKey(chKey, "0000000000000");
	
	if (tempString.length() > (index-1)*3 + 1)
	{
		return bool(tempString[(index-1)*3 + 1] - '0');
	}
	
	return false;
}

bool DataManager::xmlCargoEnable(int cargoId, int index)
{
	char chKey[32];
	sprintf(chKey, "Cargo%d", cargoId);
	
	string tempString = getStringForKey(chKey, "0000000000000");
	
	if (tempString.length() > (index-1)*3 + 2)
	{
		return bool(tempString[(index-1)*3 + 2] - '0');
	}
	
	return false;
}
								   
int DataManager::xmlCargoUserId(int cargoId, int index)
{
	char chKey[32];
	sprintf(chKey, "Cargo%d", cargoId);
	
	string tempString = getStringForKey(chKey, "0000000000000");
	
	if (tempString.length() > (index-1)*3 + 3)
	{
		return tempString[(index-1)*3 + 3] - '0';
	}
	
	return false;
}
								   
int DataManager::xmlGamegound()
{
	return getIntegerForKey("GameRound", 3);
}
						   
int DataManager::xmlWingold()
{
	return getIntegerForKey("WinGold", 0);
}

