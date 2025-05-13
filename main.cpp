/* -------------------------------
　ゲーム制作：課題２(インベーダー)
　　 氏名 　：2416026　中川原 諒
 --------------------------------- */

#include<DxLib.h>
#include "main.h"		//システムデータ
#include "SceneTitle.h"    //タイトルシーンデータ
#include "SceneGame.h"	   //ゲームシーンデータ
#include "SceneGameOver.h" //ゲームオーバーデータ
#include "Player.h"		   //プレイヤーデータ


/* --- <<< 変数定義 >>> --- */

GAME_PARAM gameParam; //ゲームの状態
bool gameEndFlag;	   //ゲーム終了フラグ[true:GameEnd,false:GameActive]

int preInputKey; //前フレームの入力キーを取得
int curInputKey; //現在の入力を取得

/*  フレームレート制御系  */

int curTime		  = 0; //現在の時間
int lateFrameTime = 0; //前回のフレーム実行時間

int frameCount			= 0;	//フレームカウント
int updateFrameRateTime = 0;	//フレームレート 更新時間
float frameRate			= 0.0f; //表示用フレームレート


/*--- <<< WinMain関数 >>> ---*/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	bool init = StartInit(); //初期化処理
	if (init == false)
	{
		return -1; //失敗時に強制終了
	}

	/*--- <<< ゲームループ >>> ---*/

	while (ProcessMessage() == 0)
	{
		/* <<< フレームレート >>> */

		Sleep(1); //システムに処理を返す

		//現在時刻を取得する
		curTime = GetNowCount();

		if (curTime - lateFrameTime >= FRAME_RATE)
		{
			FrameRateUpdate();

			Update(); //ゲームメイン処理


			/* <<< 入力キー初期化 >>> */
			preInputKey = curInputKey;
			curInputKey = CheckHitKey(KEY_INPUT_RETURN);

			ScreenFlip();//裏画面を表画面にコピー

			Draw(); //描画処理
			
		}

		if (gameEndFlag == true) 
		{
			break;
		}
	}

	Release(); //解放処理

	/*--- <<< システム終了処理 >>> ---*/
	DxLib_End();
	return 0;
}


/* --- <<< 初期化処理[初回] >>> --- */
bool StartInit(void)
{
	/*--- <<< システム処理 >>> ---*/
	SetWindowText("課題2：インベーダーゲーム");
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	SetWaitVSyncFlag(0); //垂直同期を切る

	if (DxLib_Init() == -1)
	{
		OutputDebugString("DxLibが初期化されていません\n");
		return false;
	}

	gameParam   = GAME_PARAM::TITLE; //ゲーム状態 初期化
	gameEndFlag = false;			 //ゲーム終了フラグ 無効化

	preInputKey = CheckHitKey(KEY_INPUT_RETURN); //前入力判定値 入力状態取得
	curInputKey = preInputKey;					 //現在入力判定 前入力判定値を取得

	//タイトルシーン 初期化
	if (!SceneTitleInit())
	{
		OutputDebugString("タイトルデータ が初期化されていません\n");
		return false;
	}

	//ゲームシーン 初期化
	if (!StartSceneGameInit())
	{
		OutputDebugString("ゲームシーン が初期化されていません\n");
		return false;
	}

	//ゲームオーバーシーン 初期化
	if (!SceneGameOverStartInit())
	{
		OutputDebugString("ゲームオーバーが初期化されていません\n");
		return false;
	}

	return true; //正常に終了
}


/* --- <<< ゲームメイン処理 >>> --- */
void Update(void) 
{
	/*  タイトル処理  */
	switch (gameParam) 
	{

		/*  タイトル処理  */
		case GAME_PARAM::TITLE:

		/*  タイトル処理終了時 ゲーム処理に遷移  */
		if (SceneTitleUpdate() == false)
		{
			gameParam = GAME_PARAM::GAME;
		}
		break;


		/*  ゲーム処理  */
		case GAME_PARAM::GAME:

		/*  ゲーム処理終了時 ゲームオーバー処理に遷移  */
		if (SceneGameUpdate() == false)
		{
			gameParam = GAME_PARAM::GAMEOVER;
		}
		break;


		/*  ゲームオーバー処理  */
		case GAME_PARAM::GAMEOVER:

		/*  ゲームオーバー処理終了時 タイトル処理に遷移  */
		if (SceneGameOverUpdate() == false)
		{
			gameParam = GAME_PARAM::TITLE;
		}
		break;
			
	}


	/*  ゲーム終了処理  */
	if (gameParam != GAME_PARAM::TITLE &&
		CheckHitKey(KEY_INPUT_ESCAPE) == 1)
	{
		GameEnd();
	}
}


/* --- <<< 描画処理 >>> --- */
void Draw(void) 
{
	SetDrawScreen(DX_SCREEN_BACK); //描画画面の内容を裏画面に設定
	ClearDrawScreen();			   //描画画面の内容削除

	//フレームレート描画
	DrawFormatString(SCREEN_SIZE_X - 90, 0, GetColor(255, 255, 255), "FPS[%.1f]", frameRate);


	
	switch (gameParam)
	{

		/*　タイトル描画　*/
		case GAME_PARAM::TITLE:
			SceneTitleDraw();
		break;


		/*　ゲーム描画　*/
		case GAME_PARAM::GAME:
			SceneGameDraw();
		break;

		/*　ゲームオーバー描画　*/
		case GAME_PARAM::GAMEOVER:
			SceneGameOverDraw();
		break;
	}
}


/* --- <<< 解放処理 >>> --- */
void Release(void) 
{
	SceneGameOverRelease();	//ゲームオーバー 解放
	SceneGameRelease();		//ゲーム 解放
	SceneTitleRelease();	//タイトル 解放

	//初期化処理とは逆に解放処理を行う(初期化[親→子→孫で生成]、解放処理[孫→子→親で解放を行うため])
}


/* --- <<< ゲーム終了処理 >>> --- */
void GameEnd(void)
{
	gameEndFlag = true; //ゲーム終了化
}


void FrameRateUpdate(void)
{
	lateFrameTime = curTime; //前フレーム時間を現在フレーム時間から取得

	frameCount++; //フレーム計算値を増やす

	//現在時間 - フレーム更新時間
	int nDifTime = curTime - updateFrameRateTime;

	//1000ミリ秒経過まで繰り返す
	if (nDifTime > 1000)
	{
		float fFrameConut = (float)(frameCount * 1000); //ミリ秒単位に変換する

		frameRate = fFrameConut / nDifTime; //フレーム数(ミリ秒) ÷ 現在時間 - フレーム更新時間

		frameCount = 0; //フレーム計算 初期化

		//フレームレート更新時間 更新
		updateFrameRateTime = curTime;
	}
}