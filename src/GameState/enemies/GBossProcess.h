#ifndef MODITE_GBOSSPROCESS_H
#define MODITE_GBOSSPROCESS_H

#include "GEnemyProcess.h"

class GBossProcess : public GEnemyProcess {
public:
  GBossProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aSlot, TUint16 aAttribute);
  ~GBossProcess() OVERRIDE;

public:
  void DeathAnimationDone() {
    mDeathCounter--;
  }

protected:
  TInt16 mStateTimer, mAttackTimer, mHitTimer; // state timer variables
  TInt16 mDeathCounter; // number of death animation processes spawned/outstanding
  TInt16 mSpellCounter; // number of spell animation processes spawned/outstanding
};


#endif //MODITE_GBOSSPROCESS_H
