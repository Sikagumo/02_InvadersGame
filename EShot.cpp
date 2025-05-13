/* ------------
	“G’eŠÛˆ—
 -------------- */

#include <DxLib.h>
#include "main.h"
#include "EShot.h"
#include "Enemy.h"
#include "Player.h"
#include "SceneGame.h"


/* --- <<< •Ï”’è‹` >>> --- */

int eBulletImage; //’eŠÛ‰æ‘œ;

float curShotMinRecastTime; //Œ»İ‚ÌÅ¬”­Ë‘Ò‚¿ŠÔ(“ïˆÕ“x•ÏX—p)

int eBulletsPosX[ENEMY_VALUE_X];			//Še“G’eŠÛ ‰¡ˆÊ’u
int eBulletsPosY[ENEMY_VALUE_X];			//Še“G’eŠÛ cˆÊ’u
float eShotCurRecastTimes[ENEMY_VALUE_X];	//Še“G’eŠÛ ”­ËŠÔŠuŠÔ

bool eBulletShotActiveFlag[ENEMY_VALUE_X]; //Še“G’eŠÛ ”­Ë‰Â”\ƒtƒ‰ƒO[trueFShot = Active, falseFInactive]

/*  Še“G’eŠÛ—LŒøE–³Œø‰»ˆ—  */
bool StateBulletShotActiveFlag(int value, bool flag)
{
	return eBulletShotActiveFlag[value] = flag;
}

/* --- <<< ˆ—Œn >>> --- */

/* <<< “G’eŠÛ‰Šú‰»ˆ—[‰‰ñ] >>> */
bool EShotStartInit(void)
{
	//“G’eŠÛ‰æ‘œ ƒOƒ‰ƒtƒBƒbƒN“o˜^
	eBulletImage = LoadGraph("image/eshot.png");
	if (eBulletImage == -1)
	{
		OutputDebugString("“G’eŠÛ‰æ‘œ‚ª“Ç‚İ‚Ü‚ê‚Ä‚¢‚Ü‚¹‚ñ\n");
		return false;
	}


	/*  Še’eŠÛ‰Šú‰»  */
	for (int x = 0; x < ENEMY_VALUE_X; x++)
	{
		/*  ’eŠÛˆÊ’u ‰Šú‰»  */
		eBulletsPosX[x] = ((GAME_AREA_SIZE_X / 2) + GAME_AREA_MIN_SIZE_X) - (E_BULLET_IMAGE_SIZE_X / 2);
		eBulletsPosY[x] = GAME_AREA_MIN_SIZE_Y;

		eBulletShotActiveFlag[x] = false; //’eŠÛ –³Œø‰»

		//”­Ë‘Ò‚¿ŠÔ ‰Šú‰»
		eShotCurRecastTimes[x] = static_cast<float>(GetRand(E_SHOT_RECAST_TIME_MAX) + E_SHOT_RECAST_TIME_MIN);
	}

	return true; //I—¹ˆ—
}


/* <<< “G’eŠÛ ”­Ëˆ— >>> */
void EShotUpdate(int posX, int posY, bool activeflag)
{
	/* <<< ’eŠÛˆ— >>> */
	
	for (int x = 0; x < ENEMY_VALUE_X; x++)
	{
		/*  “G—LŒø + ”­Ë‰Â”\ + ƒQ[ƒ€”ÍˆÍ “GÀİ  */
		if (activeflag == true &&
			eBulletShotActiveFlag[x] == false &&
			posY >= GAME_AREA_MIN_SIZE_Y)
		{
			//”­Ëˆ—
			if (eShotCurRecastTimes[x] > 0)
			{
				eShotCurRecastTimes[x] -= 1; //‘Ò‹@ŠÔ Œ¸­ˆ—
			}

			//”­Ë‘Ò‚¿ˆ—
			else
			{
				eBulletShotActiveFlag[x] = true; //’eŠÛ—LŒø‰»

				/* @”­ËˆÊ’uæ“¾ @*/ 
				eBulletsPosX[x] = posX + (E_BULLET_IMAGE_SIZE_X / 2);
				eBulletsPosY[x] = posY + (ENEMY_IMAGE_SIZE_Y / 2) + (ENEMY_IMAGE_SIZE_Y / 2);
			}

			if (GetRand(10 * 60) == 0)
			{
				eBulletShotActiveFlag[x] = true; //’eŠÛ—LŒø‰»

				/*  ”­ËˆÊ’uæ“¾  */
				eBulletsPosX[x] = posX + (E_BULLET_IMAGE_SIZE_X / 2);
				eBulletsPosY[x] = posY + (ENEMY_IMAGE_SIZE_Y / 2) + (ENEMY_IMAGE_SIZE_Y / 2);
			}
		}

		//’eŠÛËoˆ—
		if (eBulletShotActiveFlag[x] == true)
		{
			//‰æ–ÊŠO‚ğ’´‚¦‚é‚Ü‚ÅŒJ‚è•Ô‚·
			if (eBulletsPosY[x] < (GAME_AREA_SIZE_Y + GAME_AREA_MIN_SIZE_Y) - E_BULLET_IMAGE_SIZE_Y)
			{
				eBulletsPosY[x] += E_BULLET_SHOT_SPEED; //’eŠÛˆÚ“®ˆ—
			}
			else
			{
				eBulletShotActiveFlag[x] = false; //’eŠÛ”­Ë‰Â”\‰»

				//‘Ò‹@ŠÔ‰Šú‰»
				eShotCurRecastTimes[x]   = static_cast<float>(GetRand(E_SHOT_RECAST_TIME_MAX) + E_SHOT_RECAST_TIME_MIN);
			}
		}

		//“G‚Ì“–‚½‚è”»’è
		PlayerCollision(x, posX, posY, activeflag, eBulletsPosX[x], eBulletsPosY[x], eBulletShotActiveFlag[x]);
	}
}


/* --- <<< “G’eŠÛ •`‰æˆ— >>> --- */
void EShotDraw(int posX, bool activeflag)
{
	/* <<< ’eŠÛŒn >>> */
	if (eBulletShotActiveFlag[posX] == true &&
		activeflag == true)
	{
		//’eŠÛ•`‰æˆ—
		DrawGraph(eBulletsPosX[posX], eBulletsPosY[posX], eBulletImage, true);
	}
	
}

/* --- <<< “G’eŠÛ ‰ğ•úˆ— >>> --- */
void EShotRelease(void)
{
	DeleteGraph(eBulletImage); //ƒƒ‚ƒŠ‚©‚ç“G’eŠÛ‰æ‘œ íœ
}