// DemoDlg.h : header file
//
//{{AFX_INCLUDES()
#include "CZKFPEngX.h"
//}}AFX_INCLUDES

#if !defined(AFX_DEMODLG_H__CCFDAC80_A8D2_40AA_B4F2_413AF751CF94__INCLUDED_)
#define AFX_DEMODLG_H__CCFDAC80_A8D2_40AA_B4F2_413AF751CF94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <comdef.h>

/////////////////////////////////////////////////////////////////////////////
// CDemoDlg dialog

class CDemoDlg : public CDialog
{
// Construction
public:
	CDemoDlg(CWnd* pParent = NULL);	// standard constructor

	long FPID;
	long fpcHandle;

	long matchType;
	
	CString sRegTemplate;
	CString sRegTemplate10;

	BOOL bVerWithCard;
// Dialog Data
	//{{AFX_DATA(CDemoDlg)
	enum { IDD = IDD_DEMO_DIALOG };
	CZKFPEngX zkfpEng;
	CString	m_Cur;
	CString	m_SN;
	CString	m_Count;
	int		m_Block;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBTNInit();
	afx_msg void OnOnImageReceivedZkfpengx2(BOOL FAR* AImageValid);
	afx_msg void OnBTNSave();
	afx_msg void OnBTNReg();
	afx_msg void OnBTNIdentify();
	afx_msg void OnBTNVer();
	afx_msg void OnOnCaptureZkfpengx2(BOOL ActionResult, const VARIANT FAR& ATemplate);
	afx_msg void OnOnEnrollZkfpengx2(BOOL ActionResult, const VARIANT FAR& ATemplate);
	afx_msg void OnOnFeatureInfoZkfpengx2(long AQuality);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnGetCardSN();
	afx_msg void OnReadcard();
	afx_msg void OnWriteCard();
	afx_msg void OnTemplateToCard();
	afx_msg void OnButton6();

	int GetCardByteSerial(BYTE  *pSerial);
	int ReadBLOCK(int blockIndex, BYTE *pData, BYTE *key);
	int ReadBlock2(BYTE blockIndex, BYTE *pData, int blockLen);
	int WriteBLOCK(BYTE blockIndex, BYTE *pData, BYTE *key);
	int WriteBlock2(BYTE blkIndex, BYTE *pData, int blockLen);
	CString GetSnr();
	CString GetCardStrSerial();

	void EnableButton(bool bEnable);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnred();
	afx_msg void OnBnClickedBtngreen();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedBtnbeep();
	afx_msg void OnBnClickedBtnpwd();
	afx_msg void OnBnClickedBtnwritepwd();
	afx_msg void OnBnClickedBtnreadpwd();
	afx_msg void OnBnClickedRadio9();
	afx_msg void OnBnClickedRadio10();
	afx_msg void OnBnClickedBtndisconnect();
	afx_msg void OnBnClickedBtnregbyimage();
	afx_msg void OnBnClickedBtnidenbyimage();
	afx_msg void OnBnClickedBtnsn();
	afx_msg void OnBnClickedBtncardnumber();
	afx_msg void OnBnClickedBtnwritetmp();
	afx_msg void OnBnClickedBtnreadtmp();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMODLG_H__CCFDAC80_A8D2_40AA_B4F2_413AF751CF94__INCLUDED_)
