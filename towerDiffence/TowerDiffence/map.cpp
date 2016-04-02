#include "map.h"

//==========================================================
// デフォルトコンストラクタ
//==========================================================
Map::Map() : Entity()
{
	readMapFile(1);
};

////==========================================================
// マップを描画
//==========================================================
void Map::draw()
{
	Image::draw();
}

////========================================================
// 1フレーム毎に呼び出し
//==========================================================
void Map::update(float frameTime)
{

}

//==========================================================
// マップデータの更新　引き数は更新したい座標値と更新する値
//==========================================================
void Map::updateMapData(float x, float y, int val)
{
	// 座標をインデックスに変更
	int px = (int)(x / 32);
	int py = (int)(y / 32);

	// 座標値を更新
	tileMap[px][py] = val;
}

//==========================================================
// マップ当たり判定の更新 引き数は更新したい座標値と更新する値
// val -> 0 : 判定なし　val -> 1 : 判定あり/
//==========================================================
void Map::updateMapCol(float x, float y, int val)
{
	// 座標値をインデックスに変更
	int px = (int)(x / 32);
	int py = (int)(y / 32);

	// 座標値を更新
	tileCol[px][py] = val;
}

//==========================================================
// マップ上のオブジェクトデータ更新　引き数は更新したい座標値と更新する値
//==========================================================
void Map::updateMapObj(float x, float y, int val)
{
	// 座標値をインデックスに変更
	int px = (int)(x / 32);
	int py = (int)(y / 32);

	// 座標値を更新
	tileObj[px][py] = val;
}

//==========================================================
// マップ上のオブジェクトデータ更新　引き数は更新したい座標値と更新する値
//==========================================================
void Map::updateMapObjInt(int y, int x, int val)
{
	// 座標値を更新
	tileObj[y][x] = val;
}

//==========================================================
// 指定されたステージのマップデータを読み込む
//==========================================================
void Map::readMapFile(int stageNum)
{
	string mapDataFilename = "stageData\\stage" + std::to_string(stageNum) + "_mapdata.csv";
	string colDataFilename = "stageData\\stage" + std::to_string(stageNum) + "_coldata.csv";
	string objDataFilename = "stageData\\stage" + std::to_string(stageNum) + "_objdata.csv";

	ifstream* ifs = new ifstream(mapDataFilename);
	if (!ifs)
		exit(1);
	//csvファイルを1行ずつ読み込む
	string str;
	int y = 0, x = 0;
	while (getline(*ifs, str)) {
		string token;
		istringstream stream(str);
		x = 0;
		//1行のうち、文字列とコンマを分割する
		while (getline(stream, token, ',')) {
			//すべて文字列として読み込まれるため
			//数値は変換が必要
			int temp = stof(token); //stof(string str) : stringをfloatに変換
			tileMap[y][x] = temp;
			x += 1;
			if (x == mapNS::MAP_WIDTH)
				break;
		}
		y += 1;
		if (y == mapNS::MAP_HEIGHT)
			break;
	}

	ifs = new ifstream(colDataFilename);
	if (!ifs)
		exit(1);
	//csvファイルを1行ずつ読み込む
	y = 0;
	x = 0;
	while (getline(*ifs, str)) {
		string token;
		istringstream stream(str);
		x = 0;
		//1行のうち、文字列とコンマを分割する
		while (getline(stream, token, ',')) {
			//すべて文字列として読み込まれるため
			//数値は変換が必要
			int temp = stof(token); //stof(string str) : stringをfloatに変換
			tileCol[y][x] = temp;
			tileColInit[y][x] = temp;
			x += 1;
			if (x == mapNS::MAP_WIDTH)
				break;
		}
		y += 1;
		if (y == mapNS::MAP_HEIGHT)
			break;
	}

	ifs = new ifstream(objDataFilename);
	if (!ifs)
		exit(1);
	//csvファイルを1行ずつ読み込む
	y = 0;
	x = 0;
	while (getline(*ifs, str)) {
		string token;
		istringstream stream(str);
		x = 0;
		//1行のうち、文字列とコンマを分割する
		while (getline(stream, token, ',')) {
			//すべて文字列として読み込まれるため
			//数値は変換が必要
			int temp = stof(token); //stof(string str) : stringをfloatに変換
			tileObj[y][x] = temp;
			x += 1;
			if (x == mapNS::MAP_WIDTH)
				break;
		}
		y += 1;
		if (y == mapNS::MAP_HEIGHT)
			break;
	}

	safeDelete(ifs);
}