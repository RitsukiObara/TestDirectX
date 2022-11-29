//===================================
//
//�p�[�e�B�N���w�b�_�[[Particle.h]
//Author ��������
//
//===================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include"main.h"

//�p�[�e�B�N���̗v��
typedef enum
{
	PTCL_EXPL = 0,					//�����p�[�e�B�N��
	PTCL_MAX
}ParticleType;

//�v���g�^�C�v�錾
void InitParticle(void);									//�p�[�e�B�N���̏���������
void UninitParticle(void);									//�p�[�e�B�N���̏I������
void UpdateParticle(void);									//�p�[�e�B�N���̍X�V����
void DrawParticle(void);									//�p�[�e�B�N���̕`�揈��
void SetParticle(D3DXVECTOR3 pos, int nType);				//�p�[�e�B�N���̐ݒ菈��

#endif