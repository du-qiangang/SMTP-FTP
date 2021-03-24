// UDPClient.cpp : ʵ���ļ�
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


// CUDPClient ��Ա����

void CUDPClient::OnSend(int nErrorCode)
{
	CFTPClientDlg* dlg = ((CFTPClientDlg*)theApp.GetMainWnd());
	HWND hHwnd = AfxGetMainWnd()->m_hWnd;
	if (checkLink)
	{
		OpeartionPack pack;
		pack.Operation = 8;
		dlg->Record += L"���ڳ�����������������ӡ���\r\n";
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
			dlg->Record += L"�����ļ� " + upLoadFileName + L" ʧ�ܣ�����ԭ�򣺴��ͳ�ʱ��\r\n";
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
			reSend = 0;
			sendFile = false;
			return;
		}
		if (reSend)
		{
			dlg->Record += L"�����ط�\r\n";
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
		dlg->Record += L"�������ļ� " + upLoadFileName + L" �С���\r\n";
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
		dlg->Record += L"���ڳ���������������ȡ�ļ�Ŀ¼����\r\n";
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
		dlg->Record += L"���������ļ� " + downloadFileName + L" �С���\r\n";
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
			out.Format(L"�����ļ���%d���������/����������������ط�\r\n", nowPosition + 1);
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
			out.Format(L"�ɹ������ļ���%d��\r\n", pack3->BlockNumber);
			dlg->Record += out;
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
			if (pack3->Length < 512)
			{
				KillTimer(hHwnd, pack3->BlockNumber + 65537);
				dlg->Record += L"�ļ����سɹ���\r\n";
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
			dlg->CheckLink = L"����������ɹ����ӵ��������ˣ�";
			dlg->Record += L"�ɹ����ӵ��������ˣ�\r\n";
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
			KillTimer(hHwnd, -1);
			break;
		case 0:
			sendFile = false;
			endFile = false;
			reSend = 0;
			nowPosition = 0;
			dlg->Record += L"��������׼���ã���ʼ�����ļ�\r\n";
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
			KillTimer(hHwnd, 0);
			if (openReadFile(filePath) == 0)
			{
				dlg->Record += L"�ļ���ȡʧ�ܣ�����·�����ļ����Ƿ���ȷ��\r\n";
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
			out.Format(L"��%d��ɹ�����Է���������\r\n", pack1->BlockNumber);
			dlg->Record += out;
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
			KillTimer(hHwnd, pack1->BlockNumber);
			if (endFile)
			{
				dlg->Record += L"�ļ��ϴ���ɣ�\r\n";
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
			out.Format(L"��%d�鵽��������˳�������׼���ط�\r\n", pack1->BlockNumber);
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
			dlg->Record += L"�����ļ�ѡ��ɹ���\r\n";
			dlg->DownloadString = dialog.chooseFile;
			downloadPath = dialog.downloadPath;
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
		}
	}
	if (pack.Operation == 12)
	{
		dlg->Record += L"��������׼���÷����ļ�\r\n";
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
		out.Format(L"���ص�%d����ִ��������ط�\r\n", idEvent - 65536);
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
			dlg->Record += L"�ļ���������ʱ\r\n";
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
			KillTimer(hHwnd, -4);
			break;
		case -3:
			dlg->Record += L"�޷��ӷ������˻�ȡ�ļ�������ԭ�����ӳ�ʱ��\r\n";
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
			dlg->CheckLink = L"����������޷����ӵ��������ˣ����������ӳ�ʱ��";
			dlg->Record += L"�޷����ӵ��������ˣ�����ԭ�����ӳ�ʱ��\r\n";
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
			KillTimer(hHwnd, -1);
			break;
		case 0:
			dlg->MySock.reSend++;
			dlg->Record += L"�ļ���������ʱ\r\n";
			dlg->UpdateData(FALSE);
			dlg->SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
			dlg->MySock.AsyncSelect(FD_WRITE);
			KillTimer(hHwnd, 0);
			break;
		default:
			CString out;
			if (dlg->MySock.nowPosition > idEvent)
			{
				out.Format(L"�ļ������%d����ֶ�����׼���ط�\r\n", idEvent);
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