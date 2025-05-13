/* -----------------------
	タイトルシーンデータ
 ------------------------- */

/* --- <<< 列挙型定義 >>> --- */

//タイトル状態
enum class TITLE_PARAM
{
	START, //初期状態
	SELECT //キャラ選択状態
};


/* --- <<< 定数定義 >>> --- */

const int TITLE_IMAGE_SIZE_X = 423; //タイトル画像 横サイズ[Default:423]
const int TITLE_IMAGE_SIZE_Y = 198; //タイトル画像 縦サイズ[Default:198]

const int START_TEXT_SIZE_X = 175; //開始テキスト 横サイズ[Default:175]
const int START_TEXT_SIZE_Y = 100; //開始テキスト 縦サイズ[Default:100]

const float START_TEXT_VISIBLE_TIME   = 5.0f; //開始テキスト 可視化時間[Default:5.0f]
const float START_TEXT_INVISIBLE_TIME = 2.0f; //開始テキスト 不可視化時間[Default:2.0f]

const int SELECT_IMAGE_SIZE_X = 320; //キャラ選択画像 横サイズ[Default:320]
const int SELECT_IMAGE_SIZE_Y = 480; //キャラ選択画像 縦サイズ[Default:480]

const float SELECT_PERFORM_TIME = 1.5f; //選択演出 描画時間


/* --- <<< 関数プロトタイプ宣言 >>> --- */

bool SceneTitleInit(void);	  //タイトル 初期化処理
bool SceneTitleUpdate(void);  //タイトル 処理
void SceneTitleDraw(void);	  //タイトル 描画処理
void SceneTitleRelease(void); //タイトル 画像解放処理

void TitleStartProc(void);	//タイトル 初期状態処理
void TitleSelectProc(void); //タイトル キャラ選択処理

/*  入力キー系  */
void InputKeyCheckInit(int num, int inputKey);  //入力キー初期化処理
void InputKeyCheckUpdate(int num, int inputKey); //入力キー更新処理