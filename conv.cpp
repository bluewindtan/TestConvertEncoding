#include "conv.h"
#include <stdio.h>
#include <iostream>
using namespace std;

#ifdef WIN32
#include <windows.h>
#else
#include <iconv.h>
#include <errno.h>
#endif

///////////////////////////////////////////////////////////
static const std::string Encoding_UTF8 = "utf-8";
static const std::string CONV_IGNORE = "//IGNORE";
static const unsigned short LENGTH_MULTIPLE = 4;

///////////////////////////////////////////////////////////
// define the encoding of current version 
// Chinese; HongKong & TaiWan; Singapore & Malaysia; Indonesia;
unsigned int CConv::m_nCurrentEncoding = 0;
std::string CConv::m_strCurrentEncoding = "gb18030";

bool CConv::m_bNeedConvert = true;

//// Thailand 
//unsigned int CConv::m_nCurrentEncoding = 874;
//std::string CConv::m_strCurrentEncoding = "CP874";

//// Brazil
//unsigned int CConv::m_nCurrentEncoding = 28591;
//std::string CConv::m_strCurrentEncoding = "ISO-8859-1";

//// Korea 
//unsigned int CConv::m_nCurrentEncoding = 949;
//std::string CConv::m_strCurrentEncoding = "CP949";

//// Vietnam
//unsigned int CConv::m_nCurrentEncoding = 65001;
//std::string CConv::m_strCurrentEncoding = "UTF-8";

///////////////////////////////////////////////////////////

std::string CConv::ToUTF8(const string& strEncoding)
{
	if (!CConv::m_bNeedConvert)
	{
		return strEncoding;
	}
	string strOutUTF8 = strEncoding;
#ifdef WIN32
	if (CP_UTF8 != m_nCurrentEncoding)
	{
		WCHAR * str1;
		int nLength = MultiByteToWideChar(m_nCurrentEncoding, 0, strEncoding.c_str(), -1, NULL, 0);
		if (nLength > 0)
		{
			str1 = new WCHAR[nLength + 1];
			int nReturn = MultiByteToWideChar(m_nCurrentEncoding, 0, strEncoding.c_str(), -1, str1, nLength);
			if (nReturn == nLength)
			{
				nLength = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
				if (nLength > 0)
				{
					char * str2 = new char[nLength + 1];
					nReturn = WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, nLength, NULL, NULL);
					if (nReturn == nLength)
					{
						strOutUTF8 = str2;
					}
					delete[] str2;
					str2 = NULL;
				}
			}
			delete[] str1;
			str1 = NULL;
		}
	}
#else
	if (0 != Encoding_UTF8.compare(m_strCurrentEncoding))
	{
		iconv_t cd;
		int rc; 
		size_t inlen = strEncoding.size();
		char * szEncoding = new char[inlen+1];
		szEncoding[inlen] = 0;
		memcpy(szEncoding,strEncoding.c_str(),inlen);
		char **pin = &szEncoding;

		string strConvFrom = m_strCurrentEncoding + CONV_IGNORE;
		string strConvTo = Encoding_UTF8 + CONV_IGNORE;

		cd = iconv_open(strConvTo.c_str(), strConvFrom.c_str());
		if (0 == cd)
		{
			delete[] szEncoding;
			cout << "iconv_open Error =" << errno << " : encoding=" << strConvTo << ", " << strConvFrom << std::endl;
			return strOutUTF8; 
		}

		size_t outLen = strEncoding.size() * LENGTH_MULTIPLE + 1;
		char * outbuf = new char [outLen];
		char **pout = &outbuf; 

		memset(outbuf,0,outLen); 
		char* pInBuf = szEncoding;
		char* pOutBuf = outbuf;
		int nRet = iconv(cd,pin,&inlen,pout,&outLen);
		if (-1 == nRet)
		{
			cout << "ConvertUTF82Encoding Error =" << errno << " : " << strEncoding << ". " 
				<< strConvFrom << " --> " << strConvTo << std::endl;
		}
		else
		{
			string strTemp(pOutBuf);
			strOutUTF8.assign(strTemp);
		}
		iconv_close(cd); 

		delete[] pInBuf;
		delete[] pOutBuf;
	}
#endif

	return strOutUTF8;
}

std::string CConv::UTF8To(const string& strUTF8)
{
	if (!CConv::m_bNeedConvert)
	{
		return strUTF8;
	}
	string strOutEncoding = strUTF8;
#ifdef WIN32
	if (CP_UTF8 != m_nCurrentEncoding)
	{
		int nLength = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
		if (nLength > 0)
		{
			unsigned short * wstrEncoding = new unsigned short[nLength + 1];
			memset(wstrEncoding, 0, nLength * 2 + 2);

			int nReturn = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, (LPWSTR)wstrEncoding, nLength);
			if (nReturn == nLength)
			{
				nLength = WideCharToMultiByte(m_nCurrentEncoding, 0, (LPCWCH)wstrEncoding, -1, NULL, 0, NULL, NULL);
				if (nLength > 0)
				{
					char *szEncoding = new char[nLength + 1];
					memset(szEncoding, 0, nLength + 1);

					nReturn = WideCharToMultiByte(m_nCurrentEncoding,0, (LPCWCH)wstrEncoding, -1, szEncoding, nLength, NULL, NULL);
					if (nReturn == nLength)
					{
						string strTemp(szEncoding);
						strOutEncoding.assign(strTemp);
					}
					delete[] szEncoding;
					szEncoding = NULL;
				}
			}
			delete[] wstrEncoding;
			wstrEncoding = NULL;
		}
	}
#else
	if (0 != Encoding_UTF8.compare(m_strCurrentEncoding))
	{
		iconv_t cd;
		int rc; 
		size_t inlen = strUTF8.size();
		char * utfChar = new char[inlen+1];
		utfChar[inlen] = 0;
		memcpy(utfChar,strUTF8.c_str(),inlen);
		char **pin = &utfChar;

		string strConvFrom = Encoding_UTF8 + CONV_IGNORE;
		string strConvTo = m_strCurrentEncoding + CONV_IGNORE;

		cd = iconv_open(strConvTo.c_str(), strConvFrom.c_str());
		if (0 == cd)
		{
			delete[] utfChar;
			cout << "iconv_open Error =" << errno << " : encoding=" << strConvTo << ", "  << strConvFrom << std::endl;
			return strOutEncoding; 
		}
		size_t outLen = strUTF8.size() * LENGTH_MULTIPLE + 1;
		char * outbuf = new char [outLen];
		char **pout = &outbuf; 

		memset(outbuf,0,outLen); 
		char* pInBuf = utfChar;
		char* pOutBuf = outbuf;
		int nRet = iconv(cd,pin,&inlen,pout,&outLen);
		if (-1 == nRet)
		{
			cout << "ConvertUTF82Encoding Error =" << errno << " : " << strUTF8 << ". " 
				<< strConvFrom << " --> " << strConvTo << std::endl;
		}
		else
		{
			string strTemp(pOutBuf);
			strOutEncoding.assign(strTemp);
		}
		iconv_close(cd); 

		delete[] pInBuf;
		delete[] pOutBuf;
	}
#endif

	return strOutEncoding;
}

std::string CConv::ToUTF8(const char * strEncoding, int nLength)
{
	char* szBuff = new char[nLength+1];
	szBuff[nLength] = 0;
	strncpy(szBuff,strEncoding,nLength);
	std::string szEncoding = szBuff;
	delete[] szBuff;
	return CConv::ToUTF8(szEncoding);
}

std::string CConv::UTF8To(const char * strUTF8, int nLength)
{
	char* szBuff = new char[nLength+1];
	szBuff[nLength] = 0;
	strncpy(szBuff,strUTF8,nLength);
	std::string szUTF8 = szBuff;
	delete[] szBuff;
	return CConv::UTF8To(szUTF8);
}

std::string CConv::ToUTF8(const char * strEncoding)
{
	std::string szEncoding = strEncoding;
	return CConv::ToUTF8(szEncoding);
}

std::string CConv::UTF8To(const char * strUTF8)
{
	std::string szUTF8 = strUTF8;
	return CConv::UTF8To(szUTF8);
}
