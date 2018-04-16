// AddSectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AddSection.h"
#include "AddSectionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddSectionDlg dialog

CAddSectionDlg::CAddSectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddSectionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddSectionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAddSectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddSectionDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAddSectionDlg, CDialog)
	//{{AFX_MSG_MAP(CAddSectionDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_LL, OnButtonLl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddSectionDlg message handlers

BOOL CAddSectionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAddSectionDlg::OnPaint() 
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
void CAddSectionDlg:: OnDropFiles(HDROP hDropInfo)
{
	char szFilePath[MAX_PATH];
	::DragQueryFile( hDropInfo, 0, szFilePath, MAX_PATH );
	
	// 调用控件的接口播放拖进的文件
	SetDlgItemText(IDC_EDIT_PATH, szFilePath );
	
	CWnd::OnDropFiles(hDropInfo);
}
// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAddSectionDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAddSectionDlg::OnButtonExit() 
{
	// TODO: Add your control notification handler code here
	exit(0);
}

void CAddSectionDlg::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	CString strMsg[] = {"Add Section Sucess", "Open file failed", "fread fd failed", "Add Section Failed"};
	char szPath[MAX_PATH] = {0};
	char szSecName[MAX_PATH] = {0};
	char szSize[100] = {0};
	if( !GetDlgItemText(IDC_EDIT_PATH, szPath, MAX_PATH) || !GetDlgItemText(IDC_EDIT_SIZE, szSize, 100)
		|| !GetDlgItemText(IDC_EDIT_NAME, szSecName, MAX_PATH) )
	{
		AfxMessageBox("输入数据不完整!\n");
		return;
	}
	if(!IsELF(szPath))
	{
		AfxMessageBox("不是有效的文件!\n");
		return;
	}
	long nSecLen = strtol(szSize, NULL, 16);
	int iRet = addSectionFun(szPath, szSecName, nSecLen);
	AfxMessageBox(strMsg[iRet]);
}

void CAddSectionDlg::OnButtonLl() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(true,NULL,NULL,NULL,"所有文件(*.*)|*.*;",NULL);   
	
	if(dlg.DoModal()==IDOK)            //显示对话框并返回(IDOK或IDCANCEL)分别是按了打开和取消
	{
		SetDlgItemText(IDC_EDIT_PATH, dlg.GetPathName());   ////比如:C:\asd.bat
	}
}

int CAddSectionDlg::addSectionFun(char *lpPath, char *szSecname, unsigned int nNewSecSize)
{
	char name[MAX_PATH];
	FILE *fdr, *fdw;
	char *base = NULL;
	Elf32_Ehdr *ehdr;
	Elf32_Phdr *t_phdr, *load1, *load2, *dynamic;
	Elf32_Shdr *s_hdr;
	int flag = 0;
	int i = 0;
	unsigned mapSZ = 0;
	unsigned nLoop = 0;
	unsigned int nAddInitFun = 0;
	unsigned int nNewSecAddr = 0;
	unsigned int nModuleBase = 0;
	memset(name, 0, sizeof(name));
	if(nNewSecSize == 0)
	{
		return 0;
	}
	fdr = fopen(lpPath, "rb");
	strcpy(name, lpPath);
	if(strchr(name, '.'))
	{
		strcpy(strchr(name, '.'), "_new.so");
	}
	else
	{
		strcat(name, "_new");
	}
	fdw = fopen(name, "wb");
	if(fdr == NULL || fdw == NULL)
	{
		printf("Open file failed");
		return 1;
	}
	fseek(fdr, 0, SEEK_END);
	mapSZ = ftell(fdr);
	base = (char*)malloc(mapSZ * 2 + nNewSecSize);
	memset(base, 0, mapSZ * 2 + nNewSecSize);
	fseek(fdr, 0, SEEK_SET);
	fread(base, 1, mapSZ, fdr);
	if(base == (void*) -1)
	{
		printf("fread fd failed");
		return 2;
	}
	//printf("Map base = %p\n", base);
	ehdr = (Elf32_Ehdr*) base;
	t_phdr = (Elf32_Phdr*)(base + sizeof(Elf32_Ehdr));
	for(i=0;i<ehdr->e_phnum;i++)
	{
		if(t_phdr->p_type == PT_LOAD)
		{
			if(flag == 0)
			{
				load1 = t_phdr;
				flag = 1;
				nModuleBase = load1->p_vaddr;
				printf("load1 = %p, offset = 0x%x \n", load1, load1->p_offset);

			}
			else
			{
				load2 = t_phdr;
				printf("load2 = %p, offset = 0x%x \n", load2, load2->p_offset);
			}
		}
		if(t_phdr->p_type == PT_DYNAMIC)
		{
			dynamic = t_phdr;
			printf("dynamic = %p, offset = 0x%x \n", dynamic, dynamic->p_offset);
		}
		t_phdr ++;
	}
	s_hdr = (Elf32_Shdr*)(base + ehdr->e_shoff);
	nNewSecAddr = ALIGN(load2->p_paddr + load2->p_memsz - nModuleBase, load2->p_align);
	if(load1->p_filesz < ALIGN(load2->p_paddr + load2->p_memsz, load2->p_align) )
	{
		if( (ehdr->e_shoff + sizeof(Elf32_Shdr) * ehdr->e_shnum) != mapSZ)
		{
			if(mapSZ + sizeof(Elf32_Shdr) * (ehdr->e_shnum + 1) > nNewSecAddr)
			{
				printf("无法添加节\n");
				return 3;
			}
			else
			{
				memcpy(base + mapSZ, base + ehdr->e_shoff, sizeof(Elf32_Shdr) * ehdr->e_shnum);
				ehdr->e_shoff = mapSZ;
				mapSZ += sizeof(Elf32_Shdr) * ehdr->e_shnum;
				s_hdr = (Elf32_Shdr*)(base + ehdr->e_shoff);
			}
		}
	}
	else
	{
		nNewSecAddr = load1->p_filesz;
	}
	printf("还可添加 %d 个节\n", (nNewSecAddr - ehdr->e_shoff) / sizeof(Elf32_Shdr) - ehdr->e_shnum - 1);
	int nWriteLen = nNewSecAddr + ALIGN(strlen(szSecname) + 1, 0x10) + nNewSecSize;
	char *lpWriteBuf = (char *)malloc(nWriteLen);
	memset(lpWriteBuf, 0, nWriteLen);
	s_hdr[ehdr->e_shstrndx].sh_size = nNewSecAddr - s_hdr[ehdr->e_shstrndx].sh_offset + strlen(szSecname) + 1;
	strcpy(lpWriteBuf + nNewSecAddr, szSecname);
	
	
	Elf32_Shdr newSecShdr = {0};
	newSecShdr.sh_name = nNewSecAddr - s_hdr[ehdr->e_shstrndx].sh_offset;
	newSecShdr.sh_type = SHT_PROGBITS;
	newSecShdr.sh_flags = SHF_WRITE | SHF_ALLOC | SHF_EXECINSTR;
	nNewSecAddr += ALIGN(strlen(szSecname) + 1, 0x10);
	newSecShdr.sh_size = nNewSecSize;
	newSecShdr.sh_offset = nNewSecAddr;
	newSecShdr.sh_addr = nNewSecAddr + nModuleBase;
	newSecShdr.sh_addralign = 4;
	load1->p_filesz = nWriteLen;
	load1->p_memsz = nNewSecAddr + nNewSecSize;
	load1->p_flags = 7;		//可读 可写 可执行
	ehdr->e_shnum++;
	memcpy(lpWriteBuf, base, mapSZ);
	memcpy(lpWriteBuf + mapSZ, &newSecShdr, sizeof(Elf32_Shdr));

	
	fseek(fdw, 0, SEEK_SET);
	fwrite(lpWriteBuf, 1, nWriteLen, fdw);
	fclose(fdw);
	fclose(fdr);
	free(base);
	free(lpWriteBuf);
//	char szCmdBuf[256] = {0};
//	sprintf(szCmdBuf, "adb push %s /data/local/tmp", name);
//	WinExec(szCmdBuf, SW_HIDE);
	return 0;
}

BOOL CAddSectionDlg::IsELF(char *lpPath)
{
	FILE *lpFile = fopen(lpPath, "rb");
	if(lpFile == NULL)
	{
		return FALSE;
	}
	fseek(lpFile, 0, SEEK_END);
	unsigned int nLen = ftell(lpFile);
	char *lpBuf = (char*)malloc(nLen);
	fseek(lpFile, 0, SEEK_SET);
	fread(lpBuf, 1, nLen, lpFile);
	fclose(lpFile);
	Elf32_Ehdr* ehdr = (Elf32_Ehdr*) lpBuf;
	Elf32_Shdr shdr_tmp = {0};
	if(memcmp(ehdr->e_ident + 1, "ELF", 3) || ehdr->e_shoff > nLen
		|| memcmp(lpBuf + ehdr->e_shoff, &shdr_tmp, sizeof(Elf32_Shdr)))
	{
		free(lpBuf);
		return FALSE;
	}
	
	free(lpBuf);
	return TRUE;
}
