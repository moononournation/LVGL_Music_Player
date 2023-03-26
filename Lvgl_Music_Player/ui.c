// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.2.2
// LVGL version: 8.2.0
// Project name: Lvgl_Music_Player

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t *ui_Screen1;
lv_obj_t *ui_ScaleVolume;
lv_obj_t *ui_Panel2;
lv_obj_t *ui_ScaleProgress;
lv_obj_t *ui_ImageCover;
lv_obj_t *ui_RollerPlayList;
lv_obj_t *ui_RollerLyrics;
lv_obj_t *ui_LabelProgress;
lv_obj_t *ui_LabelDuration;
lv_obj_t *ui____initial_actions0;
const lv_img_dsc_t *Winamp480x_array[1] = {&ui_img_winamp480x320_png};
const lv_img_dsc_t *Winampx_array[1] = {&ui_img_winamp480x480_png};

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////

///////////////////// SCREENS ////////////////////
void ui_Screen1_screen_init(void)
{
ui_Screen1 = lv_obj_create(NULL);
lv_obj_clear_flag( ui_Screen1, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM );    /// Flags

ui_ScaleVolume = lv_arc_create(ui_Screen1);
lv_obj_set_width( ui_ScaleVolume, 460);
lv_obj_set_height( ui_ScaleVolume, 460);
lv_obj_set_align( ui_ScaleVolume, LV_ALIGN_CENTER );
lv_arc_set_range(ui_ScaleVolume, 0,21);
lv_arc_set_value(ui_ScaleVolume, 2);
lv_arc_set_bg_angles(ui_ScaleVolume,205,335);

ui_Panel2 = lv_obj_create(ui_Screen1);
lv_obj_set_width( ui_Panel2, 480);
lv_obj_set_height( ui_Panel2, 40);
lv_obj_set_x( ui_Panel2, 0 );
lv_obj_set_y( ui_Panel2, 90 );
lv_obj_set_align( ui_Panel2, LV_ALIGN_CENTER );
lv_obj_clear_flag( ui_Panel2, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_Panel2, lv_color_hex(0x666600), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_Panel2, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_ScaleProgress = lv_slider_create(ui_Screen1);
lv_obj_set_width( ui_ScaleProgress, 320);
lv_obj_set_height( ui_ScaleProgress, 10);
lv_obj_set_x( ui_ScaleProgress, 0 );
lv_obj_set_y( ui_ScaleProgress, 90 );
lv_obj_set_align( ui_ScaleProgress, LV_ALIGN_CENTER );

ui_ImageCover = lv_img_create(ui_Screen1);
lv_obj_set_width( ui_ImageCover, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_ImageCover, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_ImageCover, 0 );
lv_obj_set_y( ui_ImageCover, -120 );
lv_obj_set_align( ui_ImageCover, LV_ALIGN_CENTER );

ui_RollerPlayList = lv_roller_create(ui_Screen1);
lv_roller_set_options( ui_RollerPlayList, "Song 1\nSong 2\nSong 3\nSong 4\nSong 5", LV_ROLLER_MODE_INFINITE );
lv_obj_set_width( ui_RollerPlayList, 480);
lv_obj_set_height( ui_RollerPlayList, 128);
lv_obj_set_x( ui_RollerPlayList, 0 );
lv_obj_set_y( ui_RollerPlayList, 176 );
lv_obj_set_align( ui_RollerPlayList, LV_ALIGN_CENTER );

ui_RollerLyrics = lv_roller_create(ui_Screen1);
lv_roller_set_options( ui_RollerLyrics, "Option 1\nOption 2\nOption 3", LV_ROLLER_MODE_NORMAL );
lv_obj_set_width( ui_RollerLyrics, 480);
lv_obj_set_height( ui_RollerLyrics, 108);
lv_obj_set_x( ui_RollerLyrics, 0 );
lv_obj_set_y( ui_RollerLyrics, 16 );
lv_obj_set_align( ui_RollerLyrics, LV_ALIGN_CENTER );

ui_LabelProgress = lv_label_create(ui_Screen1);
lv_obj_set_width( ui_LabelProgress, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_LabelProgress, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_LabelProgress, -195 );
lv_obj_set_y( ui_LabelProgress, 90 );
lv_obj_set_align( ui_LabelProgress, LV_ALIGN_CENTER );
lv_label_set_text(ui_LabelProgress,"00:00");

ui_LabelDuration = lv_label_create(ui_Screen1);
lv_obj_set_width( ui_LabelDuration, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_LabelDuration, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_LabelDuration, 195 );
lv_obj_set_y( ui_LabelDuration, 90 );
lv_obj_set_align( ui_LabelDuration, LV_ALIGN_CENTER );
lv_label_set_text(ui_LabelDuration,"00:00");

}

void ui_init( void )
{
lv_disp_t *dispp = lv_disp_get_default();
lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
lv_disp_set_theme(dispp, theme);
ui_Screen1_screen_init();
ui____initial_actions0 = lv_obj_create(NULL);
lv_disp_load_scr( ui_Screen1);
}
