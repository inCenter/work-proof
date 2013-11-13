// XMSAALib.h  Version 1.0
//
// Author:  Hans Dietrich
//          hdietrich@gmail.com
//
// License:
//     This software is released under the Code Project Open License (CPOL),
//     which may be found here:  http://www.codeproject.com/info/eula.aspx
//     You are free to use this software in any way you like, except that you 
//     may not sell this source code.
//
//     This software is provided "as is" with no expressed or implied warranty.
//     I accept no liability for any damage or loss of business that this 
//     software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef XMSAALIB_H
#define XMSAALIB_H

#include <winuser.h>
#include "oleacc.h"

typedef void (CALLBACK *XWINEVENTURLPROC)(IAccessible *pAccChild, 
										  VARIANT *pvarChild,
										  DWORD event, 
										  HWND hwnd);

typedef BOOL (CALLBACK *XENUMACCESSIBLEPROC)(IAccessible *pAccChild, 
											 VARIANT *pvarChild,
											 HWND hwndChild, 
											 int nLevel, 
											 LPARAM lParam);

//=============================================================================
class CXMSAALib
//=============================================================================
{
// Construction
public:
	CXMSAALib();
	virtual ~CXMSAALib();

	// Attributes
public:

// Operations
public:
	static BOOL EnumAccessible(HWND hwnd, XENUMACCESSIBLEPROC lpEnumAccessibleProc, LPARAM lParam);

	static BOOL GetObjectChildCount(IAccessible *pAcc, long& nCount);
    static BOOL GetObjectDescription(IAccessible *pAcc, VARIANT *pvarChild, BSTR& bstrDescriptionOut, UINT cchDescription);
	static BOOL GetObjectLocation(IAccessible *pAcc, VARIANT *pvarChild, RECT& rect);
    static BOOL GetObjectName(IAccessible *pAcc, VARIANT *pvarChild, BSTR& bstrNameOut, UINT cchName);
    static BOOL GetObjectRole(IAccessible *pAcc, VARIANT *pvarChild, UINT& nRole);
    static BOOL GetObjectState(IAccessible* pAcc, VARIANT* pvarChild, UINT& nState);
    static BOOL GetObjectValue(IAccessible *pAcc, VARIANT *pvarChild, BSTR& bstrValueOut, UINT cchValue);


// Implementation
protected:
	HWINEVENTHOOK m_eHook;

	static BOOL FindChild(IAccessible *pIAcc, IAccessible **pIAccChild, VARIANT *varChild, 
			XENUMACCESSIBLEPROC lpEnumAccessibleProc, LPARAM lParam, BOOL bFirstTime);
};

#endif //XMSAALIB_H
