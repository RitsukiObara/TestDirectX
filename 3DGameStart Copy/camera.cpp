//===========================================
//
//�J�����̃��C������[camera.cpp]
//Author ��������
//
//===========================================
#include "main.h"
#include "camera.h"
#include "input.h"
#include "model.h"

//�}�N����`
#define ROT_Y_SPEED			(0.04f)				//Y���̉�]�̑��x
#define POS_SPEED			(7.0f)				//�ړ����x
#define DIS_SPEED			(4.0f)				//�����̈ړ���
#define ROTATION_SPEED		(0.05f)				//��荞�ݏ������s����̃��f���̑��x
#define DRAW_MIN_Z			(10.0f)				//Z���̍ŏ��l
#define DRAW_MAX_Z			(2000.0f)			//Z���̍ő�l
#define CAMERA_DISTANCE		(300.0f)			//�J�����̋���
#define CAMERA_INIT_POS_Y	(300.0f)			//�J�����̏����ʒu(Y��)
#define ROTATION_ROT		(0.02f)				//�J�����̊p�x�̕␳�{��
#define MIN_DISTANCE		(50.0f)				//�����̍ŏ��l
#define MAX_DISTANCE		(600.0f)			//�����̍ő�l
#define POSR_POINT			(40.0f)				//�Ǐ]���[�h�̒����_�̈ʒu
#define POSV_POINT			(40.0f)				//�Ǐ]���[�h�̎��_�̈ʒu
#define CORRECT_POSR		(0.22f)				//�����_�̕␳�{��
#define CORRECT_POSV		(0.20f)				//���_�̕␳�{��

//�v���g�^�C�v�錾
void MoveCamera(void);							//�J�����̈ړ�����
void MovePosV(void);							//�J�����̎��_���쏈��
void MovePosR(void);							//�J�����̒����_���쏈��
void MoveRange(void);							//�J�����̋������쏈��
void ChaseCamera(Model *pModel);				//�Ǐ]����
void ResetCamera(void);							//�J�����̏��̃��Z�b�g����
void RotationCamera(Model *pModel);				//�J�����̉�荞�ݏ���

//�O���[�o���ϐ�
Camera g_camera;								//�J�����̏��
CameraMode g_CameraMode;						//�J�����̃��[�h

//==========================================
//�J�����̏���������
//==========================================
void InitCamera(void)
{
	//�J�����̏��̃��Z�b�g����
	ResetCamera();

	//�J�����̃��[�h������������
	g_CameraMode = CAMERA_CHASE;

	//��荞�ݏ��������Ȃ�
	g_camera.bRotation = true;
}

//========================================
//�e�̃Q�[�W�̏I������
//========================================
void UninitCamera(void)
{
	
}

//========================================
//�J�����̍X�V����
//========================================
void UpdateCamera(void)
{
	Model *pModel = GetModel();						//���f���̏�������������

	if (g_CameraMode == CAMERA_EDIT)
	{//�G�f�B�b�g���[�h�������ꍇ
		//�J�����̈ړ�����
		MoveCamera();

		//�J�����̎��_���쏈��
		MovePosV();							

		//�J�����̒����_���쏈��
		MovePosR();							

		//�J�����̋������쏈��
		MoveRange();							

		if (g_camera.rot.y > D3DX_PI)
		{//3.14f���傫���Ȃ����ꍇ
			//-3.14f�ɕ␳����
			g_camera.rot.y = -D3DX_PI;
		}
		else if (g_camera.rot.y < -D3DX_PI)
		{//-3.14f��菬�����Ȃ����ꍇ
			//3.14f�ɕ␳����
			g_camera.rot.y = D3DX_PI;
		}

		//�J�����̎��_���X�V����
		g_camera.posV.x = g_camera.posR.x + sinf(g_camera.rot.y) * -g_camera.Dis;
		g_camera.posV.z = g_camera.posR.z + cosf(g_camera.rot.y) * -g_camera.Dis;

		//�J�����̒����_���X�V����
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.Dis;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.Dis;
	}
	else if (g_CameraMode == CAMERA_CHASE)
	{//�Ǐ]���[�h�������ꍇ
		//�Ǐ]����
		ChaseCamera(pModel);

		if (GetKeyboardTrigger(DIK_F4) == true)
		{//F4�L�[���������ꍇ
			//��荞�݃��[�h�����ւ���
			g_camera.bRotation = g_camera.bRotation ? false : true;
		}

		if (g_camera.bRotation == true)
		{//��荞�ݏ������s��
			if (pModel->move.x <= ROTATION_SPEED &&
				pModel->move.z <= ROTATION_SPEED &&
				pModel->move.x >= -ROTATION_SPEED &&
				pModel->move.z >= -ROTATION_SPEED)
			{//�ړ��ʂ�0.01f�ȉ����A�ړ��ʂ�-0.01f�ȏゾ�����ꍇ
				//�J�����̉�荞�ݏ���
				RotationCamera(pModel);
			}
		}
	}

	if (GetKeyboardTrigger(DIK_SPACE) == true)
	{//�X�y�[�X�L�[���������ꍇ
		//�J�����̏��̃��Z�b�g����
		ResetCamera();
	}

	if (GetKeyboardTrigger(DIK_F8) == true)
	{//F8�L�[���������ꍇ
		if (g_CameraMode == CAMERA_EDIT)
		{//�G�f�B�b�g���[�h�������ꍇ
			//�Ǐ]���[�h�ɕύX����
			g_CameraMode = CAMERA_CHASE;
		}
		else
		{//�Ǐ]���[�h�������ꍇ
			//�G�f�B�b�g���[�h�ɕύX����
			g_CameraMode = CAMERA_EDIT;
		}

		//�J�����̏��̃��Z�b�g����
		ResetCamera();
	}
}

//=====================================
//�J�����̐ݒ菈��
//=====================================
void SetCamera(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(45.0f),								//����p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,			//��ʂ̃A�X�y�N�g��
		DRAW_MIN_Z,											//Z�l�̍ŏ��l
		DRAW_MAX_Z);										//Z�l�̍ő�l

	//�v���W�F�N�g�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera.posV,										//���_
		&g_camera.posR,										//�����_
		&g_camera.VecU);									//������x�N�g��

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//===================================
//�J�����̎擾����
//===================================
Camera GetCamera(void)
{
	//�J������Ԃ�
	return g_camera;
}

//===================================
//�J�����̃��[�h�̎擾����
//===================================
CameraMode GetCameraMode(void)
{
	//�J�����̃��[�h��Ԃ�
	return g_CameraMode;
}

//===================================
//�J�����̏��̃��Z�b�g����
//===================================
void ResetCamera(void)
{
	//���_�ƒ����_�̋���������������
	g_camera.Dis = CAMERA_DISTANCE;

	//���_�̈ʒu������������
	g_camera.posV = D3DXVECTOR3(0.0f, CAMERA_INIT_POS_Y, -g_camera.Dis);

	//�����_�̈ʒu������������
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//����������������
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�J�����̏�����x�N�g��������������
	g_camera.VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//�J�����̖ړI�̒����_������������
	g_camera.posRDest = g_camera.posR;

	//�J�����̖ړI�̎��_������������
	g_camera.posVDest = g_camera.posV;

	//�J�����̖ړI�̌���������������
	g_camera.rotDest = g_camera.rot.y;

	//�J�����̌����̍���������������
	g_camera.rotDiff = g_camera.rotDest - g_camera.rot.y;
}

//======================================
//�J�����̉�荞�ݏ���
//======================================
void RotationCamera(Model *pModel)
{
	//�J�����̖ړI�̌�����ݒ肷��
	g_camera.rotDest = (pModel->rot.y);

	//�J�����̌����̍�����ݒ肷��
	g_camera.rotDiff = g_camera.rotDest - g_camera.rot.y;

	if (g_camera.rotDiff > D3DX_PI)
	{//�p�x��3.14f���傫�������ꍇ
		//�p�x����1�������炷
		g_camera.rotDiff -= (2 * D3DX_PI);
	}
	else if (g_camera.rotDiff < -D3DX_PI)
	{//�p�x��-3.14f��菬���������ꍇ
		//�p�x��1����������
		g_camera.rotDiff += (2 * D3DX_PI);
	}

	//�J�����̌�����␳����
	g_camera.rot.y += g_camera.rotDiff * ROTATION_ROT;

	if (g_camera.rot.y > D3DX_PI)
	{//�p�x��3.14f���傫�������ꍇ
		//�p�x����1�������炷
		g_camera.rot.y = -D3DX_PI;
	}
	else if (g_camera.rot.y < -D3DX_PI)
	{//�p�x��-3.14f��菬���������ꍇ
		//�p�x��1����������
		g_camera.rot.y = D3DX_PI;
	}
}

//==============================
//�J�����̈ړ�����
//==============================
void MoveCamera(void)
{
	if (GetKeyboardPress(DIK_W) == true)
	{//W�L�[���������ꍇ
		if (GetKeyboardPress(DIK_D) == true)
		{//D�L�[���������ꍇ
			g_camera.posR.x += sinf(D3DX_PI * 0.25f + g_camera.rot.y) * POS_SPEED;
			g_camera.posV.x += sinf(D3DX_PI * 0.25f + g_camera.rot.y) * POS_SPEED;

			g_camera.posR.z += cosf(D3DX_PI * 0.25f + g_camera.rot.y) * POS_SPEED;
			g_camera.posV.z += cosf(D3DX_PI * 0.25f + g_camera.rot.y) * POS_SPEED;
		}
		else if (GetKeyboardPress(DIK_A) == true)
		{//A�L�[���������ꍇ
			g_camera.posR.x += sinf(-D3DX_PI * 0.25f + g_camera.rot.y) * POS_SPEED;
			g_camera.posV.x += sinf(-D3DX_PI * 0.25f + g_camera.rot.y) * POS_SPEED;

			g_camera.posR.z += cosf(-D3DX_PI * 0.25f + g_camera.rot.y) * POS_SPEED;
			g_camera.posV.z += cosf(-D3DX_PI * 0.25f + g_camera.rot.y) * POS_SPEED;
		}
		else
		{//W�L�[�݂̂��������ꍇ
			g_camera.posR.x += sinf(g_camera.rot.y) * POS_SPEED;
			g_camera.posV.x += sinf(g_camera.rot.y) * POS_SPEED;

			g_camera.posR.z += cosf(g_camera.rot.y) * POS_SPEED;
			g_camera.posV.z += cosf(g_camera.rot.y) * POS_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{//S�L�[���������ꍇ
		if (GetKeyboardPress(DIK_D) == true)
		{//D�L�[���������ꍇ
			g_camera.posR.x += sinf(D3DX_PI * 0.75f + g_camera.rot.y) * POS_SPEED;
			g_camera.posV.x += sinf(D3DX_PI * 0.75f + g_camera.rot.y) * POS_SPEED;

			g_camera.posR.z += cosf(D3DX_PI * 0.75f + g_camera.rot.y) * POS_SPEED;
			g_camera.posV.z += cosf(D3DX_PI * 0.75f + g_camera.rot.y) * POS_SPEED;
		}
		else if (GetKeyboardPress(DIK_A) == true)
		{//A�L�[���������ꍇ
			g_camera.posR.x += sinf(-D3DX_PI * 0.75f + g_camera.rot.y) * POS_SPEED;
			g_camera.posV.x += sinf(-D3DX_PI * 0.75f + g_camera.rot.y) * POS_SPEED;

			g_camera.posR.z += cosf(-D3DX_PI * 0.75f + g_camera.rot.y) * POS_SPEED;
			g_camera.posV.z += cosf(-D3DX_PI * 0.75f + g_camera.rot.y) * POS_SPEED;
		}
		else
		{//S�L�[�݂̂��������ꍇ
			g_camera.posR.x -= sinf(g_camera.rot.y) * POS_SPEED;
			g_camera.posV.x -= sinf(g_camera.rot.y) * POS_SPEED;

			g_camera.posR.z -= cosf(g_camera.rot.y) * POS_SPEED;
			g_camera.posV.z -= cosf(g_camera.rot.y) * POS_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{//D�L�[���������ꍇ
		g_camera.posR.x += sinf(D3DX_PI * 0.5f + g_camera.rot.y) * POS_SPEED;
		g_camera.posV.x += sinf(D3DX_PI * 0.5f + g_camera.rot.y) * POS_SPEED;

		g_camera.posR.z += cosf(D3DX_PI * 0.5f + g_camera.rot.y) * POS_SPEED;
		g_camera.posV.z += cosf(D3DX_PI * 0.5f + g_camera.rot.y) * POS_SPEED;
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{//A�L�[���������ꍇ
		g_camera.posR.x -= sinf(D3DX_PI * 0.5f + g_camera.rot.y) * POS_SPEED;
		g_camera.posV.x -= sinf(D3DX_PI * 0.5f + g_camera.rot.y) * POS_SPEED;

		g_camera.posR.z += cosf(D3DX_PI * 0.5f - g_camera.rot.y) * POS_SPEED;
		g_camera.posV.z += cosf(D3DX_PI * 0.5f - g_camera.rot.y) * POS_SPEED;
	}
}

//================================
//�J�����̎��_���쏈��
//================================
void MovePosV(void)
{
	if (GetKeyboardPress(DIK_Y) == true)
	{//Y�L�[���������ꍇ
		//���_����ɓ�����
		g_camera.posV.y += POS_SPEED;
	}
	else if (GetKeyboardPress(DIK_N) == true)
	{//N�L�[���������ꍇ
		//���_�����ɓ�����
		g_camera.posV.y -= POS_SPEED;
	}
	else if (GetKeyboardPress(DIK_Z) == true)
	{//Z�L�[���������ꍇ
		//���_���E�ɉ�
		g_camera.rot.y -= ROT_Y_SPEED;
	}
	else if (GetKeyboardPress(DIK_C) == true)
	{//C�L�[���������ꍇ
		//���_�����ɉ�
		g_camera.rot.y += ROT_Y_SPEED;
	}
}

//====================================
//�J�����̒����_���쏈��
//====================================
void MovePosR(void)
{
	if (GetKeyboardPress(DIK_T) == true)
	{//T�L�[���������ꍇ
		//�����_����ɓ�����
		g_camera.posR.y += POS_SPEED;
	}
	else if (GetKeyboardPress(DIK_B) == true)
	{//B�L�[���������ꍇ
		//�����_�����ɓ�����
		g_camera.posR.y -= POS_SPEED;
	}
	else if (GetKeyboardPress(DIK_Q) == true)
	{//Q�L�[���������ꍇ
		//�����_���E�ɉ�
		g_camera.rot.y -= ROT_Y_SPEED;

		//�����_���X�V����
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.Dis;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.Dis;
	}
	else if (GetKeyboardPress(DIK_E) == true)
	{//E�L�[���������ꍇ
		//�����_�����ɉ�
		g_camera.rot.y += ROT_Y_SPEED;

		//�����_���X�V����
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.Dis;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.Dis;
	}
}

//================================
//�J�����̋������쏈��
//================================
void MoveRange(void)
{
	if (GetKeyboardPress(DIK_U) == true)
	{//U�L�[���������ꍇ
		g_camera.Dis -= DIS_SPEED;

		if (g_camera.Dis <= MIN_DISTANCE)
		{//100.0f�ȉ��̏ꍇ
			//100.0f�ɂ���
			g_camera.Dis = MIN_DISTANCE;
		}
	}
	else if (GetKeyboardPress(DIK_M) == true)
	{//M�L�[���������ꍇ
		g_camera.Dis += DIS_SPEED;

		if (g_camera.Dis >= MAX_DISTANCE)
		{//800.0f�ȏ�̏ꍇ
			//800.0f�ɂ���
			g_camera.Dis = MAX_DISTANCE;
		}
	}
}

//=====================================
//�Ǐ]����
//=====================================
void ChaseCamera(Model *pModel)
{
	//�ړI�̒����_��ݒ肷��
	g_camera.posRDest.x = pModel->pos.x + sinf(pModel->rot.y) * POSR_POINT;
	g_camera.posRDest.z = pModel->pos.z + cosf(pModel->rot.y) * POSR_POINT;

	//�ړI�̎��_��ݒ肷��
	g_camera.posVDest.x = g_camera.posRDest.x + sinf(g_camera.rot.y) * -g_camera.Dis + sinf(pModel->rot.y) * POSV_POINT;
	g_camera.posVDest.z = g_camera.posRDest.z + cosf(g_camera.rot.y) * -g_camera.Dis + cosf(pModel->rot.y) * POSV_POINT;

	//�����_��␳
	g_camera.posR.x += (g_camera.posRDest.x - g_camera.posR.x) * CORRECT_POSR;
	g_camera.posR.z += (g_camera.posRDest.z - g_camera.posR.z) * CORRECT_POSR;

	//���_��␳
	g_camera.posV.x += (g_camera.posVDest.x - g_camera.posV.x) * CORRECT_POSV;
	g_camera.posV.z += (g_camera.posVDest.z - g_camera.posV.z) * CORRECT_POSV;
}