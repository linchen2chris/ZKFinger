// DemoDlg.cpp : implementation file
//
#include "stdafx.h"
#include "Demo.h"
#include "DemoDlg.h"

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
	m_Block = 1;
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
	DDX_Text(pDX, IDC_EDIT_Block, m_Block);
	DDV_MinMaxInt(pDX, m_Block, 0, 63);
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
	ON_BN_CLICKED(IDC_BTNREAD, OnReadcard)
	ON_BN_CLICKED(IDC_BTNWRITE, OnWriteCard)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTNRED, &CDemoDlg::OnBnClickedBtnred)
	ON_BN_CLICKED(IDC_BTNGREEN, &CDemoDlg::OnBnClickedBtngreen)
	ON_BN_CLICKED(IDC_BTNBEEP, &CDemoDlg::OnBnClickedBtnbeep)
	ON_BN_CLICKED(IDC_BTNWRITEPWD, &CDemoDlg::OnBnClickedBtnwritepwd)
	ON_BN_CLICKED(IDC_BTNREADPWD, &CDemoDlg::OnBnClickedBtnreadpwd)
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
	
	if (matchType == 1)
	{
		if(bVerWithCard)
		{
			BOOL ARegFeatureChanged;

			if(zkfpEng.VerRegFingerFile("fingerprint2.tpl", ATemplate, false, &ARegFeatureChanged))
			{
				SetDlgItemText(IDC_EDTHINT, "Verify Succeed(from Mifare Card)!");
			}
			else
			{
				SetDlgItemText(IDC_EDTHINT, "Verify Failed");
			}
			bVerWithCard = false;
		}
		else
		{
			sTemp = zkfpEng.GetTemplateAsString();
			BSTR bTemp;
			if (IsDlgButtonChecked(IDC_RADIO9) != 0)
			{
				bTemp = sRegTemplate.AllocSysString();
			}
			else
			{
				bTemp = sRegTemplate10.AllocSysString();
			}

			if (zkfpEng.VerFingerFromStr(&bTemp, (LPCTSTR)sTemp, FALSE, &RegChanged))
			{
				SetDlgItemText(IDC_EDTHINT, "Verify Succeed");
			}
			else
			{
				SetDlgItemText(IDC_EDTHINT, "Verify Failed");
			}
		}
	}	
	else if (matchType == 2) // 1:N
	{  
		Score = 8;
		id = zkfpEng.IdentificationInFPCacheDB(fpcHandle, ATemplate, &Score, &ProcessNum);
		if (id == -1)
		{
			SetDlgItemText(IDC_EDTHINT, "Identify Failed");
		}
		else
		{
			sprintf(buffer, "Identify Succeed ID = %d Score = %d  Processed Number = %d", id, Score, ProcessNum);
			SetDlgItemText(IDC_EDTHINT, buffer);
		}
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

			// Note: 10.0Template can not be compressed
			zkfpEng.SetTemplateLen(&pTemplate, 602);
			zkfpEng.SaveTemplate("fingerprint.tpl", pTemplate);

			FPID = FPID + 1;
			UpdateData(TRUE);
			MessageBox("Register Succeed");
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
		str = str + "Register Status: still press finger " + buffer + " times";
	}
	str += " Fingerprint quality";
	ltoa(zkfpEng.get_LastQuality(), buffer, 10);
	if (AQuality != 0)
		str = str + " not good, quality="  + buffer;
	else
		str = str + " good, quality=" + buffer;
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

int CDemoDlg::ReadBLOCK(int blockIndex, BYTE *pData, BYTE *key)
{
	int ret = 0;

	if(blockIndex<0 || blockIndex>63)// mifare s50card 0~63
		ret = -1;

	if(!zkfpEng.MF_PCDRead(0 ,0, 0, blockIndex, 1, key, pData))
		ret = pData[0];

	return ret;
}

int CDemoDlg::ReadBlock2(BYTE blockIndex, BYTE *pData, int blockLen)
{
	BYTE *buf = NULL;
	BYTE key[6];

	int i = 0, index = 0, blockNum = 0;

	index = blockIndex;
	buf = pData;

	for(i = 0;i< blockLen;)
	{
		// S50 Card：1024Bytes，16 Sectors，each sector 4 blocks
		// S70 Card：4096Bytes，40 Sectors，the first 32 secotrs have per4 block，last 8 have per16 blocks.
		if(index >= 64)
			return -1;

		if(index == 0) // In first secotr, block 0 for card number... Only 2 block can use.
		{	
			index = 1;
			blockNum = 2;
		}
		else if(index< 32*4) // s50 card
		{	
			if(index%4 == 3)
				index++;
			blockNum = 3;
		}
		else if(index > 32*4) // s70 card
		{	
			if(index%16 == 15)	
				index++;
			blockNum = 3;
		}

		memset(key, 0xFF, 6); // As 0xffffffffffff for keyA
		
		if(!zkfpEng.MF_PCDRead(0, 0, 0, index, blockNum, key, buf))
			return buf[0];

		index += blockNum;
		buf = buf + 16*blockNum;
		i += blockNum;
	}
	return 0;
}

int CDemoDlg::WriteBLOCK(BYTE blockIndex, BYTE *pData, BYTE *key)
{
	int ret = 0;

	if(blockIndex<0 || blockIndex>64) // mifare s50card 0~63 
		ret = -1;
	if(blockIndex%4 == 3)	// It's password block
		ret = -1;

	if(!zkfpEng.MF_PCDWrite(0, 0, 0, blockIndex, 1, key, pData))
		ret = pData[0];
	else
		ret = 0;
	return ret;
}

int CDemoDlg::WriteBlock2(BYTE blkIndex, BYTE *pData, int blockLen)
{
	BYTE *buf = NULL;
	BYTE key[6] = {0};
	int i = 0, index = 0, blockNum = 0;

	index = blkIndex;
	
	buf = pData;

	for(i = 0; i< blockLen;) 
	{
		// S50 Card：1024Bytes，16 Sectors，each sector 4 blocks
		// S70 Card：4096Bytes，40 Sectors，the first 32 secotrs have per4 block，last 8 have per16 blocks.
		if(index >= 64)
			return -1;

		if(index == 0) // In first secotr, block 0 for card number... Only 2 block can use.
		{	
			index = 1;
			blockNum = 2;
		}
		else if(index <32*4) // s50 card
		{	
			if(index%4 == 3)
				index++;
			blockNum = 3;
		}
		else if(index > 32*4) // s70 card
		{	
			if(index%16 == 15)
				index++;
			blockNum = 3;
		}

		memset(key, 0xff, 6); //0xffffffffffff as keyA

		if(!zkfpEng.MF_PCDWrite(0, 0, 0, index, blockNum, key, buf))
		{
			return buf[0];
		}
		index += blockNum;
		buf = buf + 16*blockNum;
		i += blockNum;
	}
	return 0;
}

void CDemoDlg::OnReadcard() 
{
	CString str;
	BYTE buf[256] = {0};
	BYTE key[6] = {0};
	int ret = 0, Block = 0;

	UpdateData(TRUE);
	Block = m_Block;
	memset(key, 0xFF, 6); // 0xFFFFFFFFFFFF as keyA
	ret =  ReadBLOCK(Block, buf, key);
	if(ret == 0)
		str.Format("BLOCK=0x%02X, Data=0x %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X ",
					Block, buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7],
					buf[8], buf[9], buf[10], buf[11], buf[12], buf[13], buf[14], buf[15]);
	else
		str.Format("Errorcode=%X", ret);
	SetDlgItemText(IDC_EDTHINT, "Read card:" + str);	
}

void CDemoDlg::OnWriteCard() 
{
	CString str;
	BYTE buf[16] = {0};
	BYTE key[6] = {0};
	int ret = 0, Block = 0;

	UpdateData(TRUE);
	if(m_Block%4 ==3 || m_Block == 0)
	{
		MessageBox("Parameter error!");
		m_Block = 1;
		UpdateData(FALSE);
	}
	Block = m_Block;

	memset(buf, 0x66, 16);

	memset(key, 0xFF, 6); // 0xFFFFFFFFFFFF as keyA
	ret =  WriteBLOCK(Block, buf, key);
	if(ret == 0)
		str.Format("BLOCK=0x%02X, succeed ",Block);
	else
		str.Format("Errorcode=%X", ret);
	SetDlgItemText(IDC_EDTHINT, "Write card:" + str);	
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

void CDemoDlg::OnBnClickedBtnwritepwd()
{
	CString str;
	BYTE buf[16];
	BYTE key[6];

	// keyA
	memset(buf, 0x66, 6);
	// FF 07 80 69 as default
	buf[6] = 0xFF;
	buf[7] = 0x07;
	buf[8] = 0x80;
	buf[9] = 0x69;

	// keyB
	memset(&buf[10], 0xFF, 6);

	memset(key, 0xFF, 6); // 0xFFFFFFFFFFFF as keyA	

	// Only modify the first sector's keyA
	if(zkfpEng.MF_PCDWrite(0, 0, 0, 3, 1, key, buf))
	{
		str.Format("BLOCK = 0x%02X, success ", 3);
	}
	else
		str.Format("Errorcode=%X", buf[0]);
	SetDlgItemText(IDC_EDTHINT, "Write card:" +  str);
}

void CDemoDlg::OnBnClickedBtnreadpwd()
{
	CString temp;
	BYTE buf[256] = {0};
	BYTE key[6] = {0};
	int ret = 0, Block = 0;

	UpdateData(TRUE);

	memset(key, 0x66, 6); // 0x666666666666 as keyA
	// Only Read block 1
	Block = 1;
	ret =  ReadBLOCK(1, buf, key);
	if(ret == 0)
		temp.Format("BLOCK = 0x%02X, Data=0x %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X ",
				Block, buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7],
				buf[8], buf[9], buf[10], buf[11], buf[12], buf[13], buf[14], buf[15]);
	else
		temp.Format("Errorcode=%X", ret);
	SetDlgItemText(IDC_EDTHINT, "Read card:" + temp);
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
