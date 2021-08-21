#include "stdafx.h"
#include "Analysis.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "AnalysisDoc.h"
#include "Figure.h"
#include "AnalysisView.h"
#include "SinTable.h"

void CAnalysisView::CreateSharedMemoryArea()
{

	hFile = CreateFileMapping(INVALID_HANDLE_VALUE, 
		NULL,
		PAGE_READWRITE, 
		0, 
		1024 * 4, 
		"ALongFileName");
	if (hFile == NULL){
		 printf("SHARE MEMORY LIB: Unable to create a file.");
		 exit(1);
	}

	hView = (LPINT) MapViewOfFile(hFile, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if (hView == NULL){
		 printf("Unable to create a VIEW.");
		 exit(1);
	}
	printf("SHARE MEMORY LIB: Shared memory has been successfully created....\n");

	aux = hView;
}    

//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void CAnalysisView::OpenSharedMemory()
{
   hFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE,	"ALongFileName");            
	if (hFile == NULL){
		 printf("SHARE MEMORY LIB: Unable to open the shared area.\n");
		 exit(1);
	}

	hView = (LPINT) MapViewOfFile(hFile, FILE_MAP_ALL_ACCESS, /*FILE_MAP_WRITE | FILE_MAP_READ,*/0,	0,	0);
	if (hView == NULL){
		 printf("SHARE MEMORY LIB: Unable to create a VIEW.\n");
		 exit(1);
	}
	printf("SHARE MEMORY LIB: Shared memory has been successfully openned....\n");

	aux = hView;
}
//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void CAnalysisView::DestroySharedMemoryArea ()
{

	if (!UnmapViewOfFile(hView)){ 
       printf("Could not unmap view of file."); 
	} 
	
	CloseHandle(hFile); 
    
	printf("SHARE MEMORY LIB: Shared memory has been successfully destroyed....\n");

	printf("The end.\n");
}
//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void CAnalysisView::WriteOnSharedMemory(int data, int position)
{
//	printf("Writing data on the shared area...\n");
	aux[position] = data;
}

//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void CAnalysisView::ReadFromSharedMemory(int *data, int position)
{
//	printf("Reading data on the shared area...\n");
	*data = aux[position];
}
