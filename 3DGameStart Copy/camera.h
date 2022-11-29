//===================================
//
//�J�����w�b�_�[[camera.h]
//Author ��������
//
//===================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//�J�����̍\����
typedef struct
{
	D3DXVECTOR3 posV;					//���_
	D3DXVECTOR3 posR;					//�����_
	D3DXVECTOR3 posVDest;				//�ړI�̎��_
	D3DXVECTOR3 posRDest;				//�ړI�̒����_
	D3DXVECTOR3 rot;					//����
	float rotDest;						//�ړI�̌���
	float rotDiff;						//�����̍���
	float Dis;							//����
	D3DXVECTOR3 VecU;					//������x�N�g��
	D3DXMATRIX mtxProjection;			//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView;					//�r���[�}�g���b�N�X
	bool bRotation;						//��荞�ݏ��������邩�ǂ���
}Camera;

//�J�����̃��[�h
typedef enum
{
	CAMERA_EDIT = 0,					//�G�f�B�b�g���[�h
	CAMERA_CHASE,						//�Ǐ]���[�h
	CAMERA_MAX
}CameraMode;

//�v���g�^�C�v�錾
void InitCamera(void);					//�J�����̏���������
void UninitCamera(void);				//�J�����̏I������
void UpdateCamera(void);				//�J�����̍X�V����
void SetCamera(void);					//�J�����̐ݒ菈��
Camera GetCamera(void);					//�J�����̎擾����
CameraMode GetCameraMode(void);			//�J�����̃��[�h�̎擾����

#endif