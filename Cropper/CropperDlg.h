
// CropperDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include <vector>
#include <algorithm>
using namespace std;


// CCropperDlg �Ի���
class CCropperDlg : public CDialogEx
{
// ����
public:
	CCropperDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CROPPER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//��ȡ�����ļ���
	BOOL GetPosterDir();
	//��ȡ�ļ�����
	int GetFileType(const CString& filename);
	//��ȡ�ü����ͼƬ
	CString GetThumbPosterName(CString posterName);
	CString GetFileName(CString filename);
	//��ȡ�����ļ��������к����ļ�
	BOOL ListDirFiles(CString strDir, vector<CString>& fileList);
	//����ü��ĺ���
	int CropPosters(CString strOutputDir,vector<CString>& fileList);
	//�����ļ���
	BOOL MkDir(CString strBaseDir,CString strDirName);
	//����û��������
	void CheckUserInput();
	//��ȡ���Ŀ¼�ļ�������
	CString GetOutputDirName();
	//��ȡ�ļ���չ��
	CString GetFileExtension(CString filename);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnPic();
protected:
	CString m_posterDir; //����Ŀ¼;
	CEdit m_editPosterDir;//��Ҫת���ĺ��������ļ���
public:
	CEdit m_editPosterScaleRatio; //�������ű���
	CEdit m_editPosterHeight; //�����̶��߶�
	CString m_scaleRatio;
	CString m_posterHeight;
	CButton m_btnGen;//���ɺ���
	BOOL m_bHasErrors; //�Ƿ��д���
	afx_msg void OnBnClickedBtnGen();
	CStatic m_genTip;
};
