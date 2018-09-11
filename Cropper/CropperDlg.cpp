
// CropperDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Cropper.h"
#include "CropperDlg.h"
#include "afxdialogex.h"
#include "cximage\include\ximage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define PRINT_LINE(FUNC_NAME,LINE_NUM) \
    {\
    TCHAR szDebugInfo[512] = {0};\
    _stprintf_s(szDebugInfo,512,_T("咸鱼日志: Func: %s , Line: %d\n"),FUNC_NAME,LINE_NUM);\
    OutputDebugString(szDebugInfo);\
    }
 
#define  PRINT_INFO()  PRINT_LINE(__FUNCTIONW__, __LINE__)


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCropperDlg 对话框



CCropperDlg::CCropperDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCropperDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_posterDir = _T("");
	m_bHasErrors = FALSE;
}

void CCropperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POSTER_DIR, m_editPosterDir);
	DDX_Control(pDX, IDC_SCALE_RATIO, m_editPosterScaleRatio);
	DDX_Control(pDX, IDC_IMG_HEIGHT, m_editPosterHeight);
	DDX_Control(pDX, IDC_BTN_GEN, m_btnGen);
	DDX_Text(pDX,IDC_POSTER_DIR,m_posterDir);
	DDX_Text (pDX, IDC_SCALE_RATIO, m_scaleRatio);
	DDX_Text(pDX,IDC_IMG_HEIGHT,m_posterHeight);
	DDX_Control(pDX, IDC_GEN_TIP, m_genTip);
}

BEGIN_MESSAGE_MAP(CCropperDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_PIC, &CCropperDlg::OnBnClickedBtnPic)
	ON_BN_CLICKED(IDC_BTN_GEN, &CCropperDlg::OnBnClickedBtnGen)
END_MESSAGE_MAP()


// CCropperDlg 消息处理程序

BOOL CCropperDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCropperDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCropperDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCropperDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CCropperDlg::GetPosterDir(){
	CString dirPath;	
	BROWSEINFO bi;  
	LPITEMIDLIST pidl;  
	bi.hwndOwner = this->m_hWnd;  
	bi.pidlRoot = NULL;  
	bi.pszDisplayName = (LPWSTR)(LPCTSTR)dirPath;  
	bi.lpszTitle = _T("请选择海报目录");  
	bi.ulFlags = BIF_RETURNONLYFSDIRS;  
	bi.lpfn = NULL;  
	bi.lParam = NULL;  
	bi.iImage = NULL;  
	if((pidl = SHBrowseForFolder(&bi)) != NULL)  
	{  
		//得到文件夹的全路径，不要的话，只得本文件夹名  
		if(SUCCEEDED(SHGetPathFromIDList(pidl, (LPWSTR)(LPCTSTR)dirPath)))
		{  
			m_editPosterDir.SetWindowTextW(dirPath);
			m_posterDir = dirPath;
			return TRUE;
		}  
	}
	return FALSE;
}

void CCropperDlg::OnBnClickedBtnPic()
{
	if(!GetPosterDir()){
	  return;
	}
}

void CCropperDlg::CheckUserInput(){
	UpdateData (TRUE);
	if(GetFileAttributes(m_posterDir)!= FILE_ATTRIBUTE_DIRECTORY){
		m_bHasErrors = TRUE;
		AfxMessageBox(_T("海报目录不合法"));
		return;
	}
	int scaleRatio = _ttoi(m_scaleRatio);
	int posterHeight = _ttoi(m_posterHeight);
	if(scaleRatio <30){
		m_bHasErrors = TRUE;
		AfxMessageBox(_T("缩放比例不能小于30%"));
		return;
	}
	if(scaleRatio>100){
		m_bHasErrors = TRUE;
		AfxMessageBox(_T("缩放比例不能大于100%"));
		return;
	}
	if(posterHeight <200){
		m_bHasErrors = TRUE;
		AfxMessageBox(_T("海报高度不能小于200px"));
		return;
	}
	if(posterHeight >1000){
		m_bHasErrors = TRUE;
		AfxMessageBox(_T("海报高度不能小于1000px"));
		return;
	}
}

void CCropperDlg::OnBnClickedBtnGen()
{
	CheckUserInput();
	if(m_bHasErrors){
		return;
	}
	vector<CString> posterList;
	BOOL bRet = ListDirFiles(m_posterDir,posterList);
	if(!bRet){
	   return;
	}
	if(posterList.size() == 0){
		AfxMessageBox(_T("亲! 目录中没有找到图片哦!"));
		return;
	}
	CropPosters(m_posterDir,posterList);
	AfxMessageBox(_T("恭喜你! 咸鱼翻身了!"));
}

CString CCropperDlg::GetOutputDirName(){
	CString now;
	CTime tm = CTime::GetCurrentTime();
	now = tm.Format("fish_%Y%m%d");
	return now;
}

//输出裁剪所有海报文件
int CCropperDlg::CropPosters(CString strOutputDir,vector<CString>& fileList){
	vector<CString>::iterator ptrFile;
	int i=0;
	MkDir(m_posterDir,GetOutputDirName());
	CString strSubDir;
	strSubDir.Format(_T("%s%s%s"),m_posterDir,_T("\\"),GetOutputDirName());
	int scaleRatio = _ttoi(m_scaleRatio);
	int posterHeight = _ttoi(m_posterHeight);
	CString strTotal;
	strTotal.Format(_T("%d"),fileList.size());
	clock_t startTime,endTime;
	startTime = clock();
	for(ptrFile = fileList.begin(); ptrFile != fileList.end(); ptrFile++){
		CxImage image;
		CString strOutputDir;
		CString strIndex;
		i++;
		strOutputDir.Format(_T("%03d"),i);
		strIndex.Format(_T("%d"),i);
		bool bLoad = image.Load((*ptrFile).GetBuffer((*ptrFile).GetLength()),GetFileType(*ptrFile));
		if(bLoad){
			MkDir(strSubDir,strOutputDir);
			CString pathFileCopy;
			CString pathFileThumb;
			pathFileCopy.Format(_T("%s%s%s%s%s"),strSubDir,_T("\\"),strOutputDir,_T("\\"),GetFileName(*ptrFile));
			pathFileThumb.Format(_T("%s%s%s%s%s"),strSubDir,_T("\\"),strOutputDir,_T("\\"),GetThumbPosterName(*ptrFile));
			CxImage thumbImg;
			CxImage thumbImg2;
			image.Resample(image.GetWidth()*scaleRatio/100,image.GetHeight()*scaleRatio/100,0,&thumbImg);
			thumbImg.Crop(CRect(0,0,image.GetWidth(),posterHeight),&thumbImg2);
			thumbImg2.Save(pathFileThumb,GetFileType(pathFileThumb));
			image.Save(pathFileCopy,GetFileType(pathFileCopy));
			endTime = clock(); 
			int dfPassTime = (int)(endTime - startTime) / CLOCKS_PER_SEC;
			CString strElapsed;
			strElapsed.Format(_T("%d"),dfPassTime);
			m_genTip.SetWindowTextW(_T("总数")+strTotal+_T(" ,当前第 ")+strIndex+_T(" 条 ,耗时 ")+strElapsed+_T(" 秒"));
			TRACE("save file %s success!\n",(*ptrFile).GetString());
		}
	}
	return fileList.size();
}

CString CCropperDlg::GetFileName(CString filename){
	CString name = filename.Right(filename.GetLength()-filename.ReverseFind('\\')-1);
	return name;
}

CString CCropperDlg::GetThumbPosterName(CString filename){
	CString ext = filename.Right(filename.GetLength()-filename.ReverseFind('.')-1);
	CString name = filename.Right(filename.GetLength()-filename.ReverseFind('\\')-1);
	name = name.Left(name.ReverseFind('.'));
	CString strOut;
	strOut.Format(_T("%s%s%s"),name,_T("_S."),ext);
	return strOut;
}

CString CCropperDlg::GetFileExtension(CString filename){
	return filename.Right(filename.GetLength()-filename.ReverseFind('.')-1);
}

int CCropperDlg::GetFileType(const CString& filename)  
{  
	CString ext = filename.Right(filename.GetLength()-filename.ReverseFind('.')-1);  
	int type = 0;  
	if (ext == "bmp") type = CXIMAGE_FORMAT_BMP;  
	#if CXIMAGE_SUPPORT_JPG  
	else if (ext=="jpg"||ext=="jpeg") type = CXIMAGE_FORMAT_JPG;  
	#endif  
	#if CXIMAGE_SUPPORT_GIF  
	else if (ext == "gif") type = CXIMAGE_FORMAT_GIF;  
	#endif  
	#if CXIMAGE_SUPPORT_PNG  
	else if (ext == "png") type = CXIMAGE_FORMAT_PNG;  
	#endif  
	#if CXIMAGE_SUPPORT_MNG  
	else if (ext=="mng"||ext=="jng") type = CXIMAGE_FORMAT_MNG;  
	#endif  
	#if CXIMAGE_SUPPORT_ICO  
	else if (ext == "ico") type = CXIMAGE_FORMAT_ICO;  
	#endif  
	#if CXIMAGE_SUPPORT_TIF  
	else if (ext=="tiff"||ext=="tif") type = CXIMAGE_FORMAT_TIF;  
	#endif  
	#if CXIMAGE_SUPPORT_TGA  
	else if (ext=="tga") type = CXIMAGE_FORMAT_TGA;  
	#endif  
	#if CXIMAGE_SUPPORT_PCX  
	else if (ext=="pcx") type = CXIMAGE_FORMAT_PCX;  
	#endif  
	#if CXIMAGE_SUPPORT_WBMP  
	else if (ext=="wbmp") type = CXIMAGE_FORMAT_WBMP;  
	#endif  
	#if CXIMAGE_SUPPORT_WMF  
	else if (ext=="wmf"||ext=="emf") type = CXIMAGE_FORMAT_WMF;  
	#endif  
	#if CXIMAGE_SUPPORT_J2K  
	else if (ext=="j2k"||ext=="jp2") type = CXIMAGE_FORMAT_J2K;  
	#endif  
	#if CXIMAGE_SUPPORT_JBG  
	else if (ext=="jbg") type = CXIMAGE_FORMAT_JBG;  
	#endif  
	#if CXIMAGE_SUPPORT_JP2  
	else if (ext=="jp2"||ext=="j2k") type = CXIMAGE_FORMAT_JP2;  
	#endif  
	#if CXIMAGE_SUPPORT_JPC  
	else if (ext=="jpc"||ext=="j2c") type = CXIMAGE_FORMAT_JPC;  
	#endif  
	#if CXIMAGE_SUPPORT_PGX  
	else if (ext=="pgx") type = CXIMAGE_FORMAT_PGX;  
	#endif  
	#if CXIMAGE_SUPPORT_RAS  
	else if (ext=="ras") type = CXIMAGE_FORMAT_RAS;  
	#endif  
	#if CXIMAGE_SUPPORT_PNM  
	else if (ext=="pnm"||ext=="pgm"||ext=="ppm") type = CXIMAGE_FORMAT_PNM;  
	#endif  
	else type = CXIMAGE_FORMAT_UNKNOWN;
	return type;  
} 

//遍历文件夹下面的所有海报文件
BOOL CCropperDlg::ListDirFiles(CString strBaseDir, vector<CString>& fileList){
	PRINT_INFO();
	CFileFind ff;
	if(strBaseDir.Right(1) != _T("\\")){
		strBaseDir.Format(_T("%s%s"),strBaseDir, _T("\\"));
	}
	PRINT_INFO();
	strBaseDir += _T("*.*");
	BOOL ret = ff.FindFile(strBaseDir);
	if(!ret){
		return FALSE;
	}
	PRINT_INFO();
	while (ret)
    {
		ret = ff.FindNextFile();
        if (ff.IsDirectory() && !ff.IsDots()) //只遍历一层文件夹
        {
            //CString path = ff.GetFilePath();
			//ListDirFiles(path, fileList);
        }
        else if (!ff.IsDirectory() && !ff.IsDots())
        {
            CString name = ff.GetFileName();
            CString path = ff.GetFilePath();
			CString ext = GetFileExtension(name);
			if(ext=="jpg"||ext=="png" 
				||ext =="gif"||ext=="jpeg"
				||ext=="bmp"||ext=="ico"){
				TRACE(_T("path = %s\n"),path);
				fileList.push_back(path);
			}
        }
	}
	return TRUE;
}
//创建文件夹
BOOL CCropperDlg::MkDir(CString strBaseDir,CString strDirName){
	CString dirPath;
	dirPath.Format(_T("%s%s%s"),strBaseDir,_T("\\"),strDirName);
	return CreateDirectory(dirPath,NULL);
}
