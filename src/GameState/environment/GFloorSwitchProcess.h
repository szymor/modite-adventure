#ifndef MODITE_GFLOOR_SWITCHPROCESS_H
#define MODITE_GFLOOR_SWITCHPROCESS_H

#include <BProcess.h>
#include "GAnchorSprite.h"
#include "GGameState.h"

class GFloorSwitchProcess : public BProcess {
public:
  GFloorSwitchProcess(GGameState *aGameState, TUint16 aParam, TFloat aX, TFloat aY, TBool aWooden);
  ~GFloorSwitchProcess() OVERRIDE;
public:
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;

protected:
  GGameState *mGameState;
  TUint16 mImage;
  TUint16 mParam;
  TUint8 mGroup, mOrder;
  TBool mState, mAnimating;
  GAnchorSprite *mSprite;
};

#endif //MODITE_GFLOOR_SWITCHPROCESS_H
