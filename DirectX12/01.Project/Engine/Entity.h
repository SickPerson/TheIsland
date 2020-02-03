#pragma once
class CEntity
{
public:
	CEntity();
	virtual ~CEntity();

private:
	wstring			m_strName;

public:
	void SetName( wstring strName );
	const wstring& GetName();
};

