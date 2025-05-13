/* --------------------
	ゲームオーバー処理
 ---------------------- */

#include <DxLib.h>
#include "main.h" //システムデータ
#include "SceneGameOver.h"//ゲームオーバーデータ

/* --- <<< 変数定義 >>> --- */

int gameoverImage;			 //ゲームオーバータイトル 画像
float curPerformWaitTime;	 //現在演出 待機時間
float returnTextPerformTime; //"戻る"テキスト 演出時間

bool gameoverImageActiveFlag; //ゲームオーバータイトル画像 有効化フラグ

RETURN_TEXT_PARAM returnTextParam; //"戻る"状態演出

GAMEOVER_PERFORM_PARAM performParam; //ゲームオーバー演出処理

int gameoverPreInputKey; // 前 Spaceキー入力判定値
int gameoverCurInputKey; //現在Spaceキー入力判定値


/* <<< ゲームオーバー初期化[初回] >>> */
bool SceneGameOverStartInit(void)
{
	//ゲームオーバー画像 グラフィック登録
	gameoverImage = LoadGraph("image/GameOver.png");
	if (gameoverImage == -1)
	{
		OutputDebugString("ゲームオーバー画像が読み込まれていません。");
		return false;
	}

	SceneGameOverInit(); //初期化処理

	return true;
}


/* <<< ゲームオーバー初期化[共有] >>> */
void SceneGameOverInit(void)
{
	//演出初期化処理
	performParam = GAMEOVER_PERFORM_PARAM::NONE;

	curPerformWaitTime	  = GAMEOVER_IMAGE_WAIT_TIME; //ゲームオーバー演出時間 初期化
	returnTextPerformTime = RETURN_TEXT_VISIBLE_TIME; //"戻る"テキスト演出時間 初期化

	gameoverImageActiveFlag = false; //ゲームオーバータイトル描画フラグ 有効化

	returnTextParam = RETURN_TEXT_PARAM::INACTIVE; //"戻る"テキスト描画処理 無効化

	gameoverPreInputKey = CheckHitKey(KEY_INPUT_RETURN); // 前 Enterキー判定値 取得
	gameoverCurInputKey = gameoverPreInputKey;			 //現在Enterキー判定値 前Enter判定値取得
}


/* <<< ゲームオーバー処理 >>> */
bool SceneGameOverUpdate(void)
{
	gameoverPreInputKey = gameoverCurInputKey;			 // 前 Enterキー判定値 前フレームの現在Enter判定値を取得
	gameoverCurInputKey = CheckHitKey(KEY_INPUT_RETURN); //現在Enterキー判定値 入力値取得

	/*  演出スキップ処理 (連続入力制限あり)  */

	if (performParam == GAMEOVER_PERFORM_PARAM::PROC_ACTIVE)
	{
		if (gameoverPreInputKey == 0 && gameoverCurInputKey == 1)
		{
			SceneGameOverInit(); //ゲームオーバー初期化処理
			return false; //シーン遷移
		}
	}

	/*  ゲームオーバー処理  */
	else if (gameoverPreInputKey == 0 && gameoverCurInputKey == 1)
	{
		gameoverImageActiveFlag = true; //ゲームオーバータイトル画像 有効化
		returnTextParam = RETURN_TEXT_PARAM::VISIBLE; //"戻る"テキスト 有効化

		performParam = GAMEOVER_PERFORM_PARAM::PROC_ACTIVE; //"処理有効化" 状態遷移
	}
	
	


	/*  戻るボタン演出  */
	if (returnTextParam != RETURN_TEXT_PARAM::INACTIVE)
	{
		if (returnTextPerformTime > 0)
		{
			returnTextPerformTime -= 0.1f; //演出時間 減少処理
		}

		//演出状態 遷移処理
		else
		{
			if (returnTextParam == RETURN_TEXT_PARAM::INVISIBLE)
			{
				returnTextPerformTime = RETURN_TEXT_VISIBLE_TIME;	//可視化時間 取得
				returnTextParam		  = RETURN_TEXT_PARAM::VISIBLE; //"戻る"テキスト 可視化
			}
			else if (returnTextParam == RETURN_TEXT_PARAM::VISIBLE)
			{
				returnTextPerformTime = RETURN_TEXT_INVISIBLE_TIME;	  //不可視化時間 取得
				returnTextParam		  = RETURN_TEXT_PARAM::INVISIBLE; //"戻る"テキスト 不可視化
			}
		}
	}

	GameOverPerformance(); //各演出遷移処理
	return true;
}


/* <<< 演出遷移処理 >>> */
void GameOverPerformance(void)
{
	/*  初期演出 状態  */
	if (performParam == GAMEOVER_PERFORM_PARAM::NONE)
	{
		//初期状態 → タイトル画像演出 遷移処理
		PerformWaitTime(GAMEOVER_PERFORM_PARAM::TITLE_TEXT, GAMEOVER_IMAGE_WAIT_TIME);
	}


	/*  タイトル画像演出 状態  */
	else if (performParam == GAMEOVER_PERFORM_PARAM::TITLE_TEXT)
	{
		gameoverImageActiveFlag = true; //ゲームオーバータイトル画像 有効化

		//タイトル画像演出 → "戻る"テキスト演出 遷移処理
		PerformWaitTime(GAMEOVER_PERFORM_PARAM::RETURN_TEXT, RETURN_TEXT_WAIT_TIME);
	}


	/*  "戻る"テキスト演出 状態  */
	else if (performParam == GAMEOVER_PERFORM_PARAM::RETURN_TEXT)
	{
		//"戻る"テキスト演出 描画状態化
		if (returnTextParam == RETURN_TEXT_PARAM::INACTIVE) 
		{
			returnTextParam = RETURN_TEXT_PARAM::VISIBLE;
		}

		//"戻る"テキスト演出 → 処理有効化 遷移処理
		PerformWaitTime(GAMEOVER_PERFORM_PARAM::PROC_ACTIVE, PROCESS_ACTIVE_WAIT_TIME);
	}
}


/* <<< 演出待機処理 >>> */
void PerformWaitTime(GAMEOVER_PERFORM_PARAM nextperform, float nexttime)
{
	//演出待機処理
	if (curPerformWaitTime > 0)
	{
		curPerformWaitTime -= 0.1f; //演出待機時間 減少処理
	}

	//演出状態 遷移処理
	else
	{
		performParam	   = nextperform; //演出状態 遷移
		curPerformWaitTime = nexttime;	  //次の演出待機時間を取得
	}
}



/* <<< ゲームオーバー描画処理 >>> */
void SceneGameOverDraw(void)
{
	
	if (gameoverImageActiveFlag == true)
	{
		//ゲームオーバー画像 描画
		DrawGraph((SCREEN_SIZE_X / 2 - GAMEOVER_IMAGE_SIZE_X / 2),
			(SCREEN_SIZE_Y / 2 - GAMEOVER_IMAGE_SIZE_Y),
			gameoverImage, true);
	}


	//"戻る"テキスト 描画
	if (returnTextParam == RETURN_TEXT_PARAM::VISIBLE)
	{
		//テキスト描画
		DrawFormatString((SCREEN_SIZE_X / 2 - RETURN_TEXT_SIZE_X / 2),
			(SCREEN_SIZE_Y / 2 + RETURN_TEXT_SIZE_Y),
			0xFFFFFF, "Enterキーでタイトルに戻る");
	}

}


/* <<< ゲームオーバー解放処理 >>> */
void SceneGameOverRelease(void)
{
	DeleteGraph(gameoverImage); //ゲームオーバー画像解放
}