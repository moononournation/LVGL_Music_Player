// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.2.1
// LVGL VERSION: 8.2.0
// PROJECT: Lvgl_Music_Player

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t *ui_Screen1;
lv_obj_t *ui_ImageCover;
lv_obj_t *ui_ArcTime;
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
lv_obj_clear_flag( ui_Screen1, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_ImageCover = lv_canvas_create(ui_Screen1);
lv_obj_set_width( ui_ImageCover, 240);
lv_obj_set_height( ui_ImageCover, 240);
lv_obj_set_align( ui_ImageCover, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_ImageCover, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_ImageCover, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_ArcTime = lv_arc_create(ui_Screen1);
lv_obj_set_width( ui_ArcTime, 460);
lv_obj_set_height( ui_ArcTime, 460);
lv_obj_set_align( ui_ArcTime, LV_ALIGN_CENTER );
lv_arc_set_value(ui_ArcTime, 50);
lv_arc_set_bg_angles(ui_ArcTime,150,30);

ui_LabelLyric = lv_label_create(ui_Screen1);
lv_obj_set_width( ui_LabelLyric, 400);
lv_obj_set_height( ui_LabelLyric, 100);
lv_obj_set_x( ui_LabelLyric, 0 );
lv_obj_set_y( ui_LabelLyric, 180 );
lv_obj_set_align( ui_LabelLyric, LV_ALIGN_CENTER );
lv_label_set_text(ui_LabelLyric,"一千里一千里走了多久想不起顫抖");
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
