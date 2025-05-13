/* ------------------
　プレイヤーデータ
 -------------------- */

/* --- <<< 列挙型定義 >>> --- */

//プレイヤー画像の状態
enum class PLAYER_IMAGE_PARAM
{
	CHARA_FEMALE, //フィメール画像選択
	CHARA_MALE	  //　マーレ　画像選択
};

//プレイヤーの状態[P_MOVE_NONE:上向き状態, P_MOVE_LEFT:左向き状態, P_MOVE_RIGHT:右向き状態]
enum class PLAYER_MOVE_PARAM
{
	P_MOVE_NONE  = 9, //上向き状態
	P_MOVE_LEFT  = 3, //左向き状態
	P_MOVE_RIGHT = 6  //右向き状態
};


/* ----- <<< 定数定義 >>> ----- */

/* <<< 画像系 >>> */

const int PLAYER_MAX_IMAGE_SIZE_X = 96;  //プレイヤー画像 横サイズ[Default:96]
const int PLAYER_MAX_IMAGE_SIZE_Y = 128; //プレイヤー画像 縦サイズ[Default:128]

const int PLAYER_IMAGE_COUNT_X = 3; //プレイヤー画像 横分割数[Default:3]
const int PLAYER_IMAGE_COUNT_Y = 4; //プレイヤー画像 縦分割数[Default:4]

const int PLAYER_IMAGE_SIZE_X = 32; //プレイヤー画像 各横サイズ[Default:32]
const int PLAYER_IMAGE_SIZE_Y = 32; //プレイヤー画像 各横サイズ[Default:32]

const int PLAYER_IMAGE_VALUE = 12; //プレイヤー 画像数[Default:12]
const float PLAYER_IMAGE_MAX_STATE_TIME = 0.35f; //プレイヤー画像 状態遷移時間

const int PLAYER_IMAGE_DEFAULT_VALUE = 10; //開始時の画像番号状態
const int PLAYER_IMAGE_FRAME_VALUE	 = 10; //画像フレーム倍率


/* <<< パラメータ系 >>> */

const int PLAYER_LIFE = 3; //プレイヤー残機
const int PLAYER_MOVE_SPEED_X = 5; //左右移動速度[Default：1]


/* --- <<< 関数プロトタイプ宣言 >>> --- */

bool PlayerInit(void);	  //プレイヤー 初回初期化処理
void PlayerReStart(void); //プレイヤー変数 初期化処理
bool PlayerUpdate(void);  //プレイヤー メイン処理
void PlayerDraw(void);	  //プレイヤー 描画処理
void PlayerRelease(void); //プレイヤー画像 解放処理

/*  プレイヤー画像系関数  */
void PlayerImageState(void);		//プレイヤー画像 遷移処理
void PlayerImageSelect(void);		//プレイヤー画像 選択処理
bool PlayerImageChange(void);		//プレイヤー画像 変更処理
int GetPlayerImage(int imageValue); //プレイヤー画像 取得

bool GetPlayerActiveFlag(void);		 //プレイヤー有効化フラグ 取得
bool SetPlayerActiveFlag(bool flag); //プレイヤー有効化フラグ[有効化・無効化]処理
void PlayerCollision(int value, int enemyposX_, int enemyposY_, bool enemyflag_, int bulletposX_, int bulletposY_, bool bulletflag_);//プレイヤー当たり判定 処理

int GetPlayerLife(void);	   //プレイヤー残機 取得
int PlayerLifeDecrement(void); //プレイヤー残機 減少処理