#include "enemy.h"

//==========================================================
// デフォルトコンストラクタ
//==========================================================
Enemy::Enemy() : Entity()
{
	spriteData.width = enemyNS::WIDTH;			// 雑魚敵のサイズ
	spriteData.height = enemyNS::HEIGHT;
	spriteData.x = enemyNS::X;					// 画面上の位置
	spriteData.y = enemyNS::Y;
	spriteData.rect.bottom = enemyNS::HEIGHT;	// 画面の一部を選択
	spriteData.rect.right = enemyNS::WIDTH;
	frameDelay = enemyNS::MOVE_ANIMATION_DELAY;
	// 初期の方向は右
	direction = enemyNS::RIGHT;
	oldDirection = direction;
	startFrame = enemyNS::MOVE_RIGHT_START_FRAME;
	endFrame = enemyNS::MOVE_RIGHT_END_FRAME;
	oldStartFrame = startFrame;
	oldEndFrame = endFrame;
	currentFrame = startFrame;
	// Boxの衝突判定用
	edge.left = -enemyNS::WIDTH / 2.0;
	edge.right = enemyNS::WIDTH / 2.0;
	edge.top = -enemyNS::HEIGHT / 8.0;
	edge.bottom = enemyNS::HEIGHT / 2.0;
	collisionType = entityNS::BOX;
	// 初期の状態は城に向かって移動
	state = enemyNS::MOVE_CASTLE;
	goalPos.x = castleNS::X + castleNS::WIDTH / 2;
	goalPos.y = castleNS::Y + castleNS::HEIGHT / 2;
	// ダメージは受けていない状態からスタート
	isDamaged = false;
	// タイマーをリセット
	damageTimer = 0.0f;
	totalDamageTime = 0.0f;
	// 距離計測用カウンターをリセット
	distanceCounter = 0.0f;
	// 描画フラグはオン
	drawFlag = true;
	// 攻撃判定のコリジョンは無効状態からスタート
	attackCollisionFlag = false;
	// 範囲内にはいない状態からスタート
	inCertainRange = false;
}

//==========================================================
// パラメータ初期化
//==========================================================
void Enemy::reset()
{
	active = true;
	visible = true;
	health = 100;

	spriteData.x = enemyNS::X;					// 画面上の位置
	spriteData.y = enemyNS::Y;
	frameDelay = enemyNS::MOVE_ANIMATION_DELAY;
	// 初期の方向は右
	direction = enemyNS::RIGHT;
	oldDirection = direction;
	startFrame = enemyNS::MOVE_RIGHT_START_FRAME;
	endFrame = enemyNS::MOVE_RIGHT_END_FRAME;
	oldStartFrame = startFrame;
	oldEndFrame = endFrame;
	currentFrame = startFrame;
	// 初期の状態は移動
	state = enemyNS::MOVE_CASTLE;
	goalPos.x = castleNS::X + castleNS::WIDTH / 2;
	goalPos.y = castleNS::Y + castleNS::HEIGHT / 2;
	// ダメージは受けていない状態からスタート
	isDamaged = false;
	// タイマーをリセット
	damageTimer = 0.0f;
	totalDamageTime = 0.0f;
	// 距離計測用カウンターをリセット
	distanceCounter = 0.0f;
	// 描画フラグはオン
	drawFlag = true;
	// 攻撃判定のコリジョンは無効状態からスタート
	attackCollisionFlag = false;
	// 範囲内にはいない状態からスタート
	inCertainRange = false;
}

//==========================================================
// 雑魚敵を初期化
// 実行後：成功した場合はtrue、失敗した場合はfalseを戻す
//==========================================================
bool Enemy::initialize(Game *gamePtr, int width, int height, int ncols,
	TextureManager *textureM)
{
	return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//==========================================================
// 雑魚敵を描画
//==========================================================
void Enemy::draw()
{
	// 描画フラグがオンのときのみ描画
	if (drawFlag)
		Image::draw();	// 雑魚敵を描画
}

//=============================================================================
// Update
// 通常、フレームごとに1回呼び出す
// frameTimeは、移動とアニメーションの速さを制御するために使用
//=============================================================================
void Enemy::update(float frameTime, Map *map)
{
	// 非アクティブなら何もしない
	if (!active)
		return;
	// 攻撃中のフラグをオフ
	attackCollisionFlag = false;
	// 状態ごとに処理を分岐
	switch (state)
	{
	case enemyNS::MOVE_CASTLE:		// 城への移動
	case enemyNS::MOVE_BARRICADE:	// バリケードへの移動
		// 目標への向きに応じて自分の向きを修正
		// 向きを変更する場合はアニメーションをリセット
		switch (goalDirection)
		{
		case enemyNS::LEFT:
			if (direction != goalDirection)
			{
				changeDirection(enemyNS::MOVE_LEFT_START_FRAME, enemyNS::MOVE_LEFT_END_FRAME);
			}
			// 移動可能だったら
			if (checkCanMove(spriteData.x - enemyNS::MOVE_SPEED * frameTime, spriteData.y, map)) {
				// 左に移動
				spriteData.x -= enemyNS::MOVE_SPEED * frameTime;
			}
			distanceCounter -= enemyNS::MOVE_SPEED * frameTime;
			break;
		case enemyNS::RIGHT:
			if (direction != goalDirection)
			{
				changeDirection(enemyNS::MOVE_RIGHT_START_FRAME, enemyNS::MOVE_RIGHT_END_FRAME);
			}
			// 移動可能だったら
			if (checkCanMove(spriteData.x + enemyNS::MOVE_SPEED * frameTime, spriteData.y, map)) {
				// 右に移動
				spriteData.x += enemyNS::MOVE_SPEED * frameTime;
			}
			distanceCounter -= enemyNS::MOVE_SPEED * frameTime;
			break;
		case enemyNS::UP:
			if (direction != goalDirection)
			{
				changeDirection(enemyNS::MOVE_UP_START_FRAME, enemyNS::MOVE_UP_END_FRAME);
			}
			// 移動可能だったら
			if (checkCanMove(spriteData.x, spriteData.y - enemyNS::MOVE_SPEED * frameTime, map)) {
				// 上に移動
				spriteData.y -= enemyNS::MOVE_SPEED * frameTime;
			}
			distanceCounter -= enemyNS::MOVE_SPEED * frameTime;
			break;
		case enemyNS::DOWN:
			if (direction != goalDirection)
			{
				changeDirection(enemyNS::MOVE_DOWN_START_FRAME, enemyNS::MOVE_DOWN_END_FRAME);
			}
			// 移動可能だったら
			if (checkCanMove(spriteData.x, spriteData.y + enemyNS::MOVE_SPEED * frameTime, map)) {
				// 下に移動
				spriteData.y += enemyNS::MOVE_SPEED * frameTime;
			}
			distanceCounter -= enemyNS::MOVE_SPEED * frameTime;
			break;
		case enemyNS::NONE:
			break;
		}
		// 攻撃直後ではないはずなのでフラグをオフ
		isAttacked = false;
		// 画像のみアップデート
		Entity::updateOnlyImage(frameTime);
		break;
	case enemyNS::ATTACK_BRAVE:
	case enemyNS::ATTACK_CASTLE:
	case enemyNS::ATTACK_BARRICADE:
		// アニメーションが終了していたら攻撃終了
		// 状態を移動時に戻す
		if (animComplete)
		{
			isAttacked = true;
			mode = imageNS::HORIZONTAL;
			direction = oldDirection;
			state = enemyNS::MOVE_CASTLE;
			loop = true;
			startFrame = oldStartFrame;
			endFrame = oldEndFrame;
			currentFrame = startFrame;
			animTimer = 0.0f;
			animComplete = false;
			setRect();
			Entity::updateOnlyImage(frameTime);
		}
		break;
	case enemyNS::PRE_ATTACK:
		// タイマーを加算、一定時間を超えたら攻撃に移る
		attackTimer += frameTime;
		if (isAttacked && attackTimer > enemyNS::CONTINUE_ATTACK_TIME)
		{
			attackTimer = 0.0f;
			state = attackState;
			attackCollisionFlag = true;
		}
		else if (!isAttacked && attackTimer > enemyNS::ATTACK_TIME)
		{
			attackTimer = 0.0f;
			state = attackState;
			attackCollisionFlag = true;
		}
		break;
	case enemyNS::GAURD:
		break;
	default:
		break;
	}

	/*
	// 状態遷移前の処理
	switch (state)
	{
	case enemyNS::MOVE:		// 移動時
		// 目標への向きに応じて自分の向きを修正
		// 向きを変更する場合はアニメーションをリセット
		switch (goalDirection)
		{
		case enemyNS::LEFT:
			if (direction != goalDirection)
			{
				changeDirection(enemyNS::MOVE_LEFT_START_FRAME, enemyNS::MOVE_LEFT_END_FRAME);
			}
			// 移動可能だったら
			if (checkCanMove(spriteData.x - enemyNS::MOVE_SPEED * frameTime, spriteData.y, map)) {
				// 左に移動
				spriteData.x -= enemyNS::MOVE_SPEED * frameTime;
			}
			distanceCounter -= enemyNS::MOVE_SPEED * frameTime;
			break;
		case enemyNS::RIGHT:
			if (direction != goalDirection)
			{
				changeDirection(enemyNS::MOVE_RIGHT_START_FRAME, enemyNS::MOVE_RIGHT_END_FRAME);
			}
			// 移動可能だったら
			if (checkCanMove(spriteData.x + enemyNS::MOVE_SPEED * frameTime, spriteData.y, map)) {
				// 右に移動
				spriteData.x += enemyNS::MOVE_SPEED * frameTime;
			}
			distanceCounter -= enemyNS::MOVE_SPEED * frameTime;
			break;
		case enemyNS::UP:
			if (direction != goalDirection)
			{
				changeDirection(enemyNS::MOVE_UP_START_FRAME, enemyNS::MOVE_UP_END_FRAME);
			}
			// 移動可能だったら
			if (checkCanMove(spriteData.x, spriteData.y - enemyNS::MOVE_SPEED * frameTime, map)) {
				// 上に移動
				spriteData.y -= enemyNS::MOVE_SPEED * frameTime;
			}
			distanceCounter -= enemyNS::MOVE_SPEED * frameTime;
			break;
		case enemyNS::DOWN:
			if (direction != goalDirection)
			{
				changeDirection(enemyNS::MOVE_DOWN_START_FRAME, enemyNS::MOVE_DOWN_END_FRAME);
			}
			// 移動可能だったら
			if (checkCanMove(spriteData.x, spriteData.y + enemyNS::MOVE_SPEED * frameTime, map)) {
				// 下に移動
				spriteData.y += enemyNS::MOVE_SPEED * frameTime;
			}
			distanceCounter -= enemyNS::MOVE_SPEED * frameTime;
			break;
		case enemyNS::NONE:
			break;
		}
		// 攻撃直後ではないはずなのでフラグをオフ
		isAttacked = false;
		// 画像のみアップデート
		Entity::updateOnlyImage(frameTime);
		break;
	case enemyNS::ATTACK:	// 攻撃時
		// アニメーションが終了していたら攻撃終了
		// 状態を移動時に戻す
		if (animComplete)
		{
			isAttacked = true;
			mode = imageNS::HORIZONTAL;
			direction = oldDirection;
			state = enemyNS::ATTACK;
			loop = true;
			startFrame = oldStartFrame;
			endFrame = oldEndFrame;
			currentFrame = startFrame;
			animTimer = 0.0f;
			animComplete = false;
			setRect();
			Entity::updateOnlyImage(frameTime);
		}
		break;
	case enemyNS::PRE_ATTACK:
		// タイマーを加算、一定時間を超えたら攻撃に移る
		attackTimer += frameTime;
		if (isAttacked && attackTimer > enemyNS::CONTINUE_ATTACK_TIME)
		{
			attackTimer = 0.0f;
			state = enemyNS::ATTACK;
			attackCollisionFlag = true;
		}
		else if (!isAttacked && attackTimer > enemyNS::ATTACK_TIME)
		{
			attackTimer = 0.0f;
			state = enemyNS::ATTACK;
			attackCollisionFlag = true;
		}
		break;
	case enemyNS::GAURD:
		break;
	default:
		break;
	}
	*/

	// 状態遷移後の処理
	switch (state)
	{
	case enemyNS::MOVE_CASTLE:
	case enemyNS::MOVE_BARRICADE:
		updateMoving(frameTime);
		break;
	case enemyNS::ATTACK_BRAVE:
	case enemyNS::ATTACK_CASTLE:
	case enemyNS::ATTACK_BARRICADE:
		updateAttacking(frameTime);
		break;
	case enemyNS::PRE_ATTACK:
		Entity::updateWithoutImage(frameTime);
		break;
	case enemyNS::GAURD:
		Entity::update(frameTime);
		break;
	default:
		Entity::update(frameTime);
		break;
	}

	// ダメージを受けているなら一定時間ごとにアニメーションを点滅
	if (isDamaged)
	{
		damageTimer += frameTime;
		totalDamageTime += frameTime;
		if (damageTimer > 0.15f)
		{
			if (drawFlag)
				drawFlag = false;
			else
				drawFlag = true;
			damageTimer = 0.0f;
		}
		if (totalDamageTime > enemyNS::DAMAGE_TIME)
		{
			damageTimer = 0.0f;
			totalDamageTime = 0.0f;
			drawFlag = true;
			isDamaged = false;
		}
	}
	// 移動可能だったら
	if (checkCanMove(spriteData.x + frameTime * velocity.x, spriteData.y + frameTime * velocity.y, map))
	{
		spriteData.x += frameTime * velocity.x;     // キャラをX方向に動かす
		spriteData.y += frameTime * velocity.y;     // キャラをY方向に動かす
	}
	// 画面の端まで来たら進めない
	if (spriteData.x > GAME_WIDTH - enemyNS::WIDTH * getScale())	// 画面右端を超えたら
		spriteData.x = GAME_WIDTH - enemyNS::WIDTH * getScale();	// 画面右端に移動
	if (spriteData.x < 0)											// 画面左端を超えたら
		spriteData.x = 0;											// 画面左端に移動
	if (spriteData.y < rectNS::HEIGHT - 10)								// 画面上端を超えたら
		spriteData.y = rectNS::HEIGHT - 10;								// 画面上端に移動
	if (spriteData.y > GAME_HEIGHT - enemyNS::HEIGHT * getScale())  // 画面下端を超えたら
		spriteData.y = GAME_HEIGHT - enemyNS::HEIGHT * getScale();	// 画面下端に移動

}

//==========================================================
// 移動時のアップデート関数
//==========================================================
void Enemy::updateMoving(float frameTime)
{
	// アニメーションのアップデートは単独で行われるのでそれ以外をアップデート
	Entity::updateWithoutImage(frameTime);
}

//==========================================================
// 攻撃時のアップデート関数
//==========================================================
void Enemy::updateAttacking(float frameTime)
{
	Entity::update(frameTime);
}

//==========================================================
// ダメージ
//==========================================================
void Enemy::damage(WEAPON weapon)
{
	switch (weapon)
	{
	case FIRE:
		health -= braveNS::FIRE_DAMAGE;
		break;
	case BRAVE_ATTACK:
		health -= braveNS::ATTACK_DAMAGE;
		break;
	case BRAVE_SECOND_ATTACK:
		break;
	case ENEMY_ATTACK:
		break;
	default:
		break;
	}
	if (health <= 0)
		dead();
	isDamaged = true;
}

//==========================================================
// 死亡時に呼び出される関数
//==========================================================
void Enemy::dead()
{
	active = false;
	visible = false;
	health = 0;
}

//==========================================================
// 人工知能
//==========================================================
void Enemy::ai(float frameTime, Entity &ent, Barricade *barricades)
{
	// 攻撃中ならば行動選択は行わない
	if (state == enemyNS::ATTACK_BARRICADE || state == enemyNS::ATTACK_BRAVE || state == enemyNS::ATTACK_CASTLE || state == enemyNS::PRE_ATTACK)
	{
		return;
	}

	/*
	// 近くにプレイヤーがいる場合、
	if (nearPlayer || nearBarricade)
	{
		changeAttack();
	}
	*/

	/*
	// それ以外は移動モードに切り替え
	state = enemyNS::MOVE;
	*/
	int subX, subY;
	/*
	// プレイヤーとの位置が一定範囲内ならばtrue、それ以外はfalse
	inCertainRange = checkDistancePlayer(ent.getX(), ent.getY());

	if (inCertainRange) // 範囲内にいたらプレイヤーを目指す
	{
		subX = ent.getX() - getX();
		subY = ent.getY() - getY();
	}
	else  // 範囲内でなければ一番近くのバリケードの位置を目指す
	{
		VECTOR2 nearBarricade = searchNearBarricade(ent, barricades);
		subX = nearBarricade.x - getX();
		subY = nearBarricade.y - getY();
	}*/
	if (state == enemyNS::MOVE_CASTLE || state == enemyNS::MOVE_BARRICADE)
	{
		subX = goalPos.x - getX();
		subY = goalPos.y - getY();
	}

	// 同じ方向に32ピクセル移動するごとに方向を決めなおす
	if (distanceCounter <= 0.0f)
	{
		distanceCounter = 32.0f;
		if (abs(subX) > abs(subY))
		{
			if (subX > 0)
				goalDirection = enemyNS::RIGHT;
			else
				goalDirection = enemyNS::LEFT;
			return;
		}
		else
		{
			if (subY > 0)
				goalDirection = enemyNS::DOWN;
			else
				goalDirection = enemyNS::UP;
			return;
		}
	}
}

//==========================================================
// 
//==========================================================
void Enemy::changeDirection(int strF, int endF)
{
	direction = goalDirection;
	startFrame = strF;
	endFrame = endF;
	currentFrame = startFrame;
	animTimer = 0.0f;
	setRect();
}

//==========================================================
// 移動可能かチェック
//==========================================================
bool Enemy::checkCanMove(float x, float y, Map *map)
{
	// 1マス32pixelのため32で割る
	// -16はめり込みを防止するために半マス分引いてる
	// +αは微調整…
	int map_x = (int)((x - 8) / 32) + 1;
	int map_y = (int)((y - 8) / 32) + 2;

	if (map_x <= 0)
		map_x = 0;
	if (map_x >= mapNS::MAP_WIDTH)
		map_x = mapNS::MAP_WIDTH - 1;
	if (map_y <= 0)
		map_y = 0;
	if (map_y >= mapNS::MAP_HEIGHT)
		map_y = mapNS::MAP_HEIGHT - 1;

	if (map->getMapCol(map_y, map_x) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}


//==========================================================
// プレイヤーとの距離が一定範囲内ならtrue、それ以外はfalseを返す
//==========================================================
bool Enemy::checkDistancePlayer(int px, int py)
{
	int subX = px - getX();
	int subY = py - getY();

	int dist = (int)sqrt(subX * subX + subY * subY);

	if (dist < 100) return true;
	else return false;
}

//==========================================================
// 一番近くのバリケードの位置を返す関数
// バリケードが存在しない場合はプレイヤーの位置を返す
//==========================================================
VECTOR2 Enemy::searchNearBarricade(Entity &ent, Barricade *barricade)
{

	// 最小の距離差
	int minX = 0, minY = 0;
	// アクティブなバリケードの数を数える用
	int count = 0;

	for (int i = 0; i < mapNS::BARRICADE_NUM; i++)
	{
		// もしバリケードnがアクティブなら
		if (barricade[i].getActive())
		{
			// 探索1つ目の場合
			if (count == 0)
			{
				minX = barricade[i].getX();
				minY = barricade[i].getY();
			}
			else // 2つ目以降
			{
				// もし最小値を更新したら
				if (barricade[i].getX() * barricade[i].getX() + barricade[i].getY() * barricade[i].getY() <
					minX * minX + minY * minY)
				{
					// 新たに代入
					minX = barricade[i].getX();
					minY = barricade[i].getY();
				}
			}
			// カウントを増やす
			count++;
		}
	}

	// ベクター2クラス生成
	VECTOR2 vec;

	// もしバリケードが1つ以上存在したら
	if (count > 0) {
		// 最小値を代入
		vec.x = minX;
		vec.y = minY;
	}
	else {
		// プレイヤーの位置を代入
		vec.x = ent.getX();
		vec.y = ent.getY();
	}

	// ベクトル返す
	return vec;
}

//==========================================================
// 一番近くのバリケードのインデックスを返す関数
//==========================================================
int Enemy::searchNearBarricadeIndex(Barricade *barricade) 
{

	// 最小の距離差
	int minX = 0, minY = 0;
	// 最小の距離のインデックス
	int minIndex = 0;
	// アクティブなバリケードの数を数える用
	int count = 0;

	for (int i = 0; i < mapNS::BARRICADE_NUM; i++)
	{
		// もしバリケードnがアクティブなら
		if (barricade[i].getActive())
		{
			// 探索1つ目の場合
			if (count == 0)
			{
				minX = barricade[i].getX();
				minY = barricade[i].getY();
				minIndex = i;
			}
			else // 2つ目以降
			{
				// もし最小値を更新したら
				if (barricade[i].getX() * barricade[i].getX() + barricade[i].getY() * barricade[i].getY() <
					minX * minX + minY * minY)
				{
					// 新たに代入
					minX = barricade[i].getX();
					minY = barricade[i].getY();
					minIndex = i;
				}
			}
			// カウントを増やす
			count++;
		}
	}
	return minIndex;
}

//==========================================================
// 雑魚敵からある点までの直線上にバリケードが存在したらtrue、存在しない場合はfalseを返す。
//==========================================================
bool Enemy::checkBarricadeOnLine(float x, float y, Map *map) {
	float a = (spriteData.y - y) / (spriteData.x - x);
	float b = (spriteData.x * y - x * spriteData.y) / (spriteData.x - x);
	if (spriteData.x <= x)
	{
		for (int i = spriteData.x; i <= x; ++i)
		{
			if (map->getMapObj(i, a * i + b) == 0)
				return true;
			if (i == x)
				return false;
		}
	}
}

//==========================================================
// 攻撃モードにチェンジするときに呼び出す関数
//==========================================================
void Enemy::changeAttack()
{
	// 攻撃直前モードにチェンジ
	loop = false;
	distanceCounter = 0.0f;
	goalDirection = enemyNS::NONE;
	state = enemyNS::PRE_ATTACK;
	mode = imageNS::VERTICAL;
	// アニメーション終了時にフレームを戻すために保存
	oldStartFrame = startFrame;
	oldEndFrame = endFrame;
	oldDirection = direction;
	// 方向に応じてアニメーションを切り替え
	switch (direction)
	{
	case enemyNS::LEFT:
		startFrame = enemyNS::ATTACK_LEFT_START_FRAME;
		endFrame = enemyNS::ATTACK_LEFT_END_FRAME;
		break;
	case enemyNS::RIGHT:
		startFrame = enemyNS::ATTACK_RIGHT_START_FRAME;
		endFrame = enemyNS::ATTACK_RIGHT_END_FRAME;
		break;
	case enemyNS::UP:
		startFrame = enemyNS::ATTACK_UP_START_FRAME;
		endFrame = enemyNS::ATTACK_UP_END_FRAME;
		break;
	case enemyNS::DOWN:
		startFrame = enemyNS::ATTACK_DOWN_START_FRAME;
		endFrame = enemyNS::ATTACK_DOWN_END_FRAME;
		break;
	default:
		break;
	}
	currentFrame = startFrame;
	animTimer = 0.0f;
	setRect();
	return;
}