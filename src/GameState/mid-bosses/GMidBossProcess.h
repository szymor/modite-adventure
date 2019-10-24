#ifndef GMIDBOSSPROCESS_H
#define GMIDBOSSPROCESS_H

// see https://github.com/ModusCreateOrg/modite-adventure/wiki/Mid-Boss-Design-Guidelines

#include <BProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

enum {
  MB_IDLE_STATE,
  MB_WALK_STATE,
  MB_BALL_STATE,   // bounce of walls N times, hit player
  MB_RETURN_STATE, // returning to starting position
  MB_REVERT_STATE, // revert back to original shape
  MB_ATTACK_STATE, // projectile attack
  MB_HIT_STATE,
  MB_DEATH_STATE,
  MB_SPELL_STATE, // hit with magic spell
};

class GMidBossProcess : public BProcess {
public:
  GMidBossProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aSlot);

  ~GMidBossProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;

protected:
  TBool MaybeAttack();

  TBool MaybeHit();

protected:
  virtual void NewState(TUint16 aState, DIRECTION aDirection);

  virtual void Idle(DIRECTION aDirection) = 0;
  TBool IdleState();

  virtual void Walk(DIRECTION aDirection) = 0;
  TBool WalkState();

  virtual void Ball(DIRECTION aDirection) = 0;
  TBool BallState();

  virtual void Return(DIRECTION aDirection) = 0;
  TBool ReturnState();

  virtual void Revert(DIRECTION aDirection) = 0;
  TBool RevertState();

  virtual void Attack(DIRECTION aDirection) = 0;
  TBool AttackState();

  virtual void Hit(DIRECTION aDirection) = 0;
  TBool HitState();

  virtual void Death(DIRECTION aDirection) = 0;
  TBool DeathState();

  // hit by magic spell
  virtual void Spell(DIRECTION aDirection) = 0;
  TBool SpellState();

protected:
  GGameState *mGameState;
  GGamePlayfield *mPlayfield;
  GAnchorSprite *mSprite;
  TFloat mStartX, mStartY;
  TInt16 mState, mStep;
  TInt16 mAttackTimer;
  TInt16 mStateTimer;
  TInt16 mHitPoints;
};

#endif
