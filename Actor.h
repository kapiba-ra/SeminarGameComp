#pragma once
#include <raylib.h>
#include <vector>

/// <summary>
/// ゲームオブジェクト全般の基底クラス
/// </summary>
class Actor
{
public:	
	// bitフラグで管理
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
		Edead		// この状態のActorはdeleteされる
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
	// この関数を読んだ後はcomputeRectangle()を呼ぶようにする
	void setPosition(Vector2 pos) { mPosition = pos; }
	void setForward(int forward) { mForward = forward; }
	// setPositionと同時に呼び出す
	virtual void computeRectangle() {} ;

protected:
	Type mType;
	State mState;
	Sequence* mSequence;
	Vector2 mPosition;	  // テクスチャの中心座標
	float mScale;		  // アクターの拡大率,1以外にするならcomputeRectangle()で注意する
	int mForward;		  // +1 : 右向き, -1 : 左向き
	// 当たり判定用の矩形,丸とか色々使うなら派生クラスに持っていく
	Rectangle mRectangle;
	std::vector<class Component*> mComponents;
};

