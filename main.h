/* ---------------
　システム関係データ
 ----------------- */

/* --- <<< 列挙型定義 >>> --- */

//ゲームの状態
enum class GAME_PARAM
{
	TITLE,	 //タイトル処理
	GAME,	 //ゲーム処理
	GAMEOVER //ゲームオーバー処理
};


/* --- <<< 定数定義 >>> --- */

//画像サイズ
const int SCREEN_SIZE_X = 640; //縦[Default：640]
const int SCREEN_SIZE_Y = 480; //横[Default：480]

const int PIXEL_SIZE = 16; //1ピクセルサイズ

//設定フレームレート(ミリ秒変換)
constexpr float FRAME_RATE(1000 / 60);


/* --- <<< 関数プロトタイプ宣言 >>> --- */

bool StartInit(void); //初期化処理[初回]
void Update(void);	  //更新処理
void Draw(void);	  //描画処理	
void Release(void);	  //解放処理

void GameEnd(void);			//ゲーム終了処理
void FrameRateUpdate(void); //フレームレート 計算処理