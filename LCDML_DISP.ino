// =====================================================================
//
// Output function
//
// =====================================================================

/* ******************************************************************** */
void LCDML_lcd_menu_display()
/* ******************************************************************** */
{
  // check if menu needs an update
  if (LCDML_DISP_update() && konf_state) {   
	
	setText2G
    
    // init vars
    uint8_t n_max             = (LCDML.getChilds() >= _LCDML_DISP_rows) ? _LCDML_DISP_rows : (LCDML.getChilds());
    uint8_t scrollbar_min     = 0;
    uint8_t scrollbar_max     = LCDML.getChilds();
    uint8_t scrollbar_cur_pos = LCDML.getCursorPosAbs();
    uint8_t scroll_pos        = ((1.*n_max * _LCDML_DISP_rows) / (scrollbar_max - 1) * scrollbar_cur_pos);


    // update content
    if (LCDML_DISP_update_content()) {
      // clear menu
      LCDML_lcd_menu_clear();

      // display rows
      for (uint8_t n = 0; n < n_max; n++)
      {
        // set cursor
        tft.setCursor(1*mnoznik2, dy + n*mnoznik);
        // set content
        tft.print(LCDML.content[n]);
      }
    }

    // update cursor and scrollbar
    if (LCDML_DISP_update_cursor()) {

      // display rows
      for (uint8_t n = 0; n < n_max; n++)
      {
        //set cursor
        tft.setCursor(0, dy + n*mnoznik);

        //set cursor char
        if (n == LCDML.getCursorPos()) {
          tft.write(_LCDML_DISP_cfg_cursor);
        } else {
          tft.write(' ');
        }

        // delete or reset scrollbar
        if (_LCDML_DISP_cfg_scrollbar == 1) {
          if (scrollbar_max > n_max) {
            tft.setCursor((_LCDML_DISP_cols - 1)*mnoznik2, dy + n*mnoznik);
            tft.write((uint8_t)0);
          }
          else {
            tft.setCursor((_LCDML_DISP_cols - 1)*mnoznik2, dy + n*mnoznik);
            tft.print(' ');
          }
        }
      }

      // display scrollbar
      if (_LCDML_DISP_cfg_scrollbar == 1) {
        if (scrollbar_max > n_max) {
          //set scroll position
          if (scrollbar_cur_pos == scrollbar_min) {
            // min pos
            tft.setCursor((_LCDML_DISP_cols - 1)*mnoznik2, dy + 0);
            tft.write((uint8_t)1);
          } else if (scrollbar_cur_pos == (scrollbar_max - 1)) {
            // max pos
            tft.setCursor((_LCDML_DISP_cols - 1)*mnoznik2, dy + (n_max - 1)*mnoznik);
            tft.write((uint8_t)4);
          } else {
            // between
            tft.setCursor((_LCDML_DISP_cols - 1)*mnoznik2, dy + (scroll_pos / n_max)*mnoznik);
            tft.write((uint8_t)(scroll_pos % n_max) + 1);
          }
        }
      }
    }
  }
  // reinit some vars
  LCDML_DISP_update_end();
}


// lcd clear
void LCDML_lcd_menu_clear()
{
	tft.fillScreen(ILI9341_BLACK);
	show_down_up();
	show_enter();
	show_setup();
	show_esc();
	tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
	tft.setCursor(0, dy + 0);
}




