#include <string.h>
unsigned int GetCrc32(char* InStr,unsigned int len){        
  unsigned int Crc32Table[256];      
  int i,j;        
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
  for(int m=0; m<len; m++){          
    Crc = (Crc >> 8) ^ Crc32Table[(Crc & 0xFF) ^ InStr[m]];        
  }     
       
  Crc ^= 0xFFFFFFFF;     
  return Crc;        
}        
int main(int argc, char* argv[])
{
	char buf[17]={0x49,0x48,0x44,0x52,0x00,0x00,0x00,0x1A,0x00,0x00,0x00,0x1A,0x08,0x04,0x00,0x00,0x00};
	unsigned int crc32=GetCrc32(buf,sizeof(buf));
	printf("%08X\n",crc32);
	return 0;
}
