//==========================================================
/// @file
/// @brief    Mapクラス
/// @author   阿部拳之
///
/// @attention  このファイルの利用は、同梱のREADMEにある
///             利用条件に従ってください

#ifndef _MAP_H		// このファイルが複数の箇所でインクルードされる場合に、
#define _MAP_H		// 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"
#include <iostream>
#include <string>
#include <sstream> //文字ストリーム
#include "gameError.h"

using namespace std;

// Mapクラスの定数
// ゲーム内でのステータス以外をここに記述
namespace mapNS
{
	const int TEXTURE_SIZE = 32;  // 画像サイズ（1タイル分）
	const int TEXTURE_COLS = 16;  // 行数
	const int MAP_HEIGHT = 23;    // マップの高さ
	const int MAP_WIDTH = 40;     // マップの幅
	const int BARRICADE_NUM = 8;  // 初期バリケードの数
}

class Map : public Entity
{
private:
	// マップ
	int mMapData[mapNS::MAP_HEIGHT][mapNS::MAP_WIDTH];
	// 当たり判定のデータ、ゲームを通して移動できない箇所は1か2に設定する、岩は1に、城は2にする
	int mColData[mapNS::MAP_HEIGHT][mapNS::MAP_WIDTH];
	int mColDataInit[mapNS::MAP_HEIGHT][mapNS::MAP_WIDTH];
	// マップ上のオブジェクトデータ、0 -> バリケード
	int mObjData[mapNS::MAP_HEIGHT][mapNS::MAP_WIDTH];
public:
	// コンストラクタ
	Map();

	// 継承されたメンバ関数
	virtual void draw();
	virtual void update(float frameTime);

	// 新しく追加するメンバ関数
	// マップデータの取得　引数は取得したい座標のインデックス
	int getMapData(int x, int y)
	{
		return mMapData[x][y];
	}

	// マップ当たり判定の更新 引き数は取得したい座標のインデックス
	int getMapCol(int x, int y)
	{
		return mColData[x][y];
	}

	// マップ上のオブジェクトデータ更新　引数は取得したい座標のインデックス
	int getMapObj(int x, int y)
	{
		return mObjData[x][y];
	}


	// マップデータの更新　引数は更新したい座標値と更新する値
	void updateMapData(float x, float y, int val);
	// マップ当たり判定の更新 引数は更新したい座標値と更新する値
	// val -> 0 : 判定なし　val -> 1 : 判定あり
	void updateMapCol(float x, float y, int val);
	// マップ上のオブジェクトデータ更新　引数は更新したい座標値と更新する値
	void updateMapObj(float x, float y, int val);
	// マップ上のオブジェクトデータ更新　引数は更新したい座標値と更新する値
	void updateMapObjInt(int y, int x, int val);
	// マップ上の当たり判定データすべてをリセットする関数
	void resetMapCol() {
		for (int i = 0; i < mapNS::MAP_HEIGHT; ++i)
		{
			for (int j = 0; j < mapNS::MAP_WIDTH; ++j)
			{
				mColData[i][j] = mColDataInit[i][j];
			}
		}
	}
	// 指定されたマップの当たり判定データをリセットする
	void resetMapCol(int y, int x) {
		if (x < 0 || x >= mapNS::MAP_WIDTH || y < 0 || y >= mapNS::MAP_HEIGHT) { return; }
		mColData[y][x] = mColDataInit[y][x];
	}

	// 指定されたステージのマップデータを読み込む
	void readMapFile(int stageNum);
};

#endif