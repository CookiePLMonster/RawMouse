#ifndef __CMENUMANAGER
#define __CMENUMANAGER

class CMenuManagerIII
{
public:
	BYTE		bRadioStation;
	DWORD		dwUnk;
	BYTE		bUnk;
	bool		bVibrationEnabled;
	bool		bHudEnabled;
	int			nRadarMode;
	BYTE		__pad1[257];
	bool		bMenuActive;

public:
	void		MessageScreen(const char* pText);
};

class CMenuManagerVC
{
public:
	BYTE		bRadioStation;
	DWORD		dwUnk;
	BYTE		bUnk;
	bool		bVibrationEnabled;
	bool		bHudEnabled;
	int			nRadarMode;
	BYTE		__pad1[40];
	bool		bMenuActive;

public:
	void		MessageScreen(const char* pText, bool bFullscreen);
};

class CMenuManagerSA
{
public:
	bool				m_bStatScrollUp;
	float               m_fStatsScrollSpeed;
	__int8 field_8;
	__int8 field_9[23];
	bool				bVibrationEnabled;
	unsigned char		m_bHudOn;
	__int8 field_22[2];
	__int32             m_dwRadarMode;
	__int8 field_28[4];
	__int32 field_2C;
	__int8 field_30;
	__int8 field_31;
	bool				m_bBackIntoGame;
	bool                m_bActivateMenuNextFrame;
	bool                m_bMenuAccessWidescreen;
	__int8 field_35;
	__int8 field_36[2];
	__int32 field_38;
	__int32             m_dwBrightness;
	float               m_fDrawDistance;
	bool                m_bShowSubtitles;
	bool 				m_bMapShowLocations;
	bool				m_bMapShowContacts;
	bool				m_bMapShowMission;
	bool				m_bMapShowOther;
	bool				m_bMapShowGangArea;
	bool                m_bMapLegend;
	unsigned char		m_bAspectRatioMode;
	unsigned char       m_bFrameLimiterMode;
	bool                m_bRadioAutoSelect;
	__int8 field_4E;
	__int8              m_nSfxVolume;
	__int8              m_nRadioVolume;
	bool                m_bRadioEq;
	signed char			m_nRadioStation;
	__int8 field_53;
	unsigned int		m_dwSelectedMenuItem;
	__int8 field_58;
	__int8 drawRadarOrMap;
	__int8 field_5A;
	__int8 field_5B;
	bool                m_bMenuActive;
	__int8 doGameReload;
	__int8 field_5E;
	__int8 isSaveDone;
	bool                m_bLoadingData;
	__int8 field_61[3];
	float               m_fMapZoom;
	float               m_fMapBaseX;
	float               m_fMapBaseY;
	float				m_vMousePos[2];
	bool				m_bMapLoaded;
	__int8 field_79[3];
	__int32 titleLanguage;
	__int32 textLanguage;
	unsigned char       m_nLanguage;
	unsigned char		m_nPrevLanguage;
	__int8 field_86[2];
	__int32 field_88;
	bool                m_bLanguageChanged;
	__int8 field_8D[3];
	__int32 field_90;
	__int8 field_94[24];
	__int32 field_AC;
	__int8              m_nRadioMode;
	__int8 invertPadX1;
	__int8 invertPadY1;
	__int8 invertPadX2;
	__int8 invertPadY2;
	__int8 swapPadAxis1;
	__int8 swapPadAxis2;
	bool				m_bInVehicleControlsScreen;
	bool                m_bDrawMouse;
	__int8 field_B9[3];
	__int32             m_dwMousePosLeft;
	__int32             m_dwMousePosTop;
	bool                m_bMipMapping;
	bool                m_bTracksAutoScan;
	__int16 field_C6;
	__int32             m_dwAppliedAntiAliasingLevel;
	__int32             m_dwAntiAliasingLevel;
	__int8              m_nController;
	__int8 field_D1[3];
	__int32             m_dwAppliedResolution;
	__int32             m_dwResolution;
	__int32 field_DC;
	__int32 mousePosLeftA;
	__int32 mousePosTopA;
	bool                m_bSavePhotos;
	bool                m_bMainMenuSwitch;
	__int8              m_nPlayerNumber;
	bool				m_bReinitLanguageSettings;
	__int32 field_EC;
	void* field_F0;
	__int8 field_F4;
	__int8 field_F5[3];
	union{
		struct{
			void** m_apTextures[25];
		};
		struct{
			void** m_apRadioSprites[13];
			void** m_apBackgroundTextures[8];
			void** m_apAdditionalBackgroundTextures[2];
			void** m_apMouseTextures[2];
		};
	};
	bool                m_bTexturesLoaded;
	signed char	        m_bCurrentMenuPage;
	signed char			m_bLastMenuPage;
	unsigned char		m_bSelectedSaveGame;
	unsigned char		m_bSelectedMissionPack;
	__int8 field_161;
	char                m_mpackName[8];
	__int8 field_16A[6486];
	__int32 field_1AC0;
	__int32 field_1AC4;
	__int32 field_1AC8;
	__int32 field_1ACC;
	__int32 field_1AD0;
	__int32 field_1AD4;
	__int32 field_1AD8;
	__int16 field_1ADC;
	bool                m_bChangeVideoMode;
	__int8 field_1ADF;
	__int32 field_1AE0;
	__int32 field_1AE4;
	__int8 field_1AE8;
	__int8 field_1AE9;
	__int8 field_1AEA;
	__int8 field_1AEB;
	__int32 field_1AEC;
	__int8 field_1AF0;
	__int8 field_1AF1;
	__int8 field_1AF2;
	__int8 field_1AF3;
	__int32 field_1AF4;
	__int32 field_1AF8;
	__int32 field_1AFC;
	int			m_nHoverOption;
	__int32 field_1B04;
	__int8 field_1B08;
	__int8 field_1B09;
	__int8 field_1B0A;
	__int8 field_1B0B;
	__int32 field_1B0C;
	__int8 field_1B10;
	__int8 field_1B11;
	__int8 field_1B12;
	__int8 field_1B13;
	__int8 field_1B14;
	__int8 field_1B15;
	__int8 field_1B16;
	__int8 field_1B17;
	int			m_nHelperTextIndex;
	__int32 field_1B1C;
	__int8 field_1B20;
	__int8 field_1B21;
	__int16 field_1B22;
	__int32 field_1B24;
	__int8 field_1B28;
	__int8 field_1B29;
	__int16 field_1B2A;
	__int32 field_1B2C;
	__int32 field_1B30;
	__int16 field_1B34;
	__int16 field_1B36;
	__int32 field_1B38;
	__int8 field_1B3C;
	__int8 field_1B3D;
	__int8 field_1B3E;
	__int8 field_1B3F;
	__int32 field_1B40;
	__int8 field_1B44;
	__int8 field_1B45;
	__int16 field_1B46;
	__int32 field_1B48;
	__int32 field_1B4C;
	__int8              m_nBackgroundSprite;
	__int8 field_1B51;
	__int16 field_1B52;
	__int32 field_1B54;
	__int32 field_1B58;
	__int8 field_1B5C;
	__int8 field_1B5D;
	__int16 field_1B5E;
	__int32 field_1B60;
	__int32 field_1B64;
	__int32 field_1B68;
	__int32 field_1B6C;
	__int32 field_1B70;
	__int32 field_1B74;

public:
	void		MessageScreen(const char* pText, bool bFullscreen, bool bWithinFrame);
	bool		NeedsToRefreshHelps();
};

#if defined _GTA_III	
typedef CMenuManagerIII CMenuManager;
#elif defined _GTA_VC
typedef CMenuManagerVC CMenuManager;
#elif defined _GTA_SA
typedef CMenuManagerSA CMenuManager;
#endif

extern CMenuManager&			FrontEndMenuManager;

#endif