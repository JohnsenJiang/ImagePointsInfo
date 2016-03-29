
// ImagePointsInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImagePointsInfo.h"
#include "ImagePointsInfoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


IplImage* img_src=0;  
IplImage* img_dst=0;  
IplImage* img_show=0;
int iPoint_num=0;  
vector<PointXYBGR> vec_points;  
// CImagePointsInfoDlg 对话框
//注意参数是有符号短整型，该函数的作用是使i限定为[a,b]区间内  
int bound(short i,short a,short b)  
{  
	return min(max(i,min(a,b)),max(a,b));  
}  

CvScalar getInverseColor(CvScalar c)  
{  
	CvScalar s;  
	for(int i=0;i<=2;++i)  
	{  
		s.val[i]=255-c.val[i];  
	}  
	return s;  
}  

void on_mouse( int event, int x, int y, int flags, void* ustc)  
{  
	PointXYBGR ptRGB;
	CvPoint tmp_pt = {-1,-1};  
	CvFont font;  
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4, 0, 1, CV_AA);  
	char temp[16];  
	CvSize text_size;  
	int baseline;  

	CvScalar clrPoint=cvScalar(0,255,0,0);  
	CvScalar clrText=cvScalar(255, 255, 255, 0);  

	if( event == CV_EVENT_MOUSEMOVE )  
	{  
		cvCopy(img_dst,img_show);      

		x=bound(x,0,img_show->width-1);  
		y=bound(y,0,img_show->height-1);  
		ptRGB.x = x;
		ptRGB.y = y;
		cvCircle( img_show, cvPoint(ptRGB.x, ptRGB.y), 1,clrPoint ,CV_FILLED, CV_AA, 0 );  

		sprintf(temp,"%d (%d,%d)",iPoint_num+1,x,y);  
		cvGetTextSize(temp,&font,&text_size,&baseline);  
		tmp_pt.x = bound(ptRGB.x,0,img_show->width-text_size.width);  
		tmp_pt.y = bound(ptRGB.y,text_size.height+baseline,img_show->height-1-baseline);  
		cvPutText(img_show,temp, tmp_pt, &font, clrText);  

		cvShowImage( "src", img_show );  
	}   
	else if( event == CV_EVENT_LBUTTONDOWN)  
	{  
		ptRGB.x = x;
		ptRGB.y = y;
		ptRGB.scalar = cvGet2D(img_src,y,x);//获取像素点点的BGR的值 
		vec_points.push_back(ptRGB); 
		iPoint_num++;  
		cvCircle( img_src, cvPoint(ptRGB.x, ptRGB.y), 1, clrPoint ,CV_FILLED, CV_AA, 0 );  

		sprintf(temp,"%d (%d,%d)",iPoint_num,x,y);  
		cvGetTextSize(temp,&font,&text_size,&baseline);  
		tmp_pt.x = bound(ptRGB.x,0,img_src->width-text_size.width);  
		tmp_pt.y = bound(ptRGB.y,text_size.height+baseline,img_src->height-1-baseline);  
		//cvPutText(src,temp, tmp_pt, &font, clrText);  

		cvCopy(img_src,img_dst);  
		cvShowImage( "src", img_src );  
	}   
	else if( event == CV_EVENT_RBUTTONDOWN )  
	{  
		if(!vec_points.empty())  
		{  
			cvCopy(img_dst,img_src);  

			ptRGB=vec_points.back();  
			vec_points.pop_back();   
			cvCircle( img_src, cvPoint(ptRGB.x, ptRGB.y), 1, getInverseColor(clrPoint),CV_FILLED, CV_AA, 0 );  

			sprintf(temp,"%d (%d,%d)",iPoint_num,ptRGB.x,ptRGB.y);
			--iPoint_num;  
			cvGetTextSize(temp,&font,&text_size,&baseline);  
			tmp_pt.x = bound(ptRGB.x,0,img_src->width-text_size.width);  
			tmp_pt.y = bound(ptRGB.y,text_size.height+baseline,img_src->height-1-baseline);  
			cvPutText(img_src,temp, tmp_pt, &font, getInverseColor(clrText));  

			cvCopy(img_src,img_dst);  
			cvShowImage( "src", img_src );  
		}  
	}  
}  


CImagePointsInfoDlg::CImagePointsInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImagePointsInfoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	vec_img_path.clear();
}

void CImagePointsInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImagePointsInfoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_openFiles, &CImagePointsInfoDlg::OnBnClickedBtnopenfiles)
	ON_BN_CLICKED(IDC_BTN_selectPoints, &CImagePointsInfoDlg::OnBnClickedBtnselectpoints)
END_MESSAGE_MAP()


// CImagePointsInfoDlg 消息处理程序

BOOL CImagePointsInfoDlg::OnInitDialog()
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

void CImagePointsInfoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CImagePointsInfoDlg::OnPaint()
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
HCURSOR CImagePointsInfoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CImagePointsInfoDlg::OnBnClickedBtnopenfiles()
{
	// TODO:
	OPENFILENAME ofn;   
	TCHAR szOpenFileNames[500*MAX_PATH] = {0}; 
	TCHAR szPath[MAX_PATH];
	TCHAR szFileName[500*MAX_PATH];
	TCHAR* p;
	int nLen = 0;
	ZeroMemory( &ofn, sizeof(ofn) );

	//指定这个结构的大小，以字节为单位。
	ofn.lStructSize = sizeof(ofn);
	//指向所有者对话框窗口的句柄。这个成员可以是任意有效窗口句柄，或如果对话框没有所有者它可以为NULL。
	ofn.hwndOwner = m_hWnd;  
	ofn.lpstrFilter = _T("jpg文件(*.jpg)\0*.jpg\0bmp文件(*.bmp)\0*.bmp\0所有文件(*.*)\0*.*\0");//要选择的文件后缀   
	ofn.lpstrInitialDir = _T("D:\\Program Files");//默认的文件路径  
	ofn.lpstrFile = szOpenFileNames;
	ofn.nMaxFile = sizeof(szOpenFileNames)/sizeof(*szOpenFileNames);   
	ofn.nFilterIndex = 0;
	ofn.Flags = ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ALLOWMULTISELECT   ;//标志如果是多选要加上OFN_ALLOWMULTISELECT  
	ofn.lpstrFile[0] = '/0';	

	if( GetOpenFileName( &ofn ) )
	{  
		//把第一个文件名前的复制到szPath,即:
		//如果只选了一个文件,就复制到最后一个'/'
		//如果选了多个文件,就复制到第一个NULL字符
		lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset );
		//当只选了一个文件时,下面这个NULL字符是必需的.
		//这里不区别对待选了一个和多个文件的情况
		szPath[ ofn.nFileOffset ] = '/0';
		nLen = lstrlen(szPath);

		//如果选了多个文件,则必须加上'//'
		if( szPath[nLen-1] != '\\' )   
		{
			lstrcat(szPath, TEXT("\\"));
		}

		p = szOpenFileNames + ofn.nFileOffset; //把指针移到第一个文件

		vec_img_path.clear();
		string strPath;
		while( *p )
		{   
			ZeroMemory(szFileName, sizeof(szFileName));
			lstrcat(szFileName, szPath);  //给文件名加上路径  
			lstrcat(szFileName, p);    //加上文件名  
			//string strPath(szFileName);
			strPath = szFileName;
			vec_img_path.push_back(strPath);
			cvWaitKey(0);
			//lstrcat(szFileName, TEXT("/n")); //换行   
			p += lstrlen(p) +1;     //移至下一个文件
		}
		//::MessageBox(NULL, szFileName, TEXT("MultiSelect"), MB_OK);
	}

	::MessageBox(NULL, "Files read", TEXT("info"), MB_OK);
}


void CImagePointsInfoDlg::OnBnClickedBtnselectpoints()
{
	// TODO: 
	listImages();
}
//遍历选择的文件
bool CImagePointsInfoDlg::listImages()
{
	if (!vec_img_path.size())
	{
		return false;
	}
	vec_points.clear();
	for (int i=0; i<vec_img_path.size(); i++)
	{
		img_src = cvLoadImage(vec_img_path[i].c_str());
		if (!img_dst)
		{
			img_dst = cvCreateImage(cvGetSize(img_src), img_src->depth, img_src->nChannels);			
		}
		if (!img_show)
		{
			img_show = cvCreateImage(cvGetSize(img_src), img_src->depth, img_src->nChannels);
		}
		cvCopy(img_src, img_dst);

		cvNamedWindow("src",1);  
		cvSetMouseCallback( "src", on_mouse, 0 );  
		cvShowImage("src",img_src);
		if (27 == cvWaitKey(0))
		{
			break;
		}
	}

	cvDestroyAllWindows();  
	cvReleaseImage(&img_src);  
	cvReleaseImage(&img_show);  
	cvReleaseImage(&img_dst);
	if (!outputPointsInfo())
	{
		return false;
	}
	::MessageBox(NULL, "Points Select Done", TEXT("info"), MB_OK);
	return true;
}
//输出图像点数据信息
bool CImagePointsInfoDlg::outputPointsInfo()
{
	ofstream file("sample.txt");  
	if(!file)  
	{  
		cout << "open file error!";  
		return false;  
	}
	
	CvScalar scalar;
	vector<PointXYBGR>::iterator it=vec_points.begin(); 
	for(;it!=vec_points.end();++it)  
	{
		scalar = it->scalar;
		file<< it->x
			<<' '<<it->y
			<<' '<<scalar.val[2]
			<<' '<<scalar.val[1]
			<<' '<<scalar.val[0]<<endl;  
	}  
//	file<<endl;  
	file.close();  
	return true;
}