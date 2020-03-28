#pragma once

typedef struct CharInfo {
	char id;
	int ix;
	int iy;
	int iWidth;
	int iHeight;
}CharInfo;

typedef struct FontInfo {
	string strFontName;
	string strFileName;
	int iSize;
	int iScaleX;
	int iScaleY;
	int iCount;
	map<char, CharInfo> mCharInfo;
}FontInfo;

enum class FontFileInfo {
	FONTNAME	= 1,
	FONTSIZE	= 2,
	SCALEW		= 16,
	SCALEH		= 17,
	FILENAME	= 26,
	FONTCOUNT	= 28,
	END,
};

class CFontMgr
{
	SINGLE(CFontMgr);
private:
	vector<FontInfo> m_vFontInfo;
public:
	void Init();
	FontInfo GetFontInfo(int index = 0) const;
	CharInfo GetCharInfo(char id, int index = 0);
};

