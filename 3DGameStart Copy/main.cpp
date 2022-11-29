//=========================================
//
//GameMaking[main.cpp]
//Author ��������
//
//=========================================
#include <stdio.h>			//�f�o�b�O�\���ɕK�v
#include <string.h>			//�f�o�b�O�\���ɕK�v
#include "main.h"
#include "input.h"
#include "Polygon.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "Shadow.h"
#include "wall.h"
#include "billboard.h"
#include "Effect.h"
#include "sound.h"
#include "Particle.h"
#include "Bullet.h"
#include "Explosion.h"
#include "Enemy.h"
#include "meshfield.h"
#include "Tower.h"
#include "meshwall.h"
#include "meshcylinder.h"
#include "meshdome.h"
#include "exp.h"

//�}�N����`
#define CLASS_NAME					"WindowClass"				//�E�C���h�E�N���X�̖��O
#define WINDOW_NAME					"3DGame"					//�E�C���h�E�̖��O(�L���v�V�����ɕ\��)
#define SCORE_INIT_NUMBER			(-1)						//�X�R�A�̏����l
#define BONUS_INIT_NUMBER			(0)							//�{�[�i�X�֌W�̏����l
#define RECORD_INIT_NUMBER			(-1)						//�j���[���R�[�h�̏����l

//�v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);		//����������
void Uninit(void);												//�I������
void Update(void);												//�X�V����
void Draw(void);												//�`�揈��
void DrawFPS();													//FPS�̕\��
void DrawCamera();												//�J�����֌W�̕\��
void DrawControl();												//����̕\��
void DrawDebug();												//�f�o�b�O�̕\��
void DrawDebugModel();											//���f���֌W�̕\��
void DrawDebugEnemy();											//�G�̃f�o�b�O�\��

//�O���[�o���ϐ��錾
LPDIRECT3D9 g_pD3D = NULL;										//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;							//Direct3D�f�o�C�X�ւ̃|�C���^
LPD3DXFONT g_pFont = NULL;										//�t�H���g�ւ̃|�C���^
int g_nCountFPS = 0;											//FPS�J�E���^
bool g_bDispDebug = false;										//�f�o�b�O�\����ON/OFF
MODE g_mode = MODE_PROJECT;										//���݂̃��[�h
bool g_Wire;													//���C���[�t���[����ON/OFF

//=========================================
//���C���֐�
//=========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),										//WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,												//�E�C���h�E�̃X�^�C��
		WindowProc,												//�E�C���h�E�v���V�[�W��
		0,														//0�ɂ���(�ʏ�͎g�p���Ȃ�)
		0,														//0�ɂ���(�ʏ�͎g�p���Ȃ�)
		hInstance,												//�C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),							//�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),								//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),								//�N���C�A���g�̈�̔w�i�F
		NULL,													//���j���[�o�[
		CLASS_NAME,												//�E�C���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION)							//�t�@�C���̃A�C�R��
	};
	HWND hWnd;													//�E�C���h�E�n���h��(���ʎq)
	MSG msg;													//���b�Z�[�W���i�[����ϐ�

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };				//��ʃT�C�Y�̍\����

																//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	DWORD dwCurrentTime;										//���ݎ���
	DWORD dwExecLastTime;										//�Ō�ɏ�����������
	DWORD dwFrameCount;											//�t���[���J�E���g
	DWORD dwFPSLastTime;										//�Ō��FPS���v����������

																//�E�C���h�E�𐶐�
	hWnd = CreateWindowEx(0,									//�g���E�C���h�E�X�^�C��
		CLASS_NAME,												//�E�C���h�E�N���X�̖��O
		WINDOW_NAME,											//�E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,									//�E�C���h�E�X�^�C��
		CW_USEDEFAULT,											//�E�C���h�E�̍���X���W
		CW_USEDEFAULT,											//�E�C���h�E�̍���Y���W
		(rect.right - rect.left),								//�E�C���h�E�̕�
		(rect.bottom - rect.top),								//�E�C���h�E�̍���
		NULL,													//�e�E�C���h�E�̃n���h��
		NULL,													//���j���[�n���h���܂��͎q�E�C���h�EID
		hInstance,												//�C���X�^���X�n���h��
		NULL);													//�E�C���h�E�쐬�f�[�^

																//����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//���������������s�����ꍇ
		return -1;
	}

	//����\��ݒ�
	timeBeginPeriod(1);
	dwCurrentTime = 0;											//����������
	dwExecLastTime = timeGetTime();								//���ݎ������擾(�ۑ�)
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//�E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);									//�E�C���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(hWnd);											//�N���C�A���g�̈���X�V

																//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windows�̏���
			if (msg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���
			 //�����o��
				break;
			}
			else
			{
				//���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);							//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&msg);							//�E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{//DirectX�̏���
			dwCurrentTime = timeGetTime();						//���ݎ������擾
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5�b�o��
			 //FPS���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;					//FPS�𑪒肵��������ۑ�

				dwFrameCount = 0;								//�t���[���J�E���g���N���A
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60����1�b�o��
				dwExecLastTime = dwCurrentTime;					//�����J�n�̎���[���ݎ���]��ۑ�
																//�X�V����
				Update();

				//�`�揈��
				Draw();

				dwFrameCount++;									//�t���[���J�E���g�����Z
			}
		}
	}

	//�I������
	Uninit();

	//����\��߂�
	timeEndPeriod(1);

	//�E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//�E�C���h�E�v���V�[�W��
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;							//�Ԃ�l���i�[����ϐ���ݒ肷��

	switch (uMsg)
	{
	case WM_DESTROY:					//�E�B���h�E�j���̃��b�Z�[�W
										//WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);

		//�����o��
		break;

	case WM_KEYDOWN:											//�L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:											//[ESK�L�[]�������ꂽ
																//�I�����b�Z�[�W��\��
			nID = MessageBox(hWnd, "�I�����܂���?", "�I�����b�Z�[�W", MB_YESNO);

			if (nID == IDYES)
			{//�u�͂��v��I�񂾏ꍇ
			 //�E�B���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
				DestroyWindow(hWnd);
			}
			else
			{
				return 0;										//0��Ԃ��Ȃ��ƏI�����Ă��܂�
			}
			//�����o��
			break;
		}
		//�����o��
		break;

	case WM_CLOSE:											//�R�}���h���s�̃��b�Z�[�W
															//�I�����b�Z�[�W��\��
		nID = MessageBox(hWnd, "�I�����܂���?", "�I�����b�Z�[�W", MB_YESNO);

		if (nID == IDYES)
		{//�u�͂��v��I�񂾏ꍇ
		 //�E�B���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;											//0��Ԃ��Ȃ��ƏI�����Ă��܂�
		}
		//�����o��
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);			//�K��̏�����Ԃ�
}

//=========================================
//����������
//=========================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;										//�f�B�X�v���C���[�h

	D3DPRESENT_PARAMETERS d3dpp;								//�v���[���e�[�V�����p�����[�^

																//Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));							//�p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						//�Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						//�Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;						//�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;									//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					//�_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;						//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					//�f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;									//�E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//�C���^�[�o��

																//Direct3D�f�o�C�X�̐���
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,			//�`�揈���ƒ��_�������n�[�h�E�F�A�ōs��
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,		//�`�揈�����n�[�h�E�F�A�ōs���A���_������CPU�ōs��
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,	//�`�揈���ƒ��_������CPU�ōs��
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//�J�����O�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//�����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//�f�o�b�O�\���p�̃t�H���g�̐���
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

	//�L�[�{�[�h�̏���������
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�T�E���h�̏���������
	InitSound(hWnd);

	//�Q�[���p�b�h�̏���������
	InitGamePad();

	//�e�̏���������
	InitShadow();

	//�J�����̏���������
	InitCamera();

	//���C�g�̏���������
	InitLight();

	////�|���S���̏���������
	//InitPolygon();

	//���b�V���t�B�[���h�̏���������
	Initmeshfield();

	////�ǂ̏���������
	//InitWall();

	//���b�V���E�H�[���̏���������
	Initmeshwall();

	//���f���̏���������
	InitModel();

	//�r���{�[�h�̏���������
	InitBillboard();

	//�G�t�F�N�g�̏���������
	InitEffect();

	//�p�[�e�B�N���̏���������
	InitParticle();

	//�e�̏���������
	InitBullet();

	//�����̏���������
	InitExplosion();

	//�G�̏���������
	InitEnemy();

	//���̏���������
	InitTower();

	//���b�V���V�����_�[�̏���������
	InitmeshCylinder();

	//���b�V���h�[���̏���������
	InitmeshDome();

	//�o���l�̏���������
	InitExp();

	//���C���[�t���[����OFF
	g_Wire = false;													

	////�Q�[�����y�𗬂�
	//PlaySound(SOUND_LABEL_BGM_GAME);

	return S_OK;
}

//=============================================
//�I������
//=============================================
void Uninit(void)
{
	//�T�E���h�̏I������
	UninitSound();

	//�L�[�{�[�h�̏I������
	UninitKeyboard();

	//�Q�[���p�b�h�̏I������
	UninitGamePad();

	//�e�̏I������
	UninitShadow();

	//�J�����̏I������
	UninitCamera();

	//���C�g�̏I������
	UninitLight();

	////�|���S���̏I������
	//UninitPolygon();

	//���b�V���t�B�[���h�̏I������
	Uninitmeshfield();

	////�ǂ̏I������
	//UninitWall();

	//���b�V���E�H�[���̏I������
	Uninitmeshwall();

	//���f���̏I������
	UninitModel();

	//�r���{�[�h�̏I������
	UninitBillboard();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//�p�[�e�B�N���̏I������
	UninitParticle();

	//�e�̏I������
	UninitBullet();

	//�����̏I������
	UninitExplosion();

	//�G�̏I������
	UninitEnemy();

	//���̏I������
	UninitTower();

	//���b�V���V�����_�[�̏I������
	UninitmeshCylinder();

	//���b�V���h�[���̏I������
	UninitmeshDome();

	//�o���l�̏I������
	UninitExp();

	//�f�o�b�O�\���p�t�H���g�̔j��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	//Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();

		g_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();

		g_pD3D = NULL;
	}
}

//======================================
//�X�V����
//======================================
void Update(void)
{
	//�L�[�{�[�h�̍X�V����
	UpdateKeyboard();

	//�Q�[���p�b�h�̍X�V����
	UpdateGamePad();

	//�e�̍X�V����
	UpdateShadow();

	//���C�g�̍X�V����
	UpdateLight();

	////�|���S���̍X�V����
	//UpdatePolygon();

	//���b�V���t�B�[���h�̍X�V����
	Updatemeshfield();

	////�ǂ̍X�V����
	//UpdateWall();

	//���b�V���E�H�[���̍X�V����
	Updatemeshwall();

	//���f���̍X�V����
	UpdateModel();

	//�J�����̍X�V����
	UpdateCamera();

	//�r���{�[�h�̍X�V����
	UpdateBillboard();

	//�G�t�F�N�g�̍X�V����
	UpdateEffect();

	//�p�[�e�B�N���̍X�V����
	UpdateParticle();

	//�e�̍X�V����
	UpdateBullet();

	//�����̍X�V����
	UpdateExplosion();

	//���̍X�V����
	UpdateTower();

	//�G�̍X�V����
	UpdateEnemy();

	//���b�V���V�����_�[�̍X�V����
	UpdatemeshCylinder();

	//���b�V���h�[���̍X�V����
	UpdatemeshDome();

	//�o���l�̍X�V����
	UpdateExp();

	if (GetKeyboardTrigger(DIK_F2) == true)
	{//F2�L�[������
		//�G�̐ݒ菈��
		SetEnemy(ENEMY_INSECT);
	}

	if (GetKeyboardTrigger(DIK_F3) == true)
	{//F3�L�[������
		//�o���l�̔���
		SetExp(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	if (GetKeyboardTrigger(DIK_F1) == true)
	{//F1�L�[��������
		if (g_bDispDebug == true)
		{//�L�[��ON�������ꍇ
		 //�L�[��OFF�ɂ���
			g_bDispDebug = false;
		}
		else if (g_bDispDebug == false)
		{//�L�[��OFF�������ꍇ
		 //�L�[��ON�ɂ���
			g_bDispDebug = true;
		}
	}
}

//======================================
//�`�揈��
//======================================
void Draw(void)
{
	//��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
	g_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//�`��J�n�����������ꍇ

		if (GetKeyboardTrigger(DIK_F9) == true)
		{//F9�L�[���������ꍇ
			//���C���[�t���[����ON/OFF
			g_Wire = g_Wire ? false : true;

			if (g_Wire == true)
			{//���C���[���[�hON�̏ꍇ
				//���C���[�t���[����ON�ɂ���
				g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			}
			else
			{//���C���[���[�hOFF�̏ꍇ
				//���C���[�t���[����OFF�ɂ���
				g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			}
		}

		//�J�����̐ݒ菈��
		SetCamera();

		////�|���S���̕`�揈��
		//DrawPolygon();

		////���b�V���V�����_�[�̕`�揈��
		//DrawmeshCylinder();

		////���b�V���h�[���̕`�揈��
		//DrawmeshDome();

		//���b�V���t�B�[���h�̕`�揈��
		Drawmeshfield();

		//�e�̕`�揈��
		DrawShadow();

		//���f���̕`�揈��
		DrawModel();

		//�r���{�[�h�̕`�揈��
		DrawBillboard();

		//�G�t�F�N�g�̕`�揈��
		DrawEffect();

		//�p�[�e�B�N���̕`�揈��
		DrawParticle();

		//�o���l�̕`�揈��
		DrawExp();

		////�ǂ̕`�揈��
		//DrawWall();

		//���b�V���E�H�[���̕`�揈��
		Drawmeshwall();

		//�e�̕`�揈��
		DrawBullet();

		//�����̕`�揈��
		DrawExplosion();

		//�G�̕`�揈��
		DrawEnemy();

		//���̕`�揈��
		DrawTower();

		//�f�o�b�O�̕`��
		DrawDebug();

		//�`��I��
		g_pD3DDevice->EndScene();
	}
	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//===========================================
//�f�o�C�X�̎擾
//===========================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//============================================
//FPS�̕\��
//============================================
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aStr[128];

	//������ɑ��
	sprintf(&aStr[0], "FPS:%d", g_nCountFPS);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0, 0, 0, 255));
}

//==============================================
//�J�����֌W�̕\��
//==============================================
void DrawCamera()
{
	RECT rect = { 0,18,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aStr[256];

	Camera CameraInfo = GetCamera();					//�J�����̏����擾����
	CameraMode CameraMode= GetCameraMode();				//�J�����̃��[�h���擾����

	//������ɑ��
	sprintf(&aStr[0], "�J�����̎��_:(%f,%f,%f)\n�J�����̒����_:(%f,%f,%f)\n�J�����̌���:(%f,%f,%f)\n�J�����̋���:%f\n�J�����̖ړI�̌���:[%f]\n====================================================",
		CameraInfo.posV.x, CameraInfo.posV.y, CameraInfo.posV.z,
		CameraInfo.posR.x, CameraInfo.posR.y, CameraInfo.posR.z,
		CameraInfo.rot.x, CameraInfo.rot.y, CameraInfo.rot.z,
		CameraInfo.Dis, CameraInfo.rotDest);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0, 0, 0, 255));
}

//==============================================
//����̕\��
//==============================================
void DrawControl()
{
	RECT rect = { 0,144,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aStr[256];

	if (g_Wire == true)
	{//���C���[���[�hON�������ꍇ
		//������ɑ��
		sprintf(&aStr[0], "�J�����ړ�:[W/A/S/D]\n���_����:[Y/N/Z/C]\n�����_����:[T/B/Q/E]\n�����̒���:[U/M]\n���Z�b�g:[SPACE]\n���[�h�ύX:[F8]\n���C���[:[F9]:ON\n====================================================");
	}
	else
	{//���C���[���[�hOFF�������ꍇ
		//������ɑ��
		sprintf(&aStr[0], "�J�����ړ�:[W/A/S/D]\n���_����:[Y/N/Z/C]\n�����_����:[T/B/Q/E]\n�����̒���:[U/M]\n���Z�b�g:[SPACE]\n���[�h�ύX:[F8]\n���C���[:[F9]:OFF\n====================================================");
	}

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0, 0, 0, 255));
}

//==============================================
//���f���֌W�̕\��
//==============================================
void DrawDebugModel()
{
	RECT rect = { 0,288,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aStr[256];

	Model *pModel = GetModel();					//���f���̈ʒu���擾����

	//������ɑ��
	sprintf(&aStr[0], "���f���̈ړ�:[��/��/��/��]\n�e����:[0]\n���f���̈ʒu���Z�b�g:[ENTER]\n���f���̍��W:(%f,%f,%f)\n���f���̌���:(%f,%f,%f)\n���f���̈ړ���:[%f,%f,%f]", pModel->pos.x, pModel->pos.y, pModel->pos.z, pModel->rot.x, pModel->rot.y, pModel->rot.z,pModel->move.x, pModel->move.y, pModel->move.z);
	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0, 0, 0, 255));
}

//================================================
//�G�̃f�o�b�O�\��
//================================================
void DrawDebugEnemy()
{
	RECT rect = { 0,400,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aStr[256];

	int nEnemyCount = GetEnemyCount();					//�G�̑������擾����

	//������ɑ��
	sprintf(&aStr[0], "====================================================\n�G�̐�:[%d]\n�G�̐��Y:[F2]\n�o���l�̐��Y:[F3]",nEnemyCount);
	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0, 0, 0, 255));
}

//==============================================
//�f�o�b�O�̕\��
//==============================================
void DrawDebug()
{
	//DrawFPS�̃I���I�t�X�C�b�`
#if 1

	if (g_bDispDebug == false)
	{
		//�f�o�b�O�̎��̂ݕ\������
#ifdef _DEBUG

		//FPS�̕\��
		DrawFPS();

		//�G�̑����\��
		DrawDebugEnemy();
#endif
		//�J�����̕\��
		DrawCamera();

		//����̕\��
		DrawControl();

		//���f���֌W�̕\��
		DrawDebugModel();

		//�G�̃f�o�b�O�\��
		DrawDebugEnemy();
	}

#endif
}

//==============================================
//���[�h�̐ݒ�
//==============================================
void SetMode(MODE mode)
{

}

//==============================================
//���[�h�̎擾
//==============================================
MODE GetMode(void)
{
	return g_mode;
}