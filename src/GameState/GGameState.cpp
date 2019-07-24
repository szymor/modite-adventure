#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GPlayerProcess.h"

GGameState::GGameState() : BGameEngine(gViewPort) {
  gViewPort->SetRect(TRect(0,0, 10*32, 7*32));
  gResourceManager.LoadBitmap(EXAMPLE_TILES_BMP, BKG_SLOT, IMAGE_32x32);
  gResourceManager.LoadBitmap(CHARA_HERO_BMP, PLAYER_SLOT, IMAGE_64x64);
  gResourceManager.LoadRaw(EXAMPLE_JAYS_EXAMPLE_LAYER_LAYER_1_MAP001_STM, MAP_SLOT);
  gResourceManager.LoadRaw(JAYS_EXAMPLE_TLC, CODES_SLOT);
  mPlayfield = mGamePlayfield = new GGamePlayfield(gViewPort, BKG_SLOT, MAP_SLOT, CODES_SLOT);
  mPlayerProcess = new GPlayerProcess(this, mGamePlayfield);
  AddProcess(mPlayerProcess);
}

GGameState::~GGameState() {
  gResourceManager.ReleaseRawSlot(CODES_SLOT);
  gResourceManager.ReleaseRawSlot(MAP_SLOT);
  gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT);
  gResourceManager.ReleaseBitmapSlot(BKG_SLOT);
}

void GGameState::PreRender() {
//  gDisplay.renderBitmap->Clear();
}

