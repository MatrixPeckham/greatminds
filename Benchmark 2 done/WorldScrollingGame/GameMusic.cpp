#pragma once
#include "stdafx.h"
#include "GameMusic.h"

XACTDATA data2; 

GameMusic::GameMusic(WCHAR wBF[], WCHAR sBF[], WCHAR sfxWB[], WCHAR sfxSB[], PCSTR s, Game* g) {
	soundBank=sBF;
	waveBank=wBF;
	memsB=sfxSB;
	memwB=sfxWB;
	song=s;
	game=g;
	initXACT();
	bgPause=false;
}
void GameMusic::playEffect(PCSTR str) {
	
	data2.fx=data2.soundBank2->GetCueIndex(str);
	data2.soundBank2->Play(data2.fx,0,0,NULL);
}
void GameMusic::stopEffect(PCSTR str) {
	data2.fx=data2.soundBank2->GetCueIndex(str);
	data2.soundBank2->Stop(data2.fx, 0);
}
void GameMusic::updateAudio() {
	EnterCriticalSection(&data2.cs);
	bool  stop=data2.stopped;
	bool  prepared=data2.prepared;
	LeaveCriticalSection(&data2.cs);

	if(prepared) {
		EnterCriticalSection(&data2.cs);
		data2.prepared=false;
		LeaveCriticalSection(&data2.cs);
			data2.soundBank->Play(data2.song,0,0, &data2.songcue);
	}
	if(game->getGameState()==PAUSE_GAME_STATE || game->getGameState()==GAME_OVER_GAME_STATE || game->getGameState()==LEVEL_ALMOST_LOADING_MODE) {
		bgPause=true;
		data2.songcue->Pause(true);
	}
	if(bgPause && game->getGameState()==GAME_PLAYING_GAME_STATE) {
		data2.songcue->Pause(false);
		bgPause=false;
	}
	if(stop) {}
	
	if(data2.engine) {
		data2.engine->DoWork();
	}

}
void WINAPI XACTNotificationCallback2(const XACT_NOTIFICATION *pNotification){
	if(pNotification->type==XACTNOTIFICATIONTYPE_CUEPREPARED) {

	}
	if(pNotification->type==XACTNOTIFICATIONTYPE_WAVEBANKPREPARED && pNotification->waveBank.pWaveBank==data2.wB) {
		EnterCriticalSection(&data2.cs);
		data2.prepared=true;
		LeaveCriticalSection(&data2.cs);
	}
}
void GameMusic::initXACT() {
	InitializeCriticalSection(&data2.cs);
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	XACT3CreateEngine(0, &data2.engine);
	//SetUp engine
	XACT_RUNTIME_PARAMETERS params={0};
	params.fnNotificationCallback=XACTNotificationCallback2;
	params.lookAheadTime=XACT_ENGINE_LOOKAHEAD_DEFAULT;
	data2.engine->Initialize(&params);
	
	registerNotifications();
	setUpMemWaveBank(memwB);
	setUpSoundBank2(memsB);
	setUpStreamingWaveBank(waveBank);
	setUpSoundBank(soundBank);
	data2.song=data2.soundBank->GetCueIndex(song);
}

void GameMusic::setUpStreamingWaveBank(WCHAR file[]){
	HANDLE hr=CreateFile(file, GENERIC_READ, FILE_SHARE_READ,NULL,OPEN_EXISTING,  FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING,NULL);
	if(hr!=INVALID_HANDLE_VALUE) {
		data2.fHandle=hr;
		int size=GetFileSize(data2.fHandle, NULL);
		//data2.wbData=new BYTE[size];
		XACT_WAVEBANK_STREAMING_PARAMETERS wsParams;
        ZeroMemory( &wsParams, sizeof( XACT_WAVEBANK_STREAMING_PARAMETERS ) );
		wsParams.file = data2.fHandle;
        wsParams.offset = 0;
        wsParams.packetSize = 64;
		
		data2.engine->CreateStreamingWaveBank( &wsParams, &data2.wB);
	}
}
void GameMusic::setUpMemWaveBank(WCHAR file[]){
	HANDLE hr=CreateFile(file, GENERIC_READ, FILE_SHARE_READ,NULL,OPEN_EXISTING, 0,NULL);
	if(hr!=INVALID_HANDLE_VALUE) {
		data2.fHandle=hr;
		DWORD bRead;
		DWORD size=GetFileSize(data2.fHandle, NULL);
		data2.wbMemData=new BYTE[size];
		ReadFile( hr, data2.wbMemData, size, &bRead, NULL );

		data2.engine->CreateInMemoryWaveBank(data2.wbMemData,size,0,0, &data2.wB2);
	}
}
void GameMusic::playNewSong(PCSTR s) {
	song=s;
	data2.soundBank->Stop(data2.song, 0);
	data2.song=data2.soundBank->GetCueIndex(s);
	EnterCriticalSection(&data2.cs);
		data2.prepared=true;
		LeaveCriticalSection(&data2.cs);
}
void GameMusic::setUpSoundBank(WCHAR file[]) {
	HANDLE hFile = CreateFile( file, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
	if(hFile!=INVALID_HANDLE_VALUE) {
    int fileSize = GetFileSize( hFile, NULL );
	DWORD d;
	data2.sbData = new BYTE[fileSize];
	ReadFile( hFile, data2.sbData, fileSize, &d, NULL );
	data2.engine->CreateSoundBank( data2.sbData, fileSize, 0, 0, &data2.soundBank);
	}
	CloseHandle( hFile );
}
void GameMusic::setUpSoundBank2(WCHAR file[]) {
	HANDLE hFile = CreateFile( file, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
	if(hFile!=INVALID_HANDLE_VALUE) {
    int fileSize = GetFileSize( hFile, NULL );
	DWORD d;
	data2.sbData2 = new BYTE[fileSize];
	ReadFile( hFile, data2.sbData2, fileSize, &d, NULL );
	data2.engine->CreateSoundBank( data2.sbData2, fileSize, 0, 0, &data2.soundBank2);
	}
	CloseHandle( hFile );
}
void GameMusic::registerNotifications() {
	XACT_NOTIFICATION_DESCRIPTION desc={0};
	desc.type=XACTNOTIFICATIONTYPE_WAVEBANKPREPARED;
	desc.flags=XACT_FLAG_NOTIFICATION_PERSIST;
	data2.engine->RegisterNotification(&desc);

	desc.type=XACTNOTIFICATIONTYPE_CUEPREPARED;
	desc.flags=XACT_FLAG_NOTIFICATION_PERSIST;
	desc.cueIndex=XACTINDEX_INVALID;
	data2.engine->RegisterNotification(&desc);

	desc.type=XACTNOTIFICATIONTYPE_CUESTOP;
	desc.flags=XACT_FLAG_NOTIFICATION_PERSIST;
	desc.cueIndex=XACTINDEX_INVALID;
	data2.engine->RegisterNotification(&desc);


}
