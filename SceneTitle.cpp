/* ---------------
	タイトルの処理
 ----------------- */

#include <DxLib.h>
#include "main.h"   //システムデータ
#include "Player.h" //プレイヤーデータ
#include "SceneTitle.h"  //タイトルシーンデータ　


/* --- <<< 変数定義 >>> --- */

TITLE_PARAM titleParam;   //タイトル状態
int titleImage;		   //タイトル画像

PLAYER_IMAGE_PARAM selectParam; //キャラ選択用 状態変数
int titleSelectImages[2]; //キャラ選択画像
int selecPerformImage;	   //選択演出画像

float curStartTextViewTime; //現在テキスト 描画時間
bool titleTextViewFlag;	 //開始テキスト 描画フラグ[true：View, false：NotView]

float curSelectPerformTime; //キャラ選択演出 描画時間
bool selectPerformFlag;	 //キャラ選択演出 描画フラグ[true：View, false：NotView]

int preInputKeys[4]; //前フレームの指定した入力キーを取得
int curInputKeys[4]; //現在の入力を取得


/* --- <<< 関数系 >>> --- */

/* <<< タイトル初期化処理 >>> */
bool SceneTitleInit(void)
{
	//タイトル画像 グラフィック登録
	titleImage = LoadGraph("image/TitleLogo2.png");
	if (titleImage == -1)
	{
		OutputDebugString("タイトル画像が読み込まれていません。\n");
		return false;
	}

	//マーレ選択画像 グラフィック登録
	titleSelectImages[0] = LoadGraph("image/SelectImage Male.png");
	if (titleSelectImages[0] == -1)
	{
		OutputDebugString("マーレ選択画像 が読み込まれていません。\n");
		return false;
	}

	//フィメール選択画像 グラフィック登録
	titleSelectImages[1] = LoadGraph("image/SelectImage Female.png");
	if (titleSelectImages[1] == -1)
	{
		OutputDebugString("フィメール選択画像 が読み込まれていません。\n");
		return false;
	}

	//選択演出画像 グラフィック登録
	selecPerformImage = LoadGraph("image/SelectPerform.png");
	if (selecPerformImage == -1)
	{
		OutputDebugString("選択演出画像 が読み込まれていません。\n");
		return false;
	}

	titleParam  = TITLE_PARAM::START; //タイトル状態 初期化
	selectParam = PLAYER_IMAGE_PARAM::CHARA_MALE; //選択状態 初期化

	curStartTextViewTime = START_TEXT_VISIBLE_TIME; //描画時間 初期化
	titleTextViewFlag	  = true;				    //開始テキスト描画フラグ 有効化

	curSelectPerformTime = SELECT_PERFORM_TIME; //選択演出描画時間   初期化
	selectPerformFlag	 = true;				//選択演出描画フラグ 有効化

	/*  キー入力確認 初期化  */
	InputKeyCheckInit(0, KEY_INPUT_RETURN); //Enterキー  入力確認初期化
	InputKeyCheckInit(1, KEY_INPUT_ESCAPE); //Escapeキー 入力確認初期化
	InputKeyCheckInit(2, KEY_INPUT_LEFT);	//左矢印キー 入力確認初期化
	InputKeyCheckInit(3, KEY_INPUT_RIGHT);	//右矢印キー 入力確認初期化


	return true; //正常終了
}


/* <<< キー入力判定 初期化 >>> */
void InputKeyCheckInit(int num, int inputKey)
{
	/*  指定キーの入力を行っているかの確認  */
	preInputKeys[num] = CheckHitKey(inputKey); //前入力キー判定値 取得
	curInputKeys[num] = preInputKeys[num];	   //現在入力キー判定 前入力値を反映
}


/* <<< キー入力判定 >>> */
void InputKeyCheckUpdate(int num, int inputKey)
{
	//指定キーの入力を行っているかの確認
	preInputKeys[num] = curInputKeys[num];	   //現在の入力を 前入力に反映する
	curInputKeys[num] = CheckHitKey(inputKey); //現在の入力を取得
}


/* <<< タイトルメイン処理 >>> */
bool SceneTitleUpdate(void)
{
	/*  キー入力確認  */
	InputKeyCheckUpdate(0, KEY_INPUT_RETURN); //Enterキー  入力確認
	InputKeyCheckUpdate(1, KEY_INPUT_ESCAPE); //Escapeキー 入力確認
	InputKeyCheckUpdate(2, KEY_INPUT_LEFT);	  //左矢印キー 入力確認
	InputKeyCheckUpdate(3, KEY_INPUT_RIGHT);  //右矢印キー 入力確認


	/*  タイトル初期状態 処理  */
	if (titleParam == TITLE_PARAM::START)
	{
		TitleStartProc();
	}

	/*  キャラ選択 処理  */
	else if (titleParam == TITLE_PARAM::SELECT)
	{
		TitleSelectProc();

		/*  ゲーム開始処理  */
		if (preInputKeys[0] == 0 && curInputKeys[0] == 1)
		{
			PlayerImageChange(); //プレイヤー画像 遷移処理

			//タイトル遷移状態 初期化
			titleParam  = TITLE_PARAM::START;

			//選択状態 初期化
			selectParam = PLAYER_IMAGE_PARAM::CHARA_MALE;

			return false; //シーン遷移
		}
	}
	return true;
}


/* <<< タイトル初期状態処理 >>> */
void TitleStartProc(void)
{
	/*  タイトル状態 選択画面に遷移  */
	if (preInputKeys[0] == 0 && curInputKeys[0] == 1)
	{
		titleParam = TITLE_PARAM::SELECT;
	}

	/*  ゲーム終了処理  */
	if (preInputKeys[1] == 0 && curInputKeys[1] == 1)
	{
		GameEnd();
	}

	/*  開始テキスト演出処理  */
	if (curStartTextViewTime > 0)
	{
		curStartTextViewTime -= 0.1f; //描画時間 減少処理
	}
	else
	{
		if (!titleTextViewFlag)
		{
			curStartTextViewTime = START_TEXT_VISIBLE_TIME; //描画時間 初期化
			titleTextViewFlag = true; //テキスト 可視化
		}
		else
		{
			curStartTextViewTime = START_TEXT_INVISIBLE_TIME; //描画時間 初期化
			titleTextViewFlag = false; //テキスト 不可視化
		}
	}

}


/* <<< キャラ選択処理 >>> */
void TitleSelectProc(void)
{
	//キャラ画像状態 "フィメール"に遷移
	if (preInputKeys[2] == 0 && curInputKeys[2] == 1 || 
		preInputKeys[3] == 0 && curInputKeys[3] == 1)
	{
		PlayerImageSelect(); //キャラ選択処理

		/*  選択状態 遷移処理  */
		if (selectParam == PLAYER_IMAGE_PARAM::CHARA_FEMALE)
		{
			//マーレ画像選択
			selectParam = PLAYER_IMAGE_PARAM::CHARA_MALE;
		}
		else if (selectParam == PLAYER_IMAGE_PARAM::CHARA_MALE)
		{
			//マーレ画像選択
			selectParam = PLAYER_IMAGE_PARAM::CHARA_FEMALE;
		}
	}


	//タイトル状態 初期状態遷移
	if (preInputKeys[1] == 0 && curInputKeys[1] == 1)
	{
		titleParam = TITLE_PARAM::START; //タイトル初期状態に遷移
	}
}


/* <<< タイトル描画処理 >>>> */
void SceneTitleDraw(void)
{
	/*  タイトル 初期状態時  */
	if (titleParam == TITLE_PARAM::START)
	{
		//タイトル画像 描画処理
		DrawGraph((SCREEN_SIZE_X / 2) - (TITLE_IMAGE_SIZE_X / 2),
				  (SCREEN_SIZE_Y / 2) - TITLE_IMAGE_SIZE_Y,
				  titleImage, true);

		/*  開始テキスト 可視化処理  */
		if (titleTextViewFlag == true)
		{
			//テキスト描画
			DrawFormatString((SCREEN_SIZE_X / 2 - START_TEXT_SIZE_X / 2),
				(SCREEN_SIZE_Y / 2 + START_TEXT_SIZE_Y),
				0xFFFFFF, "Enterキーでスタート");
		}
	}


	/*  タイトル キャラ選択状態時  */
	else if (titleParam == TITLE_PARAM::SELECT)
	{
		//キャラ選択画像 描画
		DrawGraph(0, 0, titleSelectImages[0], true);				   //マーレ画像
		DrawGraph((SCREEN_SIZE_X / 2), 0, titleSelectImages[1], true); //フィメール画像


		/*  選択演出 無効化処理  */
		if (selectPerformFlag == true)
		{
			if (curSelectPerformTime < 0)
			{
				curSelectPerformTime = SELECT_PERFORM_TIME; //描画時間 初期化
				selectPerformFlag	 = false;				//選択演出 無効化
			}
			else
			{
				curSelectPerformTime -= 0.1f; //描画時間 減少処理
			}


			/*  選択演出画像位置 遷移処理  */

			//マーレ選択画像位置に遷移
			if (selectParam == PLAYER_IMAGE_PARAM::CHARA_MALE)
			{
				//マーレ選択画像描画
				DrawGraph(0, 0, selecPerformImage, true);
			}

			/* フィメール選択画像位置に遷移 */
			else if (selectParam == PLAYER_IMAGE_PARAM::CHARA_FEMALE)
			{
				//フィメール選択画像描画
				DrawGraph((SCREEN_SIZE_X / 2), 0, selecPerformImage, true);
			}
		}

		/* 選択演出 不可視化処理  */
		else
		{
			if (curSelectPerformTime < 0)
			{
				curSelectPerformTime = SELECT_PERFORM_TIME; //描画時間 初期化
				selectPerformFlag	  = true;				//選択演出 有効化
			}
			else
			{
				curSelectPerformTime -= 0.1f; //描画時間 減少処理
			}
		}
	}
}


/* <<< タイトル解放処理 >>> */
void SceneTitleRelease(void)
{
	DeleteGraph(selecPerformImage);	   //選択演出画像 解放
	DeleteGraph(titleSelectImages[1]); //フィメール選択画像 解放
	DeleteGraph(titleSelectImages[0]); //マーレ選択画像 解放
	DeleteGraph(titleImage);		   //タイトル画像 解放

	//メモリから画像を解放(削除)する
}