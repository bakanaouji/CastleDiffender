//==========================================================
/// @file
/// @brief    Stageクラス
/// @author   阿部拳之
///
/// @attention  このファイルの利用は、同梱のREADMEにある
///             利用条件に従ってください

#ifndef _STAGE_H	// このファイルが複数の箇所でインクルードされる場合に、
#define _STAGE_H	// 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN

#include "enemy.h"
#include "midBoss.h"
#include "brave.h"
#include "castle.h"
#include "textureManager.h"

class Stage
{
private:
	Map map;									// ステージのマップ情報
	Barricade barricades[mapNS::BARRICADE_NUM];	// バリケードの配列
	Brave brave;								// プレイヤー
	Castle castle;								// 城
	Enemy **enemy;	    						// 敵へのポインタの配列
	float *enemyX;								// 敵の初期位置Xの配列
	float *enemyY;								// 敵の初期位置Yの配列
	float remainingTime;						// ゲーム内の残り時間
	int enemyNum;								// 敵の数
	int stageNum;								// 選択しているステージの番号
	int enemyWaveNum;							// 現在の敵の波の番号（第一波、第二波、、、）
	bool    roundOver;							// ラウンドが終了した場合、true
	bool clearedStage;							// ステージをクリアした場合、true
	bool initializedEnemies;					// 敵を初期化できたらtrue
public:
	// コンストラクタ
	Stage();
	// デストラクタ
	~Stage();

	//==========================================================
	// getter
	//==========================================================
	// マップ情報を返す
	Map& getMap() { return map; }
	// 敵の数を返す
	int getEnemyNum() { return enemyNum; }
	// ステージ番号を返す
	int getStageNum() { return stageNum; }
	// ゲーム内の残り時間を返す
	float getRemainingTime() { return remainingTime; }
	// 指定したインデックスの敵を返す
	Enemy& getEnemy(int index) { return *enemy[index]; }
	// 勇者を返す
	Brave& getBrave() { return brave; }
	// 指定したインデックスのバリケードを返す
	Barricade& getBarricade(int index) { return barricades[index]; }
	// 城を返す
	Castle& getCastle() { return castle; }
	// ゲームオーバーかどうかを返す
	bool getRoundOver() { return roundOver; }
	// ステージをクリアしたかどうかを返す
	bool getClearedStage() { return clearedStage; }
	// 敵を初期化できているかどうか
	bool getInitializedEnemies() { return initializedEnemies; }


	//==========================================================
	// setter
	//==========================================================
	// ステージをクリアしたかどうかをセット
	void setClearedStage(bool cs) { clearedStage = cs; }
	// ステージ番号をセット
	void setStageNum(int sn) { stageNum = sn; }
	// 敵を初期化できているかどうかをセット
	void setInitializedEnemies(bool initializedE) { initializedEnemies = initializedE; }

	// ステージ情報を初期化
	void reset();
	// 現在のステージ番号でゲームスタート
	void roundStart();
	// 人工知能
	void ai(float frameTime);
	// 衝突を処理
	void collisions();
	// stageを1ステップ分進める
	void runStage(float frameTime);
	// 敵の数をチェックし、マップ上に敵がいなければ新たに生成
	void checkCurrentEnemyNum();
	// 指定されたステージの敵データを読み込む
	bool readEnemyFile(int stageNum, int enemyWave);
	// 指定されたステージ、派の敵データを読み込み敵を初期化する
	void initializeEnemies(int stageNum, int enemyWave);
};
#endif