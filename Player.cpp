/* -----------------
	プレイヤー処理
 ------------------- */

#include <DxLib.h>
#include "main.h"	   //システムデータ
#include "scenegame.h" //ゲームシーンデータ
#include "PShot.h"	   //プレイヤー弾丸データ
#include "Player.h"	   //プレイヤーデータ
#include "Enemy.h"	   //敵データ
#include "EShot.h"	   //敵弾丸データ


/* --- <<< 変数定義 >>> --- */

bool playerActiveFlag; //プレイヤー有効化フラグ(true：Active, false：Inactive)
//プレイヤー有効フラグ 取得
bool GetPlayerActiveFlag(void)
{
	return playerActiveFlag;
}
//プレイヤー有効化処理
bool SetPlayerActiveFlag(bool flag)
{
	playerActiveFlag = flag;
	return playerActiveFlag;
}

int playerPosX; //プレイヤー 縦位置
int playerPosY; //プレイヤー 横位置

int playerImageNum; //プレイヤー画像 描画番号
int playerImages[PLAYER_IMAGE_VALUE * PLAYER_IMAGE_FRAME_VALUE]; //プレイヤー画像

//プレイヤー画像 取得
int GetPlayerImage(int imageValue)
{
	return playerImages[imageValue];
}


/*  プレイヤー残機  */
int playerLife;

//プレイヤー残機を取得
int GetPlayerLife(void)
{
	return playerLife;
}

//プレイヤー残機 減少処理
int PlayerLifeDecrement(void)
{
	return playerLife--;
}


PLAYER_IMAGE_PARAM playerImageParam;  //プレイヤー画像状態
PLAYER_MOVE_PARAM playerMoveParam;	   //プレイヤー移動状態



/* --- <<< 処理系 >>> --- */

/* <<< プレイヤー初回初期化処理 >>> */
bool PlayerInit(void)
{
	playerImageParam = PLAYER_IMAGE_PARAM::CHARA_MALE; //キャラ"マーレ"選択

	PlayerImageChange(); //キャラクター画像 グラフィック登録
	
	PShotInit(playerPosX, playerPosY);

	PlayerReStart();

	return true; //終了処理
}


/* <<< プレイヤー変数 初期化処理 >>> */
void PlayerReStart(void)
{
	playerMoveParam = PLAYER_MOVE_PARAM::P_MOVE_NONE; //移動状態

	playerLife = PLAYER_LIFE; //残機初期化

	//プレイヤー位置初期化
	playerPosX = ((GAME_AREA_SIZE_X / 2) + GAME_AREA_MIN_SIZE_X) - (PLAYER_IMAGE_SIZE_X / 2);
	playerPosY = (GAME_AREA_SIZE_Y + GAME_AREA_MIN_SIZE_Y) - PLAYER_IMAGE_SIZE_Y;

	SetPlayerActiveFlag(true); //プレイヤー有効化処理

	//画像番号 初期化
	playerImageNum = static_cast<int>(PLAYER_MOVE_PARAM::P_MOVE_NONE) * PLAYER_IMAGE_FRAME_VALUE;
}


/* <<< プレイヤーメイン処理 >>> */
bool PlayerUpdate(void)
{
	if (playerActiveFlag == true)
	{
		PlayerImageState(); //プレイヤー画像 遷移処理

		switch (playerMoveParam)
		{
			/*  上向き状態  */

			case PLAYER_MOVE_PARAM::P_MOVE_NONE:

			//プレイヤー画像 左移動状態遷移
			if (CheckHitKey(KEY_INPUT_A) == 1 || CheckHitKey(KEY_INPUT_LEFT) == 1)
			{
				//プレイヤー画像 左向き画像状態遷移
				playerImageNum = static_cast<int>(PLAYER_MOVE_PARAM::P_MOVE_LEFT) * PLAYER_IMAGE_FRAME_VALUE;

				//プレイヤー画像 左向き状態遷移
				playerMoveParam = PLAYER_MOVE_PARAM::P_MOVE_LEFT;

			}

			//プレイヤー画像 右移動状態遷移
			if (CheckHitKey(KEY_INPUT_D) == 1 || CheckHitKey(KEY_INPUT_RIGHT) == 1)
			{
				//プレイヤー画像 右向き画像状態遷移
				playerImageNum = (static_cast<int>(PLAYER_MOVE_PARAM::P_MOVE_RIGHT)) * PLAYER_IMAGE_FRAME_VALUE;

				//プレイヤー画像 右向き状態遷移
				playerMoveParam = PLAYER_MOVE_PARAM::P_MOVE_RIGHT;
			}

			break;


			/*  左向き状態  */

		case PLAYER_MOVE_PARAM::P_MOVE_LEFT:

			playerPosX -= PLAYER_MOVE_SPEED_X; //移動処理

			//プレイヤー画像 上向き状態遷移
			if (CheckHitKey(KEY_INPUT_A) == 0 && CheckHitKey(KEY_INPUT_LEFT) == 0)
			{
				//プレイヤー画像 上向き画像状態遷移
				playerImageNum = (static_cast<int>(PLAYER_MOVE_PARAM::P_MOVE_NONE)) * PLAYER_IMAGE_FRAME_VALUE;

				//プレイヤー画像 上向き状態遷移
				playerMoveParam = PLAYER_MOVE_PARAM::P_MOVE_NONE;
			}


			if (playerPosX < GAME_AREA_MIN_SIZE_X)
			{
				playerPosX = GAME_AREA_MIN_SIZE_X; //移動制限
			}

			break;


			/*  右向き状態  */

		case PLAYER_MOVE_PARAM::P_MOVE_RIGHT:

			playerPosX += PLAYER_MOVE_SPEED_X; //右移動処理

			//プレイヤー画像 上向き状態遷移
			if (CheckHitKey(KEY_INPUT_D) == 0 && CheckHitKey(KEY_INPUT_RIGHT) == 0)
			{
				//プレイヤー画像 上向き画像状態遷移
				playerImageNum = (static_cast<int>(PLAYER_MOVE_PARAM::P_MOVE_NONE)) * PLAYER_IMAGE_FRAME_VALUE;

				//プレイヤー画像 上向き状態遷移
				playerMoveParam = PLAYER_MOVE_PARAM::P_MOVE_NONE;
			}

			if (playerPosX > (GAME_AREA_SIZE_X + GAME_AREA_MIN_SIZE_X) - PLAYER_IMAGE_SIZE_X)
			{
				//移動制限
				playerPosX = ((GAME_AREA_SIZE_X + GAME_AREA_MIN_SIZE_X) - PLAYER_IMAGE_SIZE_X);
			}

			break;
		}

		/*  敵位置が画面下超過時  */
		if (GetEnemyUnder() > (GAME_AREA_SIZE_Y - PLAYER_IMAGE_SIZE_Y))
		{
			EnemyInitGame(); //ゲーム中初期化処理

			PlayerLifeDecrement(); //残機減少

			PShotInit(playerPosX, playerPosY); //プレイヤー弾丸 初期化
			SetPlayerActiveFlag(false); //プレイヤー無効化
		}
	}
	else 
	{
		return false; //シーン遷移
	}


	//射出有効化処理
	if (CheckHitKey(KEY_INPUT_SPACE) == 1 &&
		playerMoveParam  == PLAYER_MOVE_PARAM::P_MOVE_NONE &&
		playerActiveFlag == true)
	{
		PShotActive(playerPosX, playerPosY); //射出有効化
	}
	return true;
}


/* <<< プレイヤー画像遷移処理>>> */
void PlayerImageState(void)
{
	/*  正面移動  */
	if (CheckHitKey(KEY_INPUT_A) == 0 && CheckHitKey(KEY_INPUT_LEFT)  == 0 &&
		CheckHitKey(KEY_INPUT_D) == 0 || CheckHitKey(KEY_INPUT_RIGHT) == 0)
	{
		//プレイヤー画像状態が上限を超過時 状態初期化 [初期画像番号×フレーム倍率]
		if (playerImageNum >= (static_cast<int>(PLAYER_MOVE_PARAM::P_MOVE_NONE) + 2) * PLAYER_IMAGE_FRAME_VALUE ||
			playerImageNum <  (static_cast<int>(PLAYER_MOVE_PARAM::P_MOVE_NONE))	 * PLAYER_IMAGE_FRAME_VALUE)
		{
			//正面向き状態状態化
			playerImageNum = static_cast<int>(PLAYER_MOVE_PARAM::P_MOVE_NONE) * PLAYER_IMAGE_FRAME_VALUE;
		}
		
		else
		{
			playerImageNum++; //移動アニメーション遷移処理
		}
	}

	/*  左移動  */
	if (CheckHitKey(KEY_INPUT_A) == 1 || CheckHitKey(KEY_INPUT_LEFT) == 1)
	{
		//プレイヤー画像状態が上限を超過時 状態初期化 [初期画像番号×フレーム倍率]
		if (playerImageNum >= (static_cast<int>(PLAYER_MOVE_PARAM::P_MOVE_LEFT) + 2) * PLAYER_IMAGE_FRAME_VALUE + 1 ||
			playerImageNum <  (static_cast<int>(PLAYER_MOVE_PARAM::P_MOVE_LEFT))	 * PLAYER_IMAGE_FRAME_VALUE)
		{
			//左向き状態化
			playerImageNum = static_cast<int>(PLAYER_MOVE_PARAM::P_MOVE_LEFT) * PLAYER_IMAGE_FRAME_VALUE;
		}

		else
		{
			playerImageNum++; //移動アニメーション遷移処理
		}
	}


	/*  右移動  */

	if (CheckHitKey(KEY_INPUT_D) == 1 || CheckHitKey(KEY_INPUT_RIGHT) == 1)
	{
		//プレイヤー画像状態が上限を超過時 状態初期化 [初期画像番号×フレーム倍率]
		if (playerImageNum >= (static_cast<int>(PLAYER_MOVE_PARAM::P_MOVE_RIGHT) + 2) * PLAYER_IMAGE_FRAME_VALUE + 1 ||
			playerImageNum <  (static_cast<int>(PLAYER_MOVE_PARAM::P_MOVE_RIGHT))	  * PLAYER_IMAGE_FRAME_VALUE)
		{
			//右向き状態化
			playerImageNum = static_cast<int>(PLAYER_MOVE_PARAM::P_MOVE_RIGHT) * PLAYER_IMAGE_FRAME_VALUE;
		}

		else
		{
			//移動アニメーション遷移処理
			playerImageNum++;
		}
	}
}


/* <<< プレイヤー当たり判定 >>> */
void PlayerCollision(int value, int enemyposX_, int enemyposY_, bool enemyflag_, int bulletposX_, int bulletposY_, bool bulletflag_)
{
	/* <<< 敵本体に衝突 >>> */
	if (playerActiveFlag == true)
	{
		if (
			/*  敵に衝突  */
			playerPosX < (enemyposX_ + ENEMY_IMAGE_SIZE_X) &&
			playerPosY < (enemyposY_ + ENEMY_IMAGE_SIZE_Y) &&
			(playerPosX + PLAYER_IMAGE_SIZE_X) > enemyposX_ &&
			(playerPosY + PLAYER_IMAGE_SIZE_Y) > enemyposY_ &&
			enemyflag_ == true ||

			/*  敵弾丸に衝突  */
			bulletposX_ < (playerPosX + PLAYER_IMAGE_SIZE_X) &&
			bulletposY_ < (playerPosY + PLAYER_IMAGE_SIZE_Y) &&
			(bulletposX_ + E_BULLET_IMAGE_SIZE_X) > playerPosX &&
			(bulletposY_ + E_BULLET_IMAGE_SIZE_Y) > playerPosY &&
			bulletflag_ == true
			)
		{
			PlayerLifeDecrement(); //残機減少

			PShotInit(playerPosX, playerPosY); //プレイヤー弾丸 初期化
			StateBulletShotActiveFlag(value, false); //敵弾丸無効化処理
			SetPlayerActiveFlag(false); //プレイヤー無効化
			
		}
	}
}


/* <<< 描画処理 >>> */
void PlayerDraw(void)
{
	/* <<< プレイヤー描画 >>> */
	
	if (playerActiveFlag == true)
	{
		//プレイヤー描画
		DrawGraph(playerPosX, playerPosY, playerImages[playerImageNum / PLAYER_IMAGE_FRAME_VALUE], true);
	}
}


/* <<< プレイヤー解放処理 >>> */
void PlayerRelease(void)
{
	for (int p = 0; p < PLAYER_IMAGE_VALUE; p++) 
	{
		DeleteGraph(playerImages[p]); //プレイヤー画像解放
	}
}


/* <<< プレイヤー画像 遷移処理 >>> */
bool PlayerImageChange(void)
{
	int chara;

	/* << プレイヤー画像 分割読み込み >> */

	/* <<< フィメール選択時 >>> */
	if (playerImageParam == PLAYER_IMAGE_PARAM::CHARA_FEMALE)
	{
		chara = LoadDivGraph("image/Female 05-2.png", //画像ハンドル
			PLAYER_IMAGE_VALUE,	  //画像分割数
			PLAYER_IMAGE_COUNT_X, //分割数(x)
			PLAYER_IMAGE_COUNT_Y, //分割数(y)
			PLAYER_IMAGE_SIZE_X,  //各画像サイズ(x)
			PLAYER_IMAGE_SIZE_Y,  //各画像サイズ(y)
			playerImages);		  //画像格納用変数

		if (chara == -1)
		{
			OutputDebugString("フィメールの画像 が読み込まれていません\n");
			return false;
		}
	}

	/* <<< マーレ選択時 >>> */
	else if (playerImageParam == PLAYER_IMAGE_PARAM::CHARA_MALE)
	{
		chara = LoadDivGraph("image/Male 05-1.png", //画像ハンドル
			PLAYER_IMAGE_VALUE,	  //画像分割数
			PLAYER_IMAGE_COUNT_X, //分割数(x)
			PLAYER_IMAGE_COUNT_Y, //分割数(y)
			PLAYER_IMAGE_SIZE_X,  //各画像サイズ(x)
			PLAYER_IMAGE_SIZE_Y,  //各画像サイズ(y)
			playerImages);		  //画像格納用変数

		if (chara == -1)
		{
			OutputDebugString("マーレの画像 が読み込まれていません\n");
			return false;
		}
	}

	return true;
}


/* <<< プレイヤー画像 選択処理 >>> */
void PlayerImageSelect(void)
{
	//マーレに遷移
	if (playerImageParam == PLAYER_IMAGE_PARAM::CHARA_FEMALE)
	{
		playerImageParam = PLAYER_IMAGE_PARAM::CHARA_MALE;
	}

	//フィメールに遷移
	else if (playerImageParam == PLAYER_IMAGE_PARAM::CHARA_MALE)
	{
		playerImageParam = PLAYER_IMAGE_PARAM::CHARA_FEMALE;
	}
}