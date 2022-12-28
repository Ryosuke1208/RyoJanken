#include "DxLib.h"
/*******************************************************/
//WINDOW設定
#define WINDOW_W 800
#define WINDOW_H 450
//ラジアン値→DEGREEへの変換に使うため定義
#define PI 3.1415926535897932384626433832795f
//画像の数（プレイヤー、PC、それぞれ3つなので3でおｋ）
#define IMG_NUM 3
//SEの数
#define SE_NUM 6
/*******************************************************/

//プレイヤーとＰＣの出す手の情報
struct HAND {
	int hand;
	int img[IMG_NUM];
	int px, py;
	double extRate;
	double angle;
	int life;
};
//主なデータ格納用構造体
struct DATA {
	int INIT = 0;
	int TITLE = 1;
	int PLAY = 2;
	int RESULT = 3;
	int state = INIT;

	int GU = 0;
	int CHOKI = 1;
	int PA = 2;
	struct HAND player;
	struct HAND pc;

	int heartImg = 0;
	int resultSize = 0;

	int se[SE_NUM];
	int se_flg = 0;

	int j_flg = 0;
	
	int fontHandle1 = 0;
	int fontHandle2 = 0;

	char keyState[256] = { 0 };
	char oldKeyState[256] = { 0 };
};

//画像読み込み処理
void loadImage(struct DATA* d) {
	d->player.img[0] = LoadGraph("assets\\playerGu.png");
	d->player.img[1] = LoadGraph("assets\\playerChoki.png");
	d->player.img[2] = LoadGraph("assets\\playerPa.png");

	d->pc.img[0] = LoadGraph("assets\\pcGu.png");
	d->pc.img[1] = LoadGraph("assets\\pcChoki.png");
	d->pc.img[2] = LoadGraph("assets\\pcPa.png");

	d->heartImg = LoadGraph("assets\\heart.png");
}
//音声読み込み処理
void loadSe(struct DATA* d) {
	d->se[0] = LoadSoundMem("assets\\start.wav");
	d->se[1] = LoadSoundMem("assets\\aiko.wav");
	d->se[2] = LoadSoundMem("assets\\win.wav");
	d->se[3] = LoadSoundMem("assets\\lose.wav");
	d->se[4] = LoadSoundMem("assets\\f_win.wav");
	d->se[5] = LoadSoundMem("assets\\f_lose.wav");
}
//初期化用
void init(struct DATA* d){
	d->player.hand = d->GU;
	d->player.px = 180;
	d->player.py = 205;
	d->player.extRate = 1.0;
	d->player.angle = 0.0;
	d->player.life = 3;

	d->pc.hand = d->GU;
	d->pc.px = WINDOW_W - 180;
	d->pc.py = 205;
	d->pc.extRate = 1.0;
	d->pc.angle = 0.0;
	d->pc.life = 3;

	d->resultSize = 0;
	d->state = d->TITLE;

	d->fontHandle1 = CreateFontToHandle(NULL, 40, -1);
	d->fontHandle2 = CreateFontToHandle(NULL, 100, -1);
}
//描画処理用
void draw(struct DATA* d) {
	//TITLEのときだけ表示してreturnで抜ける
	if (d->state == d->TITLE) {
		DrawStringToHandle(-5, 150, "じゃんけんげーむ", GetColor(255, 255, 255), d->fontHandle2);
		DrawStringToHandle(100, 300, "☆エンターキーではじまるよ☆彡", GetColor(255, 255, 255), d->fontHandle1);
		return;
	}

	DrawRotaGraph(d->player.px, d->player.py, d->player.extRate, d->player.angle, d->player.img[d->player.hand], TRUE, FALSE);
	DrawRotaGraph(d->pc.px, d->pc.py, d->pc.extRate, d->pc.angle, d->pc.img[d->pc.hand], TRUE, FALSE);

	for (int i = 0; i < d->player.life; i++) {
		DrawGraph(150 + 50 * (i - 1), 55, d->heartImg, TRUE);
	}
	for (int i = 0; i < d->pc.life; i++) {
		DrawGraph(580 + 50 * (i - 1), 55, d->heartImg, TRUE);
	}
	if (d->state == d->RESULT) {
		SetFontSize(d->resultSize);
		if (d->player.life > 0) {
			DrawString(220, 110, "勝ち", GetColor(255, 0, 0));
		}
		else {
			DrawString(220, 110, "負け", GetColor(0, 0, 255));
		}
	}

	DrawStringToHandle(100, 330, "A = ぐ～", GetColor(255, 255, 255), d->fontHandle1);
	DrawStringToHandle(300, 330, "S = ちょき", GetColor(255, 255, 255), d->fontHandle1);
	DrawStringToHandle(550, 330, "D = ぱ～", GetColor(255, 255, 255), d->fontHandle1);
	DrawStringToHandle(100, 390, "SPACE = 再戦", GetColor(255, 255, 255), d->fontHandle1);
	DrawStringToHandle(400, 390, "ESC = 終わり", GetColor(255, 255, 255), d->fontHandle1);
}
//タイトル画面表示用
void title(struct DATA* d) {
	if (d->keyState[KEY_INPUT_RETURN] == 1 && d->oldKeyState[KEY_INPUT_RETURN] == 0) d->state = d->PLAY;
}
//ゲームプレイ用
void play(struct DATA* d) {
	if (!(d->keyState[KEY_INPUT_A] == 1 && d->oldKeyState[KEY_INPUT_A] == 0) &&
		!(d->keyState[KEY_INPUT_S] == 1 && d->oldKeyState[KEY_INPUT_S] == 0) &&
		!(d->keyState[KEY_INPUT_D] == 1 && d->oldKeyState[KEY_INPUT_D] == 0)) {
		return;
	}
	if (d->j_flg == 0) {
		PlaySoundMem(d->se[0], DX_PLAYTYPE_BACK);
		WaitTimer(2500);
		d->j_flg = 1;
	}
	if (d->keyState[KEY_INPUT_A] == 1 && d->oldKeyState[KEY_INPUT_A] == 0) d->player.hand = d->GU;
	if (d->keyState[KEY_INPUT_S] == 1 && d->oldKeyState[KEY_INPUT_S] == 0) d->player.hand = d->CHOKI;
	if (d->keyState[KEY_INPUT_D] == 1 && d->oldKeyState[KEY_INPUT_D] == 0) d->player.hand = d->PA;
	d->pc.hand = GetRand(2);
	//勝ち負け判定
	if (d->player.hand == d->pc.hand) {
		PlaySoundMem(d->se[1], DX_PLAYTYPE_BACK);
		return;
	}
	else if ((d->player.hand + 1) % 3 == d->pc.hand) {
		d->pc.life--;
		if (!(d->player.life == 0 || d->pc.life == 0)) {
			PlaySoundMem(d->se[2], DX_PLAYTYPE_BACK);
		}
	}
	else {
		d->player.life--;
		if (!(d->player.life == 0 || d->pc.life == 0)) {
			PlaySoundMem(d->se[3], DX_PLAYTYPE_BACK);
		}
	}
	if (d->player.life == 0 || d->pc.life == 0) {
        d->state = d->RESULT;
		d->se_flg = 1;
    }
	d->j_flg = 0;
}
//結果表示用
void result(struct DATA* d) {
	if (d->player.life == 0) {
		if(d->se_flg == 1){
			PlaySoundMem(d->se[5], DX_PLAYTYPE_BACK);
			d->se_flg = 0;
		}
		d->player.py += 1;
		d->player.angle += PI / 180;
	}
	else {
		if (d->se_flg == 1) {
			PlaySoundMem(d->se[4], DX_PLAYTYPE_BACK);
			d->se_flg = 0;
		}
		d->pc.py += 1;
		d->pc.angle += -(PI / 180);
	}
	if (d->resultSize < 180) {
		d->resultSize += 10;
	}
	if (d->keyState[KEY_INPUT_SPACE] == 1 && d->oldKeyState[KEY_INPUT_SPACE] == 0) {
		d->state = d->INIT;
	}
}

//main関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (DxLib_Init() == -1)		
	{
		return -1;			
	}

	//WINDOWの初期設定　関数化？？？　まあ別にいいかなぁ
	ChangeWindowMode(TRUE);
	SetGraphMode(WINDOW_W, WINDOW_H, 32);
	SetDrawScreen(DX_SCREEN_BACK);
	SetBackgroundColor(180, 180, 180);
	

	//DATA構造体の定義
	struct DATA d;
	//メモリへのデータの読み込み
	loadImage(&d);
	loadSe(&d);
	//キーの押下状態の保存用配列の初期化
	for (int i = 0; i < 256; i++) {
		d.keyState[i] = 0;
		d.oldKeyState[i] = 0;
	}

	while (ProcessMessage() == 0) {
		//現在のキーボードの状態をすべて取得
		GetHitKeyStateAll(d.keyState);
		//画面の初期化
		ClearDrawScreen();
		//メインの処理
		if (d.state == d.INIT) init(&d);
		else if (d.state == d.TITLE) title(&d);
		else if (d.state == d.PLAY) play(&d);
		else if (d.state == d.RESULT) result(&d);
		if (!(d.state == d.INIT)) {
			draw(&d);
		}
		//
		//裏画面の表示
		ScreenFlip();
		//ESCで終了させる
		if (d.keyState[KEY_INPUT_ESCAPE] == 1 && d.oldKeyState[KEY_INPUT_ESCAPE] == 0) break;
		//キーボードの押下情報を保存する
		for (int i = 0; i < 256; i++) {
			d.oldKeyState[i] = d.keyState[i];
		}
	}
		
	::DxLib_End();				

	return 0;				
}