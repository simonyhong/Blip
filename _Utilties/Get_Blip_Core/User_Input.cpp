#include "stdafx.h"
#include "Analysis.h"
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "AnalysisDoc.h"

#include "AnalysisView.h"
#include "SinTable.h"
#include "Dialog_DeleteConfirm.h"


void CAnalysisView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//if(m_nShowMode==GRAPH_3D){ThD.KeyDown(nChar); goto ToEND;}
	m_KeyCode=nChar;
	switch(nChar){
		case VK_DELETE:
			break;
		default: break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CAnalysisView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	m_KeyCode=-1;
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

int CAnalysisView::Confirm_Deletion()
{
	Dialog_DeleteConfirm ConfirmDlg;

	ConfirmDlg.m_FileExts_toDelete="Files: ";
	for(int i=0;i<m_NumExts;i++){
		ConfirmDlg.m_FileExts_toDelete+=m_MatchExtStr[i];  
		ConfirmDlg.m_FileExts_toDelete+=" ";  
	}

	ConfirmDlg.m_FileExts_toDelete+="		\nAll files in the Release folders except *.exe files.		\nAll Debug & ipch folders";  
	ConfirmDlg.m_Check1=m_isToCopyExe;
	if(ConfirmDlg.DoModal()==IDOK){
		m_isToCopyExe=ConfirmDlg.m_Check1;
		return 1;
	}
	else return 0;
}


//This routine extracts numbers from a string. 
//There could be many isolated numbers scattered among letters, but it registers only the last one (number e.g. 123 from 76DR123.txt).
int CAnalysisView::Find_Last_Num_inString(char *GivenString)
{
	char *p = NULL;
	char *q = NULL;
	int iCount=-1,foundNum;
	int num = 0, sign=0;
	char *szString;

	foundNum=0;
	szString=GivenString;
	p=szString;
	while(*p){
		if(*p>='0' && *p<='9'){
			num=0;//This one is here to make sure that the only the last group of digits are captured (e.g. 123 from 76DR123.txt).
			q=p;
			while(*q>='0' && *q<='9'){
				++iCount ;
				++q ;
			}
			q=p;
			while(*q>='0' && *q<='9'){
				num = num + (*q-48) * pow((double)10,(double)iCount);
				--iCount;
				++q ;
				++p;
			}
			foundNum = 1 ;
		}
		else{
			if(*p=='-')sign=1; else sign=0;
			++p ;
		}
	}		
	if(foundNum==0 || num<0){
		//char strbuf[1000];
		//_snprintf(strbuf, 999, "The name does not contain a number", GivenString);
		//AfxMessageBox(strbuf);
		return INT_MAX;
	}
	if(sign==1) return -num;
	else		return  num;
}