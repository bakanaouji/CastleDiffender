#include "enemy.h"

//==========================================================
// �f�t�H���g�R���X�g���N�^
//==========================================================
Enemy::Enemy() : Entity()
{
	spriteData.width = enemyNS::WIDTH;			// �G���G�̃T�C�Y
	spriteData.height = enemyNS::HEIGHT;
	spriteData.x = enemyNS::X;					// ��ʏ�̈ʒu
	spriteData.y = enemyNS::Y;
	spriteData.rect.bottom = enemyNS::HEIGHT;	// ��ʂ̈ꕔ��I��
	spriteData.rect.right = enemyNS::WIDTH;
	frameDelay = enemyNS::MOVE_ANIMATION_DELAY;
	// �����̕����͉E
	direction = enemyNS::RIGHT;
	oldDirection = direction;
	startFrame = enemyNS::MOVE_RIGHT_START_FRAME;
	endFrame = enemyNS::MOVE_RIGHT_END_FRAME;
	oldStartFrame = startFrame;
	oldEndFrame = endFrame;
	currentFrame = startFrame;
	// �~�̏Փ˔���p
	radius = enemyNS::WIDTH / 2.0;
	collisionType = entityNS::CIRCLE;
	// �����̏�Ԃ͈ړ�
	state = enemyNS::MOVE;
	// �_���[�W�͎󂯂Ă��Ȃ���Ԃ���X�^�[�g
	isDamaged = false;
	// �^�C�}�[�����Z�b�g
	damageTimer = 0.0f;
	totalDamageTime = 0.0f;
	// �����v���p�J�E���^�[�����Z�b�g
	distanceCounter = 0.0f;
	// �`��t���O�̓I��
	drawFlag = true;
}

//==========================================================
// �G���G��������
// ���s��F���������ꍇ��true�A���s�����ꍇ��false��߂�
//==========================================================
bool Enemy::initialize(Game *gamePtr, int width, int height, int ncols,
	TextureManager *textureM)
{
	return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//==========================================================
// �G���G��`��
//==========================================================
void Enemy::draw()
{
	// �`��t���O���I���̂Ƃ��̂ݕ`��
	if (drawFlag)
		Image::draw();	// �G���G��`��
}

//=============================================================================
// Update
// �ʏ�A�t���[�����Ƃ�1��Ăяo��
// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
//=============================================================================
void Enemy::update(float frameTime, Map *map)
{
	if (!active)
		return;
	// ��ԑJ�ڑO�̏���
	switch (state)
	{
	case enemyNS::MOVE:		// �ړ���
		// �ڕW�ւ̌����ɉ����Ď����̌������C��
		// ������ύX����ꍇ�̓A�j���[�V���������Z�b�g
		switch (goalDirection)
		{
		case enemyNS::LEFT:
			if (direction != goalDirection)
			{
				changeDirection(enemyNS::MOVE_LEFT_START_FRAME, enemyNS::MOVE_LEFT_END_FRAME);
			}
			// �ړ��\��������
			if (checkCanMove(spriteData.x - enemyNS::MOVE_SPEED * frameTime, spriteData.y, map)) {
				// ���Ɉړ�
				spriteData.x -= enemyNS::MOVE_SPEED * frameTime;
			}
			distanceCounter -= enemyNS::MOVE_SPEED * frameTime;
			break;
		case enemyNS::RIGHT:
			if (direction != goalDirection)
			{
				changeDirection(enemyNS::MOVE_RIGHT_START_FRAME, enemyNS::MOVE_RIGHT_END_FRAME);
			}
			// �ړ��\��������
			if (checkCanMove(spriteData.x + enemyNS::MOVE_SPEED * frameTime, spriteData.y, map)) {
				// �E�Ɉړ�
				spriteData.x += enemyNS::MOVE_SPEED * frameTime;
			}
			distanceCounter -= enemyNS::MOVE_SPEED * frameTime;
			break;
		case enemyNS::UP:
			if (direction != goalDirection)
			{
				changeDirection(enemyNS::MOVE_UP_START_FRAME, enemyNS::MOVE_UP_END_FRAME);
			}
			// �ړ��\��������
			if (checkCanMove(spriteData.y - enemyNS::MOVE_SPEED * frameTime, spriteData.y, map)) {
				// ��Ɉړ�
				spriteData.y -= enemyNS::MOVE_SPEED * frameTime;
			}
			distanceCounter -= enemyNS::MOVE_SPEED * frameTime;
			break;
		case enemyNS::DOWN:
			if (direction != goalDirection)
			{
				changeDirection(enemyNS::MOVE_DOWN_START_FRAME, enemyNS::MOVE_DOWN_END_FRAME);
			}
			// �ړ��\��������
			if (checkCanMove(spriteData.y + enemyNS::MOVE_SPEED * frameTime, spriteData.y, map)) {
				// ���Ɉړ�
				spriteData.y += enemyNS::MOVE_SPEED * frameTime;
			}
			distanceCounter -= enemyNS::MOVE_SPEED * frameTime;
			break;
		case enemyNS::NONE:
			break;
		}
		// �U������ł͂Ȃ��͂��Ȃ̂Ńt���O���I�t
		isAttacked = false;
		// �摜�̂݃A�b�v�f�[�g
		Entity::updateOnlyImage(frameTime);
		break;
	case enemyNS::ATTACK:	// �U����
		// �A�j���[�V�������I�����Ă�����U���I��
		// ��Ԃ��ړ����ɖ߂�
		if (animComplete)
		{
			isAttacked = true;
			mode = imageNS::HORIZONTAL;
			direction = oldDirection;
			state = enemyNS::MOVE;
			loop = true;
			startFrame = oldStartFrame;
			endFrame = oldEndFrame;
			currentFrame = startFrame;
			animTimer = 0.0f;
			animComplete = false;
			setRect();
			Entity::updateOnlyImage(frameTime);
		}
		break;
	case enemyNS::PRE_ATTACK:
		// �^�C�}�[�����Z�A��莞�Ԃ𒴂�����U���Ɉڂ�
		attackTimer += frameTime;
		if (isAttacked && attackTimer > enemyNS::CONTINUE_ATTACK_TIME)
		{
			attackTimer = 0.0f;
			state = enemyNS::ATTACK;
		}
		else if (!isAttacked && attackTimer > enemyNS::ATTACK_TIME)
		{
			attackTimer = 0.0f;
			state = enemyNS::ATTACK;
		}
		break;
	case enemyNS::GAURD:
		break;
	default:
		break;
	}
	// ��ԑJ�ڌ�̏���
	switch (state)
	{
	case enemyNS::MOVE:
		updateMoving(frameTime);
		break;
	case enemyNS::ATTACK:
		updateAttacking(frameTime);
		break;
	case enemyNS::PRE_ATTACK:
		Entity::updateWithoutImage(frameTime);
		break;
	case enemyNS::GAURD:
		Entity::update(frameTime);
		break;
	default:
		Entity::update(frameTime);
		break;
	}

	// �_���[�W���󂯂Ă���Ȃ��莞�Ԃ��ƂɃA�j���[�V������_��
	if (isDamaged)
	{
		damageTimer += frameTime;
		totalDamageTime += frameTime;
		if (damageTimer > 0.15f)
		{
			if (drawFlag)
				drawFlag = false;
			else
				drawFlag = true;
			damageTimer = 0.0f;
		}
		if (totalDamageTime > enemyNS::DAMAGE_TIME)
		{
			damageTimer = 0.0f;
			totalDamageTime = 0.0f;
			drawFlag = true;
			isDamaged = false;
		}
	}

	spriteData.x += frameTime * velocity.x;     // �L������X�����ɓ�����
	spriteData.y += frameTime * velocity.y;     // �L������Y�����ɓ�����
}

//==========================================================
// �ړ����̃A�b�v�f�[�g�֐�
//==========================================================
void Enemy::updateMoving(float frameTime)
{
	// �A�j���[�V�����̃A�b�v�f�[�g�͒P�Ƃōs����̂ł���ȊO���A�b�v�f�[�g
	Entity::updateWithoutImage(frameTime);
}

//==========================================================
// �U�����̃A�b�v�f�[�g�֐�
//==========================================================
void Enemy::updateAttacking(float frameTime)
{
	Entity::update(frameTime);
}

//==========================================================
// �_���[�W
//==========================================================
void Enemy::damage(WEAPON weapon)
{
	switch (weapon)
	{
	case FIRE:
		break;
	case BRAVE_ATTACK:
		health -= braveNS::ATTACK_DAMAGE;
		break;
	case BRAVE_SECOND_ATTACK:
		break;
	case ENEMY_ATTACK:
		break;
	default:
		break;
	}
	if (health <= 0)
		dead();
	isDamaged = true;
}

//==========================================================
// ���S���ɌĂяo�����֐�
//==========================================================
void Enemy::dead()
{
	active = false;
	visible = false;
	health = 0;
}
//==========================================================
// �l�H�m�\
//==========================================================
void Enemy::ai(float frameTime, Entity &ent)
{
	// �U�����Ȃ�΍s���I���͍s��Ȃ�
	if (state == enemyNS::ATTACK || state == enemyNS::PRE_ATTACK)
	{
		return;
	}
	// �߂��Ƀv���C���[������ꍇ�A
	if (nearPlayer)
	{
		// �U�����O���[�h�Ƀ`�F���W
		loop = false;
		distanceCounter = 0.0f;
		goalDirection = enemyNS::NONE;
		state = enemyNS::PRE_ATTACK;
		mode = imageNS::VERTICAL;
		// �A�j���[�V�����I�����Ƀt���[����߂����߂ɕۑ�
		oldStartFrame = startFrame;
		oldEndFrame = endFrame;
		oldDirection = direction;
		// �����ɉ����ăA�j���[�V������؂�ւ�
		switch (direction)
		{
		case enemyNS::LEFT:
			startFrame = enemyNS::ATTACK_LEFT_START_FRAME;
			endFrame = enemyNS::ATTACK_LEFT_END_FRAME;
			break;
		case enemyNS::RIGHT:
			startFrame = enemyNS::ATTACK_RIGHT_START_FRAME;
			endFrame = enemyNS::ATTACK_RIGHT_END_FRAME;
			break;
		case enemyNS::UP:
			startFrame = enemyNS::ATTACK_UP_START_FRAME;
			endFrame = enemyNS::ATTACK_UP_END_FRAME;
			break;
		case enemyNS::DOWN:
			startFrame = enemyNS::ATTACK_DOWN_START_FRAME;
			endFrame = enemyNS::ATTACK_DOWN_END_FRAME;
			break;
		default:
			break;
		}
		currentFrame = startFrame;
		animTimer = 0.0f;
		setRect();
		return;
	}
	// ����ȊO�͈ړ����[�h�ɐ؂�ւ�
	state = enemyNS::MOVE;
	// �v���C���̌��݈ʒu���擾
	int playerX = ent.getX();
	int playerY = ent.getY();
	int subX = playerX - getX();
	int subY = playerY - getY();
	
	// ����������32�s�N�Z���ړ����邲�Ƃɕ��������߂Ȃ���
	if (distanceCounter <=  0.0f)
	{
		distanceCounter = 32.0f;
		if (abs(subX) > abs(subY))
		{
			if (subX > 0)
				goalDirection = enemyNS::RIGHT;
			else
				goalDirection = enemyNS::LEFT;
			return;
		}
		else
		{
			if (subY > 0)
				goalDirection = enemyNS::DOWN;
			else
				goalDirection = enemyNS::UP;
			return;
		}
	}
}

//==========================================================
// 
//==========================================================
void Enemy::changeDirection(int strF, int endF)
{
	direction = goalDirection;
	startFrame = strF;
	endFrame = endF;
	currentFrame = startFrame;
	animTimer = 0.0f;
	setRect();
}

//==========================================================
// �ړ��\���`�F�b�N
//==========================================================
bool Enemy::checkCanMove(float x, float y, Map *map)
{
	// 1�}�X32pixel�̂���32�Ŋ���
	// -16�͂߂荞�݂�h�~���邽�߂ɔ��}�X�������Ă�
	// +���͔������c
	int map_x = (int)((x - 8) / 32) + 1;
	int map_y = (int)((y - 8) / 32) + 2;

	if (map_x <= 0)
		map_x = 0;
	if (map_x >= mapNS::MAP_WIDTH)
		map_x = mapNS::MAP_WIDTH - 1;
	if (map_y <= 0)
		map_y = 0;
	if (map_y >= mapNS::MAP_HEIGHT)
		map_y = mapNS::MAP_HEIGHT - 1;

	if (map->getMapCol(map_y, map_x) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}