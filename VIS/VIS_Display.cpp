
#include "stdafx.h"
#include "VIS.h"

#include<stdlib.h>

#include "ThreeD.h"
#include "VIS_Doc.h"
#include "Figures.h"
#include "VIS_View.h"
#include <time.h>

void CVISView::WhichFunction(int messageID)                      
{	
	if(strcmp(M_StringMsg_fromTAS_toVIS,"PvisAllOff")==0){
		PvisAllOff_implement();
	}
	else if(strcmp(M_StringMsg_fromTAS_toVIS,"PvisSetBackColor")==0){
		PvisSetBackColor_implement();
	}
	else if(strcmp(M_StringMsg_fromTAS_toVIS,"PvisDrawBar")==0){
		PvisDrawBar_implement();
	}
	else if(strcmp(M_StringMsg_fromTAS_toVIS,"PvisDrawDisk")==0){
		PvisDrawDisk_implement();
	}
	else if(strcmp(M_StringMsg_fromTAS_toVIS,"PvisSwitchStim")==0){
		PvisSwitchStim_implement();
	}
	else if(strcmp(M_StringMsg_fromTAS_toVIS,"PvisDrawPic")==0){
		PvisDrawPic_implement();
	}
	else if(GetTickCount()-m_PrevTicks>1000){
		m_PrevTicks=GetTickCount();
		AfxMessageBox("Unknown command received.");
	}
	if(messageID!=*M_MsgID_fromTAS_toVIS && (GetTickCount()-m_PrevTicks>1000)){
		m_PrevTicks=GetTickCount();
		AfxMessageBox("The message has changed during this function; this should not happen");
	}
	Msg_to_TAS(messageID);
}

void CVISView::Msg_to_TAS(int messageID)
{
	////m_Figure.PutText(m_Figure.m_dMinX,m_Figure.m_dMaxY,"Writing data on the shared area...");
	////Wait if there is something that TAS has not resolved yet
	//while(*M_MsgID_fromVIS_toTAS>0){
	//	Sleep(1);
	//	if(m_bIsON ==0|| *M_Is_TAS_Running_4MOXYVIS==0){
	//		break;
	//	}
	//}
	//strcpy(M_StringMsg_fromVIS_toTAS, message);   
	if(*M_Is_TAS_Running_4MOXYVIS==1)*M_MsgID_fromVIS_toTAS=messageID;
	else							 *M_MsgID_fromVIS_toTAS=0;
}

void CVISView::Which_MOXY_Function()                      
{	
	if(*M_Msg_from_MOXY_to_VIS==1){
		PvisShowStimAtHereNOW_implement();
		*M_Msg_from_MOXY_to_VIS=0;
	}
	else if(*M_Msg_from_MOXY_to_VIS==2){
		PvisShowFigAtHereNOW_implement();
		*M_Msg_from_MOXY_to_VIS=0;
	}	
	else if(*M_Msg_from_MOXY_to_VIS==3){
		PvisHideStimAtHereNOW_implement();
		*M_Msg_from_MOXY_to_VIS=0;
	}	
}
 /* Clear screen */
void CVISView::PvisAllOff_implement()                     
{	
	VirMemDC();
	CClientDC objDC(this);

	m_DC[0].SelectObject(&m_backgroundBrush);
	m_Figure.m_normTx=m_savedClientRect.left;  m_Figure.m_normTy=m_savedClientRect.top;   
	m_Figure.m_normBx=m_savedClientRect.right;  m_Figure.m_normBy=m_savedClientRect.bottom; 
	m_Figure.m_LateralMargin=0;
	m_Figure.Set_MinMax_n_Labels_XY("","",-m_ScreenSize_inDegrees_4VIS,m_ScreenSize_inDegrees_4VIS,-m_ScreenSize_inDegrees_4VIS,m_ScreenSize_inDegrees_4VIS,xFRAME);		
	m_Figure.Frame_Graph2D(&m_DC[0]);
	objDC.BitBlt(m_savedClientRect.left,m_savedClientRect.top,m_savedClientRect.right-m_savedClientRect.left,m_savedClientRect.bottom-m_savedClientRect.top,&m_DC[0],m_savedClientRect.left,m_savedClientRect.top,SRCCOPY);

	/////////////////////
	m_DC[0].SelectObject(&m_backgroundBrush);
}

void CVISView::PvisSetBackColor_implement()                    
{
	m_backgroundBrush.DeleteObject(); //This is the only way to change the color
	m_backgroundBrush.CreateSolidBrush( *M_backgroundScreenRGB );
	m_DC[0].SelectObject(&m_backgroundBrush);
	m_backgroundScreenPen.DeleteObject(); 
	m_backgroundScreenPen.CreatePen(PS_SOLID, 5,*M_backgroundScreenRGB);
	m_DC[0].SelectObject(&m_backgroundScreenPen);

	m_Figure.Frame_Graph2D(&m_DC[0]);
}


void CVISView::PvisDrawBar_implement()
{
	int nSaveDC=m_DC[0].SaveDC();
	CDC *pDC=&m_DC[0];

	int i,idx;
	for(i=0;i<*M_N;i++){
		idx=M_VisObjIDs[i];
		//This function displays the rectangle AND 
		//saves the screen points of the rectangle in "m_topleftX_inPixel[idx],m_topleftY_inPixel[idx],m_width_inPixels[idx],m_height_inPixels[idx]".
		//The rectangle coordinates "m_topleftX_inPixel[idx],m_topleftY_inPixel[idx],m_width_inPixels[idx],m_height_inPixels[idx]" will be used to "unvail" the objects by PvisSwitchStim_implement().
		m_Figure.FillSolidRect_SizeAroundFovia_ReportBackTopLeftXY(M_cntrX[idx], M_cntrY[idx],M_width[idx], M_hight[idx], M_rgb[idx],pDC, m_topleftX_inPixel[idx],m_topleftY_inPixel[idx],m_width_inPixels[idx],m_height_inPixels[idx]);
	}
	m_DC[0].RestoreDC(nSaveDC); //Make sure that there is no intervening RestoreDC() between the stored nSaveDC and this. The intervening RestoreDC() pop out the needed stored DeviceContext configuration.
}


void CVISView::PvisDrawDisk_implement()
{
	int nSaveDC=m_DC[0].SaveDC();
	CDC *pDC=&m_DC[0];

	int i,idx;
	for(i=0;i<*M_N;i++){
		idx=M_VisObjIDs[i];
		//This function displays a disk AND 
		//saves the screen points of the disk "m_topleftX_inPixel[idx],m_topleftY_inPixel[idx],m_width_inPixels[idx],m_height_inPixels[idx]".
		//The disk coordinates "m_topleftX_inPixel[idx],m_topleftY_inPixel[idx],m_width_inPixels[idx],m_height_inPixels[idx]" will be used to "unvail" the objects by PvisSwitchStim_implement().
		m_Figure.Ellipse_SizeAroundFovia_ReportBackTopLeftXY(M_cntrX[idx], M_cntrY[idx],M_width[idx], M_hight[idx], M_rgb[idx],pDC, m_topleftX_inPixel[idx],m_topleftY_inPixel[idx],m_width_inPixels[idx],m_height_inPixels[idx]);
	}
	m_DC[0].RestoreDC(nSaveDC); //Make sure that there is no intervening RestoreDC() between the stored nSaveDC and this. The intervening RestoreDC() pop out the needed stored DeviceContext configuration.
}

void CVISView::PvisSwitchStim_implement()
{
	CClientDC objDC(this);
	int nSaveDC=m_DC[0].SaveDC();
	CDC *pDC=&m_DC[0];

	int i,idx;
	for(i=0;i<*M_N;i++){
		idx=M_VisObjIDs[i];
		if(M_Ojb_Switch[idx]){//ON
			//This one displays those screen areas specified by "m_topleftX_inPixel[idx],m_topleftY_inPixel[idx],m_width_inPixels[idx],m_height_inPixels[idx]," from "FillSolidRect_ReportBackTopLeftXY()"
			objDC.BitBlt(m_topleftX_inPixel[idx],m_topleftY_inPixel[idx],m_width_inPixels[idx],m_height_inPixels[idx],pDC,m_topleftX_inPixel[idx],m_topleftY_inPixel[idx],SRCCOPY);
		}
		else{//OFF
			objDC.FillSolidRect(m_topleftX_inPixel[idx],m_topleftY_inPixel[idx],m_width_inPixels[idx],m_height_inPixels[idx],*M_backgroundScreenRGB);
		}
	}
	objDC.FillSolidRect(*M_PhotoDiodeFlash_PosX,*M_PhotoDiodeFlash_PosY,*M_PhotoDiodeFlash_PosWidth,*M_PhotoDiodeFlash_PosHeight,RGB(255,255,255)); 
	m_StartTime=clock();
	m_DC[0].RestoreDC(nSaveDC); //Make sure that there is no intervening RestoreDC() between the stored nSaveDC and this. The intervening RestoreDC() pop out the needed stored DeviceContext configuration.
}


//Supported picture file types: 
//     bmp, gif, jpg, ico, emf, wmf
void CVISView::PvisDrawPic_implement()
{
	char pFileName[1024];
	int i,idx, result;
	long hmWidth;
	long hmHeight;
	for(i=0;i<*M_N;i++){
		idx=M_VisObjIDs[i];
		m_Figure.Rect_SizeAroundFovia_ReportBackTopLeftXY(M_cntrX[idx], M_cntrY[idx],M_width[idx], M_hight[idx], m_topleftX_inPixel[idx],m_topleftY_inPixel[idx],m_width_inPixels[idx],m_height_inPixels[idx]);
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
			m_pPicture[idx]->Render(m_DC[0], m_topleftX_inPixel[idx],m_topleftY_inPixel[idx], m_width_inPixels[idx], m_height_inPixels[idx], 0, hmHeight, hmWidth, -hmHeight, &m_savedClientRect);
		}
		else{
			AfxMessageBox("m_pPicture[idx] has no picture info?");
		}
	}
}
void CVISView::PvisShowFigAtHereNOW_implement()
{
	char pFileName[1024];
	int i,idx, result;
	long hmWidth;
	long hmHeight;
	CClientDC objDC(this);
	CDC *pDC=&m_DC[0];
	idx=0;
	m_Figure.Rect_SizeAroundFovia_ReportBackTopLeftXY(M_cntrX[idx], M_cntrY[idx],M_width[idx], M_hight[idx], m_topleftX_inPixel[idx],m_topleftY_inPixel[idx],m_width_inPixels[idx],m_height_inPixels[idx]);
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
		m_pPicture[idx]->Render(m_DC[0], m_topleftX_inPixel[idx],m_topleftY_inPixel[idx], m_width_inPixels[idx], m_height_inPixels[idx], 0, hmHeight, hmWidth, -hmHeight, &m_savedClientRect);
		objDC.BitBlt(m_topleftX_inPixel[idx],m_topleftY_inPixel[idx],m_width_inPixels[idx],m_height_inPixels[idx],pDC,m_topleftX_inPixel[idx],m_topleftY_inPixel[idx],SRCCOPY);
	}
	else{
		AfxMessageBox("m_pPicture[idx] has no picture info?");
	}
}

void CVISView::PvisShowStimAtHereNOW_implement()
{
	int nSaveDC=m_DC[0].SaveDC();
	CClientDC objDC(this);
	CDC *pDC=&m_DC[0];

	int i,idx;
	for(i=0;i<*M_UserClickedN;i++){
		idx=M_UserClickedVisObjIDs[i];
		//This function displays the rectangle AND 
		//saves the screen points of the rectangle in "m_topleftX_inPixel[idx],m_topleftY_inPixel[idx],m_width_inPixels[idx],m_height_inPixels[idx]". These coordinates are used to HIDE them later.
		m_Figure.FillSolidRect_SizeAroundFovia_ReportBackTopLeftXY(M_cntrX[idx], M_cntrY[idx],M_width[idx], M_hight[idx], M_rgb[idx],pDC, m_topleftX_inPixel[idx],m_topleftY_inPixel[idx],m_width_inPixels[idx],m_height_inPixels[idx]);
		objDC.BitBlt(m_topleftX_inPixel[idx],m_topleftY_inPixel[idx],m_width_inPixels[idx],m_height_inPixels[idx],pDC,m_topleftX_inPixel[idx],m_topleftY_inPixel[idx],SRCCOPY);
	}
	m_DC[0].RestoreDC(nSaveDC); //Make sure that there is no intervening RestoreDC() between the stored nSaveDC and this. The intervening RestoreDC() pop out the needed stored DeviceContext configuration.
}

void CVISView::PvisHideStimAtHereNOW_implement()
{
	int nSaveDC=m_DC[0].SaveDC();
	CClientDC objDC(this);
	CDC *pDC=&m_DC[0];

	int i,idx;
	for(i=0;i<*M_UserClickedN;i++){
		idx=M_UserClickedVisObjIDs[i];
		objDC.FillSolidRect(m_topleftX_inPixel[idx],m_topleftY_inPixel[idx],m_width_inPixels[idx],m_height_inPixels[idx],*M_backgroundScreenRGB);
	}
	m_DC[0].RestoreDC(nSaveDC); //Make sure that there is no intervening RestoreDC() between the stored nSaveDC and this. The intervening RestoreDC() pop out the needed stored DeviceContext configuration.
}


// This function loads a file into an IStream.
//Supported Files Types: bmp, gif, jpg, ico, emf, wmf
int CVISView::LoadPictureFile(LPCTSTR FileName, LPPICTURE &pPicture)
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
