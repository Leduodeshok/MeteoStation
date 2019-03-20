/* ===================================================================== *
 *                                                                       *
 * Dynamic content                                                       *
 *                                                                       *
 * ===================================================================== *
 */


uint8_t g_dynParam = 100; // when this value comes from an EEPROM, load it in setup
                          // at the moment here is no setup function (To-Do)
void mDyn_para(uint8_t line)
// *********************************************************************
{
  // check if this function is active (cursor stands on this line)
  if (line == LCDML.MENU_getCursorPos())
  {
    // make only an action when the cursor stands on this menu item
    //check Button
    if(LCDML.BT_checkAny())
    {
      if(LCDML.BT_checkEnter())
      {
        // this function checks returns the scroll disable status (0 = menu scrolling enabled, 1 = menu scrolling disabled)
        if(LCDML.MENU_getScrollDisableStatus() == 0)
        {
          // disable the menu scroll function to catch the cursor on this point
          // now it is possible to work with BT_checkUp and BT_checkDown in this function
          // this function can only be called in a menu, not in a menu function
          LCDML.MENU_disScroll();
        }
        else
        {
          // enable the normal menu scroll function
          LCDML.MENU_enScroll();
        }

        // do something
        // ...
        
        LCDML.BT_resetEnter();
      }

      // This check have only an effect when MENU_disScroll is set
      if(LCDML.BT_checkUp())
      {
        g_dynParam++;
        LCDML.BT_resetUp();
      }

      // This check have only an effect when MENU_disScroll is set
      if(LCDML.BT_checkDown())
      {
        g_dynParam--;
        LCDML.BT_resetDown();
      }


      if(LCDML.BT_checkLeft())
      {
        g_dynParam++;
        LCDML.BT_resetLeft();
      }
      
      if(LCDML.BT_checkRight())
      {
        g_dynParam--;
        LCDML.BT_resetRight();
      }
    }
  }

  char buf[20];
  sprintf (buf, "dynValue: %d", g_dynParam);

  // setup function
  u8g.drawStr( _LCDML_DISP_box_x0+_LCDML_DISP_font_w + _LCDML_DISP_cur_space_behind,  (_LCDML_DISP_font_h * (1+line)), buf);     // the value can be changed with left or right

}

// *********************************************************************
// *********************************************************************



void mCod_post(uint8_t line){
// ********************************************************************* 
 


  
  //Verifie si la fonction est active
  
  if(line==LCDML.MENU_getCursorPos()){
    
      if(LCDML.BT_checkEnter()){
          if(LCDML.MENU_getScrollDisableStatus()==0){
            LCDML.MENU_disScroll();
          }
          else{
            esp.print("*"+codePostale+"#");
            LCDML.MENU_enScroll();
            
          }
          
          LCDML.BT_resetEnter();
        }
      
        if(LCDML.BT_checkUp()){
          if(enc<8){
            enc+=1;
          }
          LCDML.BT_resetUp();
        } 

        if(LCDML.BT_checkDown()){
          if(enc>0){
            enc-=1;
          }
          LCDML.BT_resetDown();
        }
      }     
   
    
    
    char codpo[20];
    switch(enc){
      case 0: choixVille="Nice";codePostale="06000"; break;
      case 1: choixVille="Lille";codePostale="59000"; break;
      case 2: choixVille="Paris";codePostale="75000"; break;
      case 3: choixVille="Nantes";codePostale="44000"; break;
      case 4: choixVille="Montpellier";codePostale="34000"; break;
      case 5: choixVille="Orleans";codePostale="45000"; break;
      case 6: choixVille="Bordeaux";codePostale="33000"; break;
      case 7: choixVille="Rouen";codePostale="76000"; break;
      case 8: choixVille="New-York";codePostale="10150"; break;
    }
    message= "meteo de: " + choixVille;
    message.toCharArray(codpo,20);
    Serial.println(choixVille);
    Serial.println(enc);
    port=1;
    u8g.drawStr( _LCDML_DISP_box_x0+_LCDML_DISP_font_w + _LCDML_DISP_cur_space_behind,  (_LCDML_DISP_font_h * (1+line)), codpo);
    
    
}
