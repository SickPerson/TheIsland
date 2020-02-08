#pragma once

// Resource Type
enum class RES_TYPE
{
	MATERIAL,
	MESH,
	TEXTURE,
	SOUND,
	SHADER,
	END
};

// RootSignature Type
enum class ROOT_SIG_TYPE
{
	INPUT_ASSEM,
	CONST_BUFFER,
	END
};

// Rasterizer Type
enum class RS_TYPE
{
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE,
	WIRE_FRAME,
	END
};

// Blend Type
enum class BLEND_TYPE
{
	DEFAULT,
	ALPHABLEND,
	ONEBLEND,
	END
};

// DepthStencil Type
enum class DEPTH_STENCIL_TYPE
{
	DEFAULT,
	LESS_EQUAL,
	GRATER,
	GRATER_EQUAL,
	END,
};

// Constant Register
enum class CONST_REGISTER
{
	b0,
	b1,
	b2,
	b3,
	b4,
	END
};

// Texture Register
enum class TEXTURE_REGISTER
{
	
	t0	= (UINT)CONST_REGISTER::END,
	t1,
	t2,
	t3,
	t4,
	t5,
	t6,
	t7,
	t8,
	t9,
	t10,
	t11,
	t12,
	END
};

// Component Type
enum class COMPONENT_TYPE
{
	TRANSFORM,
	MESHRENDER,
	CAMERA,
	COLLIDER2D,
	COLLIDER3D,
	ANIMATOR2D,
	ANIMATOR3D,
	LIGHT2D,
	LIGHT3D,
	END,
	SCRIPT,
};

// Shader Type
enum class SHADER_TYPE
{
	ST_VERTEX = 0x01,
	ST_HULL = 0x02,
	ST_DOMAIN = 0x04,
	ST_GEOMETRY = 0x08,
	ST_COMPUTE = 0x10,
	ST_PIXEL = 0x20,
	END
};

// Shader Param
enum class SHADER_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,
	INT_END,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,
	FLOAT_END,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,
	VEC2_END,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,
	VEC4_END,

	MATRIX_0,
	MATRIX_1,
	MATRIX_2,
	MATRIX_3,
	MATRIX_END,

	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_END,
};

// Collider2D Type
enum class COLLIDER2D_TYPE
{
	RECT,
	CIRCLE,
};

// Debug Type
enum class DBG_TYPE
{
	DBG_ERROR,
	DBG_WARNNING,
	DBG_MSG,
};

// Event Type
enum class EVENT_TYPE
{
	CREATE_OBJECT,			// wParam : GameObject, lParam : Layer Idx
	DELETE_OBJECT,			// wParam : GameObject,
	ADD_CHILD,				// wParam : Parent Object, lParam : Child Object
	CLEAR_PARENT,			// wParam : Target Object
	TRANSFER_LAYER,			// wParam : Target Object, lParam : (HIWORD)Layer Index (LOWORD)bMoveAll
	TRANSFER_SCENE,
	ACTIVATE_GAMEOBJECT,	// wParam : GameObject Adress
	DEACTIVATE_GAMEOBJECT,	// wParam : GameObject Adress

	ACTIVATE_COMPONENT,		// wParam : Component Adress
	DEACTIVATE_COMPONENT,	// wParam : Component Adress

	END,
};

// Light Type
enum class LIGHT_TYPE
{
	DIR,
	POINT,
	SPOT,
	END,
};

// Dir Type
enum class DIR_TYPE
{
	RIGHT,
	UP,
	FRONT,
	END,
};

// Face Type
enum class FACE_TYPE
{
	FT_NEAR,
	FT_FAR,
	FT_UP,
	FT_DOWN,
	FT_LEFT,
	FT_RIGHT,
	END
};

// RenderTarget Type
enum class RT_TYPE
{
	SWAPCHAIN,
	DIFFUSE,
	NORMAL,
	POSITION,
	LIGHT,
	SPECULAR,
	SHADOWMAP,
	POSTEFFECT1,
	POSTEFFECT2,
	END
};

// MRT Type
enum class MRT_TYPE
{
	// MRT				RTÁ¶ÇÕ
	SWAPCHAIN,		// SWAPCHAIN
	DEFFERED,		// DIFFUSE, NORMAL, POSIOTION
	LIGHT,			// LIGHT, SPECULAR
	SHADOWMAP,		// SHADOWMAP
	POSTEFFECT,		// POSTEFFECT1, POSTEFFECT2
	END
};