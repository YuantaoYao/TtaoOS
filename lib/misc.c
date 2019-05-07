#include "stdio.h"
#include "const.h"


void Get_Compile_Time(char *Year, char *Month, char *Day);
int isCharEqu(char src, char des);
/*****************************************************************************
 *                                spin
 *****************************************************************************/
PUBLIC void spin(char * func_name)
{
	char Year[4],Month[2],Day[2];
	Get_Compile_Time(Year, Month, Day);
	//printf("\n %s \n", Month);
	while (1) {}
}

/*****************************************************************************
 *                           assertion_failure
 *************************************************************************//**
 * Invoked by assert().
 *
 * @param exp       The failure expression itself.
 * @param file      __FILE__
 * @param base_file __BASE_FILE__
 * @param line      __LINE__
 *****************************************************************************/
PUBLIC void assertion_failure(char *exp, char *file, char *base_file, int line)
{
	printl( "assert(%s) failed: file: %s, base_file: %s, ln: %d",
	       exp, file, base_file, line);

	/**
	 * If assertion fails in a TASK, the system will halt before
	 * printl() returns. If it happens in a USER PROC, printl() will
	 * return like a common routine and arrive here. 
	 * @see sys_printx()
	 * 
	 * We use a forever loop to prevent the proc from going on:
	 */
	spin("assertion_failure()");

	/* should never arrive here */
    __asm__ __volatile__("ud2");
}

void Get_Compile_Time(char *Year, char *Month, char *Day){

	int t = 0;
	int i;
	int j;
	const char *pMonth[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	const char Date[12] = __DATE__;//取编译时间

	Year[0] = Date[7];
	Year[1] = Date[8];
	Year[2] = Date[9];
	Year[3] = Date[10];
	Year[4] = '\0';
	
	char firstChar = Date[4];
	printf("\n %c \n", firstChar);
	Day[0] = Date[4];
	Day[1] = Date[5];
	Day[2] = '\0';
	
	for(i=0;i<12;i++){
		const char* mon = pMonth[i];
		for(j=0;j<12;j++){
			if(isCharEqu(Date[j], mon[t])){
				t++;
				if(t == 3){
					i++;
					if(i<10){
						Month[0] = '0';
						itoa(&Month[1] , i, 10) ;
					}else{
						itoa(Month , i, 10) ;
					}
					Month = (char *)i++;
				}
			}else{
				t = 0;
			}	
		}
	}
}

int isCharEqu(char src, char des){
	return src == des;
}

