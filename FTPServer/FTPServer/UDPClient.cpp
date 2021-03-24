// UDPClient.cpp : 实现文件
//

#include "stdafx.h"
#include "FTPServer.h"
#include "UDPClient.h"
#include "FTPServerDlg.h"



// CUDPClient

CUDPClient::CUDPClient()
{
	checkReSend = false;
	testPack = false;
	sendFileList = false;
	sendFile = false;
	nowUserNumber = 0;
	nowDownloadNumber = 0;
}

CUDPClient::~CUDPClient()
{
}


// CUDPClient 成员函数

void CUDPClient::OnSend(int nErrorCode)
{
	CString head;
	CString out;
	CFTPServerDlg* dlg = ((CFTPServerDlg*)theApp.GetMainWnd());
	HWND hHwnd = AfxGetMainWnd()->m_hWnd;
	if (sendFileList)
	{
		FilePack packFile;
		CString returnStr;
		packFile.Operation = 10;
		returnStr = findAllFile();
		int length = returnStr.GetLength();
		for (int i = 5;i < length;i++)
			if (returnStr.GetAt(i) == '\\')
				packFile.Content[i - 5] = 0;
			else
				packFile.Content[i - 5] = returnStr.GetAt(i);
		packFile.Content[length - 5] = 0;
		packFile.Content[length - 4] = 0;
		SendTo(&packFile, length - 1, tempPort, tempIP, 0);
		sendFileList = false;
	}
	if (sendFile)
	{
		sendFile = false;
		download[user[nowUser].nowDownload].file2.Seek(512 * user[nowUser].nowReceiveBlock, CFile::begin);
		user[nowUser].nowReceiveBlock++;
		char readBuffer[513];
		int realRead = download[user[nowUser].nowDownload].file2.Read(readBuffer, 512);
		if (realRead < 512)
			user[nowUser].endFile = true;
		ContentPack pack;
		pack.Operation = 3;
		pack.BlockNumber = user[nowUser].nowReceiveBlock;
		pack.Length = realRead;
		for (int i = 0;i < realRead;i++)
			pack.Content[i] = readBuffer[i];
		SendTo(&pack, realRead + 6, user[nowUser].port, user[nowUser].IP, 0);
		head.Format(L"IP:%s 端口:%d ", user[nowUser].IP, user[nowUser].port);
		out.Format(L"第%d块已发送至客户端\r\n", user[nowUser].nowReceiveBlock);
		dlg->Record += head + out;
		dlg->UpdateData(FALSE);
		dlg->SendDlgItemMessage(IDC_EDIT1, WM_VSCROLL, SB_BOTTOM, 0);
		SetTimer(hHwnd, nowUser + 2147483648, 200, CheckSend);
		SetTimer(hHwnd, nowUser * 65536 + user[nowUser].nowReceiveBlock, 1000, CheckSend);
	}
	AsyncSelect(FD_READ);
	CAsyncSocket::OnSend(nErrorCode);
}

void CUDPClient::OnReceive(int nErrorCode)
{
	CFTPServerDlg* dlg = ((CFTPServerDlg*)theApp.GetMainWnd());
	HWND hHwnd = AfxGetMainWnd()->m_hWnd;
	Pack beforePack;
	ReceiveFrom(&beforePack, 1000, tempIP, tempPort, 0);
	CString head;
	head.Format(L"IP:%s 端口:%d ", tempIP, tempPort);
	CString out;
	int now = checkUser(tempIP, tempPort);
	TitlePack *pack1 = (TitlePack*)&beforePack;
	ContentPack *pack2 = (ContentPack*)&beforePack;
	ACKPack *pack3 = (ACKPack*)&beforePack;
	ACKPack pack;
	int i;
	char tempStr[100];
	switch (beforePack.Operation)
	{
	case 1:
		for (i = 0;pack1->Content[i] != 0;i++)
			tempStr[i] = pack1->Content[i];
		tempStr[i] = 0;
		user[now].sendFileName.Format(L"%s", CStringW(tempStr));
		user[now].nowSendBlock = 0;
		openWriteFile(now, path + user[now].sendFileName);
		pack.Operation = 4;
		pack.BlockNumber = 0;
		out.Format(L"准备接收文件 %s\r\n", user[now].sendFileName);
		dlg->Record += head + out;
		dlg->UpdateData(FALSE);
		dlg->SendDlgItemMessage(IDC_EDIT1, WM_VSCROLL, SB_BOTTOM, 0);
		SendTo(&pack, sizeof(ACKPack), tempPort, tempIP, 0);
		break;
	case 3:
		if (!testPack)
		{
			if (!checkReSend && (pack2->BlockNumber > user[now].nowSendBlock + 1))
			{
				pack.Operation = 5;
				pack.BlockNumber = user[now].nowSendBlock + 1;
				SendTo(&pack, sizeof(ACKPack), tempPort, tempIP, 0);
				checkReSend = true;
				out.Format(L"接收文件第%d块出现乱序，请求重发\r\n", user[now].nowSendBlock + 1);
				dlg->Record += head + out;
				dlg->UpdateData(FALSE);
				dlg->SendDlgItemMessage(IDC_EDIT1, WM_VSCROLL, SB_BOTTOM, 0);
				break;
			}
			if (pack2->BlockNumber == user[now].nowSendBlock + 1)
			{
				checkReSend = false;
				user[now].file.Write(pack2->Content, pack2->Length);
				user[now].nowSendBlock++;
				out.Format(L"成功接收文件第%d块\r\n", pack2->BlockNumber);
				dlg->Record += head + out;
				dlg->UpdateData(FALSE);
				dlg->SendDlgItemMessage(IDC_EDIT1, WM_VSCROLL, SB_BOTTOM, 0);
				if (pack2->Length < 512)
				{
					dlg->Record += head + L"文件传入服务器成功！\r\n";
					dlg->UpdateData(FALSE);
					dlg->SendDlgItemMessage(IDC_EDIT1, WM_VSCROLL, SB_BOTTOM, 0);
					closeFile(now);
				}
			}
			pack.Operation = 4;
			pack.BlockNumber = pack2->BlockNumber;
			SendTo(&pack, sizeof(ACKPack), tempPort, tempIP, 0);
		}
		break;
	case 4:
		out.Format(L"第%d块成功到达客户端\r\n", pack3->BlockNumber);
		dlg->Record += head + out;
		dlg->UpdateData(FALSE);
		dlg->SendDlgItemMessage(IDC_EDIT1, WM_VSCROLL, SB_BOTTOM, 0);
		KillTimer(hHwnd, now * 65536 + pack3->BlockNumber);
		if (user[now].endFile)
		{
			dlg->Record += head + L"文件传至客户端成功！\r\n";
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT1, WM_VSCROLL, SB_BOTTOM, 0);
		}
		break;
	case 5:
		//for (int j = 0;j < 2;j++) //减慢速度，提高稳定性
		//{
			out.Format(L"第%d块到达客户端出现丢包/乱序，准备重发\r\n", pack3->BlockNumber);
			dlg->Record += head + out;
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT1, WM_VSCROLL, SB_BOTTOM, 0);
			user[now].nowReceiveBlock = pack3->BlockNumber - 1;
			nowUser = now;
			sendFile = true;
			KillTimer(hHwnd, now * 65536 + pack3->BlockNumber);
			AsyncSelect(FD_WRITE);
		/*}*/
		break;
	case 8:
		dlg->Record += head + L"客户端请求测试连接\r\n";
		dlg->UpdateData(FALSE);
		dlg->SendDlgItemMessage(IDC_EDIT1, WM_VSCROLL, SB_BOTTOM, 0);
		pack.Operation = 4;
		pack.BlockNumber = 65535;
		SendTo(&pack, sizeof(ACKPack), tempPort, tempIP, 0);
		break;
	case 9:
		dlg->Record += head + L"客户端请求文件列表\r\n";
		dlg->UpdateData(FALSE);
		dlg->SendDlgItemMessage(IDC_EDIT1, WM_VSCROLL, SB_BOTTOM, 0);
		sendFileList = true;
		AsyncSelect(FD_WRITE);
		break;
	case 11:
		for (i = 0;pack1->Content[i] != 0;i++)
			tempStr[i] = pack1->Content[i];
		tempStr[i] = 0;
		user[now].receiveFileName.Format(L"%s", CStringW(tempStr));
		user[now].nowReceiveBlock = 0;
		user[now].endFile = false;
		pack.Operation = 12;
		pack.BlockNumber = 0;
		SendTo(&pack, sizeof(ACKPack), tempPort, tempIP, 0);
		user[now].nowDownload = checkDownload(user[now].receiveFileName);
		nowUser = now;
		sendFile = true;
		out.Format(L"客户端请求下载文件 %s\r\n", user[now].receiveFileName);
		dlg->Record += head + out;
		dlg->UpdateData(FALSE);
		dlg->SendDlgItemMessage(IDC_EDIT1, WM_VSCROLL, SB_BOTTOM, 0);
		AsyncSelect(FD_WRITE);
		break;
	default:
		break;
	}
	CAsyncSocket::OnReceive(nErrorCode);
}

int CUDPClient::checkUser(CString IP, UINT port)
{
	for (int i = 0;i < nowUserNumber;i++)
	{
		if (user[i].IP == IP&&user[i].port == port)
			return i;
	}
	user[nowUserNumber].IP = IP;
	user[nowUserNumber].port = port;
	nowUserNumber++;
	return nowUserNumber - 1;
}

int CUDPClient::checkDownload(CString file)
{
	for (int i = 0;i < nowDownloadNumber;i++)
	{
		if (download[i].downloadFile == file)
			return i;
	}
	download[nowDownloadNumber].downloadFile = file;
	openReadFile(nowDownloadNumber, path + file);
	nowDownloadNumber++;
	return nowDownloadNumber - 1;
}

int CUDPClient::openWriteFile(int userID, CString filePath)
{
	return user[userID].file.Open(filePath, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
}

int CUDPClient::openReadFile(int downloadID, CString filePath)
{
	int i = download[downloadID].file2.Open(filePath, CFile::modeRead | CFile::typeBinary);
	return i;
}

void CUDPClient::closeFile(int userID)
{
	user[userID].file.Close();
}

CString CUDPClient::findAllFile()
{
	CString fileNameList;
	bool finding = finder.FindFile(findWild);
	while (finding)
	{
		finding = finder.FindNextFile();
		fileNameList += finder.GetFileName() + L"\\";
	}
	return fileNameList;
}

VOID CALLBACK CUDPClient::CheckSend(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	CFTPServerDlg* dlg = ((CFTPServerDlg*)theApp.GetMainWnd());
	HWND hHwnd = AfxGetMainWnd()->m_hWnd;
	if (idEvent >= 2147483648)
	{
		if (!dlg->MySock.user[idEvent - 2147483648].endFile)
		{
			dlg->MySock.nowUser = idEvent - 2147483648;
			dlg->MySock.sendFile = true;
			dlg->MySock.AsyncSelect(FD_WRITE);
		}
		KillTimer(hHwnd, idEvent);
	}
	else
	{
		if (dlg->MySock.user[dlg->MySock.nowUser].nowReceiveBlock > idEvent % 65536)
		{
			dlg->MySock.nowUser = idEvent / 65536;
			dlg->MySock.user[dlg->MySock.nowUser].nowReceiveBlock = idEvent % 65536 - 1;
			dlg->MySock.sendFile = true;
			CString head;
			head.Format(L"IP:%s 端口:%d ", dlg->MySock.user[dlg->MySock.nowUser].IP, dlg->MySock.user[dlg->MySock.nowUser].port);
			CString out;
			out.Format(L"文件传输第%d块出现丢包，准备重发\r\n", idEvent % 65536);
			dlg->Record += head + out;
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT1, WM_VSCROLL, SB_BOTTOM, 0);
			KillTimer(hHwnd, idEvent);
			dlg->MySock.AsyncSelect(FD_WRITE);
		}
	}
}