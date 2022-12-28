//Dxlib�͐}�`�╶���̎n�_�������Ɏw��ł��Ȃ�����}�W�b�N�i���o�[�������o�Ă��܂��E�E�E
#include "DxLib.h"
//WINDOW�ݒ�
#define WINDOW_W 800
#define WINDOW_H 450
//���W�A���l��DEGREE�ւ̕ϊ��Ɏg�����ߒ�`
#define PI 3.1415926535897932384626433832795f
//�摜�̐��i�v���C���[�APC�A���ꂼ��3�Ȃ̂�3�ł����j
#define IMG_NUM 3
//SE�̐�
#define SE_NUM 6

//Font�p�̕ϐ��@�����ȂƂ��Ŏg����ŃO���[�o���ɂ��Ă�
int g_fontHandle1;
int g_fontHandle2;
//�L�[�̉�����Ԃ̕ۑ��p�z��@�����ȂƂ��Ŏg����ŃO���[�o���ɂ��Ă�
char g_keyState[256];
char g_oldKeyState[256];
//�v���C���[�Ƃo�b�̏o����̏��
struct HAND {
	int hand;
	int img[IMG_NUM];
	int px, py;
	double extRate;
	double angle;
	int life;
};
//��ȃf�[�^�i�[�p�\����
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
};

//�摜�ǂݍ��ݏ���
void loadImage(struct DATA* d) {
	d->player.img[0] = LoadGraph("assets\\playerGu.png");
	d->player.img[1] = LoadGraph("assets\\playerChoki.png");
	d->player.img[2] = LoadGraph("assets\\playerPa.png");

	d->pc.img[0] = LoadGraph("assets\\pcGu.png");
	d->pc.img[1] = LoadGraph("assets\\pcChoki.png");
	d->pc.img[2] = LoadGraph("assets\\pcPa.png");

	d->heartImg = LoadGraph("assets\\heart.png");
}
//�����ǂݍ��ݏ���
void loadSe(struct DATA* d) {
	d->se[0] = LoadSoundMem("assets\\start.wav");
	d->se[1] = LoadSoundMem("assets\\aiko.wav");
	d->se[2] = LoadSoundMem("assets\\win.wav");
	d->se[3] = LoadSoundMem("assets\\lose.wav");
	d->se[4] = LoadSoundMem("assets\\f_win.wav");
	d->se[5] = LoadSoundMem("assets\\f_lose.wav");
}
//�������p
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

	g_fontHandle1 = CreateFontToHandle(NULL, 40, -1);
	g_fontHandle2 = CreateFontToHandle(NULL, 100, -1);
}
//�^�C�g����ʕ\���p
void title(struct DATA* d) {
	DrawStringToHandle(-5, 150, "����񂯂񂰁[��", GetColor(255, 255, 255), g_fontHandle2);
	DrawStringToHandle(100, 300, "���G���^�[�L�[�ł͂��܂�恙�c", GetColor(255, 255, 255), g_fontHandle1);
	if (g_keyState[KEY_INPUT_RETURN] == 1 && g_oldKeyState[KEY_INPUT_RETURN] == 0) d->state = d->PLAY;
}
//�Q�[���v���C�p
void play(struct DATA* d) {
	if (!(g_keyState[KEY_INPUT_A] == 1 && g_oldKeyState[KEY_INPUT_A] == 0) &&
		!(g_keyState[KEY_INPUT_S] == 1 && g_oldKeyState[KEY_INPUT_S] == 0) &&
		!(g_keyState[KEY_INPUT_D] == 1 && g_oldKeyState[KEY_INPUT_D] == 0)) {
		return;
	}
	if (d->j_flg == 0) {
		PlaySoundMem(d->se[0], DX_PLAYTYPE_BACK);
		WaitTimer(2500);
		d->j_flg = 1;
	}
	if (g_keyState[KEY_INPUT_A] == 1 && g_oldKeyState[KEY_INPUT_A] == 0) d->player.hand = d->GU;
	if (g_keyState[KEY_INPUT_S] == 1 && g_oldKeyState[KEY_INPUT_S] == 0) d->player.hand = d->CHOKI;
	if (g_keyState[KEY_INPUT_D] == 1 && g_oldKeyState[KEY_INPUT_D] == 0) d->player.hand = d->PA;
	d->pc.hand = GetRand(2);
	//������������
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
//���ʕ\���p
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
	if (g_keyState[KEY_INPUT_SPACE] == 1 && g_oldKeyState[KEY_INPUT_SPACE] == 0) {
		d->state = d->INIT;
	}
}
//PLAY���̕`�揈���p
void draw(struct DATA* d) {
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
			DrawString(220, 110, "����", GetColor(255, 0, 0));
		}
		else {
			DrawString(220, 110, "����", GetColor(0, 0, 255));
		}
	}
	
	DrawStringToHandle(100, 330, "A = ���`", GetColor(255, 255, 255), g_fontHandle1);
	DrawStringToHandle(300, 330, "S = ���傫", GetColor(255, 255, 255), g_fontHandle1);
	DrawStringToHandle(550, 330, "D = �ρ`", GetColor(255, 255, 255), g_fontHandle1);
	DrawStringToHandle(100, 390, "SPACE = �Đ�", GetColor(255, 255, 255), g_fontHandle1);
	DrawStringToHandle(400, 390, "ESC = �I���", GetColor(255, 255, 255), g_fontHandle1);
}
//main�֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (DxLib_Init() == -1)		
	{
		return -1;			
	}

	//WINDOW�̏����ݒ�@�֐����H�H�H�@�܂��ʂɂ������Ȃ�
	ChangeWindowMode(TRUE);
	SetGraphMode(WINDOW_W, WINDOW_H, 32);
	SetDrawScreen(DX_SCREEN_BACK);
	SetBackgroundColor(180, 180, 180);
	//�L�[�̉�����Ԃ̕ۑ��p�z��̏�����
	for(int i = 0; i < 256; i++){
		g_keyState[i] = 0;
		g_oldKeyState[i] = 0;
	}

	//DATA�\���̂̒�`
	struct DATA d;
	//�������ւ̃f�[�^�̓ǂݍ���
	loadImage(&d);
	loadSe(&d);

	while (ProcessMessage() == 0) {
		//���݂̃L�[�{�[�h�̏�Ԃ����ׂĎ擾
		GetHitKeyStateAll(g_keyState);
		//��ʂ̏�����
		ClearDrawScreen();
		//���C���̏���
		if (d.state == d.INIT) init(&d);
		else if (d.state == d.TITLE) title(&d);
		else if (d.state == d.PLAY) play(&d);
		else if (d.state == d.RESULT) result(&d);
		if (d.state == d.PLAY || d.state == d.RESULT) {
			draw(&d);
		}
		//
		//����ʂ̕\��
		ScreenFlip();
		//ESC�ŏI��������
		if (g_keyState[KEY_INPUT_ESCAPE] == 1 && g_oldKeyState[KEY_INPUT_ESCAPE] == 0) break;
		//�L�[�{�[�h�̉�������ۑ�����
		for (int i = 0; i < 256; i++) {
			g_oldKeyState[i] = g_keyState[i];
		}
	}
		
	::DxLib_End();				

	return 0;				
}