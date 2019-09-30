#ifndef MODITE_GORCPROCESS_H
#define MODITE_GORCPROCESS_H

#include <GameState/enemies/GEnemyProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

class GOrcProcess : public GEnemyProcess {
public:
  GOrcProcess(GGameState *aGameState, GGamePlayfield *aPlayfield, TFloat aX, TFloat aY);

  ~GOrcProcess() OVERRIDE;

protected:
  void NewState(TUint16 aState, DIRECTION aDirection) override;

  TBool IdleState() OVERRIDE;

  TBool WalkState() OVERRIDE;

  TBool DeathState() OVERRIDE;

protected:
  TInt16 mStateTimer;
};

#endif //MODITE_GORCPROCESS_H
