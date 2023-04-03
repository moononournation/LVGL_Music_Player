// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.2.3
// LVGL version: 8.2.0
// Project name: Winamp480x320

#ifndef _WINAMP480X320_UI_H
#define _WINAMP480X320_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
  #if __has_include("lvgl.h")
    #include "lvgl.h"
  #elif __has_include("lvgl/lvgl.h")
    #include "lvgl/lvgl.h"
  #else
    #include "lvgl.h"
  #endif
#else
  #include "lvgl.h"
#endif

#include "ui_events.h"
extern lv_obj_t *ui_Screen1;
extern lv_obj_t *ui_ImageFrame;
extern lv_obj_t *ui_RollerPlayList;
extern lv_obj_t *ui_ScaleVolume;
extern lv_obj_t *ui_ScaleProgress;
extern lv_obj_t *ui_RollerLyrics;
extern lv_obj_t *ui_Panel2;
extern lv_obj_t *ui_ImageCover;
extern lv_obj_t *ui_LabelProgress;
extern lv_obj_t *ui_LabelPlaying;
extern lv_obj_t *ui_ButtonPrev;
extern lv_obj_t *ui_ButtonPlay;
extern lv_obj_t *ui_ButtonPause;
extern lv_obj_t *ui_ButtonStop;
extern lv_obj_t *ui_ButtonNext;
extern lv_obj_t *ui____initial_actions0;


LV_IMG_DECLARE( ui_img_winamp480x320_png);   // assets/Winamp480x320.png
LV_IMG_DECLARE( ui_img_volume_png);   // assets/volume.png
LV_IMG_DECLARE( ui_img_progress_png);   // assets/progress.png
LV_IMG_DECLARE( ui_img_prev_png);   // assets/prev.png
LV_IMG_DECLARE( ui_img_play_png);   // assets/play.png
LV_IMG_DECLARE( ui_img_pause_png);   // assets/pause.png
LV_IMG_DECLARE( ui_img_stop_png);   // assets/stop.png
LV_IMG_DECLARE( ui_img_next_png);   // assets/next.png
LV_IMG_DECLARE( ui_img_winamp480x480_png);   // assets/Winamp480x480.png


LV_FONT_DECLARE( ui_font_Cubic11);
LV_FONT_DECLARE( ui_font_NotoSansCJKhk);
LV_FONT_DECLARE( ui_font_NotoSerifCJKhk);


void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
