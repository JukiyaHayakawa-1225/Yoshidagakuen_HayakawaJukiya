//=============================================================================
//
// サウンド処理 [sound.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSound
{
public:	//誰からもアクセス可能
	//*************************************
	// サウンドファイル
	//*************************************
	typedef enum
	{
		SOUND_LABEL_TITLE = 0,		// BGM0 (OP)
		SOUND_LABEL_TUTORIAL,			// BGM1	(TUTORIAL)
		SOUND_LABEL_GAME,			// BGM2	(GAME)
		SOUND_LABEL_RESULT,			// BGM3	(CLEAR)
		SOUND_LABEL_RANKING,			// BGM4	(GAMEOVER)
		SOUND_LABEL_SE_PAUSE,		//ポーズ
		SOUND_LABEL_SE_DAMAGE,		//ポーズ
		SOUND_LABEL_SE_PRESS,		//ポーズ
		SOUND_LABEL_SE_SLASH,		//ポーズ
		SOUND_LABEL_SE_COUNT,		//ポーズ
		SOUND_LABEL_SE_SKILL0,
		SOUND_LABEL_SE_SKILL1,
		SOUND_LABEL_SE_SKILL2,
		SOUND_LABEL_SE_CURE,
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	//*****************************************************************************
	// パラメータ構造体定義
	//*****************************************************************************
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント(-1でループ再生)
	} SOUNDPARAM;

	CSound();
	~CSound();
	HRESULT InitSound(HWND hWnd);
	void UninitSound(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);
private:	//自分だけがアクセス可能
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2オブジェクトへのポインタ
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// マスターボイスへのポインタ
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイスへのポインタ
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータへのポインタ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

	// 各音素材のパラメータ
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =
	{
		{ "data/BGM/title.wav", -1 },		// BGM0 (OP)
		{ "data/BGM/tutorial.wav", -1 },		// BGM1	(TUTORIAL)
		{ "data/BGM/game.wav", -1 },		// BGM2	(GAME)
		{ "data/BGM/result.wav", 0 },		// BGM3	(CLEAR)
		{ "data/BGM/ranking.wav", -1 },		// BGM5	(RANKING)
		{ "data/SE/pause.wav", 0 },			// 死亡
		{ "data/SE/damage.wav", 0 },		// 死亡
		{ "data/SE/press.wav", 0 },			// 死亡
		{ "data/SE/slash.wav", 0 },			// 死亡
		{ "data/SE/count.wav", 0 },			// 死亡
		{ "data/SE/skill0.wav", 0 },			// 死亡
		{ "data/SE/skill1.wav", 0 },			// 死亡
		{ "data/SE/skill2.wav", 0 },			// 死亡
		{ "data/SE/cure.wav", 0 },			// 死亡

	};
};
#endif
