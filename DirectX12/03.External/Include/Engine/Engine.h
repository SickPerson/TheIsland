#pragma once

// simd 사용 안함
#define _XM_NO_INTRINSICS_

#include <d3d12.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include "SimpleMath.h"


#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

typedef DirectX::SimpleMath::Vector2 Vec2;
typedef DirectX::SimpleMath::Vector3 Vec3;
typedef DirectX::SimpleMath::Vector4 Vec4;
using DirectX::SimpleMath::Matrix;


#include <string>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "Flag.h"
#include "Macro.h" // 매크로
#include "Types.h"
#include "d3dx12.h"
#include "Ptr.h"
#include "Extern.h"
