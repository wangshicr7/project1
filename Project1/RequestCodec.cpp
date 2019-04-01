#include "RequestCodec.h"

#include<iostream>
using namespace std;

//无参构造函数 创建一个解码对象
RequestCodec::RequestCodec()
{
	m_msg = new RequestMsg;
}

//有参构造 创建一个编码对象
RequestCodec::RequestCodec(RequestMsg * msg)
{
	m_msg = new RequestMsg;

	m_msg->cmdType = msg->cmdType;
	strcpy(m_msg->clientId, msg->clientId);

	strcpy(m_msg->authCode, msg->authCode);
	strcpy(m_msg->serverId, msg->serverId);
	strcpy(m_msg->r1, msg->r1);

}
//析构
RequestCodec::~RequestCodec()
{
	delete m_msg;
}

//请求报文编码函数
int RequestCodec::msgEncode(char ** outData, int &len)
{
	ITCAST_INT ret = -1;
	//1.编码cmdType
	ret = writeHeadNode(m_msg->cmdType);
	if (0 != ret)
	{
		cout << "writeHeadNode failed..." << endl;
		return ret;
	}
	//2.编码clientID
	ret = writeNextNode(m_msg->clientId, strlen(m_msg->clientId));
	if (0 != ret)
	{
		cout << "writeNextNode failed..." << endl;
		return ret;
	}
	//3.编码Authcode
	ret = writeNextNode(m_msg->authCode, strlen(m_msg->authCode));
	if (0 != ret)
	{
		cout << "writeNextNode failed..." << endl;
		return ret;
	}
	

	//4.编码serverID
	ret = writeNextNode(m_msg->serverId, strlen(m_msg->serverId));
	if (0 != ret)
	{
		cout << "writeNextNode failed..." << endl;
		return ret;
	}

	//5.编码r1
	ret = writeNextNode(m_msg->r1, strlen(m_msg->r1));
	if (0 != ret)
	{
		cout << "writeNextNode failed..." << endl;
		return ret;
	}


	//6.编码结构体
	ret = packSequence(outData, len);
	if (0 != ret)
	{
		cout << "packSequence failed..." << endl;
		return ret;
	}
	//7.传出

	//8.释放内存
	freeSequence();
	return 0;
}

//请求报文解码函数
void * RequestCodec::msgDecode(char * inData, int inLen)
{
	ITCAST_INT ret = -1;
	//1.解码结构体
	ret = unpackSequence(inData, inLen);
	if (0 != ret)
	{
		cout << "unpackSequence failed..." << endl;
	}
	
	//2.解码cmdType
	int readHeadNode(m_msg->cmdType);
	if (0 != ret)
	{
		cout << "readHeadNode failed..." << endl;
	}

	//3.解码clientId
	ret = readNextNode(m_msg->clientId);
	if (0 != ret)
	{
		cout << "readNextNode failed..." << endl;
	}
	//4.解码authCode
	ret = readNextNode(m_msg->authCode);
	if (0 != ret)
	{
		cout << "readNextNode failed..." << endl;
	}

	//5.解码serverId
	ret = readNextNode(m_msg->serverId);
	if (0 != ret)
	{
		cout << "readNextNode failed..." << endl;
	}

	//6.解码r1
	ret = readNextNode(m_msg->r1);
	if (0 != ret)
	{
		cout << "readNextNode failed..." << endl;
	}

	//7.传出
	freeSequence();

	//8.释放内存
	return m_msg;
}

//内存释放 请求报文结构体内存
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
