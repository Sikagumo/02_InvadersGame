/* ------------
　敵弾丸データ
 -------------- */

/* --- << 定数定義 >>> --- */

const int E_BULLET_IMAGE_SIZE_X = 16; //敵弾丸画像 横サイズ
const int E_BULLET_IMAGE_SIZE_Y = 25; //敵弾丸画像 縦サイズ

const int E_BULLET_SHOT_SPEED = 1; //敵弾丸 発射速度

const float E_SHOT_RECAST_TIME_MIN = 300.0f; //敵弾丸 発射間隔 最小時間
const float E_SHOT_RECAST_TIME_MAX = 1000.0f; //敵弾丸 発射間隔 最大時間


/* --- <<< 関数プロトタイプ宣言 >>> --- */

bool EShotStartInit(void);							   //初期敵弾丸 初期化処理
void EShotUpdate(int posX, int posY, bool activeflag); //敵弾丸 更新処理
void EShotDraw(int posX, bool activeflag);			   //敵弾丸 描画処理
void EShotRelease(void);							   //敵弾丸 解放処理

//各敵弾丸 発射可能フラグ変更処理
bool StateBulletShotActiveFlag(int value, bool flag);