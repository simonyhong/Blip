#if !defined (TRIAL_CLASS)
#define TRIAL_CLASS

class CSpksInTrialByChan: public CObject
{
public:
	//Spike data
	int m_NnID[3];
	int	m_ChanID, m_numNns, m_numSpikesOfNn[3];
	CArray<int, int& >m_SpikeTimesNn[3]; //Relative time from the start of the trial
	CSpksInTrialByChan(){};
	virtual ~CSpksInTrialByChan();
};


#define MAX_CHAN 1000
class CXCOR_View;
class CTrial : public CObject
{
	DECLARE_SERIAL( CTrial )//To save this CObject class to a file, this needs to be decleared
	int m_TrialID;
	__int64	m_StartTimeOfTrialInMs, m_EndTimeOfTrialInMs ;

	int m_NumUserData, m_SizeOfData, m_TotNumVlaluesToRecord;
	CArray<char*, char*& > m_NameArray;
	CArray<int, int& > m_SizeOfName, m_FirstLocationOfValue, m_NumOfValues;
	CArray<double, double& > m_ValuesToRecord;
	CArray<__int64, __int64& > m_TimeOfOccurrenceInMs;

	int	m_numEventCodes;
	CArray<int, int& >	m_EventCode;
	CArray<__int64,__int64&> m_EventTimeInMs;

	int	m_numChans, m_TotNumNsAcrossChans;
	CTypedPtrList<CObList, CSpksInTrialByChan*> m_ChanList;
	CSpksInTrialByChan* m_pChan[MAX_CHAN];
	int m_numChansInList, m_NumNnChansWithSpikes;

	//Analog data (e.g. eye movement trace)
	int	m_BehaviorSamplingRate;
	CArray<int, int& >	m_BehavioralAI0, m_BehavioralAI1;
	__int64 m_firstBehaviorAiIdx, m_lastBehaviorAiIdx;
	int m_numBehaviorChannels;
	__int64	m_StartTimeOfAI_inMs, m_EndTimeOfAI_inMs;
	int	m_numDataPointsOfBehaviorAI;

	//Override function. Serialize( CArchive& archive ) is defined in CObject; now it is being overridden.
	void Serialize( CArchive& archive, CXCOR_View *pView );
	//-------------------------------------
	//MFC requires a default constructor when it re-creates your objects as they are loaded from disk. The deserialization process will fill in all member variables with the values required to re-create the object.
	CTrial();
	virtual ~CTrial();
}; //<= don't forget to put ";" at the end!!

#endif //(TRIAL_CLASS)

