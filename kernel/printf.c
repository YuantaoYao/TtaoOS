
int printf(const char *fmt, ...){
	int i;
	char buf[256];
	
	va_list args = (va_list) ((char*) (&fmt) + 4);/* 4是参数fmt所占堆栈中的大小 */
	i = vsprintf(buf, fmt, args);
	write(buf, i);
	
	return i;
}