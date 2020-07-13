#include "stdafx.h"
#include "FBXLoader.h"

#include "Mesh.h"

#include "ResMgr.h"
#include "PathMgr.h"

#include "Material.h"

CFBXLoader::CFBXLoader() :
	m_pManager(NULL),
	m_pScene(NULL),
	m_pImporter(NULL),
	m_bMixamo(false)
{
}


CFBXLoader::~CFBXLoader()
{
	m_pScene->Destroy();
	m_pManager->Destroy();

	for ( UINT i = 0; i < m_vecBone.size(); ++i )
	{
		SAFE_DELETE( m_vecBone[i] );
	}

	for ( UINT i = 0; i < m_vecAnimClip.size(); ++i )
	{
		SAFE_DELETE( m_vecAnimClip[i] );
	}

	for ( int i = 0; i < m_arrAnimName.Size(); ++i )
	{
		SAFE_DELETE( m_arrAnimName[i] );
	}
}

void CFBXLoader::Init()
{
	m_pManager = FbxManager::Create();

	if ( NULL == m_pManager )
		assert( NULL );

	FbxIOSettings* pIOSettings = FbxIOSettings::Create( m_pManager, IOSROOT );
	m_pManager->SetIOSettings( pIOSettings );

	m_pScene = FbxScene::Create( m_pManager, "" );
	if ( NULL == m_pScene )
		assert( NULL );
}

void CFBXLoader::LoadFbx( const wstring & wstrPath )
{
	m_vecContainer.clear();

	m_pImporter = FbxImporter::Create( m_pManager, "" );

	string strPath( wstrPath.begin(), wstrPath.end() );

	if (!m_pImporter->Initialize(strPath.c_str(), -1, m_pManager->GetIOSettings()))
	{
		FbxStatus eStatus = m_pImporter->GetStatus();
		assert(nullptr);
	}
		
	m_pImporter->Import( m_pScene );

	m_pScene->GetGlobalSettings().SetAxisSystem( FbxAxisSystem::Max );

	// Bone 정보 읽기
	LoadSkeleton( m_pScene->GetRootNode() );

	// Animation 이름정보
	m_pScene->FillAnimStackNameArray( m_arrAnimName );

	// Animation Clip 정보
	LoadAnimationClip();

	// 삼각화(Triangulate)
	Triangulate( m_pScene->GetRootNode() );

	// 메쉬 데이터 얻기
	LoadMeshDataFromNode( m_pScene->GetRootNode() );

	m_pImporter->Destroy();

	// 필요한 텍스쳐 로드
	LoadTexture();

	// 필요한 메테리얼 생성
	CreateMaterial();
}

int CFBXLoader::GetContainerCount()
{
	return (int)m_vecContainer.size();
}

const tContainer & CFBXLoader::GetContainer( int iIdx )
{
	return m_vecContainer[iIdx];
}

vector<tFbxBone*>& CFBXLoader::GetBones()
{
	return m_vecBone;
}

vector<tFbxAnimClip*>& CFBXLoader::GetAnimClip()
{
	return m_vecAnimClip;
}

void CFBXLoader::LoadMeshDataFromNode( FbxNode * pNode )
{
	// 노드의 메쉬정보 읽기
	FbxNodeAttribute* pAttr = pNode->GetNodeAttribute();

	if ( pAttr && FbxNodeAttribute::eMesh == pAttr->GetAttributeType() )
	{
		FbxMesh* pMesh = pNode->GetMesh();

		if ( NULL != pMesh )
			LoadMesh( pMesh );
	}

	UINT iMtrlCnt = pNode->GetMaterialCount();

	if ( iMtrlCnt > 0 )
	{
		for ( UINT i = 0; i < iMtrlCnt; ++i )
		{
			FbxSurfaceMaterial* pMtrlSur = pNode->GetMaterial( i );
			LoadMaterial( pMtrlSur );
		}
	}

	// 자식 노드 정보 읽기
	int iChildCnt = pNode->GetChildCount();
	for ( int i = 0; i < iChildCnt; ++i )
		LoadMeshDataFromNode( pNode->GetChild( i ) );
}

void CFBXLoader::LoadMesh( FbxMesh * pFbxMesh )
{
	m_vecContainer.push_back( tContainer{} );
	tContainer& Container = m_vecContainer[m_vecContainer.size() - 1];

	string strName = pFbxMesh->GetName();
	Container.strName = wstring( strName.begin(), strName.end() );

	int iVtxCnt = pFbxMesh->GetControlPointsCount();
	Container.Resize( iVtxCnt );

	FbxVector4* pFbxPos = pFbxMesh->GetControlPoints();

	for ( int i = 0; i < iVtxCnt; ++i )
	{
		Container.vecPos[i].x = ( float )pFbxPos[i].mData[0];
		Container.vecPos[i].y = ( float )pFbxPos[i].mData[2];
		Container.vecPos[i].z = ( float )pFbxPos[i].mData[1];
	}

	// 폴리곤 개수
	int iPolyCnt = pFbxMesh->GetPolygonCount();

	// 재질의 개수 ( ==> SubSet 개수 ==> Index Buffer Count)
	int iMtrlCnt = pFbxMesh->GetNode()->GetMaterialCount();
	Container.vecIdx.resize( iMtrlCnt );

	// 정점 정보가 속한 subset 을 알기위해서...
	FbxGeometryElementMaterial* pMtrl = pFbxMesh->GetElementMaterial();

	// 폴리곤을 구성하는 정점 개수
	int iPolySize = pFbxMesh->GetPolygonSize( 0 );
	if ( 3 != iPolySize )
		assert( NULL ); // Polygon 구성 정점이 3개가 아닌 경우

	UINT arrIdx[3] = {};
	UINT iVtxOrder = 0; // 폴리곤 순서로 접근하는 순번

	for ( int i = 0; i < iPolyCnt; ++i )
	{
		for ( int j = 0; j < iPolySize; ++j )
		{
			// i 번째 폴리곤에, j 번째 정점
			int iIdx = pFbxMesh->GetPolygonVertex( i, j );
			arrIdx[j] = iIdx;

			GetNormal( pFbxMesh, &Container, iIdx, iVtxOrder );
			GetTangent( pFbxMesh, &Container, iIdx, iVtxOrder );
			GetBinormal( pFbxMesh, &Container, iIdx, iVtxOrder );
			GetUV( pFbxMesh, &Container, iIdx, pFbxMesh->GetTextureUVIndex( i, j ) );

			++iVtxOrder;
		}
		UINT iSubsetIdx = pMtrl->GetIndexArray().GetAt( i );
		Container.vecIdx[iSubsetIdx].push_back( arrIdx[0] );
		Container.vecIdx[iSubsetIdx].push_back( arrIdx[2] );
		Container.vecIdx[iSubsetIdx].push_back( arrIdx[1] );
	}

	LoadAnimationData( pFbxMesh, &Container );
}

void CFBXLoader::LoadMaterial( FbxSurfaceMaterial * pMtrlSur )
{
	tFbxMaterial tMtrlInfo{};

	string str = pMtrlSur->GetName();
	tMtrlInfo.strMtrlName = wstring( str.begin(), str.end() );

	// Diff
	tMtrlInfo.tMtrl.vMtrlDiff = GetMtrlData( pMtrlSur
		, FbxSurfaceMaterial::sDiffuse
		, FbxSurfaceMaterial::sDiffuseFactor );

	// Amb
	tMtrlInfo.tMtrl.vMtrlAmb = GetMtrlData( pMtrlSur
		, FbxSurfaceMaterial::sAmbient
		, FbxSurfaceMaterial::sAmbientFactor );

	// Spec
	tMtrlInfo.tMtrl.vMtrlSpec = GetMtrlData( pMtrlSur
		, FbxSurfaceMaterial::sSpecular
		, FbxSurfaceMaterial::sSpecularFactor );

	// Emisv
	tMtrlInfo.tMtrl.vMtrlEmiv = GetMtrlData( pMtrlSur
		, FbxSurfaceMaterial::sEmissive
		, FbxSurfaceMaterial::sEmissiveFactor );

	// Texture Name
	tMtrlInfo.strDiff = GetMtrlTextureName( pMtrlSur, FbxSurfaceMaterial::sDiffuse );
	tMtrlInfo.strNormal = GetMtrlTextureName( pMtrlSur, FbxSurfaceMaterial::sNormalMap );
	tMtrlInfo.strSpec = GetMtrlTextureName( pMtrlSur, FbxSurfaceMaterial::sSpecular );

	m_vecContainer.back().vecMtrl.push_back( tMtrlInfo );
}

void CFBXLoader::GetTangent( FbxMesh * pMesh, tContainer * pContainer, int iIdx, int iVtxOrder )
{
	int iTangentCnt = pMesh->GetElementTangentCount();
	if ( 1 != iTangentCnt )
 		assert( NULL ); // 정점 1개가 포함하는 탄젠트 정보가 2개 이상이다.

	// 탄젠트 data 의 시작 주소
	FbxGeometryElementTangent* pTangent = pMesh->GetElementTangent();
	UINT iTangentIdx = 0;

	if ( pTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex )
	{
		if ( pTangent->GetReferenceMode() == FbxGeometryElement::eDirect )
			iTangentIdx = iVtxOrder;
		else
			iTangentIdx = pTangent->GetIndexArray().GetAt( iVtxOrder );
	}
	else if ( pTangent->GetMappingMode() == FbxGeometryElement::eByControlPoint )
	{
		if ( pTangent->GetReferenceMode() == FbxGeometryElement::eDirect )
			iTangentIdx = iIdx;
		else
			iTangentIdx = pTangent->GetIndexArray().GetAt( iIdx );
	}

	FbxVector4 vTangent = pTangent->GetDirectArray().GetAt( iTangentIdx );

	pContainer->vecTangent[iIdx].x = ( float )vTangent.mData[0];
	pContainer->vecTangent[iIdx].y = ( float )vTangent.mData[2];
	pContainer->vecTangent[iIdx].z = ( float )vTangent.mData[1];
}

void CFBXLoader::GetBinormal( FbxMesh * pMesh, tContainer * pContainer, int iIdx, int iVtxOrder )
{
	int iBinormalCnt = pMesh->GetElementBinormalCount();
	if ( 1 != iBinormalCnt )
		assert( NULL ); // 정점 1개가 포함하는 종법선 정보가 2개 이상이다.

	// 종법선 data 의 시작 주소
	FbxGeometryElementBinormal* pBinormal = pMesh->GetElementBinormal();
	UINT iBinormalIdx = 0;

	if ( pBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex )
	{
		if ( pBinormal->GetReferenceMode() == FbxGeometryElement::eDirect )
			iBinormalIdx = iVtxOrder;
		else
			iBinormalIdx = pBinormal->GetIndexArray().GetAt( iVtxOrder );
	}
	else if ( pBinormal->GetMappingMode() == FbxGeometryElement::eByControlPoint )
	{
		if ( pBinormal->GetReferenceMode() == FbxGeometryElement::eDirect )
			iBinormalIdx = iIdx;
		else
			iBinormalIdx = pBinormal->GetIndexArray().GetAt( iIdx );
	}
		
	FbxVector4 vBinormal = pBinormal->GetDirectArray().GetAt( iBinormalIdx );

	pContainer->vecBinormal[iIdx].x = ( float )vBinormal.mData[0];
	pContainer->vecBinormal[iIdx].y = ( float )vBinormal.mData[2];
	pContainer->vecBinormal[iIdx].z = ( float )vBinormal.mData[1];
}

void CFBXLoader::GetNormal( FbxMesh * pMesh, tContainer * pContainer, int iIdx, int iVtxOrder )
{
	int iNormalCnt = pMesh->GetElementNormalCount();
	if ( 1 != iNormalCnt )
		assert( NULL ); // 정점 1개가 포함하는 종법선 정보가 2개 이상이다.

					  // 종법선 data 의 시작 주소
	FbxGeometryElementNormal* pNormal = pMesh->GetElementNormal();
	UINT iNormalIdx = 0;

	if ( pNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex )
	{
		if ( pNormal->GetReferenceMode() == FbxGeometryElement::eDirect )
			iNormalIdx = iVtxOrder;
		else
			iNormalIdx = pNormal->GetIndexArray().GetAt( iVtxOrder );
	}
	else if ( pNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint )
	{
		if ( pNormal->GetReferenceMode() == FbxGeometryElement::eDirect )
			iNormalIdx = iIdx;
		else
			iNormalIdx = pNormal->GetIndexArray().GetAt( iIdx );
	}

	FbxVector4 vNormal = pNormal->GetDirectArray().GetAt( iNormalIdx );

	pContainer->vecNormal[iIdx].x = ( float )vNormal.mData[0];
	pContainer->vecNormal[iIdx].y = ( float )vNormal.mData[2];
	pContainer->vecNormal[iIdx].z = ( float )vNormal.mData[1];
}

void CFBXLoader::GetUV( FbxMesh * pMesh, tContainer * pContainer, int iIdx, int iUVIndex )
{
	FbxGeometryElementUV* pUV = pMesh->GetElementUV();

	UINT iUVIdx = 0;
	if ( pUV->GetReferenceMode() == FbxGeometryElement::eDirect )
		iUVIdx = iIdx;
	else
		iUVIdx = pUV->GetIndexArray().GetAt( iIdx );

	iUVIdx = iUVIndex;
	FbxVector2 vUV = pUV->GetDirectArray().GetAt( iUVIdx );
	pContainer->vecUV[iIdx].x = ( float )vUV.mData[0];
	pContainer->vecUV[iIdx].y = 1.f - ( float )vUV.mData[1]; // fbx uv 좌표계는 좌하단이 0,0
}

Vec4 CFBXLoader::GetMtrlData( FbxSurfaceMaterial * pSurface, const char * pMtrlName, const char * pMtrlFactorName )
{
	FbxDouble3  vMtrl;
	FbxDouble	dFactor = 0.f;

	FbxProperty tMtrlProperty = pSurface->FindProperty( pMtrlName );
	FbxProperty tMtrlFactorProperty = pSurface->FindProperty( pMtrlFactorName );

	if ( tMtrlProperty.IsValid() && tMtrlFactorProperty.IsValid() )
	{
		vMtrl = tMtrlProperty.Get<FbxDouble3>();
		dFactor = tMtrlFactorProperty.Get<FbxDouble>();
	}

	Vec4 vRetVal = Vec4( vMtrl.mData[0] * dFactor, vMtrl.mData[1] * dFactor, vMtrl.mData[2] * dFactor, dFactor );
	return vRetVal;
}

wstring CFBXLoader::GetMtrlTextureName( FbxSurfaceMaterial * pSurface, const char * pMtrlProperty )
{
	string strName;

	FbxProperty TextureProperty = pSurface->FindProperty( pMtrlProperty );
	if ( TextureProperty.IsValid() )
	{
		UINT iCnt = TextureProperty.GetSrcObjectCount();

		if ( 1 <= iCnt )
		{
			FbxFileTexture* pFbxTex = TextureProperty.GetSrcObject<FbxFileTexture>( 0 );
			if ( NULL != pFbxTex )
				strName = pFbxTex->GetFileName();
		}
	}

	return wstring( strName.begin(), strName.end() );
}

void CFBXLoader::LoadTexture()
{
	wstring strResPath = CPathMgr::GetResPath();

	for ( UINT i = 0; i < m_vecContainer.size(); ++i )
	{
		for ( UINT j = 0; j < m_vecContainer[i].vecMtrl.size(); ++j )
		{
			wstring strPath;
			wstring strFileName;

			strPath = CPathMgr::GetRelativePath( m_vecContainer[i].vecMtrl[j].strDiff.c_str() );
			strFileName = CPathMgr::GetFileName( m_vecContainer[i].vecMtrl[j].strDiff.c_str() );
			CResMgr::GetInst()->Load<CTexture>( strFileName, strPath );

			if (m_vecContainer[i].vecMtrl[j].strNormal.size() == 0)
			{
				continue;
			}
			else
			{
				strPath = CPathMgr::GetRelativePath(m_vecContainer[i].vecMtrl[j].strNormal.c_str());
				strFileName = CPathMgr::GetFileName(m_vecContainer[i].vecMtrl[j].strNormal.c_str());
				CResMgr::GetInst()->Load<CTexture>(strFileName, strPath);
			}

			if (m_vecContainer[i].vecMtrl[j].strSpec.size() == 0)
			{
				continue;
			}
			else
			{
				strPath = CPathMgr::GetRelativePath(m_vecContainer[i].vecMtrl[j].strSpec.c_str());
				strFileName = CPathMgr::GetFileName(m_vecContainer[i].vecMtrl[j].strSpec.c_str());
				CResMgr::GetInst()->Load<CTexture>(strFileName, strPath);
			}
		}
	}
}

void CFBXLoader::CreateMaterial()
{
	wstring strKey;
	wstring strPath;

	for ( UINT i = 0; i < m_vecContainer.size(); ++i )
	{
		for ( UINT j = 0; j < m_vecContainer[i].vecMtrl.size(); ++j )
		{
			CMaterial* pMaterial = new CMaterial;

			// Material 이름짓기
			strKey = m_vecContainer[i].vecMtrl[j].strMtrlName;
			if ( strKey.empty() )
				strKey = CPathMgr::GetFileName( m_vecContainer[i].vecMtrl[j].strDiff.c_str() );

			strPath = L"Material\\";
			strPath += strKey;
			strPath += L".mtrl";

			pMaterial->SetName( strKey );
			pMaterial->SetPath( strPath );

			pMaterial->SetShader( CResMgr::GetInst()->FindRes<CShader>( L"Std3DShader" ) );

			wstring strTexKey = CPathMgr::GetFileName( m_vecContainer[i].vecMtrl[j].strDiff.c_str() );
			Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>( strTexKey );
			if ( NULL != pTex )
				pMaterial->SetData( SHADER_PARAM::TEX_0, pTex.GetPointer() );

			strTexKey = CPathMgr::GetFileName( m_vecContainer[i].vecMtrl[j].strNormal.c_str() );
			pTex = CResMgr::GetInst()->FindRes<CTexture>( strTexKey );
			if ( NULL != pTex )
				pMaterial->SetData( SHADER_PARAM::TEX_1, pTex.GetPointer() );

			strTexKey = CPathMgr::GetFileName( m_vecContainer[i].vecMtrl[j].strSpec.c_str() );
			pTex = CResMgr::GetInst()->FindRes<CTexture>( strTexKey );
			if ( NULL != pTex )
				pMaterial->SetData( SHADER_PARAM::TEX_2, pTex.GetPointer() );


			CResMgr::GetInst()->AddRes<CMaterial>( pMaterial->GetName(), pMaterial );
		}
	}
}

void CFBXLoader::LoadSkeleton( FbxNode * pNode )
{
	int iChildCount = pNode->GetChildCount();

	LoadSkeleton_Re( pNode, 0, 0, -1 );
}

void CFBXLoader::LoadSkeleton_Re( FbxNode * pNode, int iDepth, int iIdx, int iParentIdx )
{
	FbxNodeAttribute* pAttr = pNode->GetNodeAttribute();

	if ( pAttr && pAttr->GetAttributeType() == FbxNodeAttribute::eSkeleton )
	{
		tFbxBone* pBone = new tFbxBone;

		string strBoneName = pNode->GetName();
		if ( m_bMixamo )
			pBone->strBoneName.erase( 0, 10 );
		pBone->strBoneName = wstring( strBoneName.begin(), strBoneName.end() );
		pBone->iDepth = iDepth++;
		pBone->iParentIndx = iParentIdx;

		m_vecBone.push_back( pBone );
	}

	int iChildCount = pNode->GetChildCount();
	for ( int i = 0; i < iChildCount; ++i )
	{
		LoadSkeleton_Re( pNode->GetChild( i ), iDepth, ( int )m_vecBone.size(), iIdx );
	}
}

void CFBXLoader::LoadAnimationClip()
{
	int iAnimCount = m_arrAnimName.GetCount();

	for ( int i = 0; i < iAnimCount; ++i )
	{
		FbxAnimStack* pAnimStack = m_pScene->FindMember<FbxAnimStack>( m_arrAnimName[i]->Buffer() );

		if ( !pAnimStack )
			continue;

		tFbxAnimClip* pAnimClip = new tFbxAnimClip;

		string strClipName = pAnimStack->GetName();

		if ( strClipName == "mixamo.com" )
			m_bMixamo = true;

		pAnimClip->strName = wstring( strClipName.begin(), strClipName.end() );

		

		FbxTakeInfo* pTakeInfo = m_pScene->GetTakeInfo( pAnimStack->GetName() );
		pAnimClip->tStartTime = pTakeInfo->mLocalTimeSpan.GetStart();
		pAnimClip->tEndTime = pTakeInfo->mLocalTimeSpan.GetStop();

		pAnimClip->eMode = m_pScene->GetGlobalSettings().GetTimeMode();
		pAnimClip->llTimeLength = pAnimClip->tEndTime.GetFrameCount( pAnimClip->eMode ) - pAnimClip->tStartTime.GetFrameCount( pAnimClip->eMode );

		m_vecAnimClip.push_back( pAnimClip );
	}
}

void CFBXLoader::Triangulate( FbxNode * pNode )
{
	FbxNodeAttribute* pAttr = pNode->GetNodeAttribute();

	if ( pAttr &&
		( pAttr->GetAttributeType() == FbxNodeAttribute::eMesh
			|| pAttr->GetAttributeType() == FbxNodeAttribute::eNurbs
			|| pAttr->GetAttributeType() == FbxNodeAttribute::eNurbsSurface ) )
	{
		FbxGeometryConverter converter( m_pManager );
		converter.Triangulate( pAttr, true );
	}

	int iChildCount = pNode->GetChildCount();

	for ( int i = 0; i < iChildCount; ++i )
	{
		Triangulate( pNode->GetChild( i ) );
	}
}

void CFBXLoader::LoadAnimationData( FbxMesh * pMesh, tContainer * pContainer )
{
	// Animation Data 로드할 필요가 없음
	int iSkinCount = pMesh->GetDeformerCount( FbxDeformer::eSkin );
	if ( iSkinCount <= 0 || m_vecAnimClip.empty() )
		return;

	pContainer->bAnimation = true;

	// Skin 개수만큼 반복을하며 읽는다.	
	for ( int i = 0; i < iSkinCount; ++i )
	{
		FbxSkin* pSkin = ( FbxSkin* )pMesh->GetDeformer( i, FbxDeformer::eSkin );

		if ( pSkin )
		{
			FbxSkin::EType eType = pSkin->GetSkinningType();
			if ( FbxSkin::eRigid == eType || FbxSkin::eLinear )
			{
				// Cluster 를 얻어온다
				// Cluster == Joint == 관절
				int iClusterCount = pSkin->GetClusterCount();

				for ( int j = 0; j < iClusterCount; ++j )
				{
					FbxCluster* pCluster = pSkin->GetCluster( j );

					if ( !pCluster->GetLink() )
						continue;

					string strBoneName = pCluster->GetLink()->GetName();

					if ( m_bMixamo )
						strBoneName.erase( 0, 10 );

					// 현재 본 인덱스를 얻어온다.
					int iBoneIdx = FindBoneIndex( strBoneName );
					if ( -1 == iBoneIdx )
						assert( NULL );

					FbxAMatrix matNodeTransform = GetTransform( pMesh->GetNode() );

					// Weights And Indices 정보를 읽는다.
					LoadWeightsAndIndices( pCluster, iBoneIdx, pContainer );

					// Bone 의 OffSet 행렬 구한다.
					LoadOffsetMatrix( pCluster, matNodeTransform, iBoneIdx, pContainer );

					// Bone KeyFrame 별 행렬을 구한다.
					LoadKeyframeTransform( pMesh->GetNode(), pCluster, matNodeTransform, iBoneIdx, pContainer );
				}
			}
		}
	}
	CheckWeightAndIndices( pMesh, pContainer );
}

void CFBXLoader::LoadWeightsAndIndices( FbxCluster * pCluster, int iBoneIdx, tContainer * pContainer )
{
	int iIndicesCount = pCluster->GetControlPointIndicesCount();

	for (int i = 0; i < iIndicesCount; ++i)
	{
		tWeightsAndIndices tWI = {};

		// 각 정점에게 본 인덱스 정보와, 가중치 값을 알린다.
		tWI.iBoneIdx = iBoneIdx;
		tWI.dWeight = pCluster->GetControlPointWeights()[i];

		int iVtxIdx = pCluster->GetControlPointIndices()[i];

		pContainer->vecWI[iVtxIdx].push_back(tWI);
	}
}

void CFBXLoader::LoadOffsetMatrix( FbxCluster * pCluster, const FbxAMatrix & matNodeTransform, int iBoneIdx, tContainer * pContainer )
{
	FbxAMatrix matClusterTrans;
	FbxAMatrix matClusterLinkTrans;

	pCluster->GetTransformMatrix(matClusterTrans);
	pCluster->GetTransformLinkMatrix(matClusterLinkTrans);

	// Reflect Matrix
	FbxVector4 V0 = { 1, 0, 0, 0 };
	FbxVector4 V1 = { 0, 0, 1, 0 };
	FbxVector4 V2 = { 0, 1, 0, 0 };
	FbxVector4 V3 = { 0, 0, 0, 1 };

	FbxAMatrix matReflect;
	matReflect[0] = V0;
	matReflect[1] = V1;
	matReflect[2] = V2;
	matReflect[3] = V3;

	FbxAMatrix matOffset;
	matOffset = matClusterLinkTrans.Inverse() * matClusterTrans * matNodeTransform;
	matOffset = matReflect * matOffset * matReflect;

	m_vecBone[iBoneIdx]->matOffset = matOffset;
}

void CFBXLoader::LoadKeyframeTransform( FbxNode * pNode, FbxCluster * pCluster, const FbxAMatrix & matNodeTransform, int iBoneIdx, tContainer * pContainer )
{
	if ( m_vecAnimClip.empty() )
		return;

	FbxVector4	v1 = { 1, 0, 0, 0 };
	FbxVector4	v2 = { 0, 0, 1, 0 };
	FbxVector4	v3 = { 0, 1, 0, 0 };
	FbxVector4	v4 = { 0, 0, 0, 1 };
	FbxAMatrix	matReflect;
	matReflect.mData[0] = v1;
	matReflect.mData[1] = v2;
	matReflect.mData[2] = v3;
	matReflect.mData[3] = v4;
	
	if ( m_bMixamo )
	{
		auto iterEnd = m_vecAnimClip.end();
		for ( auto iter = m_vecAnimClip.begin(); iter != iterEnd;)
		{
			if ( ( *iter )->strName != L"mixamo.com" )
			{
				SAFE_DELETE( ( *iter ) );
				iter = m_vecAnimClip.erase( iter );
				iterEnd = m_vecAnimClip.end();
			}

			else
				++iter;
		}
	}

	m_vecBone[iBoneIdx]->matBone = matNodeTransform;

	FbxTime::EMode eTimeMode = m_pScene->GetGlobalSettings().GetTimeMode();

	FbxLongLong llStartFrame = m_vecAnimClip[0]->tStartTime.GetFrameCount( eTimeMode );
	FbxLongLong llEndFrame = m_vecAnimClip[0]->tEndTime.GetFrameCount( eTimeMode );

	for ( FbxLongLong i = llStartFrame; i < llEndFrame; ++i )
	{
		tFbxKeyFrame tFrame = {};
		FbxTime   tTime = 0;

		tTime.SetFrame( i, eTimeMode );

		FbxAMatrix matFromNode = pNode->EvaluateGlobalTransform( tTime ) * matNodeTransform;
		FbxAMatrix matCurTrans = matFromNode.Inverse() * pCluster->GetLink()->EvaluateGlobalTransform( tTime );
		matCurTrans = matReflect * matCurTrans * matReflect;

		tFrame.dTime = tTime.GetSecondDouble();
		tFrame.matTransform = matCurTrans;

		m_vecBone[iBoneIdx]->vecKeyFrame.push_back( tFrame );
	}
}

int CFBXLoader::FindBoneIndex( string strBoneName )
{
	wstring BoneName = wstring( strBoneName.begin(), strBoneName.end() );

	for ( UINT i = 0; i < m_vecBone.size(); ++i )
	{
		if ( m_vecBone[i]->strBoneName == BoneName )
			return i;
	}

	return -1;
}

FbxAMatrix CFBXLoader::GetTransform( FbxNode * pNode )
{
	const FbxVector4 vT = pNode->GetGeometricTranslation( FbxNode::eSourcePivot );
	const FbxVector4 vR = pNode->GetGeometricRotation( FbxNode::eSourcePivot );
	const FbxVector4 vS = pNode->GetGeometricScaling( FbxNode::eSourcePivot );

	return FbxAMatrix( vT, vR, vS );
}

void CFBXLoader::CheckWeightAndIndices( FbxMesh * pMesh, tContainer * pContainer )
{
	vector<vector<tWeightsAndIndices>>::iterator iter = pContainer->vecWI.begin();

	int iVtxIdx = 0;
	for ( iter; iter != pContainer->vecWI.end(); ++iter, ++iVtxIdx )
	{
		if ( ( *iter ).size() > 1 )
		{
			// 가중치 값 순으로 내림차순 정렬
			sort( ( *iter ).begin(), ( *iter ).end()
				, []( const tWeightsAndIndices& left, const tWeightsAndIndices& right )
			{
				return left.dWeight > right.dWeight;
			}
			);

			double dWeight = 0.f;
			for ( UINT i = 0; i < ( *iter ).size(); ++i )
			{
				dWeight += ( *iter )[i].dWeight;
			}

			// 가중치의 합이 1이 넘어가면 처음부분에 더해준다.
			double revision = 0.f;
			if ( dWeight > 1.0 )
			{
				revision = 1.0 - dWeight;
				( *iter )[0].dWeight += revision;
			}

			if ( ( *iter ).size() >= 4 )	
			{
				( *iter ).erase( ( *iter ).begin() + 4, ( *iter ).end() );
			}
		}

		// 정점 정보로 변환, 
		float fWeights[4] = {};
		float fIndices[4] = {};

		for ( UINT i = 0; i < ( *iter ).size(); ++i )
		{
			fWeights[i] = ( float )( *iter )[i].dWeight;
			fIndices[i] = ( float )( *iter )[i].iBoneIdx;
		}

		memcpy( &pContainer->vecWeights[iVtxIdx], fWeights, sizeof( Vec4 ) );
		memcpy( &pContainer->vecIndices[iVtxIdx], fIndices, sizeof( Vec4 ) );
	}
}
