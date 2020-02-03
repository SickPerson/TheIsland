#pragma once

enum class RES_TYPE
{
	MATERIAL,
	MESH,
	TEXTURE,
	SOUND,
	SHADER,
	END
};

enum class ROOT_SIG_TYPE
{
	INPUT_ASSEM,
	CONST_BUFFER,
	END
};

enum class RS_TYPE
{
	DEFAULT,
	END
};

enum class BLEND_TYPE
{
	DEFAULT,
	END
};


enum class CONST_REGISTER
{
	b0,
	b1,
	b2,
	b3,
	b4,
	END
};

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
