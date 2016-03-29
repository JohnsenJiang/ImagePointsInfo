
// ImagePointsInfoDlg.h : 头文件
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

// CImagePointsInfoDlg 对话框
class CImagePointsInfoDlg : public CDialogEx
{
// 构造
public:
	CImagePointsInfoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_IMAGEPOINTSINFO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

private:
	//图像路径容器
	vector<string> vec_img_path;	

private:
	//遍历选择的文件
	bool listImages();
	//输出图像点数据信息
	bool outputPointsInfo();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnopenfiles();
	afx_msg void OnBnClickedBtnselectpoints();
};
