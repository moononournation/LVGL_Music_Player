// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.2.1
// LVGL VERSION: 8.2.0
// PROJECT: SquareLine_Project

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t *ui_Screen1;
lv_obj_t *ui_Spinner1;
lv_obj_t *ui_Label2;

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

ui_Spinner1 = lv_spinner_create(ui_Screen1,1000,90);
lv_obj_set_width( ui_Spinner1, 480);
lv_obj_set_height( ui_Spinner1, 480);
lv_obj_set_align( ui_Spinner1, LV_ALIGN_CENTER );
lv_obj_clear_flag( ui_Spinner1, LV_OBJ_FLAG_CLICKABLE );    /// Flags

ui_Label2 = lv_label_create(ui_Screen1);
lv_obj_set_width( ui_Label2, 320);
lv_obj_set_height( ui_Label2, 360);
lv_obj_set_align( ui_Label2, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label2,"最快樂時候 未知 最快樂\n到跌落凡塵 就知 再沒有\n想歸去 再邂逅\n這黑暗 的宇宙\n卻註定 你我 分手\n\n盼再度 遊寧靜海\n星光裡 看地球\n卻痛恨 良辰盡\n不許說未夠\n彼此也 再難回到\n當初的宇宙\n讓歴史 改變那步\n無法遠走\n\n著地之後 請忍痛放手\n變凡人之後\n教我遺忘星宿\n私奔到達過月球\n掙不脫命中魔咒\n別說和誰 印過一雙 腳印就夠\n\n這晚夜 毫無重心的感覺 再襲來\n這片地 完全無引力 怎麼愛\n不敢再次 抬頭看\n天邊的朗月\n讓代表心那塊石 沉向深海\n\n著地之後 請忍痛放手\n變凡人之後\n教我遺忘星宿\n私奔到達過月球\n掙不脫命中魔咒\n痛的 是那夜\n那段 美不勝收\n\n來年只願 取消了中秋\n慶團圓之夜 毋須更內疚\n銀河系很濶 你共我\n沒什麼重量 像蜉蝣\n離和合 哪值 半秒擔憂\n\n重頭修煉 得到過放手\n有誰還苦候\n廻光照亮污垢\n時代再快樂 不可帶著走\n沒落在深溝 那片月色\n\n有過就夠\n");
lv_obj_set_style_text_letter_space(ui_Label2, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_line_space(ui_Label2, -18, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_align(ui_Label2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_Label2, &ui_font_NotoSerifCJKhk, LV_PART_MAIN| LV_STATE_DEFAULT);

}

void ui_init( void )
{
lv_disp_t *dispp = lv_disp_get_default();
lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
lv_disp_set_theme(dispp, theme);
ui_Screen1_screen_init();
lv_disp_load_scr( ui_Screen1);
}
