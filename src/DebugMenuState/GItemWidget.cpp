#include "GItemWidget.h"
#include "GPlayer.h"
#include "Items.h"
#include "GStatProcess.h"

GItemWidget::GItemWidget(TInt aItem, BGameEngine *aState) : GButtonWidget("") {
  mItem = aItem;
  mState = aState;

  char s[128];
  GInventoryItem *i = GPlayer::mInventoryList.FindItem(mItem);
  if (i) {
    sprintf(s, "%02d %-18s", i->mCount, itemNames[mItem]);
  } else {
    sprintf(s, "00 %-18s", itemNames[mItem]);
  }
  mText = strdup(s);
}

GItemWidget::~GItemWidget() {
  delete mText;
}

TInt GItemWidget::Render(TInt aX, TInt aY) {
  aY += 20;
  GButtonWidget::Render(aX, aY);
  return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight - 4;
}

void GItemWidget::Run() {
  if (!IsActive()) {
    return;
  }

  if (gControls.WasPressed(BUTTONA | BUTTON_START)) {
    Select();
  }

  // Remove items from inventory
  if (gControls.WasPressed(BUTTONB)) {
    GInventoryItem *i = GPlayer::mInventoryList.FindItem(mItem);

    if (i) {
      char s[128];
      sprintf(s, "Removed %s", itemNames[mItem]);
      auto *p = new GStatProcess(GPlayer::mSprite->x - 20, GPlayer::mSprite->y, s);
      p->SetImageNumber(items[mItem]);
      p->SetTimeout(5 * FRAMES_PER_SECOND);
      mState->AddProcess(p);

      if (i->mCount > 1) {
        i->mCount--;
        sprintf(s, "%02d %-18s", i->mCount, itemNames[mItem]);
      } else {
        GPlayer::mInventoryList.RemoveNode(i);
        GPlayer::mInventoryList.mCount--;
        sprintf(s, "00 %-18s", itemNames[mItem]);
      }
      mText = strdup(s);
    }
    GPlayer::mInventoryList.Dump();
#ifdef ENABLE_AUDIO
    gSoundPlayer.SfxMenuIn();
#endif
  }
}

void GItemWidget::Select() {
  auto *p = new GStatProcess(GPlayer::mSprite->x, GPlayer::mSprite->y, itemNames[mItem]);
  p->SetImageNumber(items[mItem]);
  p->SetTimeout(5 * FRAMES_PER_SECOND);
  mState->AddProcess(p);
  GPlayer::mInventoryList.PickupItem(mItem);

  char s[128];
  GInventoryItem *i = GPlayer::mInventoryList.FindItem(mItem);
  sprintf(s,  "%02d %-18s", i->mCount, itemNames[mItem]);
  mText = strdup(s);

  GPlayer::mInventoryList.Dump();
#ifdef ENABLE_AUDIO
  gSoundPlayer.SfxMenuIn();
#endif
}