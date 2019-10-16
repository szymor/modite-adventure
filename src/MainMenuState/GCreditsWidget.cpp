#include "GCreditsWidget.h"
#include "Game.h"

GCreditsWidget::GCreditsWidget() : BButtonWidget("CREDITS", COLOR_TEXT, COLOR_TEXT_BG) {}

GCreditsWidget::~GCreditsWidget() {}

TInt GCreditsWidget::Render(TInt aX, TInt aY) {
  const BFont *f = gWidgetTheme.GetFont(WIDGET_TITLE_FONT);

  if (mActive) {
    gDisplay.renderBitmap->DrawStringShadow(ENull,
        STR_RIGHT_ARROW,
        f,
        aX - 16, aY,
        gWidgetTheme.GetInt(WIDGET_TEXT_BG),
        COLOR_TEXT_SHADOW,
        COLOR_TEXT_TRANSPARENT);
  }

  gDisplay.renderBitmap->DrawStringShadow(ENull,
      mText,
      f,
      aX, aY,
      gWidgetTheme.GetInt(WIDGET_TITLE_FG),
      COLOR_TEXT_SHADOW,
      COLOR_TEXT_TRANSPARENT,
      -6);

  return f->mHeight + 10;
}

void GCreditsWidget::Select() {
  gGame->SetState(GAME_STATE_CREDITS);

// TODO: @jaygarcia
#ifdef ENABLE_AUDIO
  gSoundPlayer.SfxMenuAccept();
#endif
}
