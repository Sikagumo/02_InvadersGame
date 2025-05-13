/* --------
　敵データ
 ---------- */


/* --- <<< 列挙型定義 >>> --- */

//敵画像状態
enum class ENEMY_IMAGE_PARAM
{
	MIDDLE_IMAGE, //正面 画像
	LEFT_IMAGE,   // 左  画像
	RIGHT_IMAGE   // 右  画像
};

//敵縦移動状態
enum class ENEMY_PARAM_WIDTH
{
	E_MOVE_LEFT, //左移動 状態
	E_MOVE_RIGHT //右移動 状態
};

//敵上下移動状態
enum class ENEMY_PARAM_VERT
{
	E_MOVE_WIDTH, //左右移動 状態
	E_MOVE_DOWN	  //　下移動 状態
};

//敵画像の見た目
enum class ENEMY_TYPE
{
	BLACK,  //黒色
	BLUE,   //青色
	RED,    //赤色
	GREEN,  //緑色
	YELLOW, //黄色
	PURPLE, //紫色
	MAX	    //列挙型最大値
};


/* --- <<< 定数定義 >>> --- */

const int ENEMY_IMAGE_BASE_VALUE = 7; //敵画像 種類

const int ENEMY_MAX_IMAGE_SIZE_X = 96;  //敵画像 元画像横サイズ
const int ENEMY_MAX_IMAGE_SIZE_Y = 128; //敵画像 元画像縦サイズ
const int ENEMY_IMAGE_SIZE_X = 32;		//敵画像 各横サイズ
const int ENEMY_IMAGE_SIZE_Y = 32;		//敵画像 各縦サイズ
const int ENEMY_IMAGE_FRAME_VALUE = 50;  //敵画像 画像フレーム倍率


const int ENEMY_SPEED_UP_START_COUNT = 2; //敵 移動速度 上昇開始値[Default：1]

const int ENEMY_MOVE_SPEED_X	    = 1; //敵 移動速度
const int ENEMY_MOVE_SPEED_UP_VALUE = 2; //敵 移動速度増加 倍率[Default：2]

const int ENEMY_SPAN_X = (ENEMY_IMAGE_SIZE_X * 2); //敵横位置 間隔
const int ENEMY_SPAN_Y = (ENEMY_IMAGE_SIZE_Y + 8); //敵縦位置 間隔

const int ENEMY_IMAGE_COUNT_X = 3; //敵画像 横分割数
const int ENEMY_IMAGE_COUNT_Y = 4; //敵画像 縦分割数



/* --- <<< 関数プロトタイプ宣言 >>> --- */

bool EnemyStartInit(void); //敵 初期化処理
void EnemyInitGame(void);  //ゲーム中 初期化処理
void EnemyUpdate(void);    //敵 メイン処理
void EnemyDraw(void);	   //敵画像 描画処理
void EnemyRelease(void);   //敵画像 解放処理

void EnemyMove(void);		//敵移動処理
void EnemyCollision(int posX_, int posY_, bool activeflag_); //敵衝突処理
int GetEnemyUnder(void); //下の敵の座標を取得