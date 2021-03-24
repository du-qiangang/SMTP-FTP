// UDPClient.cpp : 实现文件
//

#include "stdafx.h"
#include "FTPClient.h"
#include "UDPClient.h"
#include "FTPClientDlg.h"
#include "FileListDialog.h"


// CUDPClient

CUDPClient::CUDPClient()
{
	checkLink = false;
	sendFile = false;
	sendContent = false;
	endFile = false;
	findFile = false;
	downloadFile = false;
	checkReSend = false;
	strIP = L"";
	downloadPath = L"D:\\Client Files\\";
	port = 0;
	reSend = 0;
	nowPosition = 0;
}

CUDPClient::~CUDPClient()
{
}


// CUDPClient 成员函数

void CUDPClient::OnSend(int nErrorCode)
{
	CFTPClientDlg* dlg = ((CFTPClientDlg*)theApp.GetMainWnd());
	HWND hHwnd = AfxGetMainWnd()->m_hWnd;
	if (checkLink)
	{
		OpeartionPack pack;
		pack.Operation = 8;
		dlg->Record += L"正在尝试向服务器请求连接……\r\n";
		dlg->UpdateData(FALSE);
		dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
		SendTo(&pack, sizeof(OpeartionPack), port, strIP, 0);
		SetTimer(hHwnd, -1, 5000, CheckReceive);
		checkLink = false;
	}
	if (sendFile)
	{
		if (reSend == 3)
		{
			dlg->Record += L"发送文件 " + upLoadFileName + L" 失败，错误原因：传送超时！\r\n";
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
			reSend = 0;
			sendFile = false;
			return;
		}
		if (reSend)
		{
			dlg->Record += L"尝试重发\r\n";
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
			Sleep(1000);
		}
		CString mode = L"netascii";
		TitlePack pack;
		pack.Operation = 1;
		int position = 0;
		int length = upLoadFileName.GetLength();
		for (int i = 0;i < length;i++)
			pack.Content[position++] = upLoadFileName.GetAt(i);
		pack.Content[position++] = 0;
		length = mode.GetLength();
		for (int i = 0;i < length;i++)
			pack.Content[position++] = mode.GetAt(i);
		pack.Content[position++] = 0;
		dlg->Record += L"请求发送文件 " + upLoadFileName + L" 中……\r\n";
		dlg->UpdateData(FALSE);
		dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
		SendTo(&pack, position + 2, port, strIP, 0);
		SetTimer(hHwnd, 0, 3000, CheckReceive);
	}
	if (sendContent)
	{
		file.Seek(512 * nowPosition, CFile::begin);
		nowPosition++;
		int realRead = file.Read(readBuffer, 512);
		if (realRead < 512)
			endFile = true;
		ContentPack pack;
		pack.Operation = 3;
		pack.BlockNumber = nowPosition;
		pack.Length = realRead;
		for (int i = 0;i < realRead;i++)
			pack.Content[i] = readBuffer[i];
		SendTo(&pack, realRead + 6, port, strIP, 0);
		SetTimer(hHwnd, -2, 200, CheckReceive);
		SetTimer(hHwnd, nowPosition, 1000, CheckReceive);
	}
	if (findFile)
	{
		OpeartionPack pack;
		pack.Operation = 9;
		dlg->Record += L"正在尝试向服务器请求获取文件目录……\r\n";
		dlg->UpdateData(FALSE);
		dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
		SendTo(&pack, sizeof(OpeartionPack), port, strIP, 0);
		SetTimer(hHwnd, -3, 5000, CheckReceive);
		findFile = false;
	}
	if (downloadFile)
	{
		CString mode = L"netascii";
		TitlePack pack;
		pack.Operation = 11;
		int position = 0;
		int length = downloadFileName.GetLength();
		for (int i = 0;i < length;i++)
			pack.Content[position++] = downloadFileName.GetAt(i);
		pack.Content[position++] = 0;
		length = mode.GetLength();
		for (int i = 0;i < length;i++)
			pack.Content[position++] = mode.GetAt(i);
		pack.Content[position++] = 0;
		dlg->Record += L"请求下载文件 " + downloadFileName + L" 中……\r\n";
		dlg->UpdateData(FALSE);
		dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
		SendTo(&pack, position + 2, port, strIP, 0);
		SetTimer(hHwnd, -4, 3000, CheckReceive);
	}
	AsyncSelect(FD_READ);
	CAsyncSocket::OnSend(nErrorCode);
}

void CUDPClient::OnReceive(int nErrorCode)
{
	CFTPClientDlg* dlg = ((CFTPClientDlg*)theApp.GetMainWnd());
	HWND hHwnd = AfxGetMainWnd()->m_hWnd;
	Pack pack;
	ReceiveFrom(&pack, 1000, recvIP, recvPort, 0);
	CString out;
	ACKPack *pack1 = (ACKPack*)&pack;
	FilePack *pack2 = (FilePack*)&pack;
	ContentPack *pack3 = (ContentPack*)&pack;
	if (pack.Operation == 3)
	{
		ACKPack pack;
		if (!checkReSend && (pack3->BlockNumber > nowPosition + 1))
		{
			out.Format(L"接收文件第%d块出现乱序/丢包，请求服务器重发\r\n", nowPosition + 1);
			dlg->Record += out;
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
			pack.Operation = 5;
			pack.BlockNumber = nowPosition + 1;
			SendTo(&pack, sizeof(ACKPack), port, strIP, 0);
			checkReSend = true;
			return;
		}
		if (pack3->BlockNumber == nowPosition + 1)
		{
			checkReSend = false;
			file.Write(pack3->Content, pack3->Length);
			nowPosition++;
			SetTimer(hHwnd, pack3->BlockNumber + 65537, 1000, CheckReceive);
			KillTimer(hHwnd, pack3->BlockNumber + 65536);
			out.Format(L"成功接收文件第%d块\r\n", pack3->BlockNumber);
			dlg->Record += out;
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
			if (pack3->Length < 512)
			{
				KillTimer(hHwnd, pack3->BlockNumber + 65537);
				dlg->Record += L"文件下载成功！\r\n";
				dlg->UpdateData(FALSE);
				dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
				closeFile();
			}
		}
		pack.Operation = 4;
		pack.BlockNumber = pack3->BlockNumber;
		SendTo(&pack, sizeof(ACKPack), port, strIP, 0);
	}
	if (pack.Operation == 4)
	{
		switch (pack1->BlockNumber)
		{
		case 65535:
			dlg->CheckLink = L"连接情况：成功连接到服务器端！";
			dlg->Record += L"成功连接到服务器端！\r\n";
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
			KillTimer(hHwnd, -1);
			break;
		case 0:
			sendFile = false;
			endFile = false;
			reSend = 0;
			nowPosition = 0;
			dlg->Record += L"服务器端准备好，开始传输文件\r\n";
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
			KillTimer(hHwnd, 0);
			if (openReadFile(filePath) == 0)
			{
				dlg->Record += L"文件读取失败！请检查路径和文件名是否正确！\r\n";
				dlg->UpdateData(FALSE);
				dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
			}
			else
			{
				sendContent = true;
				AsyncSelect(FD_WRITE);
			}
			break;
		default:
			out.Format(L"第%d块成功到达对方服务器端\r\n", pack1->BlockNumber);
			dlg->Record += out;
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
			KillTimer(hHwnd, pack1->BlockNumber);
			if (endFile)
			{
				dlg->Record += L"文件上传完成！\r\n";
				dlg->UpdateData(FALSE);
				dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
				sendContent = false;
				closeFile();
			}
			break;
		}
	}
	if (pack.Operation == 5)
	{
		if (nowPosition > pack1->BlockNumber)
		{
			out.Format(L"第%d块到达服务器端出现乱序，准备重发\r\n", pack1->BlockNumber);
			dlg->Record += out;
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
			dlg->MySock.nowPosition = pack1->BlockNumber - 1;
			AsyncSelect(FD_WRITE);
		}
		KillTimer(hHwnd, pack1->BlockNumber);
	}
	if (pack.Operation == 10)
	{
		KillTimer(hHwnd, -3);
		CFileListDialog dialog;
		dialog.list = pack2->Content;
		dialog.downloadPath = downloadPath;
		if (dialog.DoModal() == IDOK)
		{
			dlg->Record += L"下载文件选择成功！\r\n";
			dlg->DownloadString = dialog.chooseFile;
			downloadPath = dialog.downloadPath;
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
		}
	}
	if (pack.Operation == 12)
	{
		dlg->Record += L"服务器端准备好发送文件\r\n";
		dlg->UpdateData(FALSE);
		dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
		KillTimer(hHwnd, -4);
		nowPosition = 0;
		openWriteFile(downloadPath + downloadFileName);
	}
	CAsyncSocket::OnReceive(nErrorCode);
}

int CUDPClient::openWriteFile(CString filePath)
{
	return file.Open(filePath, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
}

int CUDPClient::openReadFile(CString filePath)
{
	return file.Open(filePath, CFile::modeRead | CFile::typeBinary);
}

void CUDPClient::closeFile()
{
	file.Close();
}

VOID CALLBACK CUDPClient::CheckReceive(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	CFTPClientDlg* dlg = ((CFTPClientDlg*)theApp.GetMainWnd());
	HWND hHwnd = AfxGetMainWnd()->m_hWnd;
	if (idEvent >= 65536 && idEvent < 65536 * 2)
	{
		CString out;
		out.Format(L"下载第%d块出现错误，请求重发\r\n", idEvent - 65536);
		dlg->Record += out;
		dlg->UpdateData(FALSE);
		dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
		ACKPack pack;
		pack.Operation = 5;
		pack.BlockNumber = idEvent - 65536;
		dlg->MySock.SendTo(&pack, sizeof(ACKPack), dlg->MySock.port, dlg->MySock.strIP, 0);

	}
	else
	{
		switch (idEvent)
		{
		case -4:
			dlg->Record += L"文件下载请求超时\r\n";
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
			KillTimer(hHwnd, -4);
			break;
		case -3:
			dlg->Record += L"无法从服务器端获取文件，错误原因：连接超时！\r\n";
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
			KillTimer(hHwnd, -3);
			break;
		case -2:
			KillTimer(hHwnd, -2);
			if (!dlg->MySock.endFile)
				dlg->MySock.AsyncSelect(FD_WRITE);
			break;
		case -1:
			dlg->CheckLink = L"连接情况：无法连接到服务器端！（错误：连接超时）";
			dlg->Record += L"无法连接到服务器端，错误原因：连接超时！\r\n";
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
			KillTimer(hHwnd, -1);
			break;
		case 0:
			dlg->MySock.reSend++;
			dlg->Record += L"文件发送请求超时\r\n";
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
			dlg->MySock.AsyncSelect(FD_WRITE);
			KillTimer(hHwnd, 0);
			break;
		default:
			CString out;
			if (dlg->MySock.nowPosition > idEvent)
			{
				out.Format(L"文件传输第%d块出现丢包，准备重发\r\n", idEvent);
				dlg->Record += out;
				dlg->UpdateData(FALSE);
				dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
				dlg->MySock.nowPosition = idEvent - 1;
				dlg->MySock.AsyncSelect(FD_WRITE);
			}
			KillTimer(hHwnd, idEvent);
			break;
		}
	}
}