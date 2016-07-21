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

int main(int argc, char* argv[])
{
	FILE *fp;   
	unsigned char *buf=NULL;
	unsigned int len=0;
	unsigned int ChunkLen=0;
	unsigned int ChunkCRC32=0;
	unsigned int ChunkOffset=0;	
	unsigned int crc32=0;
	unsigned int i=0;
    if((fp=fopen("c:\\test\\test.png","rb+"))==NULL)
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
	while(1)
	{
		i++;
		memset(buf,0,len);
		fread(buf,4,1,fp);
		ChunkLen=(buf[0]<<24)|(buf[1]<<16)|(buf[2]<<8)|buf[3];
		fread(buf,4+ChunkLen,1,fp);
		printf("[+]ChunkName:%c%c%c%c		",buf[0],buf[1],buf[2],buf[3]);
		if(strncmp((char *)buf,"IHDR",4)==0|strncmp((char *)buf,"PLTE",4)==0|strncmp((char *)buf,"IDAT",4)==0)
			printf("Palette Chunk\n");
		printf("Ancillary Chunk\n");
		printf("   ChunkOffset:0x%08x	\n",ChunkOffset);
		printf("   ChunkLen: %10d		\n",ChunkLen);
		ChunkOffset+=ChunkLen+12;
		crc32=GetCrc32(buf,ChunkLen+4);
		printf("   ExpectCRC32:%08X\n",crc32);
		fread(buf,4,1,fp);
		ChunkCRC32=(buf[0]<<24)|(buf[1]<<16)|(buf[2]<<8)|buf[3];
		printf("   ChunkCRC32: %08X		",ChunkCRC32);
		if(crc32!=ChunkCRC32)
			printf("[!]CRC32Check Error!\n");
		else
			printf("Check Success!\n\n");
		ChunkLen=ftell(fp);
		if(ChunkLen==(len-12))
		{
			printf("\n----------------------------------------------------\n");
			printf("Total Chunk:%d\n",i);		
			break;
		}
	}
	fclose(fp);
    return 0;	
}
