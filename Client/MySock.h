#pragma once
#include <afxsock.h>
class MySock :
	public CAsyncSocket
{
public:
	MySock();
	~MySock();
public: BOOL m_bConnected;
		//�Ƿ����� 
		UINT m_nLength;
		//��Ϣ���� 
		char m_szBuffer[1024];
		//��Ϣ������
		virtual void OnConnect(int nErrorCode);
		virtual void OnReceive(int nErrorCode);
		virtual void OnSend(int nErrorCode);
};

