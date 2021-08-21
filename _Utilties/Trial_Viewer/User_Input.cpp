#include "stdafx.h"
#include "Analysis.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "AnalysisDoc.h"
#include "Figure.h"
#include "AnalysisView.h"
#include "SinTable.h"
#include "DialogueBox.h"
#include "StringEdit.h"


void CAnalysisView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//if(m_nShowMode==GRAPH_3D){ThD.KeyDown(nChar); goto ToEND;}
	m_KeyCode=nChar;
	switch(nChar){
		case VK_DELETE:
			OnDeleteFigs();
			break;
		case VK_LEFT:
			if(m_RequestedTrlToDisp>0){
				m_RequestedTrlToDisp--;
				Show2D_I();
			}
			break;
		case VK_RIGHT:
			if(m_RequestedTrlToDisp<m_NumTrialsInList-1){
				m_RequestedTrlToDisp++;
				Show2D_I();
			}
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

void CAnalysisView::OnEnChangeEditString1()
{
	StringEdit	ParameterBOX;

	if(m_CallingFrom==1)ParameterBOX.m_UserInputString1=m_GlobalTitle;
	else if(m_CallingFrom==2)ParameterBOX.m_UserInputString1=m_pSpotlightFig->m_Title.NameString;
	if(ParameterBOX.DoModal()==IDOK){
		if(m_CallingFrom==1)m_GlobalTitle=ParameterBOX.m_UserInputString1;
		else if(m_CallingFrom==2){
			m_pSpotlightFig->m_Title.NameString=ParameterBOX.m_UserInputString1;
		}
	}
}

void CAnalysisView::NnID()
{
	StringEdit	ParameterBOX;
	char Source[1024]; 
	m_IsEditOkayed=0;

	sprintf(Source,"Nn: %d",m_pSpotlightFig->m_NnID);
	ParameterBOX.m_UserInputString1=Source;
	if(ParameterBOX.DoModal()==IDOK){
		m_IsEditOkayed=1;
		strcpy(Source,ParameterBOX.m_UserInputString1);
		int TmpID;
		if(Find_Last_Num_inString(Source, TmpID)==-1){
			m_IsEditOkayed=0;
			return;
		}
		if(TmpID>=0 && TmpID<3)m_pSpotlightFig->m_NnID=TmpID;
		else AfxMessageBox("TmpID<0 || TmpID>=3");
	}
}

void CAnalysisView::ChannelID()
{
	StringEdit	ParameterBOX;
	char Source[1024]; 
	m_IsEditOkayed=0;

	sprintf(Source,"Chan: %d",m_pSpotlightFig->m_ChanID);
	ParameterBOX.m_UserInputString1=Source;
	if(ParameterBOX.DoModal()==IDOK){
		m_IsEditOkayed=1;
		strcpy(Source,ParameterBOX.m_UserInputString1);
		int TmpID;
		if(Find_Last_Num_inString(Source, TmpID)==-1){
			m_IsEditOkayed=0;
			return;
		}
		if(TmpID>=0)m_pSpotlightFig->m_ChanID=TmpID;
		else AfxMessageBox("TmpID<0");
	}
}


void CAnalysisView::OnChangeParameters() 
{
	CDialogueBox	ParameterBOX;

	ParameterBOX.m_dVariable1=m_dMaximumX;
	ParameterBOX.m_dVariable2=m_dMinimumX;
	ParameterBOX.m_dVariable3=m_dMaximumY;
	ParameterBOX.m_dMinimumY=m_dMinimumY;
	ParameterBOX.m_dMaxY2=m_dMaximumY2;
	ParameterBOX.m_dMinY2=m_dMinimumY2;
	if(ParameterBOX.DoModal()==IDOK){
		if(ParameterBOX.m_dVariable1<=ParameterBOX.m_dVariable2||ParameterBOX.m_dVariable3<=ParameterBOX.m_dMinimumY){AfxMessageBox("m_dMaximumX<=m_dMinimumX OR m_dMaximumY<=m_dMinimumY ==> Try again", MB_OK); return;}
		else{
			m_dMaximumX =ParameterBOX.m_dVariable1;
			m_dMinimumX =ParameterBOX.m_dVariable2;
			m_dMaximumY =ParameterBOX.m_dVariable3;
			m_dMinimumY =ParameterBOX.m_dMinimumY;
			m_dMaximumY2=ParameterBOX.m_dMaxY2;
			m_dMinimumY2=ParameterBOX.m_dMinY2;
		}
	}
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

//This routine extracts numbers from a string. 
//It registers only the last one (number e.g. 123 from 76DR123.txt).
int CAnalysisView::Find_Last_Num_inString(char *GivenString, int &NumToReturn)
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
		return -1;
	}
	if(sign==1) NumToReturn= -num;
	else		NumToReturn=  num;
	return 0;
}