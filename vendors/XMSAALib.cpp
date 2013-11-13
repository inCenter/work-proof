// XMSAALib.cpp  Version 1.0 - see article at CodeProject.com
//
// Author:  Hans Dietrich
//          hdietrich@gmail.com
//
// Description:
//     XMSAALib implements a class to retrieve MSAA information from a window.
//
// History
//     Version 1.0 - 2009 February 10
//     - Initial public release
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

#define _WIN32_WINNT 0x0502
#define WINVER 0x0502

#include <windows.h>
#include <winuser.h>
#include <tchar.h>
#include "XMSAALib.h"


#define SAFE_RELEASE(p_)					{ if (p_ != NULL) p_->Release(); p_ = NULL; }


//=============================================================================
CXMSAALib::CXMSAALib() :
//=============================================================================
	m_eHook(0)
{
}

//=============================================================================
CXMSAALib::~CXMSAALib()
//=============================================================================
{
}

//=============================================================================
BOOL CXMSAALib::GetObjectName(IAccessible *pAcc, 
							  VARIANT *pvarChild, 
                              BSTR &bstrNameOut,
							  UINT cchName)
//=============================================================================
{
	BOOL bRet = FALSE;

    if (pAcc && pvarChild && bstrNameOut)
	{
        bstrNameOut = NULL;
        BSTR bstrName = NULL;
		
		HRESULT hr = pAcc->get_accName(*pvarChild, &bstrName);
		
		if (SUCCEEDED(hr) && bstrName)
		{
            bstrNameOut = bstrName;
			bRet = TRUE;
		}
	}
	
	return bRet;
} 

//=============================================================================
BOOL CXMSAALib::GetObjectValue(IAccessible *pAcc, 
							   VARIANT *pvarChild, 
                               BSTR &bstrValueOut,
							   UINT cchValue)
//=============================================================================
{
	BOOL bRet = FALSE;

    if (pAcc && pvarChild && bstrValueOut)
	{
        bstrValueOut = NULL;
		BSTR bstrValue = NULL;
		
		HRESULT hr = pAcc->get_accValue(*pvarChild, &bstrValue);
		
		if (SUCCEEDED(hr) && bstrValue)
		{
            bstrValueOut = bstrValue;
			bRet = TRUE;
		}
	}
	
	return bRet;
} 

//=============================================================================
BOOL CXMSAALib::GetObjectDescription(IAccessible *pAcc, 
									 VARIANT *pvarChild, 
                                     BSTR& bstrDescriptionOut,
									 UINT cchDescription)
//=============================================================================
{
	BOOL bRet = FALSE;

    if (pAcc && pvarChild && bstrDescriptionOut)
	{
        bstrDescriptionOut = NULL;
		BSTR bstrDescription = NULL;
		
		HRESULT hr = pAcc->get_accDescription(*pvarChild, &bstrDescription);
		
		if (SUCCEEDED(hr) && bstrDescription)
		{
            bstrDescriptionOut = bstrDescription;
			bRet = TRUE;
		}
	}
	
	return bRet;
} 

//=============================================================================
BOOL CXMSAALib::GetObjectChildCount(IAccessible *pAcc, 
									long& nCount)
//=============================================================================
{
	BOOL bRet = FALSE;

	if (pAcc)
	{
		HRESULT hr = pAcc->get_accChildCount(&nCount);
		
		if (SUCCEEDED(hr))
		{
			bRet = TRUE;
		}
	}
	
	return bRet;
} 
//=============================================================================
BOOL CXMSAALib::GetObjectLocation(IAccessible *pAcc,
								  VARIANT *pvarChild, 
								  RECT& rect)
//=============================================================================
{
	BOOL bRet = FALSE;

	if (pAcc && pvarChild)
	{
		HRESULT hr = pAcc->accLocation(&rect.left, &rect.top, 
						&rect.right, &rect.bottom, *pvarChild);
		
		if (SUCCEEDED(hr))
		{
			// accLocation returns width and height
			rect.right += rect.left;
			rect.bottom += rect.top;
			bRet = TRUE;
		}
	}
	
	return bRet;
} 

//=============================================================================
BOOL CXMSAALib::GetObjectRole(IAccessible *pAcc, 
							  VARIANT *pvarChild, 
							  UINT& nRole)
//=============================================================================
{
	BOOL bRet = FALSE;
	
	nRole = 0;

	if (pAcc && pvarChild)
	{
		VARIANT varRole;
		VariantInit(&varRole);
		
		HRESULT hr = pAcc->get_accRole(*pvarChild, &varRole);
		
		if (SUCCEEDED(hr) && (varRole.vt == VT_I4))
		{
			nRole = varRole.lVal;
			bRet = TRUE;
		}
		
		VariantClear(&varRole);
	}
	
	return bRet;
}


//=============================================================================
BOOL CXMSAALib::GetObjectState(IAccessible* pAcc, 
							   VARIANT* pvarChild, 
							   UINT& nState)
//=============================================================================
{
	BOOL bRet = FALSE;

    nState = 0;

	if (pAcc && pvarChild)
	{
		VARIANT varState;
		VariantInit(&varState);
		
		HRESULT hr = pAcc->get_accState(*pvarChild, &varState);
		
		if (SUCCEEDED(hr) && (varState.vt == VT_I4))
		{
			nState = varState.lVal;
			bRet = TRUE;
		}
		
		VariantClear(&varState);
	}
	
	return bRet;
}


//=============================================================================
BOOL CXMSAALib::EnumAccessible(HWND hwnd,
							   XENUMACCESSIBLEPROC lpEnumAccessibleProc, 
							   LPARAM lParam) 
//=============================================================================
{
    BOOL bRet = FALSE;

	if (::IsWindow(hwnd) && lpEnumAccessibleProc)
	{
		IAccessible *pIAcc = NULL;

		HRESULT hr = AccessibleObjectFromWindow(hwnd, OBJID_WINDOW, 
			IID_IAccessible, (void**)&pIAcc);

		if (SUCCEEDED(hr) && pIAcc)
		{
			VARIANT varChild;
			VariantInit(&varChild);
			IAccessible *pIAccChild = NULL;
			FindChild(pIAcc, &pIAccChild, &varChild, lpEnumAccessibleProc, 
                lParam, TRUE);
			SAFE_RELEASE(pIAcc);
			VariantClear(&varChild);
			bRet = TRUE;
		}
    }
	return bRet;
}

//=============================================================================
BOOL CXMSAALib::FindChild(IAccessible *pIAccParent, 
						  IAccessible **pIAccChild, 
						  VARIANT *pvarChild,
						  XENUMACCESSIBLEPROC lpEnumAccessibleProc,
						  LPARAM lParam,
						  BOOL bFirstTime)
//=============================================================================
{
	IEnumVARIANT *pEnum = NULL;
	VARIANT varChild;
	VariantInit(&varChild);
	IAccessible *pCAcc = NULL;
	BOOL bContinue = TRUE;
	static int nLevel = 0;

	if (bFirstTime)
	{
		nLevel = 0;
		varChild.vt = VT_I4;
		varChild.lVal = CHILDID_SELF;
		bContinue = lpEnumAccessibleProc(pIAccParent, &varChild, NULL, 
						nLevel, lParam);
	}

	nLevel++;

	HRESULT hr = pIAccParent->QueryInterface(IID_IEnumVARIANT, (PVOID*) &pEnum);

	if (pEnum)
		pEnum->Reset();

	// get child count
	long nChildren = 0;
	unsigned long nFetched = 0;

    pIAccParent->get_accChildCount(&nChildren);

	for (long index = 1; (index <= nChildren) && bContinue; index++)
    {
		VariantClear(&varChild);
		SAFE_RELEASE(pCAcc);

		if (pEnum)
		{
			hr = pEnum->Next(1, &varChild, &nFetched);
			if (!SUCCEEDED(hr))
			{
				bContinue = FALSE;
				break;
			}
		}
		else
		{
			varChild.vt = VT_I4;
			varChild.lVal = index;
		}

		// get IDispatch interface for the child
		IDispatch *pDisp = NULL;
		if (varChild.vt == VT_I4)
		{
			hr = pIAccParent->get_accChild(varChild, &pDisp);
		}
		else if (varChild.vt == VT_DISPATCH)
		{
			pDisp = varChild.pdispVal;
		}

		// get IAccessible interface for the child
		if (pDisp)
		{
			hr = pDisp->QueryInterface(IID_IAccessible, (void**)&pCAcc);
			SAFE_RELEASE(pDisp);
		}

		// get information about the child
		if (pCAcc)
		{
			VariantInit(&varChild);
			varChild.vt = VT_I4;
			varChild.lVal = CHILDID_SELF;
			*pIAccChild = pCAcc;
		}
		else
		{
			*pIAccChild = pIAccParent;
        }

		HWND hwndChild = 0;
		WindowFromAccessibleObject(*pIAccChild, &hwndChild);

		// call enum callback
		bContinue = lpEnumAccessibleProc(*pIAccChild, &varChild, hwndChild, 
						nLevel, lParam);

		if (bContinue && pCAcc)
		{
			bContinue = FindChild(pCAcc, pIAccChild, pvarChild, 
							lpEnumAccessibleProc, lParam, FALSE);
			if (*pIAccChild != pCAcc)
				SAFE_RELEASE(pCAcc);
		}
	}

	SAFE_RELEASE(pCAcc);
	SAFE_RELEASE(pEnum);
	VariantClear(&varChild);

	nLevel--;

	return bContinue;
}
