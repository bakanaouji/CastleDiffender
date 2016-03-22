// TowerDiffenceはゲームのメインクラス
#include "towerDiffence.h"

//==========================================================
// コンストラクタ
//==========================================================
TowerDiffence::TowerDiffence()
{
	initialized = false;
	menuOn = true;
	rect = NULL;
}

//==========================================================
// デストラクタ
//==========================================================
TowerDiffence::~TowerDiffence()
{
	safeDelete(rect);
	releaseAll();	// すべてのグラフィックスアイテムについて、onLostDevice()を呼び出す
}

//==========================================================
// ゲームを初期化
// エラー時にGameErrorをスロー
//==========================================================
void TowerDiffence::initialize(HWND hwnd)
{
	Game::initialize(hwnd);	// GameErrorをスロー
	rect = new Rect();
	rect->initialize(graphics);

	// メニューのテクスチャ
	if (!menuTexture.initialize(graphics, MENU_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));
	// メニューの画像
	if (!menu.initialize(graphics, 0, 0, 0, &menuTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));
	menu.setScale(2);

	// マップのテクスチャ
	if (!tileTexture.initialize(graphics, TILE_IMAGES))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing map texture"));
	// マップの画像
	if (!tile.initialize(graphics, towerDiffenceNS::TEXTURE_SIZE, towerDiffenceNS::TEXTURE_SIZE, towerDiffenceNS::TEXTURE_COLS, &tileTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tile"));

	// 勇者のテクスチャ
	if (!braveTexture.initialize(graphics, BRAVE_MOVE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing brave_move texture"));
	// 勇者
	if (!brave.initialize(this, braveNS::WIDTH, braveNS::HEIGHT, braveNS::TEXTURE_COLS, &braveTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing brave_move"));
	brave.setScale(2);
	brave.setFrames(braveNS::MOVE_UP_START_FRAME, braveNS::MOVE_UP_END_FRAME);
	brave.setCurrentFrame(braveNS::MOVE_UP_START_FRAME);

	// ダッシュボード
	if (!dashboardTextures.initialize(graphics, DASHBOARD_TEXTURES))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing dashboard textures"));
	barGraph.initialize(graphics, &dashboardTextures, towerDiffenceNS::BAR_GRAPH_X, towerDiffenceNS::BAR_GRAPH_Y, 0.5f, 20, graphicsNS::RED);
	barGraph.set(100);

	return;
}

//==========================================================
// すべてのゲームアイテムを更新
//==========================================================
void TowerDiffence::update()
{
	if (menuOn)
	{
		if (input->anyKeyPressed())
		{
			menuOn = false;
			input->clearAll();
			roundStart();
		}
	}
	else
	{
		brave.update(frameTime);
	}
}

//==========================================================
// プレイの新しいラウンドを開始
//==========================================================
void TowerDiffence::roundStart()
{

}

//==========================================================
// 人工知能
//==========================================================
void TowerDiffence::ai()
{

}

//==========================================================
// 衝突を処理
//==========================================================
void TowerDiffence::collisions()
{

}

//==========================================================
// ゲームアイテムをレンダー
// プレイ中のレイヤー：マップ→黒背景→その他アイテムの順
//==========================================================
void TowerDiffence::render()
{
	graphics->spriteBegin();	// スプライトの描画を開始

	if (menuOn)
	{
		menu.draw();
	}
	else
	{
		// ステージの描画
		for (int row = 0; row<towerDiffenceNS::MAP_HEIGHT; row++)       // マップの各行を処理
		{
			tile.setY((float)(row*towerDiffenceNS::TEXTURE_SIZE));      // タイルのYを設定
			for (int col = 0; col<towerDiffenceNS::MAP_WIDTH; col++)    // マップの各列を処理
			{
				if (towerDiffenceNS::tileMap[row][col] >= 0)            // タイルが存在する場合
				{
					tile.setCurrentFrame(towerDiffenceNS::tileMap[row][col]);                       // タイルのテクスチャを設定
					tile.setX((float)(col*towerDiffenceNS::TEXTURE_SIZE) + mapX);                   // タイルのXを設定
					if (tile.getX() > -towerDiffenceNS::TEXTURE_SIZE && tile.getX() < GAME_WIDTH)   // タイルが画面上にあるかどうか
						tile.draw();    // タイルを描画
				}
			}
		}
		graphics->spriteEnd();		// スプライトの描画を開始
		rect->draw();
		graphics->spriteBegin();	// スプライトの描画を開始
		brave.draw();
		barGraph.draw(graphicsNS::FILTER);	// 体力バーを描画
	}
	graphics->spriteEnd();		// スプライトの描画を開始
}

//==========================================================
// グラフィックスデバイスが消失した場合
// グラフィックスデバイスをリセット可能にするため、
// 予約されていたビデオメモリをすべて解放
//==========================================================
void TowerDiffence::releaseAll()
{
	menuTexture.onLostDevice();
	braveTexture.onLostDevice();
	dashboardTextures.onLostDevice();
	safeOnLostDevice(rect);
	Game::releaseAll();
	return;
}

//==========================================================
// グラフィックスデバイスがリセットされた場合
// すべてのサーフェイスを再作成
//==========================================================
void TowerDiffence::resetAll()
{
	dashboardTextures.onLostDevice();
	braveTexture.onResetDevice();
	menuTexture.onResetDevice();
	safeOnResetDevice(rect);
	Game::resetAll();
	return;
}