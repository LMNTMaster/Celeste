#pragma once


#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

using namespace DirectX;
using namespace DirectX::PackedVector;

// #ifdef : #define 으로 설정된것이 있는지 체크한다.
#ifdef LMNT_EXPORT
#define	LMNT_DLL	__declspec(dllexport)
#else	
#define	LMNT_DLL	__declspec(dllimport)
#endif // LMNT_EXPORT

#define	LMNT_BEGIN	namespace LMNT	{
#define	LMNT_END		}
#define	LMNT_USING	using namespace LMNT;

#define	SAFE_DELETE(p)	if(p)	{ delete p; p = NULL; }
#define	SAFE_DELETE_ARRAY(p)	if(p)	{ delete[] p; p = NULL; }
#define	SAFE_RELEASE(p)			if(p)	{ p->Release(); p = NULL; }

#define	DECLARE_SINGLE(Type)	\
	private:\
		static Type*	m_pInstance;\
	public:\
		static Type* GetInstance()\
		{\
			if(!m_pInstance)\
				m_pInstance = new Type;\
			return m_pInstance;\
		}\
		static void DestroyInstance()\
		{\
			SAFE_DELETE(m_pInstance);\
		}\
	private:\
		Type();\
		~Type();

#define	DEFINITION_SINGLE(Type)	Type* Type::m_pInstance	= NULL;
#define	GET_SINGLE(Type)	Type::GetInstance()
#define	DESTROY_SINGLE(Type)	Type::DestroyInstance()

#define	WINDOWINSTANCE	GET_SINGLE(CCore)->GetWindowInstance()
#define	WINDOWHANDLE	GET_SINGLE(CCore)->GetWindowHandle()
#define	WINDOWRESOLUTION	GET_SINGLE(CCore)->GetWindowResolution()

#define	DEVICE	GET_SINGLE(CDevice)->GetDevice()
#define	DEVICE_CONTEXT	GET_SINGLE(CDevice)->GetContext()
#define	DEVICE_RESOLUTION	GET_SINGLE(CDevice)->GetDeviceResolution()

#define GETKEYDOWN(key) GET_SINGLE(CInput)->GetKeyDown(key)
#define GETKEY(key) GET_SINGLE(CInput)->GetKey(key)
#define GETKEYUP(key) GET_SINGLE(CInput)->GetKeyUp(key)

#define GETASYNCKEYDOWN(key) GET_SINGLE(CInput)->GetAsyncKeyDown(key)
#define GETASYNCKEY(key) GET_SINGLE(CInput)->GetAsyncKey(key)
#define GETASYNCKEYUP(key) GET_SINGLE(CInput)->GetAsyncKeyUp(key)