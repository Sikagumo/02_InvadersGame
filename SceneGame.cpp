/* ----------------
　ゲームシーン処理
 ------------------ */

#include<DxLib.h>
#include "main.h"
#include "SceneGame.h"
#include "Player.h"
#include "PShot.h"
#include "Enemy.h"
#include "EShot.h"


/* --- <<< 変数定義 >>> --- */

int gameScreen; // ゲーム領域


int curStageValue; //現在ステージ数

//ステージ数 増加処理
int CurStageValueUp(void)
{
	//ステージ数増加処理
	if (curStageValue < STAGE_VALUE)
	{
		return curStageValue++;
	}
	//ステージ数 超過時
	else
	{
		return curStageValue;
	}
}


int scoreValue;		//スコア
int highScoreValue; //ハイスコア



/* --- <<< 処理系 >>> --- */

/* <<< 初回ゲームシーン初期化 >>>*/
bool StartSceneGameInit(void)
{
	//一部初期化処理
	if (SceneGameInitGame() == false)
	{
		return false;
	}

	//ゲーム領域を作成
	gameScreen = MakeScreen(GAME_AREA_SIZE_X, GAME_AREA_SIZE_Y, false);


	highScoreValue = DEFAULT_HIGH_SCORE; //上位ハイスコア初期化
	//マーレ画像選択
	

	return true;
}


/* --- <<< ゲーム中ゲームシーン初期化 >>> --- */
bool SceneGameInitGame(void)
{
	scoreValue = 0; //現在スコア初期化

	curStageValue = 1; //ステージ数 初期化

	//プレイヤー 初期化
	if (PlayerInit() == false)
	{
		OutputDebugString("プレイヤーデータが初期化されていません\n");
		return false;
	}


	//敵 初期化
	if (EnemyStartInit() == false)
	{
		OutputDebugString("敵データが初期化されていません\n");
		return false;
	}

	return true;
}



/* --- <<< ゲームシーンメイン処理 >>> ---*/
bool SceneGameUpdate(void)
{
	//プレイヤー処理
	PlayerUpdate();

	/*  プレイヤーライフが0の時にシーン遷移 or  */
	if (GetPlayerActiveFlag() == false && GetPlayerLife() < 0 ||
		curStageValue >= STAGE_VALUE)
	{
		SceneGameInitGame();
		return false; //ゲームオーバー遷移
	}
	else
	{
		//プレイヤー有効時 処理有効化
		if (GetPlayerActiveFlag() == true)
		{
			PShotUpdate(); //プレイヤー弾丸移動処理
			EnemyUpdate(); //敵処理
		}

		else
		{
			if (CheckHitKey(KEY_INPUT_RETURN))
			{
				SetPlayerActiveFlag(true); //プレイヤー無効化
			}
		}
	}

	return true;
}

/* --- <<< ゲームシーン 描画処理 >>> --- */
void SceneGameDraw(void)
{
	//描画場所をゲーム領域にする
	SetDrawScreen(gameScreen);
	//ゲーム領域をクリアする
	ClearDrawScreen();

	SetDrawScreen(DX_SCREEN_BACK); //描画位置をスクリーンバックに設定

	DrawGraph(GAME_AREA_MIN_SIZE_X, GAME_AREA_MIN_SIZE_Y, gameScreen, true);

	DrawParameter();

	PlayerDraw(); //プレイヤー描画
	PShotDraw();  //プレイヤー弾丸描画
	EnemyDraw();  //敵描画

	if (GetPlayerActiveFlag() == false)
	{
		//復活用テキスト
		DrawFormatString((GAME_AREA_SIZE_X / 2) - (GetDrawStringWidth("Enterキーで復活",10) / 2),
			GAME_AREA_SIZE_Y / 2,
			GetColor(255,255,255),"Enterキーで復活");
	}

	//ゲーム領域 描画
	DrawBox(GAME_AREA_MIN_SIZE_X, GAME_AREA_MIN_SIZE_Y,
		GAME_AREA_SIZE_X + PIXEL_SIZE, GAME_AREA_SIZE_Y + PIXEL_SIZE,
		GetColor(255, 255, 255), false);
}


/* <<< パラメータ 描画処理 >>> */
void DrawParameter(void)
{
	//現在ステージ値 描画
	DrawFormatString(SCREEN_SIZE_X - (PIXEL_SIZE * 7),
		(PIXEL_SIZE * 2),
		GetColor(255, 255, 255), " STAGE：%1d", curStageValue);


	/*  現在スコアラベル 描画  */
	//枠
	DrawBox(GAME_AREA_SIZE_X + (PIXEL_SIZE * 2), (PIXEL_SIZE * 5),
		    GAME_AREA_SIZE_X + (PIXEL_SIZE * 4) + GetDrawStringWidth(" SCORE    ", 10),
		   (PIXEL_SIZE * 6), GetColor(255, 0, 0), true);
	//テキスト
	DrawString(GAME_AREA_SIZE_X + (PIXEL_SIZE * 2), (PIXEL_SIZE * 5),
		" SCORE", GetColor(255, 255, 255));

	//現在スコア値 描画
	DrawFormatString(GAME_AREA_SIZE_X + (PIXEL_SIZE * 2) + GetDrawStringWidth("0000000 ", 8),
		(PIXEL_SIZE * 6),
		GetColor(255, 255, 255), "% 5d ", scoreValue);


	/*  ハイスコアラベル 描画  */
	//枠
	DrawBox(GAME_AREA_SIZE_X + (PIXEL_SIZE * 2), (PIXEL_SIZE * 9),
			GAME_AREA_SIZE_X + (PIXEL_SIZE * 3) + GetDrawStringWidth(" HIGH-SCORE    ", 12),
		   (PIXEL_SIZE * 10), GetColor(255, 0, 0), true);
	//テキスト
	DrawString(GAME_AREA_SIZE_X + (PIXEL_SIZE * 2),((PIXEL_SIZE) * 9),
		" HIGH-SCORE", GetColor(255, 255, 255));

	//ハイスコア値 描画
	DrawFormatString(SCREEN_SIZE_X - (PIXEL_SIZE * 8) + GetDrawStringWidth("00000000", 8),
					(PIXEL_SIZE * 10),
		GetColor(255, 255, 255), "%06d ", highScoreValue);


	/*  プレイヤーライフラベル描画  */
	//枠
	DrawBox(GAME_AREA_SIZE_X + (PIXEL_SIZE * 2), (PIXEL_SIZE * 18),
			GAME_AREA_SIZE_X + (PIXEL_SIZE * 2) + GetDrawStringWidth(" LIFE         ", 13),
		   (PIXEL_SIZE * 19), GetColor(255, 0, 0), true);
	//テキスト
	DrawFormatString(GAME_AREA_SIZE_X + (PIXEL_SIZE * 2), (PIXEL_SIZE * 18),
		GetColor(255, 255, 255), " LIFE");

	DrawFormatString(0, 0, GetColor(255, 255, 255), "%d", highScoreValue);
	//ライフイラスト描画
	for (int i = 0; i < GetPlayerLife(); i++)
	{
		DrawGraph(GAME_AREA_SIZE_X + (PIXEL_SIZE * 2) + (PLAYER_IMAGE_SIZE_X * (i % 3)),
			(PIXEL_SIZE * 20) + (i / 3) * PLAYER_IMAGE_SIZE_X,
			GetPlayerImage(0), true);
	}
}


/* <<< ゲームシーン 解放処理 >>> */
void SceneGameRelease(void)
{
	EShotRelease();	 //敵弾丸 解放
	EnemyRelease();	 //敵 解放
	PShotRelease();	 //プレイヤー弾丸 解放
	PlayerRelease(); //プレイヤー 解放

	DeleteGraph(gameScreen); //ゲーム領域を解放
}


/* <<< スコア増加処理 >>> */
int ScoreUp(int value)
{
	scoreValue += value; //スコア増加処理

	return scoreValue;
}