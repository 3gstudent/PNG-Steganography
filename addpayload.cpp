#include<stdio.h>
#include<string.h>

unsigned int GetCrc32(unsigned char* InStr,unsigned int len){        
	unsigned int Crc32Table[256];      
	unsigned int i,j;        
	unsigned int Crc;        
	for (i = 0; i < 256; i++){        
		Crc = i;        
		for (j = 0; j < 8; j++){        
			if (Crc & 1)        
				Crc = (Crc >> 1) ^ 0xEDB88320;        
			else       
				Crc >>= 1;      
		}        
		Crc32Table[i] = Crc;        
	}        
    
	Crc=0xffffffff;        
	for(unsigned int m=0; m<len; m++){          
		Crc = (Crc >> 8) ^ Crc32Table[(Crc & 0xFF) ^ InStr[m]];        
	}     
	
	Crc ^= 0xFFFFFFFF;     
	return Crc;        
}        

void convertStrToUnChar(char* str, unsigned char* UnChar)  
{  
    int i = strlen(str), j = 0, counter = 0;  
    char c[2];  
    unsigned int bytes[2];  
  
    for (j = 0; j < i; j += 2)   
    {  
        if(0 == j % 2)  
        {  
            c[0] = str[j];  
            c[1] = str[j + 1];  
            sscanf(c, "%02x" , &bytes[0]);  
            UnChar[counter] = bytes[0];  
            counter++;  
        }  
    }  
    return;  
}     

void AddPayload(FILE *fp)
{
	char *Payload="calc.exe";
	unsigned char *buf;
	int len;
	int crc32;
	len=strlen(Payload);	
	buf=new unsigned char[len+12];
	buf[0]=len>>24&0xff;
	buf[1]=len>>16&0xff;
	buf[2]=len>>8&0xff;
	buf[3]=len&0xff;
	buf[4]='t';
	buf[5]='E';
	buf[6]='X';
	buf[7]='t';
	for(int j=0;j<len;j++)
		buf[j+8]=Payload[j];
	buf[len+8]=0XFA;
	buf[len+9]=0XC4;
	buf[len+10]=0X08;
	buf[len+11]=0X76;
	fwrite(buf,len+12,1,fp);
}

int main(int argc, char* argv[])
{
	FILE *fp,*fpnew;   
	unsigned char *buf=NULL;
	unsigned int len=0;
	unsigned int ChunkLen=0;
	unsigned int ChunkCRC32=0;
	unsigned int ChunkOffset=0;	
	unsigned int crc32=0;
	unsigned int i=0,j=0;
	unsigned char Signature[8]={0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a};	
    unsigned char IEND[12]={0x00,0x00,0x00,0x00,0x49,0x45,0x4e,0x44,0xae,0x42,0x60,0x82};	   
	
    if((fp=fopen("c:\\test\\test.png","rb+"))==NULL)
        return 0;  
	if((fpnew=fopen("c:\\test\\new.png","wb"))==NULL)
        return 0;  
	fseek(fp,0,SEEK_END);
	len=ftell(fp);
	buf=new unsigned char[len];
    fseek(fp,0,SEEK_SET);
    fread(buf,len,1,fp);
    printf("Total Len=%d\n",len);
	printf("----------------------------------------------------\n");
	fseek(fp,8,SEEK_SET);
	ChunkOffset=8;
	i=0; 
	fwrite(Signature,8,1,fpnew);
	while(1)
	{
		i++;
		j=0;
		memset(buf,0,len);
		fread(buf,4,1,fp);
		fwrite(buf,4,1,fpnew);
		ChunkLen=(buf[0]<<24)|(buf[1]<<16)|(buf[2]<<8)|buf[3];
		fread(buf,4+ChunkLen,1,fp);
		printf("[+]ChunkName:%c%c%c%c		",buf[0],buf[1],buf[2],buf[3]);
		if(strncmp((char *)buf,"IHDR",4)==0|strncmp((char *)buf,"PLTE",4)==0|strncmp((char *)buf,"IDAT",4)==0)
		{	
			printf("Palette Chunk\n");

			fwrite(buf,4+ChunkLen,1,fpnew);
		}
		else
		{
			printf("Ancillary Chunk\n");
			fseek(fpnew,-4,SEEK_CUR);
			j=1;
		}
		printf("   ChunkOffset:0x%08x	\n",ChunkOffset);
		printf("   ChunkLen: %10d		\n",ChunkLen);
		crc32=GetCrc32(buf,ChunkLen+4);
		printf("   ExpectCRC32:%08X\n",crc32);
		fread(buf,4,1,fp);
		ChunkCRC32=(buf[0]<<24)|(buf[1]<<16)|(buf[2]<<8)|buf[3];
		printf("   ChunkCRC32: %08X		",ChunkCRC32);
		if(crc32!=ChunkCRC32)
			printf("[!]CRC32Check Error!\n");
		else
		{
			printf("Check Success!\n\n");
			if(j==0)
				fwrite(buf,4,1,fpnew);
		}
		ChunkLen=ftell(fp);
		if(ChunkLen==(len-12))
		{
			printf("\n----------------------------------------------------\n");
			printf("Total Chunk:%d\n",i);		
			break;
		}
	}
	AddPayload(fpnew);
	fwrite(IEND,12,1,fpnew);
	fclose(fp);
	fclose(fpnew);
    return 0;	
}
