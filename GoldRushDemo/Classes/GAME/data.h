#ifndef _DATA_H_
#define _DATA_H_

#include "CCGeometry.h"
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
using namespace std;
using namespace cocos2d;
using namespace CocosDenshion;

#define NO_USER 0
#define GAMELAYER 101001		//游戏层标志
#define CONTROLLAYER 10102		//玩家控制面板标志

enum 
{
	HIGHEST_PRIORITY = kCCMenuTouchPriority,
	HIGH_PRIORITY = HIGHEST_PRIORITY + 50,
	NORMAL_PRIORITY = HIGH_PRIORITY + 50,
	LOW_PRIORITY = NORMAL_PRIORITY + 50
};

enum 
{
	ZORDER_HIGHEST = 1000,
	ZORDER_HIGH = 500,
	ZORDER_NORMAL = 100,
	ZORDER_LOW = 50,
	ZORDER_DEFAULT = 0
};

/*
 //股票价格
 typedef enum
 {
 kPrice1 = 0,
 kPrice2 = 5,
 kPrice3 = 10,
 kPrice4 = 20,
 kPrice5 = 30
 }STOCKPRICE;
 */

//股票
typedef struct{
	int code;					//股票代码
	string name;				//股票名称
	int count;					//股票数量
	int price;					//股票价格
	int value;					//货物到港获利
	bool bInvest;				//是否投资，每一轮都有一只股票被放弃
	CCPoint position;			//位置
	string stockFile;			//对应贴图名称
	string seaCraftName;		//对应船的名称
	string seaCraftFile;		//对应船贴图
	string seaCraftRulerFile;	//对应船的进度条贴图
}STOCK;

//股市，用map实现
//银行比索，用class实现
//骰子，用class实现

//游戏点位置（船位、港口、修船厂）
typedef struct{
	int index;			//游戏点索引
	int uid;			//玩家UID,当位置被占据后设置，默认NO_USER
	int buy;			//买价
	int sell;			//卖价
	CCPoint position;	//位置
}GAMESPOT;

//货物
typedef struct{
	STOCK *pStock;				//代表的股票
	vector<GAMESPOT> spotList;	//货物包含的位置
}CARGO;

//运输船
typedef struct{
	int id;				//船id
	int position;		//船位置[0,26]
	CARGO *pCargo;		//船上的货物
}SEACRAFT;

//同谋/雇员/帮凶
typedef struct{
	int id;				//标识 红、蓝、黄、橘、黑
	int count;			//数量
	string employeeFile;	//雇员图名称
	string employeeFile_s;	//雇员小图名称
}EMPLOYEE;

//玩家股票
typedef struct{
	STOCK *pStock;
	int count;			//股票数量
}USERSTOCK;

//用户资料
typedef struct 
{
	//个人信息
	string uin;				//平台账号ID
	int uid;				//游戏账号ID,
	string nickName;		//用户昵称
	
	//账户信息
	int margin;			//保证金
	int accountBalance;	//账户余额
	
	//游戏信息
	vector<USERSTOCK> stockList;	//拥有的股票
	EMPLOYEE  *pEmployee;			//同谋
	int	  beenz;					//游戏虚拟货币
	bool  bMaster;					//港务局长/船长
	int	  nIndex;					//游戏轮次，-1表示弃权
	int   totalBeenz;				//游戏中的实时总资产
	CCPoint postion;				//游戏界面中的位置
	bool  bPlay;					//参与游戏
	bool  bAI;						//AI玩家
}USER;

//领航
typedef struct 
{
	int step;		//步长
	GAMESPOT spot;	//位置
}NAVIGATOR;

//保险

//海盗

//港口
typedef struct 
{
	int id;					//港口id
	SEACRAFT *pSeacraft;	//到港船
	GAMESPOT spot;			//位置	
}PORTS;

//修船厂
typedef struct 
{
	int id;					//修船厂id
	SEACRAFT *pSeacraft;	//到修船厂船
	GAMESPOT spot;			//位置
}SHIPYARD;

typedef struct
{
	float x;
	float y;
}POSITION;					//位置

template <class T> 
std::string ConvertToString(T value) {
	std::stringstream ss;
	ss << value;
	return ss.str();
}


enum {
	PORTSVISIBLE1,
	PORTSENABLE1,
	PORTSUSERID1,
	PORTSVISIBLE2,
	PORTSENABLE2,
	PORTSUSERID2,
	PORTSVISIBLE3,
	PORTSENABLE3,
	PORTSUSERID3,	
	SHIPYARDVISIBLE1,
	SHIPYARDENABLE1,
	SHIPYARDSUSERID1,
	SHIPYARDENABLE2,
	SHIPYARDVISIBLE2,
	SHIPYARDSUSERID2,
	SHIPYARDENABLE3,
	SHIPYARDVISIBLE3,
	SHIPYARDSUSERID3,
	PIRATEVISIBLE1,
	PIRATEENABLE1,
	PIRATEUSERID1,
	PIRATEVISIBLE2,
	PIRATEENABLE2,
	PIRATEUSERID2,
	NAVIGATORVISIBLE1,
	NAVIGATORENABLE1,
	NAVIGATORUSERID1,
	NAVIGATORVISIBLE2,
	NAVIGATORENABLE2,
	NAVIGATORUSERID2,
	INSURANCEVISIBLE,
	INSURANCEENABLE,
	INSURANCEUSERID,
	STOCKMARKETVISIBLE,
	STOCKMARKETENABLE,
	PLAYERJOIN1,
	PLAYERAI1,
	PLAYERBEENZ1,
	PLAYERJOIN2,
	PLAYERAI2,
	PLAYERBEENZ2,
	PLAYERJOIN3,
	PLAYERAI3,
	PLAYERBEENZ3,
	PLAYERJOIN4,
	PLAYERAI4,
	PLAYERBEENZ4,
	PLAYERJOIN5,
	PLAYERAI5,
	PLAYERBEENZ5,	
	SEACRAFTPOSITION1,
	SEACRAFTCARGO1,
	SEACRAFTPOSITION2,
	SEACRAFTCARGO2,
	SEACRAFTPOSITION3,
	SEACRAFTCARGO3,
	CARGO1,
	CARGOUSERID1,
	CARGO2,
	CARGOUSERID2,
	CARGO3,
	CARGOUSERID3,
	CARGO4,
	CARGOUSERID4,
	GAMEROUND,
	
	LEVEL_COUNT
};

const string g_LevelNames[LEVEL_COUNT] = {
	"PortsVisible1",
	"PortsEnable1",
	"PortsUserID1",
	"PortsVisible2",
	"PortsEnable2",
	"PortsUserID2",
	"PortsVisible3",
	"PortsEnable3",
	"PortsUserID3",	
	"ShipyardVisible1",
	"ShipyardEnable1",
	"ShipyardsUserID1",
	"ShipyardEnable2",
	"ShipyardVisible2",
	"ShipyardsUserID2",
	"ShipyardEnable3",
	"ShipyardVisible3",
	"ShipyardsUserID3",
	"PirateVisible1",
	"PirateEnable1",
	"PirateUserID1",
	"PirateVisible2",
	"PirateEnable2",
	"PirateUserID2",
	"NavigatorVisible1",
	"NavigatorEnable1",
	"NavigatorUserID1",
	"NavigatorVisible2",
	"NavigatorEnable2",
	"NavigatorUserID2",
	"InsuranceVisible",
	"InsuranceEnable",
	"InsuranceUserID",
	"StockMarketVisible",
	"StockMarketEnable",
	"PlayerJoin1",
	"PlayerAI1",
	"PlayerBeenz1",
	"PlayerJoin2",
	"PlayerAI2",
	"PlayerBeenz2",
	"PlayerJoin3",
	"PlayerAI3",
	"PlayerBeenz3",
	"PlayerJoin4",
	"PlayerAI4",
	"PlayerBeenz4",
	"PlayerJoin5",
	"PlayerAI5",
	"PlayerBeenz5",	
	"SeacraftPosition1",
	"SeacraftCargo1",
	"SeacraftPosition2",
	"SeacraftCargo2",
	"SeacraftPosition3",
	"SeacraftCargo3",
	"Cargo1",
	"CargoUserID1",
	"Cargo2",
	"CargoUserID2",
	"Cargo3",
	"CargoUserID3",
	"Cargo4",
	"CargoUserID4",
	"GameRound"
};

#endif