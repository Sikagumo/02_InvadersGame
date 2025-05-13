/* ----------------------
	ゲームオーバーデータ
 ------------------------ */


/* --- <<< 列挙型定義 >>> --- */

//ゲームオーバー演出状態
enum class GAMEOVER_PERFORM_PARAM
{
	NONE,//初期状態
	TITLE_TEXT,  //タイトルテキスト描画演出状態
	RETURN_TEXT, //"戻る"テキスト描画演出状態
	PROC_ACTIVE  //遷移処理可能状態
};

//"戻る"テキスト演出状態
enum class RETURN_TEXT_PARAM
{
	INACTIVE, //無効化状態
	VISIBLE,  // 描画 状態
	INVISIBLE //不透明状態
};


/* --- <<< 定数定義 >>> --- */

const int GAMEOVER_IMAGE_SIZE_X = 452; //ゲームオーバー画像 横サイズ
const int GAMEOVER_IMAGE_SIZE_Y = 123; //ゲームオーバー画像 縦サイズ

const int RETURN_TEXT_SIZE_X = 200; //"戻る"テキスト 横サイズ
const int RETURN_TEXT_SIZE_Y = 98;  //"戻る"テキスト 縦サイズ

const float RETURN_TEXT_VISIBLE_TIME   = 5.0; //"戻る"テキスト 可視化時間[Default:5.0f]
const float RETURN_TEXT_INVISIBLE_TIME = 2.0; //"戻る"テキスト 不可視化時間[Default:2.0f]

const float GAMEOVER_IMAGE_WAIT_TIME  = 5.0; //ゲームオーバータイトル 演出待機時間
const float RETURN_TEXT_WAIT_TIME	  = 0.0; //"戻る"テキスト 演出待機時間
const float PROCESS_ACTIVE_WAIT_TIME  = 2.5; //遷移処理 有効化待機時間


/* --- <<< 関数プロトタイプ宣言 >>> --- */

bool SceneGameOverStartInit(void); //初期化処理[初回]
bool SceneGameOverUpdate(void);	   //メイン処理
void SceneGameOverDraw(void);	   //描画処理
void SceneGameOverRelease(void);   //解放処理

void SceneGameOverInit(void);	//初期化処理[共有]
void GameOverPerformance(void); //演出処理

//演出待機処理
void PerformWaitTime(GAMEOVER_PERFORM_PARAM nextperform, float nexttime);