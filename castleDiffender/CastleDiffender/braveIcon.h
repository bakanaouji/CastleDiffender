//==========================================================
/// @file
/// @brief    BraveIconクラス
/// @author   阿部拳之
///
/// @attention  勇者のアイコン画像を表すクラスです。

//==========================================================
#ifndef _BRAVE_ICON_H		// このファイルが複数の箇所でインクルードされる場合に、
#define _BRAVE_ICON_H		// 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "image.h"
#include "constants.h"
#include "brave.h"

//==========================================================

// BraveIconクラスの定数
// ゲーム内でのステータス以外をここに記述
namespace braveIconNS
{
	const int WIDTH = 48;			// 画像の幅（各フレーム）
	const int HEIGHT = 48;			// 画像の高さ
	const int X = 10;				// 初期位置
	const int Y = 0;
	const int TEXTURE_COLS = 4;		// テクスチャは4列
	const int START_FRAME = 0;		// アニメーションはフレーム0から開始
	const int END_FRAME = 3;		// アニメーションフレームは0、1、2、3
}

//==========================================================
// 勇者のアイコン画像を表すクラスです。
class BraveIcon : public Image
{
private:
	Brave *mBrave;	// 勇者へのポインタ。オブジェクトはこのクラス外で生成されるのでデストラクタは必要ない。
public:
	// コンストラクタ
	BraveIcon();

	//==========================================================
	// 継承されたメンバー関数
	//==========================================================

	// Update
	// プレイヤーの残り体力に応じてアニメーションを更新する。
	// 通常、フレームごとに1回呼び出す
	// frameTimeは、移動とアニメーションの速さを制御するために使用
	// 引数：frameTime　1フレームで経過した時間
	void update(const float frameTime);
	// パラメータリセット
	// roundStart()内で呼び出される
	void reset();
	
	//==========================================================
	// 新しいメンバー関数
	//==========================================================

	// Braveオブジェクトとリンクさせる。
	// initialize関数の直後に呼び出す。
	// 引数：brave	リンクするBraveオブジェクト
	void linkEntity(Brave& brave) { mBrave = &(brave); }
};
#endif // !_BRAVE_ICON_H
