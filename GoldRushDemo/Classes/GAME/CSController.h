#ifndef _CFCONTROLLER_H_
#define _CFCONTROLLER_H_

#include "DataManager.h"

////////////////////
/////游戏业务层/////

class CSController
{
public: 
	static CSController* sharedController(void)
	{
		if (!m_pInstance)  
		{
			m_pInstance = new CSController();
			m_pInstance->init();
		}
		return m_pInstance; 
	}
	static string TmpPath(string sPath);
	static string BundlePath(string sPath);                         //获取平台路径
	
	static int DiceGo();
	bool GameStart();
	bool GameEnd();
	void NextRound();
	void NextTips();
	bool BoardSeaCraft(USER *pUser, int id);
	void ToSail();	//开始航行
	void SailTo();	//航行结束
	void UpdatePlayerInfo();
	void restartLevel();
	int  GetGameRound(){return m_nGameRound;}
	bool DecideGamespot(int userId, int nGamespotID);
	void transaction();
protected:

	void InitUserIndex();							//初始化玩家轮次
	bool CheckMargin();
	bool DecideCargo(int nCode);
	bool DecideSeaCraft(SEACRAFT *pSeacraft, int nCode);
	void DistributionStock();
	void DecideMaster(int id, int nBeenz);
	
	void SeacraftToPorts(SEACRAFT *pSeacraft);
	void SeacraftToShipyard(SEACRAFT *pSeacraft);
		
	void CaculatorStock(CARGO *pCargo);
	bool CaculatorMoney(USER *pUser, int beenz);

	bool CaculatorGamespot(int userID, int nGameSpotId);
	bool CalulatorPirate(CARGO *pCargo);
	bool caculatorStock(USER *pUser, int nCode);
	void CalulatorSeacraft(CARGO *pCargo);
	void CalulatorPorts();
	void CalulatorShipyard();

	void MasterExcute(USER *pUser);
	void NavigatorExcute();

	void ClearAccounts();
	void ClearSeaCraft();
	void ClearCargoPlayer(CARGO *pCargo);

	int m_nGameRound;

private:
	bool init(void);
	virtual ~CSController(void){};
	CSController(void) {}
	static CSController *m_pInstance;

	class CFRelease // 它的唯一工作就是在析构函数中删除CSingleton的实例  
	{  
	public:  
		~CFRelease()  
		{  
			if (CSController::m_pInstance)  
				delete CSController::m_pInstance;  
		}  
	};  
	static CFRelease Garbo; // 定义一个静态成员，在程序结束时，系统会调用它的析构函数  

};

#endif