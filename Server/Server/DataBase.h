#pragma once
#include "stdafx.h"

class CDataBase
{
	DECLARE_SINGLE(CDataBase)

private:
	SQLHENV		m_hEnv;
	SQLHDBC		m_hDbc;
	SQLHSTMT	m_hStmt;

	concurrency::concurrent_queue<DATABASE_TYPE>m_ccqDataBaseQueue;
	concurrency::concurrent_queue<DATABASE_TYPE>m_ccqSaveStateQueue;

public:
	void HandleDiagnosticRecord(SQLHANDLE _hHandle, SQLSMALLINT _hType, RETCODE RetCode);
	bool ConnectDataBase();
	void DisConnectDataBase();
	void RunDataBase(DATABASE_TYPE& event);
public:
	void LogInPorcess(DATABASE_TYPE& event);
	void LogOutPorcess(DATABASE_TYPE& event);

public:
	void BindDataBaseFP();
	bool DataBaseQueueIsEmpty();
	void InsertToDataBaseQueue(DATABASE_TYPE& event);
	bool PopFromDataBaseQueue(DATABASE_TYPE& event);
	void InsertToStateQueue(DATABASE_TYPE& event);
	bool PopFromStateQueue(DATABASE_TYPE& event);
};

