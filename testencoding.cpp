#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "conv.h"

using namespace std;

int main(int argc, char* argv[])
{  
	for(int i = 0; i < argc; i++)
	{
    	printf("argv[%d] = %s.\n", i, argv[i]);	
  	}
	if ( argc != 2)
	{
		printf("error parameter.");
		return 0;
	}
	FILE *fp1;
	fp1 = fopen(argv[1], "r");  
	FILE *fp2;
	char file2[1024] = {'\0'};
	sprintf(file2, "%s_utf8", argv[1]);
	fp2 = fopen(file2, "w"); 
	char file3[1024] = {'\0'};
	sprintf(file3, "%s_ori", argv[1]);
	FILE *fp3;
	fp3 = fopen(file3, "w");  
	char buff[1024] = {'\0'};  
	memset(buff, 0, 1024);
	int count = 0;  
	while((count = fread(buff, 1, 250, fp1)) != 0)  
	{  
		std::string strOutUTF8 = CConv::ToUTF8(buff);
		fwrite(strOutUTF8.c_str(), 1, strOutUTF8.length(), fp2);
		std::string strOut = CConv::UTF8To(strOutUTF8);
		fwrite(strOut.c_str(), 1, strOut.length(), fp3);
		memset(buff, 0, 1024);
	}

	fclose(fp1);  
	fclose(fp2);
	fclose(fp3);

	return 0;
}


