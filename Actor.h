#pragma once
#include <raylib.h>
#include <vector>

/// <summary>
/// �Q�[���I�u�W�F�N�g�S�ʂ̊��N���X
/// </summary>
class Actor
{
public:	
	// bit�t���O�ŊǗ�
	enum Type
	{
		Eplayer			= 1 << 0, // 000001
		Eenemy			= 1 << 1, // 000010
		Eweapon			= 1 << 2, // 000100
		EstageObject	= 1 << 3, // 001000
		Eexplosion		= 1 << 4, // 010000
		Eitem			= 1 << 5, // 100000
	};
	enum State
	{
		Ealive,
		Edead		// ���̏�Ԃ�Actor��delete�����
	};
	Actor(class Sequence* sequenece, Type type);
	virtual ~Actor();
	
	virtual void input();
	virtual void update();

	void addComponent(class Component* component);
	void removeComponent(class Component* component);

	Type getType() const { return mType; }
	State getState() const { return mState; }
	const Vector2& getPosition() const { return mPosition; }
	virtual const Rectangle& getRectangle() const { return mRectangle; }
	Sequence* getSequence() const { return mSequence; }
	float getScale() const { return mScale; }
	int getForward() const { return mForward; }

	void setState(State state) { mState = state; }
	// ���̊֐���ǂ񂾌��computeRectangle()���ĂԂ悤�ɂ���
	void setPosition(Vector2 pos) { mPosition = pos; }
	void setForward(int forward) { mForward = forward; }
	// setPosition�Ɠ����ɌĂяo��
	virtual void computeRectangle() {} ;

protected:
	Type mType;
	State mState;
	Sequence* mSequence;
	Vector2 mPosition;	  // �e�N�X�`���̒��S���W
	float mScale;		  // �A�N�^�[�̊g�嗦,1�ȊO�ɂ���Ȃ�computeRectangle()�Œ��ӂ���
	int mForward;		  // +1 : �E����, -1 : ������
	// �����蔻��p�̋�`,�ۂƂ��F�X�g���Ȃ�h���N���X�Ɏ����Ă���
	Rectangle mRectangle;
	std::vector<class Component*> mComponents;
};

