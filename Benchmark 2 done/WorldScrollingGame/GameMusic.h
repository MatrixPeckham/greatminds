#pragma once
#include "GameAudio.h"
#include "Game.h"
#include <xact3.h>
struct XACTDATA{
IXACT3Cue* songcue;
XACTINDEX song;
XACTINDEX fx;
HANDLE fHandle;
IXACT3Engine* engine;
IXACT3SoundBank* soundBank;
IXACT3WaveBank* wB;
IXACT3SoundBank* soundBank2;
IXACT3WaveBank* wB2;

CRITICAL_SECTION cs;
bool prepared;
bool stopped;
bool cuePrep;

VOID* sbData;
VOID* sbData2;
VOID* wbMemData;
};
void WINAPI XACTNotificationCallback2( const XACT_NOTIFICATION* pNotification );

class GameMusic : GameAudio {	
private:
	WCHAR* waveBank;
	WCHAR* soundBank;
	WCHAR* memwB;
	WCHAR* memsB;
	PCSTR song;  
	Game* game;
	bool bgPause;
public:
	
	GameMusic(WCHAR[], WCHAR[], WCHAR[], WCHAR[], PCSTR, Game*);
	void registerNotifications();
	void setUpStreamingWaveBank(WCHAR[]);
	void setUpMemWaveBank(WCHAR[]);
	void initXACT();
	void updateAudio();
	void setUpSoundBank(WCHAR[]);
	void setUpSoundBank2(WCHAR[]);
	void playNewSong(PCSTR);
	void playEffect(PCSTR);
	void stopEffect(PCSTR);


};