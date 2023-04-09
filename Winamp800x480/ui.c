// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.2.3
// LVGL version: 8.2.0
// Project name: Winamp800x480

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t *ui_Screen1;
lv_obj_t *ui_ImageFrame;
lv_obj_t *ui_RollerPlayList;
lv_obj_t *ui_ScaleVolume;
lv_obj_t *ui_ScaleProgress;
lv_obj_t *ui_RollerLyrics;
lv_obj_t *ui_Panel2;
lv_obj_t *ui_ImageCover;
lv_obj_t *ui_LabelProgress;
lv_obj_t *ui_LabelPlaying;
lv_obj_t *ui_LabelKbps;
lv_obj_t *ui_LabelKHz;
lv_obj_t *ui_ButtonPrev;
lv_obj_t *ui_ButtonPlay;
lv_obj_t *ui_ButtonPause;
lv_obj_t *ui_ButtonStop;
lv_obj_t *ui_ButtonNext;
lv_obj_t *ui____initial_actions0;
const lv_img_dsc_t *ui_imgset_winamp480x[1] = {&ui_img_winamp480x320_png};
const lv_img_dsc_t *ui_imgset_winamp800x[1] = {&ui_img_winamp800x480_png};

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

ui_ImageFrame = lv_img_create(ui_Screen1);
lv_img_set_src(ui_ImageFrame, &ui_img_winamp800x480_png);
lv_obj_set_width( ui_ImageFrame, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_ImageFrame, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_ImageFrame, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_ImageFrame, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_ImageFrame, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_RollerPlayList = lv_roller_create(ui_Screen1);
lv_roller_set_options( ui_RollerPlayList, "林俊杰 - 一千年以后.mp3\n陳奕迅 - 時代曲.mp3\n陳曉東 - 甚麼吸引.mp3\n陳曉東 - 借借你肩膊.mp3\n黃子華 - 關老三.mp3\nBlack Box Recorder - Weekend.mp3\nFatboy - 塚。愛.mp3\nS.E.N.S - Alegretto.mp3", LV_ROLLER_MODE_INFINITE );
lv_obj_set_width( ui_RollerPlayList, 256);
lv_obj_set_height( ui_RollerPlayList, 332);
lv_obj_set_x( ui_RollerPlayList, -261 );
lv_obj_set_y( ui_RollerPlayList, 60 );
lv_obj_set_align( ui_RollerPlayList, LV_ALIGN_CENTER );
lv_obj_set_style_text_color(ui_RollerPlayList, lv_color_hex(0x00CC00), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_RollerPlayList, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_letter_space(ui_RollerPlayList, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_line_space(ui_RollerPlayList, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_RollerPlayList, &ui_font_Cubic11, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_radius(ui_RollerPlayList, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_set_style_text_color(ui_RollerPlayList, lv_color_hex(0x00CC00), LV_PART_SELECTED | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_RollerPlayList, 255, LV_PART_SELECTED| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_RollerPlayList, lv_color_hex(0x0000FF), LV_PART_SELECTED | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_RollerPlayList, 255, LV_PART_SELECTED| LV_STATE_DEFAULT);

ui_ScaleVolume = lv_slider_create(ui_Screen1);
lv_slider_set_range(ui_ScaleVolume, 0,31);
lv_slider_set_value( ui_ScaleVolume, 15, LV_ANIM_OFF);
if (lv_slider_get_mode(ui_ScaleVolume)==LV_SLIDER_MODE_RANGE ) lv_slider_set_left_value( ui_ScaleVolume, 0, LV_ANIM_OFF);
lv_obj_set_width( ui_ScaleVolume, 52);
lv_obj_set_height( ui_ScaleVolume, 34);
lv_obj_set_x( ui_ScaleVolume, -166 );
lv_obj_set_y( ui_ScaleVolume, -202 );
lv_obj_set_align( ui_ScaleVolume, LV_ALIGN_CENTER );
lv_obj_set_style_radius(ui_ScaleVolume, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_ScaleVolume, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_ScaleVolume, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_set_style_radius(ui_ScaleVolume, 2, LV_PART_INDICATOR| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_ScaleVolume, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_ScaleVolume, 0, LV_PART_INDICATOR| LV_STATE_DEFAULT);

lv_obj_set_style_bg_color(ui_ScaleVolume, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_ScaleVolume, 0, LV_PART_KNOB| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_ScaleVolume, &ui_img_volume_png, LV_PART_KNOB | LV_STATE_DEFAULT );
lv_obj_set_style_pad_left(ui_ScaleVolume, 4, LV_PART_KNOB| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_ScaleVolume, 3, LV_PART_KNOB| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_ScaleVolume, 2, LV_PART_KNOB| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_ScaleVolume, 2, LV_PART_KNOB| LV_STATE_DEFAULT);

ui_ScaleProgress = lv_slider_create(ui_Screen1);
lv_obj_set_width( ui_ScaleProgress, 218);
lv_obj_set_height( ui_ScaleProgress, 8);
lv_obj_set_x( ui_ScaleProgress, -259 );
lv_obj_set_y( ui_ScaleProgress, -163 );
lv_obj_set_align( ui_ScaleProgress, LV_ALIGN_CENTER );
lv_obj_add_state( ui_ScaleProgress, LV_STATE_DISABLED );     /// States
lv_obj_set_style_bg_color(ui_ScaleProgress, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_ScaleProgress, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_set_style_bg_color(ui_ScaleProgress, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_ScaleProgress, 0, LV_PART_INDICATOR| LV_STATE_DEFAULT);

lv_obj_set_style_radius(ui_ScaleProgress, 0, LV_PART_KNOB| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_ScaleProgress, lv_color_hex(0x997200), LV_PART_KNOB | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_ScaleProgress, 255, LV_PART_KNOB| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_ScaleProgress, &ui_img_progress_png, LV_PART_KNOB | LV_STATE_DEFAULT );
lv_obj_set_style_border_color(ui_ScaleProgress, lv_color_hex(0x664C00), LV_PART_KNOB | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_ScaleProgress, 255, LV_PART_KNOB| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_ScaleProgress, 1, LV_PART_KNOB| LV_STATE_DEFAULT);
lv_obj_set_style_pad_left(ui_ScaleProgress, 10, LV_PART_KNOB| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_ScaleProgress, 11, LV_PART_KNOB| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_ScaleProgress, 1, LV_PART_KNOB| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_ScaleProgress, 1, LV_PART_KNOB| LV_STATE_DEFAULT);

ui_RollerLyrics = lv_roller_create(ui_Screen1);
lv_roller_set_options( ui_RollerLyrics, "一千年以后 - 林俊杰 (JJ Lin)\n词：李瑞洵\n曲：林俊杰\n心跳乱了节奏\n梦也不自由\n爱 是个绝对承诺 不说\n撑到一千年以后\n放任无奈 淹没尘埃\n我在废墟之中守着你走来\nWoo\n我的泪光 承载不了\nWoo\n所有一切你要的爱\n因为在 一千年以后\n世界早已没有我\n无法深情挽着你的手\n浅吻着你额头\n别等到 一千年以后\n所有人都遗忘了我\n那时红色黄昏的沙漠\n能有谁 解开缠绕千年的寂寞\nWu\n放任无奈 淹没尘埃\n我在废墟之中守着你走来\nWoo\n我的泪光 承载不了 喔\n所有一切你需要的爱\n因为在 一千年以后\n世界早已没有我\n无法深情挽着你的手\n浅吻着你额头\n别等到 一千年以后\n所有人都遗忘了我\n那时红色黄昏的沙漠\n能有谁 解开缠绕千年的寂寞\n无法深情挽着你的手\n浅吻着你额头\n别等到 一千年以后\n所有人都遗忘了我\n那时红色黄昏的沙漠\n能有谁\n解开缠绕千年的寂寞\nWu\n缠绕千年的寂寞", LV_ROLLER_MODE_NORMAL );
lv_obj_set_width( ui_RollerLyrics, 508);
lv_obj_set_height( ui_RollerLyrics, 446);
lv_obj_set_x( ui_RollerLyrics, 138 );
lv_obj_set_y( ui_RollerLyrics, 3 );
lv_obj_set_align( ui_RollerLyrics, LV_ALIGN_CENTER );
lv_obj_set_style_text_letter_space(ui_RollerLyrics, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_line_space(ui_RollerLyrics, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_RollerLyrics, &ui_font_Cubic11, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_radius(ui_RollerLyrics, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_set_style_bg_color(ui_RollerLyrics, lv_color_hex(0x000000), LV_PART_SELECTED | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_RollerLyrics, 255, LV_PART_SELECTED| LV_STATE_DEFAULT);

ui_Panel2 = lv_obj_create(ui_Screen1);
lv_obj_set_width( ui_Panel2, 508);
lv_obj_set_height( ui_Panel2, 160);
lv_obj_set_x( ui_Panel2, 138 );
lv_obj_set_y( ui_Panel2, -140 );
lv_obj_set_align( ui_Panel2, LV_ALIGN_CENTER );
lv_obj_clear_flag( ui_Panel2, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_Panel2, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_Panel2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_Panel2, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_Panel2, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_ImageCover = lv_img_create(ui_Screen1);
lv_obj_set_width( ui_ImageCover, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_ImageCover, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_ImageCover, 138 );
lv_obj_set_y( ui_ImageCover, -140 );
lv_obj_set_align( ui_ImageCover, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_ImageCover, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_ImageCover, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_LabelProgress = lv_label_create(ui_Screen1);
lv_obj_set_width( ui_LabelProgress, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_LabelProgress, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_LabelProgress, -316 );
lv_obj_set_y( ui_LabelProgress, -209 );
lv_obj_set_align( ui_LabelProgress, LV_ALIGN_CENTER );
lv_label_set_text(ui_LabelProgress,"00:00");
lv_obj_set_style_text_color(ui_LabelProgress, lv_color_hex(0x00CC00), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_LabelProgress, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_LabelProgress, &ui_font_Cubic11, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_LabelPlaying = lv_label_create(ui_Screen1);
lv_obj_set_width( ui_LabelPlaying, 154);
lv_obj_set_height( ui_LabelPlaying, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_LabelPlaying, -213 );
lv_obj_set_y( ui_LabelPlaying, -209 );
lv_obj_set_align( ui_LabelPlaying, LV_ALIGN_CENTER );
lv_label_set_long_mode(ui_LabelPlaying,LV_LABEL_LONG_SCROLL_CIRCULAR);
lv_label_set_text(ui_LabelPlaying,"Title: 塚。愛 Artist: Fatboy SettingsForEncoding: Lavf59.27.100");
lv_obj_set_style_text_color(ui_LabelPlaying, lv_color_hex(0x00CC00), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_LabelPlaying, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_LabelPlaying, &ui_font_Cubic11, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_LabelKbps = lv_label_create(ui_Screen1);
lv_obj_set_width( ui_LabelKbps, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_LabelKbps, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_LabelKbps, -281 );
lv_obj_set_y( ui_LabelKbps, -187 );
lv_obj_set_align( ui_LabelKbps, LV_ALIGN_CENTER );
lv_label_set_text(ui_LabelKbps,"384");
lv_obj_set_style_text_color(ui_LabelKbps, lv_color_hex(0x00CC00), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_LabelKbps, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_LabelKbps, &ui_font_Cubic11, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_LabelKHz = lv_label_create(ui_Screen1);
lv_obj_set_width( ui_LabelKHz, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_LabelKHz, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_LabelKHz, -235 );
lv_obj_set_y( ui_LabelKHz, -187 );
lv_obj_set_align( ui_LabelKHz, LV_ALIGN_CENTER );
lv_label_set_text(ui_LabelKHz,"44");
lv_obj_set_style_text_color(ui_LabelKHz, lv_color_hex(0x00CC00), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_LabelKHz, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_LabelKHz, &ui_font_Cubic11, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_ButtonPrev = lv_btn_create(ui_Screen1);
lv_obj_set_width( ui_ButtonPrev, 39);
lv_obj_set_height( ui_ButtonPrev, 39);
lv_obj_set_x( ui_ButtonPrev, -364 );
lv_obj_set_y( ui_ButtonPrev, -132 );
lv_obj_set_align( ui_ButtonPrev, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_ButtonPrev, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_ButtonPrev, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_ButtonPrev, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_ButtonPrev, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_ButtonPrev, &ui_img_prev_png, LV_PART_MAIN | LV_STATE_DEFAULT );

ui_ButtonPlay = lv_btn_create(ui_Screen1);
lv_obj_set_width( ui_ButtonPlay, 39);
lv_obj_set_height( ui_ButtonPlay, 39);
lv_obj_set_x( ui_ButtonPlay, -323 );
lv_obj_set_y( ui_ButtonPlay, -132 );
lv_obj_set_align( ui_ButtonPlay, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_ButtonPlay, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_ButtonPlay, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_ButtonPlay, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_ButtonPlay, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_ButtonPlay, &ui_img_play_png, LV_PART_MAIN | LV_STATE_DEFAULT );

ui_ButtonPause = lv_btn_create(ui_Screen1);
lv_obj_set_width( ui_ButtonPause, 39);
lv_obj_set_height( ui_ButtonPause, 39);
lv_obj_set_x( ui_ButtonPause, -282 );
lv_obj_set_y( ui_ButtonPause, -132 );
lv_obj_set_align( ui_ButtonPause, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_ButtonPause, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_ButtonPause, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_ButtonPause, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_ButtonPause, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_ButtonPause, &ui_img_pause_png, LV_PART_MAIN | LV_STATE_DEFAULT );

ui_ButtonStop = lv_btn_create(ui_Screen1);
lv_obj_set_width( ui_ButtonStop, 39);
lv_obj_set_height( ui_ButtonStop, 39);
lv_obj_set_x( ui_ButtonStop, -241 );
lv_obj_set_y( ui_ButtonStop, -132 );
lv_obj_set_align( ui_ButtonStop, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_ButtonStop, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_ButtonStop, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_ButtonStop, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_ButtonStop, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_ButtonStop, &ui_img_stop_png, LV_PART_MAIN | LV_STATE_DEFAULT );

ui_ButtonNext = lv_btn_create(ui_Screen1);
lv_obj_set_width( ui_ButtonNext, 39);
lv_obj_set_height( ui_ButtonNext, 39);
lv_obj_set_x( ui_ButtonNext, -200 );
lv_obj_set_y( ui_ButtonNext, -132 );
lv_obj_set_align( ui_ButtonNext, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_ButtonNext, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_ButtonNext, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_ButtonNext, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_ButtonNext, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_ButtonNext, &ui_img_next_png, LV_PART_MAIN | LV_STATE_DEFAULT );

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
