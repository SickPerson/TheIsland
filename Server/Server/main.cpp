#include "stdafx.h"
#include "Core.h"

#ifdef _DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif // _DEBUG

int main() {
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

	CCore::GetInst()->Initalize();
	CCore::GetInst()->StartServer();
	CCore::GetInst()->CloseServer();
	CCore::GetInst()->DestroyInst();

	_CrtDumpMemoryLeaks();
}