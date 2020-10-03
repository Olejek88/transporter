#include <windows.h>
#include "service.h"

#define MAIN_LOG	"\\Micro_SD\\Temp\\FTP\\log3"
#define XML_FILE	"\\Micro_SD\\Temp\\FTP\\data.xml"
FILE	*Log;
FILE    *Dat;

extern	VOID ULOGW (CHAR* string, ...);		// log file function

bool WriteDataFile (byte format);
uint ParseFile (char* buf, char* param);

//----------------------------------------------------------------------------------------------------
// [Thread] service thread
VOID ServiceThread (LPVOID lpParam)
{
 while (WorkEnable)
	{
	 WriteDataFile (0);
	 Sleep (60000);
	} 
}
//----------------------------------------------------------------------------------------------------
// get data from SQL and save to CSV(0)/XML(1) file
bool WriteDataFile (byte format)
{
 TCHAR InBuffer[300];
 CHAR   buf[300],date[50];
 Dat =  fopen(XML_FILE,"w");

 GetDateFormat(LOCALE_SYSTEM_DEFAULT, DATE_SHORTDATE, NULL, NULL, InBuffer, sizeof(InBuffer));
 WideCharToMultiByte(CP_ACP, 0, InBuffer, sizeof(InBuffer), date, sizeof(date), NULL,NULL);
 GetTimeFormat(LOCALE_SYSTEM_DEFAULT, TIME_FORCE24HOURFORMAT, NULL, NULL, InBuffer, sizeof(InBuffer));
 WideCharToMultiByte(CP_ACP, 0, InBuffer, sizeof(InBuffer), buf, sizeof(buf), NULL,NULL);
 sprintf (date,"%s %s",date,buf);

 sprintf (buf,"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"); fprintf (Dat, buf);
 sprintf (buf,"<stats>\n"); fprintf (Dat, buf);
 sprintf (buf,"<stat name=\"totalDetected\" value=\"%ld\" time=\"%s\">\n",totalDetected,date); fprintf (Dat, buf); 
 sprintf (buf,"<stat name=\"totalLoaded\" value=\"%ld\" time=\"%s\">\n",totalLoaded,date); fprintf (Dat, buf); 
 sprintf (buf,"<stat name=\"todayDetected\" value=\"%ld\" time=\"%s\">\n",todayDetected,date); fprintf (Dat, buf); 
 sprintf (buf,"<stat name=\"todayLoaded\" value=\"%ld\" time=\"%s\">\n",todayLoaded,date); fprintf (Dat, buf); 
 sprintf (buf,"</stats>\n"); fprintf (Dat, buf); 

 sprintf (buf,"<inputs>\n"); fprintf (Dat, buf);
 sprintf (buf,"<input slot=\"0\" channel=\"0\" name=\"MO1_LG\" value=\"%f\" time=\"%s\">\n",MO1_LG,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"0\" channel=\"1\" name=\"MOD1_1\" value=\"%f\" time=\"%s\">\n",MOD1_1,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"0\" channel=\"2\" name=\"MOD1_2\" value=\"%f\" time=\"%s\">\n",MOD1_2,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"0\" channel=\"3\" name=\"MOD1_3\" value=\"%f\" time=\"%s\">\n",MOD1_3,date); fprintf (Dat, buf); 

 sprintf (buf,"<input slot=\"1\" channel=\"0\" name=\"VC_LG\" value=\"%f\" time=\"%s\">\n",VC_LG,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"1\" channel=\"1\" name=\"MOD2_1\" value=\"%f\" time=\"%s\">\n",MOD2_1,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"1\" channel=\"2\" name=\"MOD2_2\" value=\"%f\" time=\"%s\">\n",MOD2_2,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"1\" channel=\"3\" name=\"MOD2_3\" value=\"%f\" time=\"%s\">\n",MOD2_3,date); fprintf (Dat, buf); 

 sprintf (buf,"<input slot=\"2\" channel=\"0\" name=\"FQ1_ERROR\" value=\"%d\" time=\"%s\">\n",FQ1_ERROR,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"2\" channel=\"1\" name=\"FQ1_BUSY\" value=\"%d\" time=\"%s\">\n",FQ1_BUSY,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"2\" channel=\"2\" name=\"FQ1_OUTPUT\" value=\"%d\" time=\"%s\">\n",FQ1_OUTPUT,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"2\" channel=\"3\" name=\"FQ2_ERROR\" value=\"%d\" time=\"%s\">\n",FQ2_ERROR,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"2\" channel=\"4\" name=\"FQ2_BUSY\" value=\"%d\" time=\"%s\">\n",FQ2_BUSY,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"2\" channel=\"5\" name=\"FQ2_OUTPUT\" value=\"%d\" time=\"%s\">\n",FQ2_OUTPUT,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"2\" channel=\"6\" name=\"XUK1_ALARM\" value=\"%d\" time=\"%s\">\n",XUK1_ALARM,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"2\" channel=\"7\" name=\"XUK1_OUTPUT\" value=\"%d\" time=\"%s\">\n",XUK1_OUTPUT,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"2\" channel=\"8\" name=\"XUK2_ALARM\" value=\"%d\" time=\"%s\">\n",XUK2_ALARM,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"2\" channel=\"9\" name=\"XUK2_OUTPUT\" value=\"%d\" time=\"%s\">\n",XUK2_OUTPUT,date); fprintf (Dat, buf); 

 sprintf (buf,"<input slot=\"3\" channel=\"0\" name=\"SERV_SU\" value=\"%d\" time=\"%s\">\n",SERV_SU,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"3\" channel=\"1\" name=\"SERV_READY\" value=\"%d\" time=\"%s\">\n",SERV_READY,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"3\" channel=\"2\" name=\"SERV_ALARM\" value=\"%d\" time=\"%s\">\n",SERV_ALARM,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"3\" channel=\"3\" name=\"SERV_CON_VKL\" value=\"%d\" time=\"%s\">\n",SERV_CON_VKL,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"3\" channel=\"4\" name=\"SERV_UACTIVE\" value=\"%d\" time=\"%s\">\n",SERV_UACTIVE,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"3\" channel=\"5\" name=\"DOZA1_READY\" value=\"%d\" time=\"%s\">\n",DOZA1_READY,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"3\" channel=\"6\" name=\"DOZA2_READY\" value=\"%d\" time=\"%s\">\n",DOZA2_READY,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"3\" channel=\"7\" name=\"CONV_STOP\" value=\"%d\" time=\"%s\">\n",CONV_STOP,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"3\" channel=\"8\" name=\"DOZA1_ERROR\" value=\"%d\" time=\"%s\">\n",DOZA1_ERROR,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"3\" channel=\"9\" name=\"DOZA2_ERROR\" value=\"%d\" time=\"%s\">\n",DOZA2_ERROR,date); fprintf (Dat, buf); 

 sprintf (buf,"<input slot=\"4\" channel=\"0\" name=\"FQ1_TRIG\" value=\"%d\" time=\"%s\">\n",FQ1_TRIG,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"4\" channel=\"1\" name=\"FQ1_IN0\" value=\"%d\" time=\"%s\">\n",FQ1_IN0,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"4\" channel=\"2\" name=\"FQ1_IN1\" value=\"%d\" time=\"%s\">\n",FQ1_IN1,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"4\" channel=\"3\" name=\"FQ1_IN2\" value=\"%d\" time=\"%s\">\n",FQ1_IN2,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"4\" channel=\"4\" name=\"FQ1_IN3\" value=\"%d\" time=\"%s\">\n",FQ1_IN3,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"4\" channel=\"5\" name=\"FQ1_IN4\" value=\"%d\" time=\"%s\">\n",FQ1_IN4,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"4\" channel=\"6\" name=\"FQ1_IN5\" value=\"%d\" time=\"%s\">\n",FQ1_IN5,date); fprintf (Dat, buf); 

 sprintf (buf,"<input slot=\"5\" channel=\"0\" name=\"SERV_SON\" value=\"%d\" time=\"%s\">\n",SERV_SON,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"5\" channel=\"1\" name=\"SERV_SP1\" value=\"%d\" time=\"%s\">\n",SERV_SP1,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"5\" channel=\"2\" name=\"SERV_SP2\" value=\"%d\" time=\"%s\">\n",SERV_SP2,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"5\" channel=\"3\" name=\"SERV_ST1\" value=\"%d\" time=\"%s\">\n",SERV_ST1,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"5\" channel=\"4\" name=\"SERV_RES\" value=\"%d\" time=\"%s\">\n",SERV_RES,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"5\" channel=\"5\" name=\"DOZA1_START\" value=\"%d\" time=\"%s\">\n",DOZA1_START,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"5\" channel=\"6\" name=\"DOZA2_START\" value=\"%d\" time=\"%s\">\n",DOZA2_START,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"5\" channel=\"7\" name=\"SERV_ST2\" value=\"%d\" time=\"%s\">\n",SERV_ST2,date); fprintf (Dat, buf); 

 sprintf (buf,"<input slot=\"7\" channel=\"0\" name=\"FQ2_TRIG\" value=\"%d\" time=\"%s\">\n",FQ2_TRIG,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"7\" channel=\"1\" name=\"FQ2_IN0\" value=\"%d\" time=\"%s\">\n",FQ2_IN0,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"7\" channel=\"2\" name=\"FQ2_IN1\" value=\"%d\" time=\"%s\">\n",FQ2_IN1,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"7\" channel=\"3\" name=\"FQ2_IN2\" value=\"%d\" time=\"%s\">\n",FQ2_IN2,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"7\" channel=\"4\" name=\"FQ2_IN3\" value=\"%d\" time=\"%s\">\n",FQ2_IN3,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"7\" channel=\"5\" name=\"FQ2_IN4\" value=\"%d\" time=\"%s\">\n",FQ2_IN4,date); fprintf (Dat, buf); 
 sprintf (buf,"<input slot=\"7\" channel=\"6\" name=\"FQ2_IN5\" value=\"%d\" time=\"%s\">\n",FQ2_IN5,date); fprintf (Dat, buf); 

 sprintf (buf,"</inputs>\n"); fprintf (Dat, buf);
 fclose (Dat);

 return TRUE;
}
//----------------------------------------------------------------------------------------------------
// get data from SQL and save to CSV(0)/XML(1) file
bool LoadDataFile (byte format)
{
 CHAR   buf[340];
 Dat =  fopen(XML_FILE,"r");

 int j=0;
 while(!feof(Dat))
	{
	 fgets(buf,300,Dat);

	 if (strstr (buf,"totalDetected")) totalDetected=ParseFile (buf,"totalDetected");
	 if (strstr (buf,"totalLoaded")) totalLoaded=ParseFile (buf,"totalLoaded");
	 if (strstr (buf,"todayDetected")) todayDetected=ParseFile (buf,"todayDetected");
	 if (strstr (buf,"todayLoaded")) todayLoaded=ParseFile (buf,"todayLoaded");
	}
 ULOGW ("[ser] read stats %d|%d|%d|%d",totalDetected,totalLoaded,todayDetected,todayLoaded);
 fclose (Dat);
 return true;
}
//----------------------------------------------------------------------------------------------------
uint ParseFile (char* buf, char* param)
{
 CHAR   date[50];
 CHAR	*cur, *cur2;
 int	res=0;
 cur=strstr(buf,param); 
 if (cur!=NULL) 
	{
	 cur2=strstr (cur,"time");
	 if (cur2 && cur2-cur>20)
		{
		 //ULOGW ("[ser] cur=%d cur2=%d",cur,cur2);
		 strncpy (date,cur+strlen (param)+9,cur2-cur-11-strlen (param));
		 //ULOGW ("[ser] %s (%d)",date,cur2-cur-11-strlen (param));
		 res=atoi (date);
		 return res;
		}
	}
 return 0;
}
//----------------------------------------------------------------------------------------------------
// log/output function
VOID ULOGW (CHAR* string, ...)
{
 TCHAR InBuffer[300]; SYSTEMTIME st;
 CHAR buf[300]; CHAR buf2[30]; CHAR buf3[30]; CHAR buf4[200];
 //Log = fopen(MAIN_LOG,"a");
 
 byte cn=30;
 while (cn)
 {
 	 if (Log==NULL) { Log = fopen(MAIN_LOG,"a"); break; }
 	 else { Sleep(5); cn--; }
 	 if (cn<10) return;
 }

 GetSystemTime(&st); 
 GetDateFormat(LOCALE_SYSTEM_DEFAULT, DATE_SHORTDATE, NULL, NULL, InBuffer, sizeof(InBuffer));
 WideCharToMultiByte(CP_ACP, 0, InBuffer, sizeof(InBuffer), buf2, sizeof(buf2), NULL,NULL);
 GetTimeFormat(LOCALE_SYSTEM_DEFAULT, TIME_FORCE24HOURFORMAT, NULL, NULL, InBuffer, sizeof(InBuffer));
 WideCharToMultiByte(CP_ACP, 0, InBuffer, sizeof(InBuffer), buf3, sizeof(buf3), NULL,NULL);

 va_list arg;
 va_start(arg, string);
 _vsnprintf(buf4,sizeof (buf4), string, arg);
 va_end(arg);

 _snprintf (buf,sizeof(buf)-1,"%s %s\t%s\n",buf2,buf3,buf4); 
 //_snprintf (buf,sizeof(buf)-1,"%s %s\t%s\n",buf2,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds,buf4); 
 fprintf (Log, buf);
 printf ("%s",buf);
 fclose(Log); Log=NULL;
}
