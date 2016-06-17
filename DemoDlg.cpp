// DemoDlg.cpp : implementation file
//
#include "stdafx.h"
#include "Demo.h"
#include "DemoDlg.h"
#include "iostream"
#include <fstream>
#include <string>
#include <sstream>

#include <windows.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemoDlg dialog

CDemoDlg::CDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDemoDlg)
	m_Cur = _T("");
	m_SN = _T("");
	m_Count = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	sRegTemplate = _T("");
	sRegTemplate10 = _T("");
}

void CDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDemoDlg)
	DDX_Control(pDX, IDC_ZKFPENGX2, zkfpEng);
	DDX_Text(pDX, IDC_EDTCUR, m_Cur);
	DDX_Text(pDX, IDC_EDTSN, m_SN);
	DDX_Text(pDX, IDC_EDTCOUNT, m_Count);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CDemoDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTNInit, OnBTNInit)
	ON_BN_CLICKED(IDC_BTNReg, OnBTNReg)
	ON_BN_CLICKED(IDC_BTNIdentify, OnBTNIdentify)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTNRED, &CDemoDlg::OnBnClickedBtnred)
	ON_BN_CLICKED(IDC_BTNGREEN, &CDemoDlg::OnBnClickedBtngreen)
	ON_BN_CLICKED(IDC_BTNBEEP, &CDemoDlg::OnBnClickedBtnbeep)
	ON_BN_CLICKED(IDC_RADIO9, &CDemoDlg::OnBnClickedRadio9)
	ON_BN_CLICKED(IDC_RADIO10, &CDemoDlg::OnBnClickedRadio10)
	ON_BN_CLICKED(IDC_BTNDisConnect, &CDemoDlg::OnBnClickedBtndisconnect)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoDlg message handlers

BOOL CDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	

	bVerWithCard = false;
	FPID = 1;
	fpcHandle = 0;

	CheckRadioButton(IDC_RADIO9, IDC_RADIO10, IDC_RADIO9);
	EnableButton(false);
	char textLengthBytes[4];
	std::cin.read(textLengthBytes, 4);
	unsigned int inLen = *reinterpret_cast<unsigned int *>(textLengthBytes);
	char *inMsg = new char[inLen];
	std::cin.read(inMsg, inLen);
	std::string s(inMsg);
	delete[] inMsg;
	//memcpy(&textLengthBytes, &myByteArray[0], sizeof(textLengthBytes));
	MessageBox(s.c_str());
	//MessageBox(textLengthBytes);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDemoDlg::OnBTNInit() 
{
	char buffer[10] = {0};
	
	if (zkfpEng.InitEngine() == 0)
	{
		EnableButton(true);
		if (IsDlgButtonChecked(IDC_RADIO9) != 0)
			zkfpEng.put_FPEngineVersion("9");
		else
			zkfpEng.put_FPEngineVersion("10");
		fpcHandle = zkfpEng.CreateFPCacheDBEx();
		//long id = zkfpEng.AddRegTemplateFileToFPCacheDB(fpcHandle, FPID, "fingerprint.tpl");
	
		CStdioFile file(_T("fingerDB.txt"), CFile::modeRead);
		CString line;
		while (file.ReadString(line)) {
			zkfpEng.AddRegTemplateStrToFPCacheDB(fpcHandle, FPID, (LPCTSTR)line);
			FPID = FPID + 1;
		}
		m_SN = zkfpEng.get_SensorSN();
		ltoa(zkfpEng.get_SensorIndex(), buffer, 10);
		m_Cur = buffer;
		ltoa(zkfpEng.get_SensorCount(), buffer, 10);
		m_Count = buffer;
		SetDlgItemText(IDC_EDTCUR, m_Cur);
		SetDlgItemText(IDC_EDTCOUNT, m_Count);
		SetDlgItemText(IDC_EDTSN, m_SN);		
		MessageBox("设备连接成功！");
	} 
	else
	{
		zkfpEng.EndEngine();
		MessageBox("设备连接失败！");
	}
	matchType = 2;
}


BEGIN_EVENTSINK_MAP(CDemoDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDemoDlg)
	ON_EVENT(CDemoDlg, IDC_ZKFPENGX2, 8 /* OnImageReceived */, OnOnImageReceivedZkfpengx2, VTS_PBOOL)
	ON_EVENT(CDemoDlg, IDC_ZKFPENGX2, 10 /* OnCapture */, OnOnCaptureZkfpengx2, VTS_BOOL VTS_VARIANT)
	ON_EVENT(CDemoDlg, IDC_ZKFPENGX2, 9 /* OnEnroll */, OnOnEnrollZkfpengx2, VTS_BOOL VTS_VARIANT)
	ON_EVENT(CDemoDlg, IDC_ZKFPENGX2, 5 /* OnFeatureInfo */, OnOnFeatureInfoZkfpengx2, VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDemoDlg::OnOnImageReceivedZkfpengx2(BOOL FAR* AImageValid) 
{
	// TODO: Add your control notification handler code here
	HDC hdc;

	hdc = this->GetDC()->m_hDC;

	zkfpEng.PrintImageAt(long(hdc), 330, 2, zkfpEng.get_ImageWidth(), zkfpEng.get_ImageHeight());
}

void CDemoDlg::OnBTNReg() 
{
	zkfpEng.CancelEnroll();
	zkfpEng.put_EnrollCount(3);
	zkfpEng.BeginEnroll();
	std::string message = "{\"text\": \"This is a register message\"}";
	unsigned int len = message.length();
	// We need to send the 4 bytes of length information
	std::cout << char(((len >> 0) & 0xFF))
		<< char(((len >> 8) & 0xFF))
		<< char(((len >> 16) & 0xFF))
		<< char(((len >> 24) & 0xFF));
	// Now we can output our message 
	std::cout << message;
	std::cout.flush();
	SetDlgItemText(IDC_EDTHINT, "开始注册，请放入手指");		
}

void CDemoDlg::OnBTNIdentify() 
{
    if (zkfpEng.get_IsRegister())
        zkfpEng.CancelEnroll();
	SetDlgItemText(IDC_EDTHINT, "准备指纹签到！");		
    matchType = 2;
}

BOOL CDemoDlg::DestroyWindow() 
{
	return CDialog::DestroyWindow();
}

void CDemoDlg::OnOnCaptureZkfpengx2(BOOL ActionResult, const VARIANT FAR& ATemplate) 
{
	long id = 0, Score = 0, ProcessNum = 0;
	char buffer[64] = {0};
	CString sTemp;
	BOOL RegChanged;
	
	Score = 8;
	id = zkfpEng.IdentificationInFPCacheDB(fpcHandle, ATemplate, &Score, &ProcessNum);	
	if (id == -1)
	{
		SetDlgItemText(IDC_EDTHINT, "验证失败");
	}
	else
	{
		sprintf(buffer, "验证成功 ID = %d Score = %d  Processed Number = %d", id, Score, ProcessNum);
		std::string message;
		std::stringstream strstream;
		strstream << id;
		strstream >> message;
		unsigned int len = message.length();
		// We need to send the 4 bytes of length information
		std::cout << char(((len >> 0) & 0xFF))
			<< char(((len >> 8) & 0xFF))
			<< char(((len >> 16) & 0xFF))
			<< char(((len >> 24) & 0xFF));
		// Now we can output our message 
		std::cout << message ;
		std::cout.flush();
		SetDlgItemText(IDC_EDTHINT, buffer);
	}  
}

void CDemoDlg::OnOnEnrollZkfpengx2(BOOL ActionResult, const VARIANT FAR& ATemplate) 
{
	VARIANT pTemplate;

	if (!ActionResult)
		MessageBox("Register Failed");
	else
	{
		// After enroll, you can get 9.0&10.0 template at the same time
		sRegTemplate = zkfpEng.GetTemplateAsStringEx("9");
		sRegTemplate10 = zkfpEng.GetTemplateAsStringEx("10");

		if(sRegTemplate.GetLength() > 0)
		{
			if(sRegTemplate10.GetLength() > 0)
				zkfpEng.AddRegTemplateStrToFPCacheDBEx(fpcHandle, FPID, (LPCTSTR)sRegTemplate, (LPCTSTR)sRegTemplate10);
			else
				MessageBox("Register 10.0 failed, template length is zero");

			pTemplate = zkfpEng.DecodeTemplate1((LPCTSTR)sRegTemplate);
			MessageBox(sRegTemplate);
			std::ofstream fout;
			fout.open("fingerDB.txt", std::fstream::app);
			fout << sRegTemplate;
			fout << "\n";
			//zkfpEng.SaveTemplateStr("fingerDB.txt", sRegTemplate);
			// Note: 10.0Template can not be compressed
			zkfpEng.SetTemplateLen(&pTemplate, 602);
			zkfpEng.SaveTemplate("fingerprint.tpl", pTemplate);

			FPID = FPID + 1;
			UpdateData(TRUE);
			MessageBox("注册成功！");
		}
		else
		{
			MessageBox("Register Failed, template length is zero");
		}
  }
}

void CDemoDlg::OnOnFeatureInfoZkfpengx2(long AQuality) 
{
	CString str;
	char buffer[10] = {0};

	if (zkfpEng.get_IsRegister())
	{
		ltoa(zkfpEng.get_EnrollIndex(), buffer, 10);
		str = str + "注册状态: 需要按指纹 " + buffer + " 次";
	}
	str += " 指纹质量";
	ltoa(zkfpEng.get_LastQuality(), buffer, 10);
	if (AQuality != 0)
		str = str + " 一般, 质量="  + buffer;
	else
		str = str + " 很好, 质量=" + buffer;
	SetDlgItemText(IDC_EDTHINT, str);
}

void CDemoDlg::OnDestroy() 
{
	CDialog::OnDestroy();
}

void CDemoDlg::OnClose() 
{
	zkfpEng.EndEngine();
	if(fpcHandle > 0)
	{
		zkfpEng.FreeFPCacheDBEx(fpcHandle);
	}
	CDialog::OnClose();
}

/*
ControlSensor(ACode As Long; AValue As Long)
If ACode=11, control the green light, if it’s 12, control the red light, if it’s 13,
control the beep.
If AValue=1, lights on, if it’s 0, lights off.
*/
void CDemoDlg::OnBnClickedBtnred()
{
	zkfpEng.ControlSensor(12, 1);
	zkfpEng.ControlSensor(12, 0);
}

void CDemoDlg::OnBnClickedBtngreen()
{
	zkfpEng.ControlSensor(11, 1);
	zkfpEng.ControlSensor(11, 0);
}

void CDemoDlg::OnBnClickedBtnbeep()
{
	zkfpEng.ControlSensor(13, 1);
	zkfpEng.ControlSensor(13, 0);
}

void CDemoDlg::OnBnClickedRadio9()
{
	zkfpEng.put_FPEngineVersion("9");
}

void CDemoDlg::OnBnClickedRadio10()
{
	zkfpEng.put_FPEngineVersion("10");
}

void CDemoDlg::EnableButton(bool bEnable)
{
	GetDlgItem(IDC_BTNInit)->EnableWindow(!bEnable);
	GetDlgItem(IDC_BTNDisConnect)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTNReg)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTNIdentify)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTNRED)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTNGREEN)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTNBEEP)->EnableWindow(bEnable);
}
void CDemoDlg::OnBnClickedBtndisconnect()
{
	zkfpEng.EndEngine();
	EnableButton(false);
}