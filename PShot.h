/* -------------------
　プレイヤー弾丸データ
 --------------------- */

 /* --- <<< 定数定義 >>> -- */

const int P_BULLET_SHOT_SPEED = 10;	     //プレイヤー弾丸 発射速度[Default:6]

const int P_BULLET_IMAGE_SIZE_X = 6;  //プレイヤー弾丸 画像横サイズ[Default:6]
const int P_BULLET_IMAGE_SIZE_Y = 30; //プレイヤー弾丸 画像縦サイズ[Default:30]


/* --- <<< 関数プロトタイプ宣言 >>> --- */

bool PShotInit(int posX, int posY);	 //プレイヤー弾丸 初期化処理
void PShotUpdate(void);  //プレイヤー弾丸 発射処理
void PShotDraw(void);	 //プレイヤー弾丸 描画処理
void PShotRelease(void); //プレイヤー弾丸 解放処理

bool PShotHit(void); //発射処理
void PShotActive(int posX, int posY); //プレイヤー弾丸 発射処理