// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.2.1
// LVGL VERSION: 8.2.0
// PROJECT: Lvgl_Music_Player

#ifndef _LVGL_MUSIC_PLAYER_UI_H
#define _LVGL_MUSIC_PLAYER_UI_H

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

extern lv_obj_t *ui_Screen1;
extern lv_obj_t *ui_ImageCover;
extern lv_obj_t *ui_ArcTime;
extern lv_obj_t *ui_LabelLyric;




LV_FONT_DECLARE( ui_font_NotoSerifCJKhk);


void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
