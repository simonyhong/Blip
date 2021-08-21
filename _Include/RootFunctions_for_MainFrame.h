#if !defined(MAIN_ROOT_FUNCTIONS)

#include <math.h>
double	m_Left_in_screen, m_Top_in_screen, m_Width_in_screen, m_Height_in_screen;
char	m_RootFileNameFULL[1024];
int		RootFileOpen(char RootDir[]);
void	Find_Blip_andAddSomeinMainFrm(char AddedFolderName[], char FullPath[]);
void	GetGrandParentFolderNameinMainFrm(char* FolderPath, char *GrandParentFolderName);
int		wildcmpInMainFrm(char *wild, char *string);
int		RoundToClosestInt(double Val);
void FromA_findB_addC_giveD(char FromA[], char *findB, char addC[], char giveD[]);
CString GetExecutableNameInMainFrm();
double	Rectify(double Value){if (Value>0.)return Value; else return 0.;}
#define AddFrwdSlashAtEndIfNotHave(Dir)    if(Dir[strlen(Dir) - 1] != '\\') strcat(Dir,"\\"); //Add the forward slash.

//Opens a root file specifying the position of the MainFrame window in the screen.
int RootFileOpen(char RootDir[])
{
	Find_Blip_andAddSomeinMainFrm(RootDir,m_RootFileNameFULL);

	FILE *fp;
	if((fp=fopen(m_RootFileNameFULL,"r"))!= NULL){
		double Blip_Ver;
		char Label[64];
		fscanf(fp,"%s",Label );
		if(wildcmpInMainFrm("BLIP_Ver:*",Label)==0)return 0;
		fscanf(fp,"%lf",&Blip_Ver );
		fscanf(fp,"%s",Label );
		fscanf(fp,"%lf",&m_Left_in_screen );
		fscanf(fp,"%s",Label );
		fscanf(fp,"%lf",&m_Top_in_screen );
		fscanf(fp,"%s",Label );
		fscanf(fp,"%lf",&m_Width_in_screen );
		fscanf(fp,"%s",Label );
		fscanf(fp,"%lf",&m_Height_in_screen );
		fclose(fp);
		if(m_Left_in_screen<0||m_Left_in_screen>1) return 0;
		if(m_Top_in_screen <0||m_Top_in_screen >1) return 0;
		if(m_Width_in_screen<=0||m_Height_in_screen<=0) return 0;
		else return 1;
	}
	else{
		return 0;
	}
}

void CMainFrame::OnFileRemember()
{
	CRect rect;
	GetWindowRect(rect); 
	m_Left_in_screen=rect.left;
	m_Top_in_screen=rect.top;
	m_Width_in_screen=rect.right-rect.left;
	m_Height_in_screen=rect.bottom-rect.top;

	Find_Blip_andAddSomeinMainFrm(ROOT_PATH,m_RootFileNameFULL);
	FILE *fp;
	if((fp=fopen(m_RootFileNameFULL,"w"))!= NULL){
		char Label[64];
		//Header
		fprintf(fp,"BLIP_Ver:  " );
		fprintf(fp,"%0.5lf\n",BLIP_VERSION );
		//Window positions in Relative terms.
		fprintf(fp,"m_Left_in_screen:  " );
		fprintf(fp,"%0.5lf\n",Rectify(m_Left_in_screen/(::GetSystemMetrics(SM_CXSCREEN))) );
		fprintf(fp,"m_Top_in_screen:  " );
		fprintf(fp,"%0.5lf\n",Rectify(m_Top_in_screen/(::GetSystemMetrics(SM_CYSCREEN))) );
		fprintf(fp,"m_Width_in_screen:  " );
		fprintf(fp,"%0.5lf\n",Rectify(m_Width_in_screen/(::GetSystemMetrics(SM_CXSCREEN))) );
		fprintf(fp,"m_Height_in_screen:  " );
		fprintf(fp,"%0.5lf\n",Rectify(m_Height_in_screen/(::GetSystemMetrics(SM_CYSCREEN))) );
		fclose(fp);
	}
}

CString GetExecutableName()
{
	CString strFileName;
	GetModuleFileName(NULL, strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer();
	return strFileName;
}

void Find_Blip_andAddSomeinMainFrm(char AddedFolderName[], char FullPath[])
{
	char tmpStr[1024];
	CString ExeDirString=GetExecutableNameInMainFrm();
	strcpy(tmpStr,ExeDirString);

	FromA_findB_addC_giveD(tmpStr,"\Blip",AddedFolderName,FullPath);
}

void FromA_findB_addC_giveD(char FromA[], char *findB, char addC[], char giveD[])
{
	char tmpStr[1024], FindNameWild[100],FindName[100];
	strcpy(tmpStr,FromA);
	sprintf(FindNameWild,"*\\%s*",findB);
	sprintf(FindName,"*\\%s",findB);
	if(wildcmpInMainFrm(FindNameWild,tmpStr)==1){
		RE_Search1:;
		if(wildcmpInMainFrm(FindName,tmpStr)==1){
			strcpy(giveD,tmpStr);
		}
		else {
			if(strlen(tmpStr)==0||strcmp(tmpStr," ")==0){
				char msgBuf[256]; sprintf(msgBuf,"Couldn't find %s folder.",findB);
				AfxMessageBox(msgBuf);
				return;
			}
			GetGrandParentFolderNameinMainFrm(tmpStr,tmpStr); goto RE_Search1;
		}
		if(addC[0]!='\\')AddFrwdSlashAtEndIfNotHave(giveD);
		strcat(giveD,addC);
	}		
	else{
		char msgBuf[256]; sprintf(msgBuf,"Couldn't find %s folder.",findB);
		AfxMessageBox(msgBuf);
	}
}

//This one discards the parent folder name out of the full pathway "FolderPath"
void GetGrandParentFolderNameinMainFrm(char* FolderPath, char *GrandParentFolderName)
{
	char tmpStr[500],FldrName[500],tmpLowercase[500];
	int i;
	strcpy_s(tmpStr,FolderPath);
	_strrev(tmpStr); //It reverses the order of the string
	int	sizeofChar=strlen(tmpStr);
	for(i=2;i<sizeofChar-1;i++){//ignore the first two (\*) letters
		strncpy_s(FldrName,2,&tmpStr[i],_TRUNCATE);
		if(wildcmpInMainFrm("\\", FldrName))break;
	}
	int a=strlen(tmpStr)-(i+1);
	if(a<=0){
		AfxMessageBox("Couldn't find Blip folder."); 
		exit(1);
	} 
	_strrev(tmpStr); //It reverses the order of the string
	tmpStr[strlen(tmpStr)-(i+1)]='\0';
	strcpy(GrandParentFolderName,tmpStr);
}

CString GetExecutableNameInMainFrm()
{
	CString strFileName;
	GetModuleFileName(NULL, strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer();
	return strFileName;
}


int RoundToClosestInt(double Val)
{
	int RoundedUp,RoundedDown;
	if(Val>0){
		RoundedUp=(int)(Val+0.5);
		RoundedDown=(int)(Val);
	}
	else{
		RoundedUp=(int)(Val);
		RoundedDown=(int)(Val-0.5);
	}
	if(fabs(Val-RoundedUp)>fabs(Val-RoundedDown))return RoundedDown;
	else return RoundedUp;
}

int wildcmpInMainFrm(char *wild, char *string) 
{
  char *cp = NULL, *mp = NULL;

  while ((*string) && (*wild != '*')) {
    if((*wild != *string) && (*wild != '?')) {
      return 0;
    }
    wild++;
    string++;
  }

  while (*string) {
    if(*wild == '*') {
      if(!*++wild) {
        return 1;
      }
      mp = wild;
      cp = string+1;
    } 
	else if((*wild == *string) || (*wild == '?')) {
      wild++;
      string++;
    } 
	else {
      wild = mp;
      string = cp++;
    }
  }

  while (*wild == '*') {
    wild++;
  }
  int a=strlen(wild);
  if(a==0)return 1;
  return !*wild;
}


#endif//(MAIN_ROOT_FUNCTIONS)