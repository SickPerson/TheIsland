#include "stdafx.h"
#include "FontMgr.h"

#include "PathMgr.h"
#include <fstream>

CFontMgr::CFontMgr()
{
}


CFontMgr::~CFontMgr()
{
}

void CFontMgr::Init()
{
	std::ifstream readFont;

	wstring strPath = CPathMgr::GetResPath();
	vector<string> strFile;
	strFile.reserve(1500);

	strPath = strPath + wstring(L"Texture\\Font\\Font.fnt");
	readFont.open(strPath.c_str());

	if (readFont.is_open())
	{
		int line = 0;
		while (!readFont.eof())
		{
			string strLine;
			readFont >> strLine;
			strFile.emplace_back(strLine);
		}
	}
	readFont.close();

	FontInfo tInfo;
	tInfo.strFontName = strFile[(UINT)FontFileInfo::FONTNAME].substr(6, strFile[(UINT)FontFileInfo::FONTNAME].length() - 7);

	tInfo.strFileName = strFile[(UINT)FontFileInfo::FILENAME].substr(6, strFile[(UINT)FontFileInfo::FILENAME].length() - 7);

	tInfo.iSize = atoi(strFile[(UINT)FontFileInfo::FONTSIZE].substr(5).c_str());

	tInfo.iScaleX = atoi(strFile[(UINT)FontFileInfo::SCALEW].substr(7).c_str());

	tInfo.iScaleY = atoi(strFile[(UINT)FontFileInfo::SCALEH].substr(7).c_str());

	tInfo.iCount = atoi(strFile[(UINT)FontFileInfo::FONTCOUNT].substr(6).c_str());

	int idx = (UINT)FontFileInfo::FONTCOUNT;
	for (int i = 0; i < tInfo.iCount; ++i)
	{
		CharInfo tCharInfo;
		tCharInfo.id = atoi(strFile[idx + 2 + (i * 11)].substr(3).c_str());
		tCharInfo.ix = atoi(strFile[idx + 3 + (i * 11)].substr(2).c_str());
		tCharInfo.iy = atoi(strFile[idx + 4 + (i * 11)].substr(2).c_str());
		tCharInfo.iWidth = atoi(strFile[idx + 5 + (i * 11)].substr(6).c_str());
		tCharInfo.iHeight = atoi(strFile[idx + 6 + (i * 11)].substr(7).c_str());

		tInfo.mCharInfo.insert(make_pair(tCharInfo.id, tCharInfo));
	}
	m_vFontInfo.emplace_back(tInfo);
}
CharInfo CFontMgr::GetCharInfo(char id, int index)
{
	return m_vFontInfo[index].mCharInfo[id];
}

FontInfo CFontMgr::GetFontInfo(int index) const
{
	return m_vFontInfo[index];
}
