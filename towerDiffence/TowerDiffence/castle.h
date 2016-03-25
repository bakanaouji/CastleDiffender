#ifndef _CASTLE_H		// ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _CASTLE_H		// ���d�ɒ�`����邱�Ƃ�h���܂��B

#include "entity.h"
#include "constants.h"
#include "enemy.h"

namespace castleNS
{
	const int WIDTH = 96;			// �摜�̕��i�e�t���[���j
	const int HEIGHT = 96;			// �摜�̍���
	const int X = 18.3 * 30;	// �����ʒu
	const int Y = 9.5 * 30;
	const int COLLISION_RADIUS = 60;// �~�̏Փ˔���	
	const int START_FRAME = 0;
}

class Castle : public Entity		// Entity�N���X���p��
{
private:
	float timeCounter;				// �_���[�W�A�j���[�V�����p�̃^�C�}�[
	float totalTimeCounter;
	bool death;						// �邪���S����true
	bool isDamaged;					// �_���[�W���󂯂Ă����Ԃ��ǂ���
	bool drawFlag;					// �`��t���O�Atrue���ɕ`��
public:
	// �R���X�g���N�^
	Castle();

	// �p�����ꂽ�����o�[�֐�
	virtual void draw();
	void update(float frameTime);
	void damage(WEAPON);

	// getter
	bool isDeath() { return death; }

	// setter
	void setDeath(bool d) { death = d; }
};

#endif // !_CASTLE_H