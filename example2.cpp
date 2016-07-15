#include<stdio.h>
#include<string.h>
int main(int argc, char* argv[])
{
	FILE *fp;   
    if((fp=fopen("c:\\test\\test.png","rb+"))==NULL)
        return 0;   
	fseek(fp,0,SEEK_END);
	int len=ftell(fp);
	unsigned char *buf=new unsigned char[len];	
    fseek(fp,0,SEEK_SET);
    fread(buf,len,1,fp);
    printf("len=%d\n",len);
	for(int i=1;i<=len;i++)
	{
		printf("%02X ",buf[i-1]);
		if(i%16==0)
			printf("\n");
	}
    fclose(fp);
	printf("\n");
    return 0;	
}
