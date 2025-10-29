#pragma once
#include "Actor.h"

/// <summary>
/// •Ší
/// </summary>
class WeaponActor :
    public Actor
{
public:
    ~WeaponActor();
    virtual void onStartAttack() {};
    virtual void onHit(class Actor* target);

    void setMultiplier(float multiplier) { mAttackMultiplier = multiplier; }

protected:
    WeaponActor(class Sequence* sequence, Type owner);
    float mAttackPower;         // Šî‘bUŒ‚—Í
    float mAttackMultiplier;    // Šî‘bUŒ‚—Í‚ğæZ‚Å‘‰Á‚³‚¹‚é
    // —^‚¦‚éƒ_ƒ[ƒW‚ÍˆÈ‰º‚Ì—l‚É‚È‚é
    // mAttackPower * mAttackMultiplier
};

