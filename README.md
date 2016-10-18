# Base64
C and Java implements for Base64 encode and decode

## Unsigned short int 转换为二进制字符串
c语言中unsigned short 的数值范围是 0~65535
```c
void binary_print(unsigned short v, char *src) {
    int size = sizeof(v) * 8;
    for(int i = 0; i < size; i++) {
        int bit = v & (1 << (size - i - 1));
        if(bit == 0) src[i]='0';
	 	else src[i] ='1';
    }
	src[16] = '\0';
}

```
实验结果展示： 

![show][1]

通过实验，显示当输入范围不在0~65535之间的时候，转换过程结束。main函数中部分调用如下：
```c
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
```



## Base64的简单实现
Base64是网络上一种常见的交换码。既然是交换码，就符合交换码的主要思想：在数据通过网络传输之前对之进行编码，使之符合网络传递的要求，在接收到编码过得数据后对之进行解码，从而恢复数据。
Base64是如今互联网应用最多的一种编码，大多数的电子邮件软件都把它作为默认的编码方式。

### 编码原理：
编码的时候，将三个byte的数据，先后放入一个24bit的缓冲区中，先来的byte占高位。数据不足3byte的话，于缓冲区中剩下的bit用0补足。然后，每次取出6个bit，按照其值选择[a-zA-Z0-9+/]中的字符作为编码后的输出。不断进行，直到全部输入数据转换完成。

如果最后剩下两个输入数据，在编码结果后加1个“=”；如果最后剩下一个输入数据，编码结果后加2个“=”；如果没有剩下任何数据，就什么都不要加，这样才可以保证资料还原的正确性。

编码后的数据比原始数据略长，为原来的4/3。无论什么样的字符都会全部被编码

编码：
```c
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
```

### 解码原理
解码作为编码的逆过程，就显得比较简单了， 分析原先的编码过程，很容易得出最后的解码过程，代码实现如下：
```c
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

	if( *(src + len - 2) == '=' ) 
		*(des - 2) = '\0';
	else if( *(src + len - 1) == '=')
		*(des - 1) = '\0';
}
```

###结果展示：
![decode][2]


  [1]: https://github.com/trajepl/ljp/blob/master/pic/binary_show.png?raw=true
  [2]: https://github.com/trajepl/ljp/blob/master/pic/decodeEncode.png?raw=true
