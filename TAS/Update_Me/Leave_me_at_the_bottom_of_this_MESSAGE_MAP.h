#define Give_the_name_of_your_task_function_and_its_ID_here(FunctionName,FunctionID) (m_pDoc->FunctionArray[FunctionID]		=&CTASView::FunctionName)
UINT LAUNCH_TAS_THREAD(LPVOID pParam) //A global Function
{
	CTASView* pView=(CTASView*) pParam;
	CTASDoc *pDoc=pView->GetDocument();
	while(pDoc->m_IsPlayButtonON){
		Sleep(5);
		if(pDoc->m_isTAS_Running==0)pView->TaskScheduler();
	}
	return 0;
}

void CTASView::OnTaskScheduler() 
{
	if(m_pDoc->m_IsPlayButtonON==1){m_pDoc->m_IsPlayButtonON=0;return; }
	m_pDoc->m_IsPlayButtonON=1;
	m_pDoc->m_pTAS_Thread=AfxBeginThread(LAUNCH_TAS_THREAD, this);
}

UINT TAS_THREAD(LPVOID pParam) //A global Function
{
	CTASView* pView=(CTASView*) pParam;
	pView->GetDocument()->TAS() ;
	return 0;
}

