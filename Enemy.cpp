/* ----------
	敵処理
 ------------ */

#include <DxLib.h>
#include "main.h"	   //システムデータ
#include "EShot.h"	   //敵弾丸データ
#include "Enemy.h"     //敵データ
#include "PShot.h"	   //プレイヤー弾丸データ
#include "SceneGame.h" //ゲームシーンデータ


/* --- <<< 変数系 >>> --- */

int enemyImages[static_cast<int>(ENEMY_TYPE::MAX)][ENEMY_IMAGE_COUNT_X * ENEMY_IMAGE_COUNT_Y * ENEMY_IMAGE_FRAME_VALUE]; //敵 画像
int enemysImagesNum; //敵画像 描画番号

//複数敵の画像パターン
int enemyCurSpeed; //現在 移動速度
int enemyCounter;  //現在 敵の数

int enemysPosX[ENEMY_VALUE_Y][ENEMY_VALUE_X]; //各敵 横座標
int enemysPosY[ENEMY_VALUE_Y][ENEMY_VALUE_X]; //各敵 縦座標

ENEMY_IMAGE_PARAM enemyImageParam[ENEMY_VALUE_Y][ENEMY_VALUE_X]; //敵 画像状態
ENEMY_PARAM_WIDTH enemyParamWidth; //敵 横移動状態
ENEMY_PARAM_VERT enemyParamVert;   //敵 縦移動状態

bool enemyActiveFlags[ENEMY_VALUE_Y][ENEMY_VALUE_X]; //敵有効化フラグ[true：Visible, false：Invisible]

/* --- <<< 処理系 >>> --- */

/* <<< 初期化処理 >>> */
bool EnemyStartInit(void)
{
	/* << 敵系 >> */
	enemyCounter  = ENEMY_VALUE_X * ENEMY_VALUE_Y; //敵の数取得
	enemyCurSpeed = ENEMY_MOVE_SPEED_X;			   //現在速度 初期化
	
	for (int y = ENEMY_VALUE_Y - 1; y >= 0; y--)
	{
		for (int x = ENEMY_VALUE_X - 1; x >= 0; x--)
		{
			/* 敵位置 初期化 */
			enemysPosX[y][x] = (ENEMY_SPAN_X * x) + GAME_AREA_MIN_SIZE_X;
			enemysPosY[y][x] = (ENEMY_SPAN_Y * y) + GAME_AREA_MIN_SIZE_Y;

			enemyActiveFlags[y][x] = true; //敵 有効化
			enemyImageParam[y][x]  = ENEMY_IMAGE_PARAM::RIGHT_IMAGE; //敵画像状態 初期化

			//画像番号 初期化
			enemysImagesNum = static_cast<int>(ENEMY_IMAGE_PARAM::MIDDLE_IMAGE) * ENEMY_IMAGE_FRAME_VALUE;
		}
	}

	enemyParamWidth = ENEMY_PARAM_WIDTH::E_MOVE_RIGHT; //敵 横移動状態 初期化
	enemyParamVert  = ENEMY_PARAM_VERT::E_MOVE_WIDTH;  //敵 縦移動状態 初期化


	EShotStartInit(); //敵弾丸初期化処理


	/*  グラフィック登録  */

	//敵画像のファイルリスト
	const char* filename[static_cast<int>(ENEMY_TYPE::MAX)]
	{
		"image/Enemy 15-1.png",
		"image/Enemy 15-2.png",
		"image/Enemy 15-3.png",
		"image/Enemy 15-4.png",
		"image/Enemy 15-5.png",
		"image/Enemy 15-6.png"
	};
	
	for (int e = 0; e < static_cast<int>(ENEMY_TYPE::MAX); e++)
	{
		//敵画像読み込み
		int enemy = LoadDivGraph(filename[e],
			ENEMY_IMAGE_COUNT_X * ENEMY_IMAGE_COUNT_Y,
			ENEMY_IMAGE_COUNT_X,
			ENEMY_IMAGE_COUNT_Y,
			ENEMY_IMAGE_SIZE_X,
			ENEMY_IMAGE_SIZE_Y,
			enemyImages[e]);

		//画像読み込み失敗確認
		if (enemy == -1)
		{
			OutputDebugString("敵画像が読み込まれていません。\n");
			return false;
		}
	}


	return true; //終了処理
}


/* <<< ゲーム中 初期化処理 >>> */
void EnemyInitGame(void)
{
	for (int y = ENEMY_VALUE_Y - 1; y >= 0; y--)
	{
		for (int x = ENEMY_VALUE_X - 1; x >= 0; x--)
		{
			enemysPosX[y][x] = GAME_AREA_MIN_SIZE_X + ENEMY_SPAN_X * x; //敵 縦位置初期化
			enemysPosY[y][x] = GAME_AREA_MIN_SIZE_Y + ENEMY_SPAN_Y * y; //敵 横位置初期化
		}
	}
	enemyParamVert = ENEMY_PARAM_VERT::E_MOVE_WIDTH;  //敵 縦移動状態 初期化

	EShotStartInit(); //敵弾丸初期化処理
}


/* <<< メイン処理 >>> */
void EnemyUpdate(void)
{
	EnemyMove();	   //敵移動処理

	/* <<< 敵弾丸 発射処理  >>> */
	for (int x = 0; x < ENEMY_VALUE_X; x++)
	{
		for (int y = ENEMY_VALUE_Y - 1; y >= 0; y--)
		{
			//発射最大地点最大値が 最大Y座標より大きい時に処理
			if (enemyActiveFlags[y][x] == true)
			{
				//敵画像 正面画像状態遷移
				enemyImageParam[y][x] = ENEMY_IMAGE_PARAM::MIDDLE_IMAGE; //正面画像にする

				//敵弾丸 更新処理
				EShotUpdate(enemysPosX[y][x],
							enemysPosY[y][x],
							enemyActiveFlags[y][x]);
				break;
			}
		}
	}


	/* <<< 画像状態 遷移処理 >>> */
	for (int y = 0; y < ENEMY_VALUE_Y; y++)
	{
		for (int x = 0; x < ENEMY_VALUE_X; x++)
		{
			if (enemyImageParam[y][x] != ENEMY_IMAGE_PARAM::MIDDLE_IMAGE)
			{
				if (enemyParamWidth == ENEMY_PARAM_WIDTH::E_MOVE_RIGHT)
				{
					//敵画像 右画像状態遷移
					enemyImageParam[y][x] = ENEMY_IMAGE_PARAM::RIGHT_IMAGE;
				}

				else if (enemyParamWidth == ENEMY_PARAM_WIDTH::E_MOVE_LEFT)
				{
					//敵画像 左画像状態遷移
					enemyImageParam[y][x] = ENEMY_IMAGE_PARAM::LEFT_IMAGE;
				}
			}
		}
	}

	/*  敵画像遷移処理  */
	for (int y = 0; y < ENEMY_VALUE_Y; y++)
	{
		for (int x = 0; x < ENEMY_VALUE_X; x++)
		{
			//敵画像状態が上限を超過時 状態初期化 [初期画像番号×フレーム倍率]
			if (enemysImagesNum >= (static_cast<int>(ENEMY_IMAGE_PARAM::MIDDLE_IMAGE) + 2) * ENEMY_IMAGE_FRAME_VALUE)
			{
				enemysImagesNum = static_cast<int>(ENEMY_IMAGE_PARAM::MIDDLE_IMAGE) * ENEMY_IMAGE_FRAME_VALUE;
			}

			else
			{
				//移動アニメーション遷移処理
				enemysImagesNum++;
			}
		}
	}

	if (enemyCounter <= 0)
	{
		EnemyStartInit();				//敵初期化
		CurStageValueUp();				//ステージ進行処理
		ScoreUp(ENEMY_KNOCKDOWN_SCORE); //撃破ポイント加算
	}
}


/* <<< 移動処理 >>> */
void EnemyMove(void)
{
	int enemyPosXMin; //左側の敵 位置
	int enemyPosXMax; //右側の敵 位置

	//左側の敵位置 初期化
	enemyPosXMin = GAME_AREA_SIZE_X + GAME_AREA_MIN_SIZE_X;

	//右側の敵位置 初期化
	enemyPosXMax = GAME_AREA_MIN_SIZE_X;


	/*  敵の数が一定に減少すると 移動速度上昇  */
	if (enemyCounter <= ENEMY_SPEED_UP_START_COUNT &&
		enemyCurSpeed == ENEMY_MOVE_SPEED_X)
	{
		enemyCurSpeed *= ENEMY_MOVE_SPEED_UP_VALUE; //移動速度 遷移処理
	}

	/*  敵移動速度上昇時に敵の数が一定に達していないとき 移動速度初期化  */
	else if (enemyCounter > ENEMY_SPEED_UP_START_COUNT &&
		enemyCurSpeed == ENEMY_MOVE_SPEED_X * ENEMY_MOVE_SPEED_UP_VALUE)
	{
		enemyCurSpeed = ENEMY_MOVE_SPEED_X; //移動速度 初期化
	}


	/* <<< 移動処理 >>> */

	/*  横移動処理  */
	if (enemyParamVert == ENEMY_PARAM_VERT::E_MOVE_WIDTH)
	{
		for (int y = 0; y < ENEMY_VALUE_Y; y++)
		{
			for (int x = 0; x < ENEMY_VALUE_X; x++)
			{
				/* 右移動処理 */
				if (enemyParamWidth == ENEMY_PARAM_WIDTH::E_MOVE_RIGHT)
				{
					enemysPosX[y][x] += enemyCurSpeed; //右移動処理
				}

				/* 左移動処理 */
				else if (enemyParamWidth == ENEMY_PARAM_WIDTH::E_MOVE_LEFT)
				{
					enemysPosX[y][x] -= enemyCurSpeed; //左移動処理
				}

				/* 端 の敵取得処理 */
				if (enemyActiveFlags[y][x] == true)
				{
					//X座標最小値より小さい時に更新
					if (enemysPosX[y][x] < enemyPosXMin)
					{
						enemyPosXMin = enemysPosX[y][x];
					}

					//X座標が最大値より大きい時に更新
					if (enemysPosX[y][x] > enemyPosXMax)
					{
						enemyPosXMax = enemysPosX[y][x];
					}
				}
			}
		}

		/* 右上限位置超過時 [左移動状態に遷移 + 下移動状態に遷移] */

		if (enemyParamWidth == ENEMY_PARAM_WIDTH::E_MOVE_RIGHT)
		{
			if (enemyPosXMax + ENEMY_IMAGE_SIZE_X > (GAME_AREA_SIZE_X + GAME_AREA_MIN_SIZE_X))
			{
				enemyParamWidth = ENEMY_PARAM_WIDTH::E_MOVE_LEFT; //左移動状態 遷移
				enemyParamVert  = ENEMY_PARAM_VERT::E_MOVE_DOWN;  //下移動状態 遷移
			}
		}

		/* 左上限位置超過時 [右移動状態に遷移 + 下移動状態に遷移] */
		else if (enemyParamWidth == ENEMY_PARAM_WIDTH::E_MOVE_LEFT)
		{
			if (enemyPosXMin < GAME_AREA_MIN_SIZE_X)
			{
				enemyParamWidth = ENEMY_PARAM_WIDTH::E_MOVE_RIGHT; //右移動状態 遷移
				enemyParamVert  = ENEMY_PARAM_VERT::E_MOVE_DOWN;   //下移動状態 遷移
			}
		}
	}

	/*  下移動処理  */
	else if (enemyParamVert == ENEMY_PARAM_VERT::E_MOVE_DOWN)
	{
		for (int y = 0; y < ENEMY_VALUE_Y; y++)
		{
			for (int x = 0; x < ENEMY_VALUE_X; x++)
			{
				//ゲーム範囲下 到達時
				if (GetEnemyUnder() > (GAME_AREA_SIZE_Y + GAME_AREA_MIN_SIZE_Y))
				{
					//敵 下移動
					enemysPosY[y][x] = (GAME_AREA_SIZE_Y - (ENEMY_IMAGE_SIZE_Y + PIXEL_SIZE));
				}
				else
				{
					enemysPosY[y][x] += ENEMY_SPAN_Y;  //全敵 一段下移動
				}
			}
		}
		enemyParamVert = ENEMY_PARAM_VERT::E_MOVE_WIDTH; //横移動状態 遷移
	}
}


/* <<< 敵衝突処理 >>> */
void EnemyCollision(int posX_, int posY_, bool activeflag_)
{
	/* <<< 敵衝突判定 >>> */
	for (int y = 0; y < ENEMY_VALUE_Y; y++)
	{
		for (int x = 0; x < ENEMY_VALUE_X; x++)
		{
			if (enemysPosX[y][x] < (posX_ + P_BULLET_IMAGE_SIZE_X) &&
				enemysPosY[y][x] < (posY_ + P_BULLET_IMAGE_SIZE_Y) &&
				(enemysPosX[y][x] + ENEMY_IMAGE_SIZE_X) > posX_ &&
				(enemysPosY[y][x] + ENEMY_IMAGE_SIZE_Y) > posY_ &&
				activeflag_ == true &&
				enemyActiveFlags[y][x] == true)
			{
				enemyCounter--; //現在の敵数 減少処理

				PShotHit(); //プレイヤー弾丸 無効化
				enemyActiveFlags[y][x] = false; //着弾した敵 無効化

				ScoreUp(ENEMY_DESTROY_SCORE); //スコア増加処理
			}
		}
	}
}


/* <<< 敵 描画処理 >>> */
void EnemyDraw(void)
{
	/* <<< 各敵 描画処理 >>> */

	for (int y = 0; y < ENEMY_VALUE_Y; y++)
	{
		for (int x = 0; x < ENEMY_VALUE_X; x++)
		{
			if (enemyActiveFlags[y][x] == true && enemysPosY[y][x] >= GAME_AREA_MIN_SIZE_X)
			{
				//敵画像描画処理
				DrawGraph(enemysPosX[y][x],
						  enemysPosY[y][x],
						  enemyImages[y % static_cast<int>(ENEMY_TYPE::MAX)]
									 [enemysImagesNum / ENEMY_IMAGE_FRAME_VALUE],
						  true);
			}

			EShotDraw(x, enemyActiveFlags[y][x]);  //敵弾丸描画
		}
	}
}


/* <<< 敵画像 解放処理 >>> */
void EnemyRelease(void)
{
	for (int e = 0; e < ENEMY_IMAGE_COUNT_X * ENEMY_IMAGE_COUNT_Y; e++)
	{
		DeleteGraph(enemyImages[static_cast<int>(ENEMY_TYPE::GREEN)][e]); //メモリから敵画像 削除
	}
}


/*  敵の一番下を取得  */
int GetEnemyUnder(void)
{
	int enemyPosYMax = 0; //一番下の敵位置

	/*  一番下の敵が全員撃破時 1つ上の敵位置を取得  */
	for (int y = 0; y < ENEMY_VALUE_Y; y++)
	{
		for (int x = 0; x < ENEMY_VALUE_X; x++)
		{
			if (enemyActiveFlags[y][x] == true &&
				enemysPosY[y][x] + ENEMY_IMAGE_SIZE_Y > enemyPosYMax)
			{
				enemyPosYMax = enemysPosY[y][x] + ENEMY_IMAGE_SIZE_Y; //下位置更新処理
			}
		}
	}
	return enemyPosYMax;
}