//======================================
//
//GameMaking�̃��C���w�b�_�[[main.h]
//Author ��������
//
//======================================
#ifndef _MAIN_H_//���̃}�N����`������Ă��Ȃ�������
#define _MAIN_H_//2�d�C���N���[�h�h�~�̃}�N�����`����

#include <windows.h>
#include "d3dx9.h"						//�`�揈���ɕK�v
#define DIRECTINPUT_VERSION (0x0800)	//�r���h���̌x���Ώ��p�}�N��
#include "dinput.h"						//���͏����ɕK�v
#include"xaudio2.h"						//�T�E���h�����ɕK�v

//���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")			//�`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")		//[d3d9.lib]
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")		//�V�X�e�������擾�ɕK�v
#pragma comment(lib,"dinput8.lib")		//���͏����ɕK�v


//�}�N����`
#define SCREEN_WIDTH				(1280)						//�E�C���h�E�̕�
#define SCREEN_HEIGHT				(720)						//�E�C���h�E�̍���
#define FVF_VERTEX_2D				(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)	//���_�t�H�[�}�b�g[2D]
#define FVF_VERTEX_3D				(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)	//���_�t�H�[�}�b�g[3D]
#define CENTER_MAGNI				(0.5f)						//���S�ʒu������o�����߂̐��l(���E�����ƕ��p���Ďg�p����)
#define REVERSE_SCALE				(-1)						//�����𔽓]�����鐔�l(�F�E�����x�̔��]�̎d�l����)
#define INIT_SHADOW					(-1)						//�e�̏���������
#define ALPHA_TEST_NUMBER			(75)						//�A���t�@�e�X�g��L���ɂ����Ƃ��̐��l

//���_���[2D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;			//���_���W
	float rhw;					//���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR col;				//���_�J���[
	D3DXVECTOR2 tex;			//�e�N�X�`�����W
}VERTEX_2D;

//���_���[3D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;			//���_���W
	D3DXVECTOR3 hor;			//�@���x�N�g��
	D3DCOLOR col;				//���_�J���[
	D3DXVECTOR2 tex;			//�e�N�X�`�����W
}VERTEX_3D;

//���(���[�h)�̎��
typedef enum
{
	MODE_PROJECT = 0,			//�v���W�F�N�g���
	MODE_TITLE,					//�^�C�g�����
	MODE_CHOICE,				//�Q�[�����[�h�I�����
	MODE_TUTORIAL,				//�`���[�g���A�����
	MODE_GAME,					//�Q�[�����
	MODE_RESULT,				//���U���g���
	MODE_RANKING,				//�����L���O���
	MODE_MAX
}MODE;

//�v���g�^�C�v�錾
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);

#endif