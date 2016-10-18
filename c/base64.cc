#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char base64_index[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; 

void binary_print(unsigned short v, char *src) {
    int size = sizeof(v) * 8;
    for(int i = 0; i < size; i++) {
        int bit = v & (1 << (size - i - 1));
        if(bit == 0) src[i]='0';
	 	else src[i] ='1';
    }
	src[16] = '\0';
}


void Base64Encode(unsigned char *src, char *des) {
	int len = 0;
	 while(*(src + len) != '\0') len++;

	if(*src == '\0' || len < 1) return ;
	unsigned char *p = src;
	unsigned char *p_end = src + len;

	while(p_end - p >= 3) {
		*des ++ = base64_index[p[0] >> 2];
		*des ++ = base64_index[((p[0] << 4) & 0x30) | (p[1] >> 4)];
		*des ++ = base64_index[((p[1] << 2) & 0x3C) | (p[2] >> 6)];
		*des ++ = base64_index[p[2] & 0x3F];
		p += 3;
	}
	
	if(p_end - p > 0) {
		*des++ = base64_index[p[0] >> 2];
		if ((p_end - p) == 2) {
			*des ++ = base64_index[( (p[0] << 4) & 0x30 ) | ( p[1] >> 4 )];  
			*des ++ = base64_index[(p[1] << 2) & 0x3C]; 
         	*des++ = '=';
		}
		else if (p_end - p == 1) {  
	        *des++ = base64_index[(p[0] << 4) & 0x30];  
	        *des++ = '=';  
        	*des++ = '=';
		}
	}
	*des ='\0';
}

int base64_decode_map(const char c) {
	int i = 0;
	while(base64_index[i++] != '\0') {
		if(base64_index[i-1] == c) return i-1;
	}
	return -1;
}

void Base64Decode(char *src, unsigned char* des) {
	*des = '\0';

	int len = strlen(src);
	if(len % 4 != 0)
		return ;
	
	char *p = src;
	char *p_end = src + len;
	
	for(; p < p_end; p+=4) {
		*des ++ = ( (base64_decode_map(p[0]) << 2) & 0xFC ) | ( (base64_decode_map(p[1]) >> 4) & 0x03 );
		*des ++ = ( (base64_decode_map(p[1]) << 4) & 0xF0 ) | ( (base64_decode_map(p[2]) >> 2) & 0x0F );
		*des ++ = ( (base64_decode_map(p[2]) << 6) & 0xC0 ) | ( base64_decode_map(p[3]));
	}

	if( *(src + len - 2) == '=' ) {
		*(des - 2) = '\0';
	} else if( *(src + len - 1) == '=') {
		*(des - 1) = '\0';
	}
}

int main() {
	// convert to binary output.
	printf("-----Binary convert-----\n");
    unsigned short v;
	int flag;
    char src_bit[17];
	printf("Input a unsigned short int(end with number < 0):\n");
	scanf("%d", &flag);
    while(flag >= 0 && flag <= 65535) {
		v = flag;
		if(v < 0) break;
	    binary_print(v, src_bit);
		printf("Binary char: ");
		puts(src_bit);
		scanf("%d", &flag);
	}
	putchar(10);

	printf("-----base64 encode and decode-----\n");
	// encode(base64)
	unsigned char src[500];
	char des[1000];
	// decode(base64)
	unsigned char src_reverse[500];
	printf("Input the source String(end with '#'):\n");

	scanf("%s", src);
	while(src[0] != '#') {
		Base64Encode(src, des);
		printf("Encode: ");
		puts(des);
		Base64Decode(des, src_reverse);
		printf("Decode: ");
		puts((char *)src_reverse);
		putchar(10);
		scanf("%s", src);
	}

	printf("Input base64 encode string(end with '#'):\n");
	printf("Encode: ");
	scanf("%s", des);
	while(des[0] != '#') {
		Base64Decode(des, src_reverse);
		printf("Decode: ");
		puts((char *)src_reverse);
		putchar(10);
		printf("Encode: ");
		scanf("%s", des);
	}
	
    return 0;
}
