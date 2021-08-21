#if !defined(MAIN_ROOT_FUNCTIONS)

#include <math.h>
double	m_Left_in_screen, m_Top_in_screen, m_Width_in_screen, m_Height_in_screen;
char	m_RootFileNameFULL[1024];
int		RootFileOpen(char RootDir[]);
void	GetGrandParentFolderNameinMainFrm(char* FolderPath, char *GrandParentFolderName);
int		wildcmpInMainFrm(char *wild, char *string);
int		RoundToClosestInt(double Val);
CString GetExecutableNameInMainFrm();
int FromA_findB_addC_giveD(char FromHere[], char *FindThis, char AddThis[], char toMakeThis[]);
int		IsThereFileWithThisName(char* searchThisDir, bool isToSearchSubDirs, char* FindJustFileName);	
	double	Rectify(double Value){if (Value>0.)return Value; else return 0.;}


//Opens a root file specifying the position of the MainFrame window in the screen.
int RootFileOpen(char RootDir[])
{
	char ConfigFile[1028];
	CString ExeDirString=GetExecutableNameInMainFrm();
	strcpy(ConfigFile,ExeDirString);
	if(0==FromA_findB_addC_giveD(ConfigFile,PARENT_FOLDER_NAME,RootDir,m_RootFileNameFULL)){
		ConfigFile[strlen(ConfigFile)-14]='\0'; strcat(ConfigFile,RootDir); //Replace with the RootDir
	}

	FILE *fp;
	if((fp=fopen(m_RootFileNameFULL,"r"))!= NULL){
		double Blip_Ver;
		char Label[64];
		fscanf(fp,"%s",Label );
		if(wildcmpInMainFrm("Ver:*",Label)==0)return 0;
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


int FromA_findB_addC_giveD(char fromA[], char *findB, char addC[], char giveD[])
{
	char tmpStr[1024], FindNameWild[100],FindName[100];
	strcpy(tmpStr,fromA);
	sprintf(FindNameWild,"*\\%s*",findB);
	sprintf(FindName,"*\\%s",findB);
	if(wildcmpInMainFrm(FindNameWild,tmpStr)==1){
		RE_Search:;
		if(wildcmpInMainFrm(FindName,tmpStr)==1){
			sprintf(giveD,"%s\\",tmpStr);
		}
		else {GetGrandParentFolderNameinMainFrm(tmpStr,tmpStr); goto RE_Search;}
		if(addC[0]=='\\')RemoveForwardSlashAtEnd(giveD);
		strcat(giveD,addC);
	}		
	else{
		//See if the parent folder of fromA has "addC".
		GetGrandParentFolderNameinMainFrm(fromA,tmpStr);
		if(IsThereFileWithThisName(tmpStr,0,addC)==1){
			AddFrwdSlashAtEndIfNotHave(tmpStr);
			strcpy(giveD,tmpStr);
			strcat(giveD,addC);
		}
		else{
			sprintf(tmpStr,"Couldn't find %s",findB);
			AfxMessageBox(tmpStr);
			return 0;
		}
	}
	return 1;
}

//This one discards the parent folder name out of the full pathway "FolderPath"
void GetGrandParentFolderNameinMainFrm(char* FolderPath, char *GrandParentFolderName)
{
	char tmpStr[500],FldrName[500],tmpLowercase[500];
	int i;
	strcpy_s(tmpStr,FolderPath);
	_strrev(tmpStr); //It reverses the order of the string
	int	sizeofChar=strlen(tmpStr);
	for(i=1;i<sizeofChar-1;i++){//ignore the first (\\) letter
		strncpy_s(FldrName,2,&tmpStr[i],_TRUNCATE);
		if(wildcmpInMainFrm("\\", FldrName))break;
	}
	int a=strlen(tmpStr)-(i+1);
	if(a<=0){
		AfxMessageBox("Couldn't find the Grand Parent folder."); 
		exit(1);
	} 
	_strrev(tmpStr); //It reverses the order of the string
	tmpStr[strlen(tmpStr)-(i+1)]='\0';
	strcpy(GrandParentFolderName,tmpStr);
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

// This function will find all files in a directory and its subdirectory
int IsThereFileWithThisName(char* searchThisDir, bool isToSearchSubDirs, char* FindJustFileName)
{
	// What we need to search for files
	WIN32_FIND_DATA FindFileData = {0};
	HANDLE hFind = INVALID_HANDLE_VALUE;

	// Build the file search string
	char searchDir[2048] = {0};

	// If it already is a path that ends with \, only add the *
	if(searchThisDir[strlen(searchThisDir) - 1] == '\\') _snprintf(searchDir, 2047, "%s*", searchThisDir);
	// Otherwise, add the \* to the end of the path
	else _snprintf(searchDir, 2047, "%s\\*", searchThisDir);

	// Find the first file in the directory.
	hFind = FindFirstFile(searchDir, &FindFileData);

	// If there is no file, return
	if (hFind == INVALID_HANDLE_VALUE) return 0;

	// Find files!
	do{
		// If it's the "." directory, continue
		if(strcmp(FindFileData.cFileName, ".") == 0) continue;
		// If it's the ".." directory, continue
		if(strcmp(FindFileData.cFileName, "..") == 0) continue;		

		// If we find a directory
		if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY){
			// If we want to search subdirectories
			if(isToSearchSubDirs){
				// Holds the new directory to search
				char searchDir2[2048] = {0};
				// If it already is a path that ends with \, only add the *
				if(searchThisDir[strlen(searchThisDir) - 1] == '\\') _snprintf(searchDir2, 2047, "%s%s", searchThisDir, FindFileData.cFileName);
				// Otherwise, add the \* to the end of the path
				else _snprintf(searchDir2, 2047, "%s\\%s", searchThisDir, FindFileData.cFileName);
				IsThereFileWithThisName(searchDir2, isToSearchSubDirs,FindJustFileName);
			}
			// Do not need to process anymore
			continue;
		}
		//Found a file name.
		if(strcmp(FindFileData.cFileName, FindJustFileName)==0)return 1;
	}while(FindNextFile(hFind, &FindFileData) != 0);// Loop while we find more files

	// We are done with the finding
	FindClose(hFind);
	return 0;
}



CString GetExecutableNameInMainFrm()
{
	CString strFileName;
	GetModuleFileName(NULL, strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer();
	return strFileName;
}


#endif//(MAIN_ROOT_FUNCTIONS)