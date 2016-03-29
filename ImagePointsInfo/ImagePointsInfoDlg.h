
// ImagePointsInfoDlg.h : ͷ�ļ�
//

#pragma once
#include "cv.h"
#include "highgui.h"
#include "cxcore.h"
#include <vector>
#include <fstream>
using namespace std;

#define max(a,b)            (((a) > (b)) ? (a) : (b))  
#define min(a,b)            (((a) < (b)) ? (a) : (b)) 

typedef struct tagPointXYBGR{
	int x;
	int y;
	CvScalar scalar;
} PointXYBGR, *PPointXYBGR;

// CImagePointsInfoDlg �Ի���
class CImagePointsInfoDlg : public CDialogEx
{
// ����
public:
	CImagePointsInfoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_IMAGEPOINTSINFO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

private:
	//ͼ��·������
	vector<string> vec_img_path;	

private:
	//����ѡ����ļ�
	bool listImages();
	//���ͼ���������Ϣ
	bool outputPointsInfo();

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnopenfiles();
	afx_msg void OnBnClickedBtnselectpoints();
};
