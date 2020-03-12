#include "GItemProcess.h"
#include "GResources.h"
#include "GPlayer.h"
#include "Items.h"
#include "GStatSprite.h"
#include "GStatProcess.h"

GItemProcess *GItemProcess::SpawnItem(GGameState *aGameState, TInt aIp, TInt aItemNumber, TFloat aX, TFloat aY) {
  if (aItemNumber && aItemNumber < sizeof(items)) {
    GItemProcess *p = new GItemProcess(aGameState, aIp, aItemNumber, aX, aY);
    aGameState->AddProcess(p);
    return p;
  }
  else {
    return ENull;
  }
}

GItemProcess::GItemProcess(GGameState *aGameState, TInt aIp, TInt aItemNumber, TFloat aX, TFloat aY) 
  : GProcess(aItemNumber) {
  mGameState = aGameState;
  mSaveToStream = EFalse;
  mIp = aIp;
  mItemNumber = aItemNumber;
  if (mItemNumber && mItemNumber < sizeof(items)) {
    mSprite = new GAnchorSprite(mGameState, ITEM_PRIORITY, ENVIRONMENT_SLOT, items[mItemNumber], STYPE_OBJECT);
    char work[2048];
    sprintf(work, "ITEM %s(%d)", itemNames[mItemNumber], mItemNumber);
    mSprite->Name(work);
    mSprite->SetFlags(SFLAG_BELOW);
    mSprite->pri = PRIORITY_BELOW + 1;
    mSprite->cMask = STYPE_PLAYER;
    mSprite->w = mSprite->h = 32;
    mSprite->cx = -16;
    mSprite->x = aX;
    mSprite->y = aY;
    mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(GLOBAL_OBJECT_LAYER_BMP_SPRITES);
    mGameState->AddSprite(mSprite);
    // TODO: uncomment this if we want to spawn items in the dungeon that can be picked up and want to retain 
    //       whether already picked up
//    mGameState->EndProgram(mIp, ATTR_KEEP, mItemNumber);

    BObjectProgram *program = mGameState->mGamePlayfield->mObjectProgram;
    mFromPot = (program[mIp].mCode & TUint32(0xffff)) == ATTR_POT;
  }
  else {
    Panic("GItemProcess constructor: invalid item number: %d\n", mItemNumber);
  }
}

GItemProcess::~GItemProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GItemProcess::RunBefore() {
  return ETrue;
}

TBool GItemProcess::RunAfter() {
  if (mSprite->cType & STYPE_PLAYER) {

    // Pick up a heart, gain life and move on. No status.
    if (mItemNumber == ITEM_HEART) {
      GPlayer::AddHitPoints(30);
      gSoundPlayer.SfxItemHeart();
      return EFalse;
    }


    auto *p = new GStatProcess(mSprite->x, mSprite->y, itemNames[mItemNumber]);
    p->SetImageNumber(mSprite->mImageNumber);
    p->SetTimeout(FRAMES_PER_SECOND);
    mGameState->AddProcess(p);

    GPlayer::mInventoryList.PickupItem(mItemNumber);
    // mark ObjectProgram at mIp that item has been picked up (don't persist it)
    if (mIp != -1 && !mFromPot) {
      printf("pickedup %d %d\n", mFromPot, mIp);
      mGameState->EndProgram(mIp, ATTR_KEEP, ATTR_GONE);
    }
    
    GPlayer::mInventoryList.Dump();
    gSoundPlayer.SfxItemPickupGeneric();

    return EFalse;
  }
  return ETrue;
}
