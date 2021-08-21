#include "stdafx.h"
#include "Analysis.h"
#include<math.h>
#include "AnalysisDoc.h"
#include "Figure.h"
#include "AnalysisView.h"
#include "SinTable.h"

//The files that are to be renamed will be renamed AND moved to a new folder "_NewData" under the same parent folder.
//The names of the renamed files will have 2 sub 0 decimal digits. (e.g., 1.50.txt)
void CAnalysisView::Rename_the_number_part(char *Dir, char* JustFileName)
{
	int result, Num, errorCode, isSucceed, is_Coronal_Or_Sagittal;
	double DoubleNum=PIover12;
	char NewName[1028], BeforeNum[1028], AfterNum[1028], OldName[1028],NewDir[1028];

	if(wildcmp("_DrawingData.txt",JustFileName)==1 || wildcmp("*_Config.txt",JustFileName)==1 )return;
	if(m_Coronal_1_Sagittal_2==1)if(wildcmp("L*",JustFileName)==1)return;
	else if(m_Coronal_1_Sagittal_2==2)if(wildcmp("L*",JustFileName)==0)return;

	result=Find_Last_Double_inString_and_find_strings_before_and_after_it(JustFileName, BeforeNum, DoubleNum, AfterNum); 
	if(result==0)return; //The file does not contain a number. 
	//##########################################################
	//################# Edit: Bias and scaling #################
	//##########################################################
	//  Example:
	//			is_Coronal_Or_Sagittal=CORONAL; 
	//			DoubleNum =-1.2*(DoubleNum-16);
	//	Here, the user specifies that these MR images are coronal slices. 
	//	And directs to change the number part of the file name to a new one by subtracting 16 and multiplying -1.2 from the original number part.

	is_Coronal_Or_Sagittal=m_Coronal_1_Sagittal_2;
	DoubleNum =m_Multiplication*(DoubleNum+m_Addition);
	//##########################################################
	//##########################################################

	RemoveForwardSlashAtEnd(Dir);
	GetGrandParentFolderName(Dir,NewDir);AddFrwdSlashAtEndIfNotHave(NewDir); strcat(NewDir,"_NewData\\"); //A new Folder needs to be created; otherwise, there could be an error because of the bug in rename()==> Sometimes, rename() tries to conver the file names that have already converted by itself.
	if(m_Coronal_1_Sagittal_2==1)sprintf(NewName,"%s%0.2lf%s",NewDir, DoubleNum, AfterNum);
	else if(m_Coronal_1_Sagittal_2=2)sprintf(NewName,"%sL%0.2lf%s",NewDir, DoubleNum, AfterNum);
	if(m_isFirstTimeCreatingForder==1){
		m_isFirstTimeCreatingForder=0;
		result=CreateDirectory(NewDir,NULL); //Creates the new folder if doesn't exist already.
		if(result==0){
			errorCode=GetLastError();
			if(errorCode==ERROR_ALREADY_EXISTS)isSucceed=1;
			else isSucceed=0;
		}
		else {isSucceed=1;}
		if(isSucceed==1)sprintf(RenameFiles_Message,"Done!!\nThe renamed files in %s folder have been moved to %s folder.",Dir,NewDir);
		else sprintf(RenameFiles_Message,"Failed to create _NewData folder to put renamed files.");
	}
	AddFrwdSlashAtEndIfNotHave(Dir);
	sprintf(OldName,"%s%s",Dir,JustFileName);
	result= rename( OldName , NewName );
	if(result!=0){
		char MsgBuf[256];
		sprintf(MsgBuf," rename() of %s has failed\n The same name of file may be in the destination folder \"_NewData\"",JustFileName);
		AfxMessageBox(MsgBuf);
	}
}


#include "Rename_MRIs.h"
void CAnalysisView::OnToolsRenameMRIs()
{
	VirMemDC();

 	//######## Selection of a folder ########
   BROWSEINFO bi = { 0 };
    bi.lpszTitle = _T("Pick a Directory that contains the MR images");
    LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );
    if ( pidl != 0 )
    {
        // get the name of the folder
        SHGetPathFromIDList ( pidl, m_DataFolderPath );
        // free memory used
        IMalloc * imalloc = 0;
        if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
        {
            imalloc->Free ( pidl );
            imalloc->Release ( );
        }
    }
	else return;
 	//######################################


	//############ Input of A, M ############
	m_Coronal_1_Sagittal_2=1; m_Multiplication=1; m_Addition=0;
	CRename_MRIs	ParameterBOX;
	ParameterBOX.m_Addition=m_Addition;
	ParameterBOX.m_Multiplication=m_Multiplication;
	ParameterBOX.m_Coronal_1_Sagittal_2=m_Coronal_1_Sagittal_2;
	if(ParameterBOX.DoModal()==IDOK){
		m_Addition=ParameterBOX.m_Addition;
		m_Multiplication=ParameterBOX.m_Multiplication;
		if(m_Multiplication==1 && m_Addition==0)return;//No change will occur.
		m_Coronal_1_Sagittal_2=ParameterBOX.m_Coronal_1_Sagittal_2;
	}
	else return;
	//######################################


	GetFolderName(m_DataFolderPath,m_DataJustFolderName);
	m_isFirstTimeCreatingForder=1;
	FindAllFiles(m_DataFolderPath, 0);

	AfxMessageBox(RenameFiles_Message);
	/////////////////////
}

// This function will find all files in a directory and its subdirectory
// For each file found, it calls a function.
void CAnalysisView::FindAllFiles(char* searchThisDir, bool isToSearchSubDirs)
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
	if (hFind == INVALID_HANDLE_VALUE) return;

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
				FindAllFiles(searchDir2, isToSearchSubDirs);
			}
			// Do not need to process anymore
			continue;
		}

		//######## Found a file ==> Do someting.   ##########
		Rename_the_number_part(searchThisDir, FindFileData.cFileName);
		//###################################################

	}while(FindNextFile(hFind, &FindFileData) != 0);// Loop while we find more files

	// We are done with the finding
	FindClose(hFind);
}

//This routine extracts a double number from a string. 
//It registers only the last one (number e.g. -1.23 from 76DR-1.23.txt).
int	CAnalysisView::Find_Last_Double_inString_and_find_strings_before_and_after_it(char *GivenString, char* BeforeNum, double &NumToReturn, char *AfterNum)
{
	char *p = NULL;
	char *q = NULL;
	int iCount=-1, PointCount=0, foundNum, FirstChar=3, NumStartPos=0, NumEndPos=0,charPos=0;
	int sign=0, point=0;
	double num = 0;
	char *szString;

	foundNum=0;
	szString=GivenString;
	p=szString;
	while(*p){
		if(*p>='0' && *p<='9'){
			if(FirstChar!=3 && *(p-1)=='.'){
				point=1; NumEndPos++;
				if(FirstChar<2){
					if(*(p-2)<'0' || *(p-2)>'9'){
						num=0; NumStartPos=charPos; NumEndPos=charPos;
						if(*(p-2)=='-'){
							sign=1; 
							NumStartPos-=2;NumEndPos=charPos;
						}else sign=0;
					}
				}
			}
			else if(FirstChar!=3 && *(p-1)=='-'){
				sign=1;point=0;num=0; NumStartPos=charPos-1; NumEndPos=charPos;
			} 
			else {sign=0;point=0;num=0;NumStartPos=charPos; NumEndPos=charPos;}
			PointCount=0;
			q=p;
			while(*q>='0' && *q<='9'){
				++NumEndPos;
				++iCount ;
				++q ;
			}
			q=p;
			while(*q>='0' && *q<='9'){
				if(point==0)num = num + (*q-48) * pow((double)10,(double)iCount);
				else num = num + (*q-48) * pow((double)10,(double)(--PointCount));
				--iCount;
				++q ;
				++p;
			++charPos;
			}
			foundNum = 1 ;
		}
		else{
			++charPos;
			++p ;
		}
		FirstChar--;
	}		
	if(foundNum==0 || num<0){
		return 0;
	}
	if(sign==1) NumToReturn= -num;
	else		NumToReturn=  num;
	Copy_fromNth_HowMany(BeforeNum, GivenString, 1, NumStartPos); 
	Copy_fromNth_HowMany(AfterNum, GivenString, NumEndPos+1, strlen(GivenString)-NumEndPos); 
	return 1;
}

void CAnalysisView::Show2D_VI() 
{}