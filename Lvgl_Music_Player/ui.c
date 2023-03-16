// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.2.1
// LVGL VERSION: 8.2.0
// PROJECT: Lvgl_Music_Player

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t *ui_Screen1;
lv_obj_t *ui_ScaleVolume;
lv_obj_t *ui_ScaleProgress;
lv_obj_t *ui_ImageCover;
lv_obj_t *ui_RollerPlayList;
lv_obj_t *ui_LabelLyric;

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
lv_obj_set_width( ui_ScaleVolume, 450);
lv_obj_set_height( ui_ScaleVolume, 450);
lv_obj_set_align( ui_ScaleVolume, LV_ALIGN_CENTER );
lv_arc_set_range(ui_ScaleVolume, 0,21);
lv_arc_set_value(ui_ScaleVolume, 4);
lv_arc_set_bg_angles(ui_ScaleVolume,200,340);

ui_ScaleProgress = lv_slider_create(ui_Screen1);
lv_obj_set_width( ui_ScaleProgress, 360);
lv_obj_set_height( ui_ScaleProgress, 10);
lv_obj_set_x( ui_ScaleProgress, 0 );
lv_obj_set_y( ui_ScaleProgress, 115 );
lv_obj_set_align( ui_ScaleProgress, LV_ALIGN_CENTER );

ui_ImageCover = lv_img_create(ui_Screen1);
lv_obj_set_width( ui_ImageCover, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_ImageCover, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_ImageCover, 0 );
lv_obj_set_y( ui_ImageCover, -100 );
lv_obj_set_align( ui_ImageCover, LV_ALIGN_CENTER );

ui_RollerPlayList = lv_roller_create(ui_Screen1);
lv_roller_set_options( ui_RollerPlayList, "Song 1\nSong 2\nSong 3\nSong 4\nSong 5", LV_ROLLER_MODE_INFINITE );
lv_obj_set_width( ui_RollerPlayList, 480);
lv_obj_set_height( ui_RollerPlayList, 100);
lv_obj_set_x( ui_RollerPlayList, 0 );
lv_obj_set_y( ui_RollerPlayList, 40 );
lv_obj_set_align( ui_RollerPlayList, LV_ALIGN_CENTER );

ui_LabelLyric = lv_label_create(ui_Screen1);
lv_obj_set_width( ui_LabelLyric, 480);
lv_obj_set_height( ui_LabelLyric, 100);
lv_obj_set_x( ui_LabelLyric, 0 );
lv_obj_set_y( ui_LabelLyric, 185 );
lv_obj_set_align( ui_LabelLyric, LV_ALIGN_CENTER );
lv_label_set_text(ui_LabelLyric,"這裡將會有一串串正在播放歌曲的歌詞\n歌詞一\n歌詞二\n歌詞三");
lv_obj_set_style_text_letter_space(ui_LabelLyric, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_line_space(ui_LabelLyric, -18, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_align(ui_LabelLyric, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_LabelLyric, &ui_font_NotoSerifCJKhk, LV_PART_MAIN| LV_STATE_DEFAULT);

}

void ui_init( void )
{
lv_disp_t *dispp = lv_disp_get_default();
lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
lv_disp_set_theme(dispp, theme);
ui_Screen1_screen_init();
lv_disp_load_scr( ui_Screen1);
}
