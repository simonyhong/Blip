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

int CAnalysisView::Confirm_Deletion(char *pSource, char *pDestination)
{
	Dialog_DeleteConfirm ConfirmDlg;

	//ConfirmDlg.m_WarningMessage+="";  
	ConfirmDlg.m_SourceDir =pSource; 
	ConfirmDlg.m_DestinationDir =pDestination; 
	ConfirmDlg.m_Check1 =Get_Check_status(1 ); 
	ConfirmDlg.m_Check2 =Get_Check_status(2 ); 
	ConfirmDlg.m_Check3 =Get_Check_status(3 ); 
	ConfirmDlg.m_Check4 =Get_Check_status(4 ); 
	ConfirmDlg.m_Check5 =Get_Check_status(5 ); 
	ConfirmDlg.m_Check6 =Get_Check_status(6 ); 
	ConfirmDlg.m_Check7 =Get_Check_status(7 ); 
	ConfirmDlg.m_Check8 =Get_Check_status(8 ); 
	ConfirmDlg.m_Check9 =Get_Check_status(9 ); 
	ConfirmDlg.m_Check10=Get_Check_status(10);
	ConfirmDlg.m_Check11=Get_Check_status(11);
	ConfirmDlg.m_Check12=Get_Check_status(12);
	ConfirmDlg.m_Check13=Get_Check_status(13);
	ConfirmDlg.m_Check14=Get_Check_status(14);
	ConfirmDlg.m_Check15=Get_Check_status(15);
	if(ConfirmDlg.DoModal()==IDOK){
		Set_Check_status(1 ,ConfirmDlg.m_Check1 ); 
		Set_Check_status(2 ,ConfirmDlg.m_Check2 ); 
		Set_Check_status(3 ,ConfirmDlg.m_Check3 ); 
		Set_Check_status(4 ,ConfirmDlg.m_Check4 ); 
		Set_Check_status(5 ,ConfirmDlg.m_Check5 ); 
		Set_Check_status(6 ,ConfirmDlg.m_Check6 ); 
		Set_Check_status(7 ,ConfirmDlg.m_Check7 ); 
		Set_Check_status(8 ,ConfirmDlg.m_Check8 ); 
		Set_Check_status(9 ,ConfirmDlg.m_Check9 ); 
		Set_Check_status(10,ConfirmDlg.m_Check10);
		Set_Check_status(11,ConfirmDlg.m_Check11);
		Set_Check_status(12,ConfirmDlg.m_Check12);
		Set_Check_status(13,ConfirmDlg.m_Check13);
		Set_Check_status(14,ConfirmDlg.m_Check14);
		Set_Check_status(15,ConfirmDlg.m_Check15);
		return 1;
	}
	else return 0;
}

#include "Dialog_ReplaceConfirm.h"
int CAnalysisView::Confirm_Directory()
{
	CReplaceConfirm ConfirmDlg;


	if(ConfirmDlg.DoModal()==IDOK){
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