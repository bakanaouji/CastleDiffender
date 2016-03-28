#ifndef _CHARACTER_H	// ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _CHARACTER_H	// ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "map.h"

namespace characterNS
{
	enum DIRECTION { LEFT, RIGHT, UP, DOWN, NONE };	// �L�����N�^�[�̌����i�㉺���E�j
}

// Character�N���X
// �v���C���[��G�N���X�̊�{�ƂȂ�N���X�B
// �p�����邱�ƂŎg�p�\�B�p���Ȃ��ł̓I�u�W�F�N�g�̐����͂ł��Ȃ��B
class Character : public Entity
{
protected:
	characterNS::DIRECTION direction;	// �L�����N�^�[�̌����i�㉺���E�j
	int oldStartFrame;					// �U���A�j���[�V�����I����ɖ߂�ׂ��A�j���[�V�����t���[���ۑ��p
	int oldEndFrame;					// �U���A�j���[�V�����I����ɖ߂�ׂ��A�j���[�V�����t���[���ۑ��p
	float damageTimer;					// �_���[�W���̃A�j���[�V�����p�̃^�C�}�[
	float totalDamageTime;				// �_���[�W���̃A�j���[�V�����p�̃^�C�}�[
	bool drawFlag;						// �`��t���O�A�I���̂Ƃ������摜��`�悷��B�_���[�W���󂯂Ă����Ԏ��Ɏg�p�B�_���[�W���͈�莞�Ԃ����ɔ��]�B�_���[�W���ȊO�͏�ɃI��
	bool isDamaged;						// �_���[�W���󂯂Ă����ԂȂ�true�A����ȊO�Ȃ�false
	bool attackCollisionFlag;			// �U���p�̏Փ˔�����o��������t���O�B�U�����J�n���Ă����莞�Ԍ��true�ƂȂ�B���Ԃ𒲐����邱�Ƃɂ���ăA�j���[�V�����Ɠ���������̂��ړI�B
	Map *map;							// �X�e�[�W�}�b�v�ւ̃|�C���^

public:
	// �R���X�g���N�^
	Character();

	//==========================================================
	// �p�����ꂽ�����o�[�֐�
	//==========================================================
	// �`��֐��A�`��t���O���I���̂Ƃ��̂ݕ`�������
	virtual void draw();
	// Update
	// �ʏ�A�t���[�����Ƃ�1��Ăяo��
	// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
	virtual void update(float frameTime) = 0;
	// �l�H�m�\�BNPC�̍s�������肷��̂Ɏg�p
	virtual void ai(float frameTime, Entity &ent) = 0;
	// �L�����N�^�[�Ƀ_���[�W��^����֐�
	virtual void damage(WEAPON) = 0;

	//==========================================================
	// �V�����ǉ����郁���o�[�֐�
	//==========================================================
	// �p�����[�^���Z�b�g
	virtual void reset();
	// �ړ��\���`�F�b�N
	bool checkCanMove(float x, float y);	
	// ���S���ɌĂяo���֐�
	void dead();
	// �ړ����̃A�b�v�f�[�g�֐�
	void updateMoving(float frameTime);
	// �U�����̃A�b�v�f�[�g�֐�
	void updateAttacking(float frameTime);

	//==========================================================
	// setter
	//==========================================================
	// �}�b�v�ւ̃|�C���^���Z�b�g����֐�
	void setMapPointer(Map *m) { map = m; }

	//==========================================================
	// getter
	//==========================================================
	// �U���p�̏Փ˔�����o��������t���O��Ԃ��֐��B�U����ԂɑJ�ڂ��������true�ƂȂ��āA����ȊO��false
	bool getAttackCollisionFlag() { return attackCollisionFlag; }
	// �����Ă��������Ԃ��֐�
	characterNS::DIRECTION getDirection() { return direction; }

};

#endif // !_CHARACTER_H