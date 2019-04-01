#include "RequestCodec.h"

#include<iostream>
using namespace std;

//�޲ι��캯�� ����һ���������
RequestCodec::RequestCodec()
{
	m_msg = new RequestMsg;
}

//�вι��� ����һ���������
RequestCodec::RequestCodec(RequestMsg * msg)
{
	m_msg = new RequestMsg;

	m_msg->cmdType = msg->cmdType;
	strcpy(m_msg->clientId, msg->clientId);

	strcpy(m_msg->authCode, msg->authCode);
	strcpy(m_msg->serverId, msg->serverId);
	strcpy(m_msg->r1, msg->r1);

}
//����
RequestCodec::~RequestCodec()
{
	delete m_msg;
}

//�����ı��뺯��
int RequestCodec::msgEncode(char ** outData, int &len)
{
	ITCAST_INT ret = -1;
	//1.����cmdType
	ret = writeHeadNode(m_msg->cmdType);
	if (0 != ret)
	{
		cout << "writeHeadNode failed..." << endl;
		return ret;
	}
	//2.����clientID
	ret = writeNextNode(m_msg->clientId, strlen(m_msg->clientId));
	if (0 != ret)
	{
		cout << "writeNextNode failed..." << endl;
		return ret;
	}
	//3.����Authcode
	ret = writeNextNode(m_msg->authCode, strlen(m_msg->authCode));
	if (0 != ret)
	{
		cout << "writeNextNode failed..." << endl;
		return ret;
	}
	

	//4.����serverID
	ret = writeNextNode(m_msg->serverId, strlen(m_msg->serverId));
	if (0 != ret)
	{
		cout << "writeNextNode failed..." << endl;
		return ret;
	}

	//5.����r1
	ret = writeNextNode(m_msg->r1, strlen(m_msg->r1));
	if (0 != ret)
	{
		cout << "writeNextNode failed..." << endl;
		return ret;
	}


	//6.����ṹ��
	ret = packSequence(outData, len);
	if (0 != ret)
	{
		cout << "packSequence failed..." << endl;
		return ret;
	}
	//7.����

	//8.�ͷ��ڴ�
	freeSequence();
	return 0;
}

//�����Ľ��뺯��
void * RequestCodec::msgDecode(char * inData, int inLen)
{
	ITCAST_INT ret = -1;
	//1.����ṹ��
	ret = unpackSequence(inData, inLen);
	if (0 != ret)
	{
		cout << "unpackSequence failed..." << endl;
	}
	
	//2.����cmdType
	int readHeadNode(m_msg->cmdType);
	if (0 != ret)
	{
		cout << "readHeadNode failed..." << endl;
	}

	//3.����clientId
	ret = readNextNode(m_msg->clientId);
	if (0 != ret)
	{
		cout << "readNextNode failed..." << endl;
	}
	//4.����authCode
	ret = readNextNode(m_msg->authCode);
	if (0 != ret)
	{
		cout << "readNextNode failed..." << endl;
	}

	//5.����serverId
	ret = readNextNode(m_msg->serverId);
	if (0 != ret)
	{
		cout << "readNextNode failed..." << endl;
	}

	//6.����r1
	ret = readNextNode(m_msg->r1);
	if (0 != ret)
	{
		cout << "readNextNode failed..." << endl;
	}

	//7.����
	freeSequence();

	//8.�ͷ��ڴ�
	return m_msg;
}

//�ڴ��ͷ� �����Ľṹ���ڴ�
int RequestCodec::MsgMemFree(void ** point)
{
	if (NULL == point || NULL == *point)
	{
		cout << "parameters are NULL" << endl;
		return -1;
	}

	delete *point;
	*point = NULL;
	return 0;
}
