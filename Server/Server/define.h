#pragma once

#define	SINGLE(type)\
		private:\
			type();\
			~type();\
		public:\
			static	Type*	GetInst()\
			{\
					static type mgr;\
					return &mgr;\
			}\

#define		SAFE_DELETE(p)	if(nullptr != p) p->Release()
#define		SAFE_RELEASE(p)	if(nullptr != p) delete p; p = nullptr;