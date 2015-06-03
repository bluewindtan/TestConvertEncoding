#ifndef _CONV_H__
#define _CONV_H__

#include <string>
using namespace std;

class CConv
{
public:
	static std::string ToUTF8(const string& strEncoding);
	static std::string ToUTF8(const char * strEncoding);
	static std::string ToUTF8(const char * strEncoding, int nLength);
	
	static std::string UTF8To(const string& strUTF8);
	static std::string UTF8To(const char * strUTF8);
	static std::string UTF8To(const char * strUTF8, int nLength);
public:
	static unsigned int m_nCurrentEncoding;
	static std::string m_strCurrentEncoding;
};
#endif