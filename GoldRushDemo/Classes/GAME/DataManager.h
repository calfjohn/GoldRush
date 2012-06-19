#ifndef _DATAMANAGER_H_
#define _DATAMANAGER_H_

#include "data.h"
#include "../json/json.h"

class DataManager
{
public:

	static DataManager* sharedDataManager(void)
	{
		if (!m_pInstance)  
		{
			m_pInstance = new DataManager();
			m_pInstance->init();
		}
		return m_pInstance; 
	}

private:

	virtual ~DataManager(void){};
	DataManager(void);
	static DataManager *m_pInstance;

	class CFRelease 
	{  
	public:  
		~CFRelease()  
		{  
			if (DataManager::m_pInstance)  
				delete DataManager::m_pInstance;  
		}  
	};  
	static CFRelease Garbo;  

public:
	bool init(void);

	USER *FindUserById(int id);
	USER *FindUserByEmployee(int nEmployeeID);
	POSITION *FindPostion(int nPos);
	POSITION *FindRulerPostion(int nPos);
	SEACRAFT *FindSeaCraft(int id);
	USER *FindUserByGamespotID(int id);
	GAMESPOT *FindGamespot(int id);
	USER *GetCurrentUser();

	
	bool xmlPortsVisible(int portsId);
	bool xmlPortsEnable(int portsId);
	int xmlPortsUserId(int portsId);
	
	bool xmlShipyardVisible(int shipyardsId);
	bool xmlShipyardEnable(int shipyardsId);
	int xmlShipyardUserId(int shipyardsId);
	
	bool xmlPirateVisible(int priateId);
	bool xmlPirateEnable(int priateId);
	int xmlPirateUserId(int priateId);
	
	bool xmlNavigatorVisible(int navigatorId);
	bool xmlNavigatorEnable(int navigatorId);
	int xmlNavigatorUserId(int navigatorId);
	
	bool xmlInsuranceVisible();
	bool xmlInsuranceEnable();
	int xmlInsuranceUserId();
	
	bool xmlPlayerJoin(int userId);
	bool xmlPlayerAI(int userId);
	int xmlPlayerBeenz(int userId);	
	int xmlPlayerEmployeeCount(int userId);	
	bool xmlPlayerMaster(int userId);	
	
	int xmlSeacraftPos(int seacraftId);
	
	bool xmlAutoLoadCargo();
	bool xmlStockMarketVisible();

	bool xmlCargoInvest(int cargoId);
	bool xmlCargoVisible(int cargoId, int index);
	bool xmlCargoEnable(int cargoId, int index);
	int xmlCargoUserId(int cargoId, int index);
	
	int xmlGamegound();
	int xmlWingold();
	
	int getLevel(){return m_nLevel;}
	int getCurrentUserID(){return m_nCurrentUserId;}
	int getNoInvestStockCode() {return m_nNoInvestStockCode;}
	
	void setLevel(int nLevel){m_nLevel = nLevel;}
	void setCurrentUserId(int nUserId){m_nCurrentUserId = nUserId;}
	void setNoInvestStockCode(int nStockCode){ m_nNoInvestStockCode = nStockCode;}
	
	map<int, STOCK> m_mapStockMarket;
	map<int, CARGO> m_mapCargo;
	map<int, SEACRAFT> m_mapSeacraft;
	map<int, EMPLOYEE> m_mapEmployee;

	map<int, NAVIGATOR>	m_mapNavigator;
	GAMESPOT m_insurance;
	map<int, GAMESPOT> m_mapPirate;	
	map<int, PORTS> m_mapPorts;
	map<int, SHIPYARD> m_mapShipyard;

	map<int ,USER> m_mapPlayer;
	map<int, int> m_mapDice;

	map<int, POSITION> m_mapPosition;
	map<int, POSITION> m_mapRulerPosition;
	map<int, GAMESPOT*> m_mapGameSpot;		//可部署位置的字典表

public:
	static int GBKToUTF8(std::string & gbkStr);
	
protected:
	bool initUserDefault(int level);
	bool InitPosition();
	bool InitRulerPosition();
	bool initStockMarket();
	bool initCargo();
	bool initSeaCraft();
	bool initNavigator();
	bool initInsurance();
	bool initPirate();
	bool initPorts();
	bool initShipyard();
	bool initEmployee();
	bool initPlayer();
	bool initDice();
	bool initUserStock(USER *pUser);
	bool initGameSpot();
	
	bool getBoolForKey(const char* pKey, bool defaultValue);
	int	getIntegerForKey(const char* pKey, int defaultValue);
	float getFloatForKey(const char* pKey, float defaultValue);
	double getDoubleForKey(const char* pKey, double defaultValue);
	string getStringForKey(const char* pKey, const string & defaultValue);
	
	int m_nIndex;
	int m_nLevel;
	int m_nCurrentUserId;
	int m_nNoInvestStockCode;
};

#endif