#include "GTurretProcess.h"
#include "GEnemyProjectileProcess.h"
#include "GPlayer.h"

#define DEBUGME
#undef DEBUGME

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

const TInt16 IDLE_TIMEOUT = 30 * FACTOR;

const TInt IDLE_SPEED = 5 * FACTOR;
const TInt TAUNT_SPEED = 5 * FACTOR;
const TInt SELECT_SPEED = 5 * FACTOR;
const TInt ATTACK_SPEED = 2 * FACTOR;
const TInt HIT_SPEED = 1 * FACTOR;
const TInt WALK_SPEED = 2 * FACTOR;
const TInt DEATH_SPEED = 5 * FACTOR;

const TFloat VELOCITY = 4 / FACTOR;
const TFloat ATTACK_DISTANCE = 6 * TILESIZE;
const TFloat FLEE_DISTANCE = 3 * TILESIZE;

// region  ANIMATIONS {{{

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

/**
 * Animations
 */

/*
 ___    _ _         ______       _           _           _
|_ _|__| | | ___   / / ___|  ___| | ___  ___| |_ ___  __| |
 | |/ _` | |/ _ \ / /\___ \ / _ \ |/ _ \/ __| __/ _ \/ _` |
 | | (_| | |  __// /  ___) |  __/ |  __/ (__| ||  __/ (_| |
|___\__,_|_|\___/_/  |____/ \___|_|\___|\___|\__\___|\__,_|
 */

ANIMSCRIPT idleAnimation[] = {
  ABITMAP(TURRET_SLOT),
  ALABEL,
  ASTEP(40, IMG_GOBLIN_SNIPER_IDLE),
  ASTEP(4, IMG_GOBLIN_SNIPER_IDLE + 1),
  ASTEP(40, IMG_GOBLIN_SNIPER_IDLE + 2),
  ASTEP(4, IMG_GOBLIN_SNIPER_IDLE + 1),
  ALOOP
};

static ANIMSCRIPT selectAnimation[] = {
  ABITMAP(TURRET_SLOT),
  ALABEL,
  ASTEP(SELECT_SPEED, IMG_GOBLIN_SNIPER_SELECTED + 0),
  ASTEP(SELECT_SPEED, IMG_GOBLIN_SNIPER_SELECTED + 1),
  ASTEP(SELECT_SPEED, IMG_GOBLIN_SNIPER_SELECTED + 2),
  ALOOP
};

static ANIMSCRIPT tauntAnimation[] = {
  ABITMAP(TURRET_SLOT),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_SNIPER_IDLE + 0),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_SNIPER_IDLE + 1),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_SNIPER_IDLE + 2),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_SNIPER_IDLE + 4),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_SNIPER_IDLE + 5),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_SNIPER_IDLE + 6),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_SNIPER_IDLE + 5),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_SNIPER_IDLE + 6),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_SNIPER_IDLE + 5),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_SNIPER_IDLE + 4),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_SNIPER_IDLE + 2),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_SNIPER_IDLE + 1),
  ASTEP(TAUNT_SPEED, IMG_GOBLIN_SNIPER_IDLE + 0),
  AEND,
};

static ANIMSCRIPT deathAnimation[] = {
  ABITMAP(TURRET_SLOT),
  ASTEP(DEATH_SPEED, IMG_GOBLIN_SNIPER_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_GOBLIN_SNIPER_WALK_UP + 0),
  AFLIP(DEATH_SPEED, IMG_GOBLIN_SNIPER_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_GOBLIN_SNIPER_WALK_DOWN + 0),
  ASTEP(DEATH_SPEED, IMG_GOBLIN_SNIPER_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_GOBLIN_SNIPER_WALK_UP + 0),
  AFLIP(DEATH_SPEED, IMG_GOBLIN_SNIPER_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_GOBLIN_SNIPER_WALK_DOWN + 0),
  AEND
};

/*
 ____
|  _ \  _____      ___ __
| | | |/ _ \ \ /\ / / '_ \
| |_| | (_) \ V  V /| | | |
|____/ \___/ \_/\_/ |_| |_|
*/

static ANIMSCRIPT idleDownAnimation[] = {
  ABITMAP(TURRET_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_GOBLIN_SNIPER_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_GOBLIN_SNIPER_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_GOBLIN_SNIPER_IDLE + 2),
  ALOOP
};

static ANIMSCRIPT walkDownAnimation1[] = {
  ABITMAP(TURRET_SLOT),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_DOWN + 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_DOWN + 1),
  AEND
};

static ANIMSCRIPT walkDownAnimation2[] = {
  ABITMAP(TURRET_SLOT),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_DOWN + 2),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_DOWN + 3),
  AEND
};

static ANIMSCRIPT attackDownAnimation[] = {
  ABITMAP(TURRET_SLOT),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_SNIPER_ATTACK_DOWN + 0),
  ASTEP(ATTACK_SPEED * 2, IMG_GOBLIN_SNIPER_ATTACK_DOWN + 1),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_SNIPER_ATTACK_DOWN + 2),
  AEND
};

static ANIMSCRIPT hitDownAnimation[] = {
  ABITMAP(TURRET_SLOT),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 3),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 1),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 2),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 3),
  AFILL(-1),
  AEND
};

/*
 _          __ _
| |    ___ / _| |_
| |   / _ \ |_| __|
| |__|  __/  _| |_
|_____\___|_|  \__|

 */

static ANIMSCRIPT idleLeftAnimation[] = {
  ABITMAP(TURRET_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_GOBLIN_SNIPER_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_GOBLIN_SNIPER_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_GOBLIN_SNIPER_IDLE + 2),
  ALOOP
};

static ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(TURRET_SLOT),
  AFLIP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_RIGHT + 0),
  AFLIP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_RIGHT + 1),
  AEND
};

static ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(TURRET_SLOT),
  AFLIP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_RIGHT + 2),
  AFLIP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_RIGHT + 3),
  AEND
};

static ANIMSCRIPT attackLeftAnimation[] = {
  ABITMAP(TURRET_SLOT),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_SNIPER_ATTACK_RIGHT + 0),
  AFLIP(ATTACK_SPEED * 2, IMG_GOBLIN_SNIPER_ATTACK_RIGHT + 1),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_SNIPER_ATTACK_RIGHT + 2),
  AEND
};

static ANIMSCRIPT hitLeftAnimation[] = {
  ABITMAP(TURRET_SLOT),
  AFILL(COLOR_WHITE),
  AFLIP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_RIGHT + 3),
  AFILL(-1),
  AFLIP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_RIGHT + 0),
  AFILL(COLOR_WHITE),
  AFLIP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_RIGHT + 1),
  AFILL(-1),
  AFLIP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_RIGHT + 2),
  AFILL(COLOR_WHITE),
  AFLIP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_RIGHT + 3),
  AFILL(-1),
  AEND
};

/*
 ____  _       _     _
|  _ \(_) __ _| |__ | |_
| |_) | |/ _` | '_ \| __|
|  _ <| | (_| | | | | |_
|_| \_\_|\__, |_| |_|\__|
         |___/
 */

static ANIMSCRIPT idleRightAnimation[] = {
  ABITMAP(TURRET_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_GOBLIN_SNIPER_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_GOBLIN_SNIPER_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_GOBLIN_SNIPER_IDLE + 2),
  ALOOP
};

static ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(TURRET_SLOT),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_RIGHT + 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_RIGHT + 1),
  AEND
};

static ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(TURRET_SLOT),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_RIGHT + 2),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_RIGHT + 3),
  AEND
};

static ANIMSCRIPT attackRightAnimation[] = {
  ABITMAP(TURRET_SLOT),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_SNIPER_ATTACK_RIGHT + 0),
  ASTEP(ATTACK_SPEED * 2, IMG_GOBLIN_SNIPER_ATTACK_RIGHT + 1),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_SNIPER_ATTACK_RIGHT + 2),
  AEND
};

static ANIMSCRIPT hitRightAnimation[] = {
  ABITMAP(TURRET_SLOT),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_RIGHT + 3),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_RIGHT + 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_RIGHT + 1),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_RIGHT + 2),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_RIGHT + 3),
  AFILL(-1),
  AEND
};

/*
 _   _
| | | |_ __
| | | | '_ \
| |_| | |_) |
 \___/| .__/
      |_|
 */

static ANIMSCRIPT idleUpAnimation[] = {
  ABITMAP(TURRET_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_GOBLIN_SNIPER_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_GOBLIN_SNIPER_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_GOBLIN_SNIPER_IDLE + 2),
  ALOOP
};

static ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(TURRET_SLOT),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_UP + 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_UP + 1),
  AEND
};

static ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(TURRET_SLOT),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_UP + 2),
  ASTEP(WALK_SPEED, IMG_GOBLIN_SNIPER_WALK_UP + 3),
  AEND
};

static ANIMSCRIPT attackUpAnimation[] = {
  ABITMAP(TURRET_SLOT),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_SNIPER_ATTACK_UP + 0),
  ASTEP(ATTACK_SPEED * 2, IMG_GOBLIN_SNIPER_ATTACK_UP + 1),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_SNIPER_ATTACK_UP + 2),
  AEND
};

static ANIMSCRIPT hitUpAnimation[] = {
  ABITMAP(TURRET_SLOT),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_UP + 3),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_UP + 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_UP + 1),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_UP + 2),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_UP + 3),
  AFILL(-1),
  AEND
};

static ANIMSCRIPT hitSpellAnimation[] = {
  ABITMAP(TURRET_SLOT),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 3),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 0),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 3),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 3),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_GOBLIN_SNIPER_DAMAGE_DOWN + 3),
  AEND,
};

/* endregion }}} */

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

// constructor
GTurretProcess::GTurretProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParam)
    : GEnemyProcess(aGameState, aIp, TURRET_SLOT, aParam, VELOCITY, ATTR_TURRET) {
  mGameState = aGameState;

  mSprite->Name("ENEMY TURRET");
  mSprite->x = aX;
  mSprite->y = aY;
  mStartX = mSprite->x = aX;
  mStartY = mSprite->y = aY;
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(CHARA_GOBLIN_SNIPER_BMP_SPRITES);
  mSprite->vx = mSprite->vy = 0;

  NewState(IDLE_STATE, DIRECTION_DOWN);
}

GTurretProcess::~GTurretProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

void GTurretProcess::Idle(DIRECTION aDirection) {
  mStateTimer = IDLE_TIMEOUT;
}

void GTurretProcess::Taunt(DIRECTION aDirection) {
  mSprite->StartAnimation(tauntAnimation);
}

void GTurretProcess::Walk(DIRECTION aDirection) {
  // Turrets don't walk
}

void GTurretProcess::Attack(DIRECTION aDirection) {

  // fire 1-3 projectiled
  TFloat xx = mSprite->x,
         yy = mSprite->y;

  // Angles are in radians
  const TFloat angleToPlayer = atan2(GPlayer::mSprite->y - yy, GPlayer::mSprite->x - xx);

  TInt attackType = Random() & TUint8(3);

  // 25% chance for multi-arrow attack
  if (attackType < 3) {
    mGameState->AddProcess(new GEnemyProjectileProcess(mGameState, xx + 16, yy - 16, angleToPlayer));
  } else {
    const TFloat step = 22.5 * (M_PI/180);
    const TFloat angles[3] = { angleToPlayer, angleToPlayer + step, angleToPlayer - step };
    mGameState->AddProcess(new GEnemyProjectileProcess(mGameState, xx + 16, yy - 16, angles[0]));
    mGameState->AddProcess(new GEnemyProjectileProcess(mGameState, xx + 16, yy - 16, angles[1]));
    mGameState->AddProcess(new GEnemyProjectileProcess(mGameState, xx + 16, yy - 16, angles[2]));
  }

  switch (mSprite->mDirection) {
    case DIRECTION_UP:
      mSprite->StartAnimation(attackUpAnimation);
      break;
    case DIRECTION_DOWN:
      mSprite->StartAnimation(attackDownAnimation);
      break;
    case DIRECTION_LEFT:
      mSprite->StartAnimation(attackLeftAnimation);
      break;
    case DIRECTION_RIGHT:
      mSprite->StartAnimation(attackRightAnimation);
      break;
    default:
      Panic("GTurretProcess no attack direction\n");
      break;
  }
}

void GTurretProcess::Hit(DIRECTION aDirection) {
  switch (aDirection) {
    case DIRECTION_UP:
      mSprite->StartAnimation(hitUpAnimation);
      break;
    case DIRECTION_DOWN:
      mSprite->StartAnimation(hitDownAnimation);
      break;
    case DIRECTION_LEFT:
      mSprite->StartAnimation(hitLeftAnimation);
      break;
    case DIRECTION_RIGHT:
      mSprite->StartAnimation(hitRightAnimation);
      break;
    case DIRECTION_SPELL:
      mSprite->StartAnimation(hitSpellAnimation);
      break;
    default:
      Panic("GTurretProcess no Hit direction\n");
      break;
  }
}

void GTurretProcess::Death(DIRECTION aDirection) {
  mSprite->StartAnimation(deathAnimation);
}

TBool GTurretProcess::MaybeAttack() {
  if (mPlayerSprite->mInvulnerable) {
    return EFalse;
  }

  const TFloat distance = sqrt(pow(GPlayer::mSprite->x - mSprite->x, 2) + pow(GPlayer::mSprite->y - mSprite->y, 2));

#ifdef DEBUG_MODE
  // printf("Distance from sniper to player %f\n", distance);
#endif

  if (distance > ATTACK_DISTANCE || distance < FLEE_DISTANCE) {
    return EFalse;
  }

  TRect myRect, hisRect;
  mSprite->GetRect(myRect);
  mPlayerSprite->GetRect(hisRect);

  if (ABS(myRect.y2 - hisRect.y2) < ABS(myRect.x1 - hisRect.x1)) {
    if (myRect.x1 >= hisRect.x2) {
      // to right of player
      if (--mAttackTimer <= 0) {
        NewState(ATTACK_STATE, DIRECTION_LEFT);
      }
      return ETrue;
    }
    if (myRect.x2 <= hisRect.x1) {
      // to left of player
      if (--mAttackTimer <= 0) {
        NewState(ATTACK_STATE, DIRECTION_RIGHT);
      }
      return ETrue;
    }
  } else {
    if (myRect.y1 >= hisRect.y2) {
      // below player
      if (--mAttackTimer <= 0) {
        NewState(ATTACK_STATE, DIRECTION_UP);
      }
      return ETrue;
    }
    if (myRect.y2 <= hisRect.y1) {
      // above player
      if (--mAttackTimer <= 0) {
        NewState(ATTACK_STATE, DIRECTION_DOWN);
      }
      return ETrue;
    }
  }

  mAttackTimer = 1;
  return EFalse;
}

TBool GTurretProcess::RunAfter() {
  // Make sniper stationary (turret) for debugging
  // mSprite->vx = 0;
  // mSprite->vy = 0;
  return GEnemyProcess::RunAfter();
}
