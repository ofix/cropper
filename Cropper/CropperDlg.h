
// CropperDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <vector>
#include <algorithm>
using namespace std;


// CCropperDlg 对话框
class CCropperDlg : public CDialogEx
{
// 构造
public:
	CCropperDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CROPPER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//获取海报文件夹
	BOOL GetPosterDir();
	//获取文件类型
	int GetFileType(const CString& filename);
	//获取裁剪后的图片
	CString GetThumbPosterName(CString posterName);
	CString GetFileName(CString filename);
	//获取海报文件夹下所有海报文件
	BOOL ListDirFiles(CString strDir, vector<CString>& fileList);
	//输出裁剪的海报
	int CropPosters(CString strOutputDir,vector<CString>& fileList);
	//创建文件夹
	BOOL MkDir(CString strBaseDir,CString strDirName);
	//检查用户输入错误
	void CheckUserInput();
	//获取输出目录文件夹名称
	CString GetOutputDirName();
	//获取文件扩展名
	CString GetFileExtension(CString filename);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnPic();
protected:
	CString m_posterDir; //海报目录;
	CEdit m_editPosterDir;//需要转换的海报所在文件夹
public:
	CEdit m_editPosterScaleRatio; //海报缩放比例
	CEdit m_editPosterHeight; //海报固定高度
	CString m_scaleRatio;
	CString m_posterHeight;
	CButton m_btnGen;//生成海报
	BOOL m_bHasErrors; //是否有错误
	afx_msg void OnBnClickedBtnGen();
	CStatic m_genTip;
};
