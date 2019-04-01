#include "SequenceASN1.h"
#include <iostream>
using namespace std;
//构造函数
SequenceASN1::SequenceASN1():BaseASN1()
{
}
//析构函数
SequenceASN1::~SequenceASN1()
{
}

//编码整型 第一个节点

int SequenceASN1::writeHeadNode(int iValue)
{
	ITCAST_INT ret = -1;
	ret = DER_ItAsn1_WriteInteger(iValue, &m_header);
	if (0 != ret)
	{
		cout << "DER_ItAsn1_WriteInteger failed.." << endl;
		return ret;
	}
	//指向第一个节点
	m_next = m_header;
	return 0;
}

//编码字符串
int SequenceASN1::writeHeadNode(char * sValue, int len)
{
	ITCAST_INT ret = -1;
	//将char*转化为ANYBUF类型
	ret = DER_ITCAST_String_To_AnyBuf(&m_temp,(unsigned char*)sValue, len);
	if (0 != ret)
	{
		cout << "DER_ITCAST_String_To_AnyBuf failed...."<< endl;	
		return ret;
	}

	//编码字符串
	ret = DER_ItAsn1_WritePrintableString(m_temp, &m_header);
	if (0 != ret)
	{
		cout << "DER_ItAsn1_WritePrintableString failed...." << endl;
		return ret;
	}

	//指向第一个节点
	m_next = m_header;

	//释放内存
	DER_ITCAST_FreeQueue(m_temp);
	m_temp = NULL;
	return 0;
}

int SequenceASN1::writeNextNode(int iValue)
{
	ITCAST_INT ret = -1;
	ret = DER_ItAsn1_WriteInteger(iValue, &(m_next->next));
	if (0 != ret)
	{
		cout << "DER_ItAsn1_WriteInteger failed.." << endl;
		return ret;
	}
	//指向下一个节点
	m_next = m_next->next;
	return 0;
}

int SequenceASN1::writeNextNode(char * sValue, int len)
{
	ITCAST_INT ret = -1;
	ret = EncodeChar(sValue, len, &(m_next->next));
	if (0 != ret)
	{
		cout << "EncodeChar failed..." << endl;
		return ret;
	}

	//指向第一个节点
	m_next = m_next->next;
	return 0;
}

int SequenceASN1::readHeadNode(int & iValue)
{
	ITCAST_INT ret = -1;
	ret = DER_ItAsn1_ReadInteger(m_header, (ITCAST_UINT32*)&iValue);
	if (0 != ret)
	{
		cout << "DER_ItAsn1_ReadInteger failed..." << endl;
		return ret;
	}

	//指向下一个节点
	m_next = m_header->next;

	return 0;
}

int SequenceASN1::readHeadNode(char * sValue)
{
	ITCAST_INT ret = -1;
	ret = DER_ItAsn1_ReadPrintableString(m_header, &m_temp);
	if (0 != ret)
	{
		cout << "DER_ItAsn1_ReadPrintableString failed..." << endl;
		return ret;
	}

	//拷贝数据
	memcpy(sValue, m_temp->pData, m_temp->dataLen);
	
	//指向下一个节点
	m_next = m_header->next;

	//释放内存
	DER_ITCAST_FreeQueue(m_temp);

	return 0;
}

int SequenceASN1::readNextNode(int & iValue)
{
	ITCAST_INT ret = -1;
	ret = DER_ItAsn1_ReadInteger(m_next, (ITCAST_UINT32*)&iValue);
	if (0 != ret)
	{
		cout << "DER_ItAsn1_ReadInteger failed..." << endl;
		return ret;
	}

	//指向下一个节点
	m_next = m_next->next;

	return 0;
}

int SequenceASN1::readNextNode(char * sValue)
{
	ITCAST_INT ret = -1;
	ret = DER_ItAsn1_ReadPrintableString(m_next, &m_temp);
	if (0 != ret)
	{
		cout << "DER_ItAsn1_ReadPrintableString failed..." << endl;
		return ret;
	}

	//拷贝数据
	memcpy(sValue, m_temp->pData, m_temp->dataLen);

	//指向下一个节点
	m_next = m_next->next;

	//释放内存
	DER_ITCAST_FreeQueue(m_temp);
	return 0;
}

//编码结构体
int SequenceASN1::packSequence(char ** outData, int & outLen)
{
	ITCAST_INT ret = -1;
	//编码
	ret = DER_ItAsn1_WriteSequence(m_header, &m_temp);
	if (0 != ret)
	{
		cout << "EncodeChar failed..." << endl;
		return ret;
	}
	//分配空间
	*outData = new char[m_temp->dataLen + 1];
	//拷贝数据
	memcpy(*outData, m_temp->pData, m_temp->dataLen);
	outLen = m_temp->dataLen;

	//释放内存
	ret = DER_ITCAST_FreeQueue(m_header);
	if (0 != ret)
	{
		cout << "DER_ITCAST_FreeQueue failed..." << endl;
		return ret;
	}
	return 0;
}

int SequenceASN1::unpackSequence(char * inData, int inLen)
{
	ITCAST_INT ret = -1;
	//将char*转化为ANYBUF类型
	ret = DER_ITCAST_String_To_AnyBuf(&m_temp, (unsigned char*)inData, inLen);
	if (0 != ret)
	{
		cout << "DER_ITCAST_String_To_AnyBuf failed...." << endl;
		return ret;
	}
	//解码之后成为链表
	ret = DER_ItAsn1_ReadSequence(m_temp, &m_header);
	if (0 != ret)
	{
		cout << "DER_ItAsn1_ReadSequence failed...." << endl;
		DER_ITCAST_FreeQueue(m_temp);
		return ret;
	}

	//释放内存
	DER_ITCAST_FreeQueue(m_temp);
	return 0;
}

void SequenceASN1::freeSequence(ITCAST_ANYBUF * node)
{
}
