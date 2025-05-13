/* -------------------
	ゲームシーンデータ
 --------------------- */


 /* --- <<< 定数定義 >>> --- */

const int GAME_AREA_SIZE_X = (16 * 30); //ゲームエリア 横サイズ[最大値：画面横サイズ]
const int GAME_AREA_SIZE_Y = (16 * 29); //ゲームエリア 縦サイズ[最大値：画面縦サイズ]

const int GAME_AREA_MIN_SIZE_X = 16; ///ゲームエリア 横サイズ最小位置[最小値：0]
const int GAME_AREA_MIN_SIZE_Y = 16; ///ゲームエリア 縦サイズ最小位置[最小値：0]

const int ENEMY_VALUE_X = 2; //敵 横の数[Default:5]
const int ENEMY_VALUE_Y = 3; //敵 縦の数[Default:3]

const int STAGE_VALUE = 3; //ステージ数


/*  スコア系  */

const int ENEMY_DESTROY_SCORE = 50; //撃破ポイント

//全撃破ポイント
const int ENEMY_KNOCKDOWN_SCORE = (ENEMY_VALUE_X * ENEMY_VALUE_Y) * 1000;

const int DEFAULT_HIGH_SCORE = (ENEMY_KNOCKDOWN_SCORE / STAGE_VALUE); //初期ハイスコア


/* --- <<< 関数プロトタイプ宣言 >>> --- */

bool StartSceneGameInit(void); //初回初期化処理
bool SceneGameInitGame(void);  //初期化処理
bool SceneGameUpdate(void);	   //メイン処理
void SceneGameDraw(void);	   //描画処理
void SceneGameRelease(void);   //解放処理

void DrawParameter(void);	   //パラメータ表示処理
int ScoreUp(int value);		   //スコア増加処理

int CurStageValueUp(void);	   //現在ステージ数増加処理