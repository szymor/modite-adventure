#ifndef MODITE_GRATPROCESS_H
#define MODITE_GRATPROCESS_H

#include <GameState/enemies/GEnemyProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

class GRatProcess : public GEnemyProcess {
public:
  GRatProcess(GGameState *aGameState, GGamePlayfield *aPlayfield, TFloat aX, TFloat aY);

  ~GRatProcess() OVERRIDE;

protected:
  void NewState(TUint16 aState, DIRECTION aDirection) OVERRIDE;

protected:
  TBool CanWalk(TInt aDirection);

  TBool IdleState() OVERRIDE;

  TBool WalkState() OVERRIDE;

  TBool HitState() OVERRIDE;

  TBool DeathState() OVERRIDE;

protected:
  TInt16 mStateTimer;
};

#endif // MODITE_GRATPROCESS_H
