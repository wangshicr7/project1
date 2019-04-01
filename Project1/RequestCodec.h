#pragma once

#include "Codec.h"

struct RequestMsg
{
	//1 ��ԿЭ��  	//2 ��ԿУ��; 	// 3 ��Կע��
	int		cmdType;		// �������� 
	char	clientId[12];	// �ͻ��˱��
	char	authCode[65];	// ��֤��
	char	serverId[12];	// �������˱�� 
	char	r1[64];			// �ͻ��������
};


class RequestCodec : public Codec
{
public:

	enum CmdType { NewOrUpdate = 1, Check, Revoke, View };

	RequestCodec();
	RequestCodec(RequestMsg* msg);
	~RequestCodec();

	// ��д���ຯ��
	//���뺯��
	int msgEncode(char** outData, int &len);

	//���뺯��
	void* msgDecode(char *inData, int inLen);

	//�ڴ��ͷ�
	int MsgMemFree(void **point);

private:
	RequestMsg *m_msg;
};