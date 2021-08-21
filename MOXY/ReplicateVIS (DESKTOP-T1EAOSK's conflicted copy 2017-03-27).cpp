#include "stdafx.h"
#include "MOXY.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#include "MOXY_Doc.h"
#include "Figure_MOXY.h"
#include "RunningLineDisplay.h"
#include "MOXY_View.h"
#include "SinTable.h"
#include <time.h>

void CMOXY_View::Which_ReplicatingVIS_Function(int messageID)                      
{	
	if(strcmp(M_StringMsg_fromTAS_toMOXY,"PvisAllOff")==0){
		PvisAllOff_ReplicationOfVIS();
	}
	else if(strcmp(M_StringMsg_fromTAS_toMOXY,"PvisSetBackColor")==0){
		PvisSetBackColor_ReplicationOfVIS();
	}
	else if(strcmp(M_StringMsg_fromTAS_toMOXY,"PvisDrawBar")==0){
		PvisDrawBar_ReplicationOfVIS();
	}
	else if(strcmp(M_StringMsg_fromTAS_toMOXY,"PvisSwitchStim")==0){
		PvisSwitchStim_ReplicationOfVIS();
	}
	else if(strcmp(M_StringMsg_fromTAS_toMOXY,"PvisDrawPic")==0){
		PvisDrawPic_ReplicationOfVIS();
	}
	else{
		char ErrorMsg[1024]; sprintf_s(ErrorMsg,"Unknown command received:%s",M_StringMsg_fromTAS_toMOXY);
		AfxMessageBox(ErrorMsg);
	}
	if(messageID!=*M_MsgID_fromTAS_toMOXY)AfxMessageBox("The message has changed during this function; this should not happen");
	Msg_to_TAS(messageID);
}

void CMOXY_View::Which_MOXY_Function(int Q_Idx)                      
{	
	if(strcmp(M_StrQ_fromTAS_toMOXY[Q_Idx],"PosBox_OnOff")==0){
		RequestDisplayEvent(DISP_POSITION_BOX, M_VisObjID_Q_fromTAS_toMOXY[Q_Idx][0], M_PositionWinds[M_VisObjID_Q_fromTAS_toMOXY[Q_Idx][0]].m_OnOff); 
	}
	else if(strcmp(M_StrQ_fromTAS_toMOXY[Q_Idx],"DisplayLED")==0){
		if(M_DIO_State[M_VisObjID_Q_fromTAS_toMOXY[Q_Idx][0]]==1)RequestDisplayEvent(DIO_ON, M_VisObjID_Q_fromTAS_toMOXY[Q_Idx][0]); 
		else RequestDisplayEvent(DIO_OFF, M_VisObjID_Q_fromTAS_toMOXY[Q_Idx][0]); 
	}
	else if(strcmp(M_StrQ_fromTAS_toMOXY[Q_Idx],"AddUserVar")==0){
		Register_a_User_Picked_Variable(); 
	}
	else if(strcmp(M_StrQ_fromTAS_toMOXY[Q_Idx],"InitUserVar")==0){
		Iitialize_UserPicked_Variables_to_Display(); 
	}
	else if(strcmp(M_StrQ_fromTAS_toMOXY[Q_Idx],"UpdateFileDisp")==0){
		UpdateFileName_StroredLocal_AND_Disp_Name_n_RecodingStatus(); 
	}
	else if(strcmp(M_StrQ_fromTAS_toMOXY[Q_Idx],"UpdateRwdRect")==0){
		UpdateRwdAmount_AND_Disp_it(); 
	}
	else{
		char ErrorMsg[1024]; sprintf_s(ErrorMsg,"Unknown command received:%s",M_StrQ_fromTAS_toMOXY[Q_Idx]);
		AfxMessageBox(ErrorMsg);
	}
}

void CMOXY_View::Msg_to_TAS(int messageID)
{
	////m_Figure.PutText(m_Figure.m_dMinX,m_Figure.m_dMaxY,"Writing data on the shared area...");
	////Wait if there is something that TAS has not resolved yet
	//while(*M_MsgID_fromMOXY_toTAS>0){
	//	Sleep(1);
	//	if(m_bIsON ==0|| *M_Is_TAS_Running_4MOXYVIS==0){
	//		break;
	//	}
	//}
	if(*M_Is_TAS_Running_4MOXYVIS==1)*M_MsgID_fromMOXY_toTAS=messageID;
	else							 *M_MsgID_fromMOXY_toTAS=0;
}

void CMOXY_View::UpdateRwdAmount_AND_Disp_it()
{
	RemeasureRWDRect(*M_Dur_BigRWD);
	RequestDisplayEvent(UPDATE_RWD,1);
}

/* Clear screen */
void CMOXY_View::PvisAllOff_ReplicationOfVIS()                     
{	
	CClientDC objDC(this);

	CSingleLock SyncObj_Display(&m_sema); SyncObj_Display.Lock();
	int nSaveDC=m_DC[0].SaveDC();
	CRect PaneRect(m_pXYFig->m_nPaneTx,m_pXYFig->m_nPaneTy,m_pXYFig->m_nPaneBx,m_pXYFig->m_nPaneBy);

	CPen *pOldPen=m_DC[0].SelectObject(&m_NullPen);
	CBrush *pOldBrush=m_DC[0].SelectObject(&m_XY_BlackBrush);
	m_DC[0].Rectangle(PaneRect);
	if(m_pXYFig->m_GridNet.status==2)m_pXYFig->PlotGrid(m_XY_DisplayMode, &m_DC[0]);
	//else if(m_pXYFig->m_GridNet.status==1)m_pXYFig->PlotPolarMash(&m_DC[0]);
	 
	m_DC0Mrr.BitBlt(BIT_BLT(PaneRect,&m_DC[0]));
	objDC.BitBlt(BIT_BLT(PaneRect,&m_DC[0]));
	/////////////////////
	m_DC[0].SelectObject(pOldPen);
	m_DC[0].SelectObject(pOldBrush);
	m_DC[0].RestoreDC(nSaveDC); 
	SyncObj_Display.Unlock();
}

void CMOXY_View::PvisSetBackColor_ReplicationOfVIS()                    
{
	CSingleLock SyncObj_Display(&m_sema); SyncObj_Display.Lock();
	m_XY_BlackBrush.DeleteObject(); //This is the only way to change the color
	m_XY_BlackBrush.CreateSolidBrush( *M_backgroundScreenRGB );
	CBrush *pOldBrush=m_DC[0].SelectObject(&m_XY_BlackBrush);
	m_backgroundScreenPen.DeleteObject(); 
	m_backgroundScreenPen.CreatePen(PS_SOLID, BACKGROUND_PEN_WIDTH,*M_backgroundScreenRGB);
	m_DC[0].SelectObject(&m_backgroundScreenPen);

	m_pXYFig->Frame2D_for_VIS_Replica(m_XY_DisplayMode, &m_DC[0], this);
	m_DC[0].SelectObject(pOldBrush);
	m_DC[0].SetBkColor(RGB(255, 255, 255));//This to to prevent the color of the rectangel to be carried over to the background color of the text in other figures. There must be a better solution. 
	SyncObj_Display.Unlock();
}

void CMOXY_View::PvisDrawBar_ReplicationOfVIS()
{
	CSingleLock SyncObj_Display(&m_sema); SyncObj_Display.Lock();
	int nSaveDC=m_DC[0].SaveDC(), dummyInt;
	CDC *pDC=&m_DC[0];
	CBrush *pOldBrush=pDC->SelectObject(&m_WhiteBrushView); //This is done JUST to save the old brush. The SelectObject is not an effective one here because the Rectangle drawing procedure uses an RGB values for the brush.

	int i,idx;
	for(i=0;i<*M_N;i++){
		idx=M_VisObjIDs[i];
		//This function displays the rectangle AND 
		//saves the screen points of the rectangle in "m_topleftX[idx],m_topleftY[idx],m_width_inPixels[idx],m_height_inPixels[idx]".
		//The rectangle coordinates "m_topleftX[idx],m_topleftY[idx],m_width_inPixels[idx],m_height_inPixels[idx]" will be used to "unvail" the objects by PvisSwitchStim_ReplicationOfVIS().
		if(m_XY_DisplayMode==EYE_MONITOR_MODE)m_pXYFig->FillSolidRect_SizeAroundFovia_ReportBackTopLeftXY_VisReplica(M_cntrX[idx], M_cntrY[idx],M_width[idx], M_hight[idx], M_rgb[idx],pDC, m_topleftX[idx],m_topleftY[idx],m_width_inPixels[idx],m_height_inPixels[idx]);
		else                                  m_pXYFig->FillSolidRect_ReportBackTopLeftXY(M_cntrX[idx], M_cntrY[idx],M_width[idx], M_hight[idx], M_rgb[idx],pDC, m_topleftX[idx],m_topleftY[idx],m_width_inPixels[idx],m_height_inPixels[idx]);
	}
	m_DC[0].RestoreDC(nSaveDC); 
	m_DC[0].SelectObject(pOldBrush);
	m_DC[0].SetBkColor(RGB(255, 255, 255));//This is to prevent the color of the rectangel to be carried over to the background color of the text in other figures. There must be a better solution. 
	SyncObj_Display.Unlock();
}

void CMOXY_View::PvisSwitchStim_ReplicationOfVIS()
{
	CSingleLock SyncObj_Display(&m_sema); SyncObj_Display.Lock();
	CClientDC objDC(this);
	int nSaveDC=m_DC[0].SaveDC();
	CDC *pDC=&m_DC[0];

	int i,idx;
	for(i=0;i<*M_N;i++){
		idx=M_VisObjIDs[i];
		if(M_Ojb_Switch[idx]){//ON
			//This one displays those screen areas specified by "m_topleftX[idx],m_topleftY[idx],m_width_inPixels[idx],m_height_inPixels[idx]," from "FillSolidRect_SizeAroundFovia_ReportBackTopLeftXY_VisReplica()"
			m_DC0Mrr.BitBlt(m_topleftX[idx],m_topleftY[idx],m_width_inPixels[idx],m_height_inPixels[idx],pDC,m_topleftX[idx],m_topleftY[idx],SRCCOPY);
			objDC.BitBlt(m_topleftX[idx],m_topleftY[idx],m_width_inPixels[idx],m_height_inPixels[idx],pDC,m_topleftX[idx],m_topleftY[idx],SRCCOPY);
		}
		else{//OFF
			m_DC0Mrr.FillSolidRect(m_topleftX[idx],m_topleftY[idx],m_width_inPixels[idx],m_height_inPixels[idx],*M_backgroundScreenRGB);
			pDC->FillSolidRect(m_topleftX[idx],m_topleftY[idx],m_width_inPixels[idx],m_height_inPixels[idx],*M_backgroundScreenRGB);
			objDC.FillSolidRect(m_topleftX[idx],m_topleftY[idx],m_width_inPixels[idx],m_height_inPixels[idx],*M_backgroundScreenRGB);
		}
	}
	m_DC[0].RestoreDC(nSaveDC); 
	SyncObj_Display.Unlock();
}

//Supported picture file types: 
//     bmp, gif, jpg, ico, emf, wmf
void CMOXY_View::PvisDrawPic_ReplicationOfVIS()
{
	char pFileName[1024];
	int i,idx,result;
	long hmWidth;
	long hmHeight;
	CSingleLock SyncObj_Display(&m_sema); SyncObj_Display.Lock();
	for(i=0;i<*M_N;i++){
		idx=M_VisObjIDs[i];
		if(m_XY_DisplayMode==EYE_MONITOR_MODE)m_pXYFig->ReportBackTopLeftXY_VisReplica(M_cntrX[idx], M_cntrY[idx],M_width[idx], M_hight[idx], m_topleftX[idx],m_topleftY[idx],m_width_inPixels[idx],m_height_inPixels[idx]);
		else                                  m_pXYFig->ReportBackTopLeftXY(M_cntrX[idx], M_cntrY[idx],M_width[idx], M_hight[idx], m_topleftX[idx],m_topleftY[idx],m_width_inPixels[idx],m_height_inPixels[idx]);
		if(strcmp(m_SavedPicFileName[idx],M_PicFileName[idx])!=0){//Does not match=> Reload
			strcpy(m_SavedPicFileName[idx],M_PicFileName[idx]);
			Find_Blip_andAddSome("\\_VIS_Figures\\", pFileName); //This function stores the "...\Blip\_VIS_Figures\" info to pFileName.
			strcat(pFileName,M_PicFileName[idx]);//Now pFileName has the full name of the file.
			result=LoadPictureFile(pFileName, m_pPicture[idx]);
			if(result==-1){
				char Msg[1024];
				sprintf(Msg,"Failed to read: %s",M_PicFileName[idx]);
				AfxMessageBox(Msg);
			}
		}
		if(m_pPicture[idx]){//Match! => Reuse
			m_pPicture[idx]->get_Width(&hmWidth);
			m_pPicture[idx]->get_Height(&hmHeight);
			m_pPicture[idx]->Render(m_DC[0], m_topleftX[idx],m_topleftY[idx], m_width_inPixels[idx], m_height_inPixels[idx], 0, hmHeight, hmWidth, -hmHeight, &m_savedClientRect);
		}
		else{
			AfxMessageBox("m_pPicture[idx] has no picture info?");
		}
	}
	SyncObj_Display.Unlock();
}

void CMOXY_View::PvisShowFigAtHereNOW_ReplicationOfVIS()
{
	char pFileName[1024];
	int i,idx,result;
	long hmWidth;
	long hmHeight;
	CClientDC objDC(this);
	CDC *pDC=&m_DC[0];
	CSingleLock SyncObj_Display(&m_sema); SyncObj_Display.Lock();
	idx=0;
	if(m_XY_DisplayMode==EYE_MONITOR_MODE)m_pXYFig->ReportBackTopLeftXY_VisReplica(M_cntrX[idx], M_cntrY[idx],M_width[idx], M_hight[idx], m_topleftX[idx],m_topleftY[idx],m_width_inPixels[idx],m_height_inPixels[idx]);
	else                                  m_pXYFig->ReportBackTopLeftXY(M_cntrX[idx], M_cntrY[idx],M_width[idx], M_hight[idx], m_topleftX[idx],m_topleftY[idx],m_width_inPixels[idx],m_height_inPixels[idx]);
	if(strcmp(m_SavedPicFileName[idx],M_PicFileName[idx])!=0){//Does not match=> Reload
		strcpy(m_SavedPicFileName[idx],M_PicFileName[idx]);
		Find_Blip_andAddSome("\\_VIS_Figures\\", pFileName); //This function stores the "...\Blip\_VIS_Figures\" info to pFileName.
		strcat(pFileName,M_PicFileName[idx]);//Now pFileName has the full name of the file.
		result=LoadPictureFile(pFileName, m_pPicture[idx]);
		if(result==-1){
			char Msg[1024];
			sprintf(Msg,"Failed to read: %s",M_PicFileName[idx]);
			AfxMessageBox(Msg);
		}
	}
	if(m_pPicture[idx]){//Match! => Reuse
		m_pPicture[idx]->get_Width(&hmWidth);
		m_pPicture[idx]->get_Height(&hmHeight);
		m_pPicture[idx]->Render(m_DC[0], m_topleftX[idx],m_topleftY[idx], m_width_inPixels[idx], m_height_inPixels[idx], 0, hmHeight, hmWidth, -hmHeight, &m_savedClientRect);
		objDC.BitBlt(m_topleftX[idx],m_topleftY[idx],m_width_inPixels[idx],m_height_inPixels[idx],pDC,m_topleftX[idx],m_topleftY[idx],SRCCOPY);
		m_DC0Mrr.BitBlt(m_topleftX[idx],m_topleftY[idx],m_width_inPixels[idx],m_height_inPixels[idx],pDC,m_topleftX[idx],m_topleftY[idx],SRCCOPY);
	}
	else{
		AfxMessageBox("m_pPicture[idx] has no picture info?");
	}
	SyncObj_Display.Unlock();
}

void CMOXY_View::PvisShowStimAtHereNOW_ReplicationOfVIS()
{
	CSingleLock SyncObj_Display(&m_sema); SyncObj_Display.Lock();
	int nSaveDC=m_DC[0].SaveDC();
	CClientDC objDC(this);
	CDC *pDC=&m_DC[0];

	int i,idx;
	for(i=0;i<*M_UserClickedN;i++){
		idx=M_UserClickedVisObjIDs[i];
		//This function displays the rectangle AND 
		//saves the screen points of the rectangle in "m_topleftX[idx],m_topleftY[idx],m_width_inPixels[idx],m_height_inPixels[idx]". These coordinates are used to HIDE them later.
		if(m_XY_DisplayMode==EYE_MONITOR_MODE)m_pXYFig->FillSolidRect_SizeAroundFovia_ReportBackTopLeftXY_VisReplica(M_cntrX[idx], M_cntrY[idx],M_width[idx], M_hight[idx], M_rgb[idx],pDC, m_topleftX[idx],m_topleftY[idx],m_width_inPixels[idx],m_height_inPixels[idx]);
		else                                  m_pXYFig->FillSolidRect_ReportBackTopLeftXY(M_cntrX[idx], M_cntrY[idx],M_width[idx], M_hight[idx], M_rgb[idx],pDC, m_topleftX[idx],m_topleftY[idx],m_width_inPixels[idx],m_height_inPixels[idx]);
			objDC.BitBlt(m_topleftX[idx],m_topleftY[idx],m_width_inPixels[idx],m_height_inPixels[idx],pDC,m_topleftX[idx],m_topleftY[idx],SRCCOPY);
		m_DC0Mrr.BitBlt(m_topleftX[idx],m_topleftY[idx],m_width_inPixels[idx],m_height_inPixels[idx],pDC,m_topleftX[idx],m_topleftY[idx],SRCCOPY);
	}
	m_DC[0].RestoreDC(nSaveDC); 
	m_DC[0].SetBkColor(RGB(255, 255, 255));//This to to prevent the color of the rectangel to be carried over to the background color of the text in other figures. There must be a better solution. 
	SyncObj_Display.Unlock();
}

void CMOXY_View::PvisHideStimAtHereNOW_ReplicationOfVIS()
{
	CSingleLock SyncObj_Display(&m_sema); SyncObj_Display.Lock();
	int nSaveDC=m_DC[0].SaveDC();
	CClientDC objDC(this);
	CDC *pDC=&m_DC[0];

	int i,idx;
	for(i=0;i<*M_UserClickedN;i++){
		idx=M_UserClickedVisObjIDs[i];
		objDC.FillSolidRect(m_topleftX[idx],m_topleftY[idx],m_width_inPixels[idx],m_height_inPixels[idx],*M_backgroundScreenRGB);
		m_DC0Mrr.FillSolidRect(m_topleftX[idx],m_topleftY[idx],m_width_inPixels[idx],m_height_inPixels[idx],*M_backgroundScreenRGB);
	}
	m_DC[0].RestoreDC(nSaveDC); 
	m_DC[0].SetBkColor(RGB(255, 255, 255));//This to to prevent the color of the rectangel to be carried over to the background color of the text in other figures. There must be a better solution. 
	SyncObj_Display.Unlock();
}

// This function loads a file into an IStream.
//Supported Files Types: bmp, gif, jpg, ico, emf, wmf
int CMOXY_View::LoadPictureFile(LPCTSTR FileName, LPPICTURE &pPicture)
{
	// open file
	HANDLE hFile = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	//_ASSERTE(INVALID_HANDLE_VALUE != hFile);
	if(INVALID_HANDLE_VALUE == hFile)return -1;

	// get file size
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	//_ASSERTE(-1 != dwFileSize);
	if(-1 == dwFileSize)return -1;

	LPVOID pvData = NULL;
	// alloc memory based on file size
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
	//_ASSERTE(NULL != hGlobal);
	if(NULL == hGlobal)return -1;

	pvData = GlobalLock(hGlobal);
	//_ASSERTE(NULL != pvData);
	if(NULL == pvData)return -1;

	DWORD dwBytesRead = 0;
	// read file and store in global memory
	BOOL bRead = ReadFile(hFile, pvData, dwFileSize, &dwBytesRead, NULL);
	//_ASSERTE(FALSE != bRead);
	if(FALSE == bRead)return -1;
	GlobalUnlock(hGlobal);
	CloseHandle(hFile);

	LPSTREAM pstm = NULL;
	// create IStream* from global memory
	HRESULT hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pstm);
	_ASSERTE(SUCCEEDED(hr) && pstm);
	//if(!(SUCCEEDED(hr) && pstm))return -1;

	// Create IPicture from image file
	if (pPicture)
		pPicture->Release();
	hr = ::OleLoadPicture(pstm, dwFileSize, FALSE, IID_IPicture, (LPVOID *)&pPicture);
	_ASSERTE(SUCCEEDED(hr) && pPicture);	
	//if(!(SUCCEEDED(hr) && pPicture))return -1;
	pstm->Release();

	return 1;
}