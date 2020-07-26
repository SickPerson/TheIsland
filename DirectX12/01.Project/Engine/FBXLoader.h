#pragma once

#include "global.h"

struct tKeyFrame
{
	FbxAMatrix  matTransform;
	double		dTime;
};

struct tBone
{
	wstring				strBoneName;
	int					iDepth;			// °èÃþ±¸Á¶ ±íÀÌ
	int					iParentIndx;	// ºÎ¸ð Bone ÀÇ ÀÎµ¦½º
	FbxAMatrix			matOffset;		// Offset Çà·Ä( -> »Ñ¸® -> Local)
	FbxAMatrix			matBone;
	vector<tKeyFrame>	vecKeyFrame;
};

struct tAnimClip
{
	wstring		strName;
	FbxTime		tStartTime;
	FbxTime		tEndTime;
	FbxLongLong	llTimeLength;
	FbxTime::EMode eMode;
};

class CMesh;

class CFBXLoader
{
public:
	CFBXLoader();
	~CFBXLoader();

private:
	FbxManager*				m_pManager;
	FbxScene*				m_pScene;
	FbxImporter*			m_pImporter;

	vector<tContainer>		m_vecContainer;

	// Animation 
	vector<tBone*>			m_vecBone;
	FbxArray<FbxString*>	m_arrAnimName;
	vector<tAnimClip*>		m_vecAnimClip;

public:
	void Init();
	void LoadFbx( const wstring& wstrPath, int iShaderType = 0 );

public:
	int GetContainerCount();
	const tContainer& GetContainer( int iIdx );
	vector<tBone*>& GetBones();
	vector<tAnimClip*>& GetAnimClip();

private:
	void LoadMeshDataFromNode( FbxNode* pNode );
	void LoadMesh( FbxMesh* pFbxMesh );
	void LoadMaterial( FbxSurfaceMaterial* pMtrlSur );

	void GetTangent( FbxMesh* pMesh, tContainer* pContainer, int iIdx, int iVtxOrder );
	void GetBinormal( FbxMesh* pMesh, tContainer* pContainer, int iIdx, int iVtxOrder );
	void GetNormal( FbxMesh* pMesh, tContainer* pContainer, int iIdx, int iVtxOrder );
	void GetUV( FbxMesh* pMesh, tContainer* pContainer, int iIdx, int iUVIndex );

	Vec4 GetMtrlData( FbxSurfaceMaterial* pSurface, const char* pMtrlName, const char* pMtrlFactorName );
	wstring GetMtrlTextureName( FbxSurfaceMaterial* pSurface, const char* pMtrlProperty );
	
	void LoadTexture();
	void CreateMaterial(int iShaderType = 0);

	// Animation
	void LoadSkeleton( FbxNode* pNode );
	void LoadSkeleton_Re( FbxNode* pNode, int iDepth, int iIdx, int iParentIdx );
	void LoadAnimationClip();
	void Triangulate( FbxNode* pNode );

	void LoadAnimationData( FbxMesh* pMesh, tContainer* pContainer );
	void LoadWeightsAndIndices( FbxCluster* pCluster, int iBoneIdx, tContainer* pContainer );
	void LoadOffsetMatrix( FbxCluster* pCluster, const FbxAMatrix& matNodeTransform, int iBoneIdx, tContainer* pContainer );
	void LoadKeyframeTransform( FbxNode* pNode, FbxCluster* pCluster, const FbxAMatrix& matNodeTransform, int iBoneIdx, tContainer* pContainer );

	int FindBoneIndex( string strBoneName );
	FbxAMatrix GetTransform( FbxNode* pNode );

	void CheckWeightAndIndices( FbxMesh* pMesh, tContainer* pContainer );
};

