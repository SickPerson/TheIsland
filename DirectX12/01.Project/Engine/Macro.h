#pragma once

#define	SAFE_DELETE(p)	if(p)	{ delete p; p = NULL; }
#define	SAFE_RELEASE(p)	if(p)	{ p->Release(); p = NULL; }

#define	DECLARE_SINGLE(Type)	\
	private:\
		static Type*	m_pInst;\
	public:\
		static Type* GetInst()\
		{\
			if(!m_pInst)\
				m_pInst	= new Type;\
			return m_pInst;\
		}\
		static void DestroyInst()\
		{\
			SAFE_DELETE(m_pInst);\
		}\
	private:\
		Type();\
		~Type();

#define	DEFINITION_SINGLE(Type)	Type* Type::m_pInst	= NULL;
#define	GET_SINGLE(Type)		Type::GetInst()
#define	DESTROY_SINGLE(Type)	Type::DestroyInst()

#define DEVICE CDevice::GetInst()->GetDevice()
#define CMDLIST CDevice::GetInst()->GetCmdList()
#define CMDLIST_RES CDevice::GetInst()->GetCmdListRes()
#define ROOTSIGNATURE(Type) CDevice::GetInst()->GetRootSignature(Type)

#define KEY(Key, State) (CKeyManager::GetInst()->GetKeyState(Key) == State)
#define KEY_HOLD(Key) KEY(Key, KEY_STATE::STATE_HOLD)
#define KEY_TAB(Key) KEY(Key, KEY_STATE::STATE_TAB)
#define KEY_AWAY(Key) KEY(Key, KEY_STATE::STATE_AWAY)
#define KEY_NONE(Key) KEY(Key, KEY_STATE::STATE_NONE)

#define DT CTimeManager::GetInst()->GetDeltaTime()

#define CLONE(Type) public: Type* Clone() { return new Type(*this); }
#define CLONE_DISABLE(Type) Type* Clone() { assert(NULL); return NULL; }\
							Type(const Type& type) { assert(NULL); }

typedef DirectX::SimpleMath::Vector2 Vec2;
typedef DirectX::SimpleMath::Vector3 Vec3;
typedef DirectX::SimpleMath::Vector4 Vec4;
using DirectX::SimpleMath::Matrix;

// Max Layer
#define MAX_LAYER 32

// PathKey
#define	BASE_PATH		"BasePath"
#define	SHADER_PATH		"ShaderPath"
#define	TEXTURE_PATH	"TexturePath"
#define	DATA_PATH		"DataPath"
#define	SOUND_PATH		"SoundPath"
#define	MESH_PATH		"MeshPath"

template<typename T1, typename T2>
void Safe_Delete_Map( map<T1, T2>& map )
{
	for ( auto& pair : map )
	{
		SAFE_DELETE( pair.second );
	}

	map.clear();
}

template<typename T, int iSize>
void Safe_Delete_Array( T*( &arr )[iSize] )
{
	for ( UINT i = 0; i < iSize; ++i )
	{
		SAFE_DELETE( arr[i] );
	}
}

template<typename T>
void Safe_Delete_Vector( vector<T*> vec )
{
	for ( UINT i = 0; i < vec.size(); ++i )
	{
		SAFE_DELETE( vec[i] );
	}
}

namespace RES_TYPE_STR
{
	extern const wchar_t*	MATERIAL;
	extern const wchar_t*	MESH;
	extern const wchar_t*	TEXTURE;
	extern const wchar_t*	SOUND;
	extern const wchar_t*	SHADER;
}

extern const wchar_t* RES_TYPE_NAME[( UINT )RES_TYPE::END];

#define ST_ALL ( UINT )SHADER_TYPE::ST_VERTEX | ( UINT )SHADER_TYPE::ST_HULL | ( UINT )SHADER_TYPE::ST_DOMAIN | ( UINT )SHADER_TYPE::ST_GEOMETRY | ( UINT )SHADER_TYPE::ST_COMPUTE | ( UINT )SHADER_TYPE::ST_PIXEL