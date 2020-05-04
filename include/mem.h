//内存操作
PUBLIC void* memcpy(void* pDst, void* pSrc, int iSize);
PUBLIC void memset(void* p_dst, char ch, int size);
PUBLIC void * strcpy(char* p_dst, char* p_src);
PUBLIC int strlen(char * str);

#define phys_copy memcpy
#define phys_set memset