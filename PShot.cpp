/* -------------------
	プレイヤー弾丸処理
 -------------------- */

#include <Dxlib.h>
#include "main.h"	   //システムデータ
#include "SceneGame.h" //ゲームシーンデータ
#include "Player.h"	   //プレイヤーデータ
#include "PShot.h"     //プレイヤー弾丸データ
#include "Enemy.h"     //敵データ

/* --- <<< 変数定義 >>> --- */

int pBulletPosX; //弾丸 横位置
int pBulletPosY; //弾丸 縦位置

int pBulletImage; //弾丸画像
int pBulletCurRecastTime;   //現在発射間隔時間
bool pBulletShotActiveFlag; //発射可能フラグ(true：NowShot = Active, false：Inactive)


/* --- <<< 処理系 >>> --- */

/* <<< プレイヤー弾丸初期化処理 >>> */
bool PShotInit(int posX, int posY)
{
	//プレイヤー弾丸画像 グラフィック登録
	pBulletImage = LoadGraph("image/pshot.png");
	if (pBulletImage == -1)
	{
		OutputDebugString("弾丸画像 が読み込まれていません\n");
		return false;
	}

	/*  プレイヤー弾丸位置 初期化  */
	pBulletPosX = posX + (PLAYER_IMAGE_SIZE_X / 2) - (P_BULLET_IMAGE_SIZE_X / 2);
	pBulletPosY = posY - P_BULLET_IMAGE_SIZE_Y;

	pBulletShotActiveFlag = false; //弾丸 無効化

	return true; //正常終了
}


/* <<< プレイヤー弾丸 発射処理 >>> */
void PShotUpdate(void)
{
	//射出処理
	if (pBulletShotActiveFlag == true)
	{
		pBulletPosY -= P_BULLET_SHOT_SPEED; //弾丸移動処理

		/*  射出無効化処理  */
		if (pBulletPosY < (GAME_AREA_MIN_SIZE_Y))
		{
			pBulletShotActiveFlag = false;
		}

		//敵衝突処理
		EnemyCollision(pBulletPosX, pBulletPosY, pBulletShotActiveFlag);
	}
}


/* <<< 発射無効化処理 >>> */
bool PShotHit(void)
{
	//敵に衝突時 再度発射可能化
	if (pBulletShotActiveFlag == true)
	{
		pBulletShotActiveFlag = false;
	}

	return pBulletShotActiveFlag; //値を返す
}


/* <<< プレイヤー弾丸発射処理 >>> */
void PShotActive(int posX, int posY)
{
	if (pBulletShotActiveFlag == false)
	{
		pBulletShotActiveFlag = true; //射出有効化

		/*  プレイヤー発射位置取得  */
		pBulletPosX = posX + (PLAYER_IMAGE_SIZE_X / 2) - (P_BULLET_IMAGE_SIZE_X / 2);
		pBulletPosY = posY - P_BULLET_IMAGE_SIZE_Y;
	}
}


/* <<< プレイヤー弾丸 描画処理 >>> */
void PShotDraw(void)
{
	if (pBulletShotActiveFlag == true)
	{
		//プレイヤー弾丸描画
		DrawGraph(pBulletPosX, pBulletPosY, pBulletImage, true);
	}
}


/* <<< プレイヤー弾丸 解放処理 >>> */
void PShotRelease(void)
{
	DeleteGraph(pBulletImage); //プレイヤー弾丸画像解放
}