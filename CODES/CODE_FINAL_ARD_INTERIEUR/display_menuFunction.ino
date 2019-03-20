





/* ===================================================================== *
 *                                                                       *
 * Menu Callback Function                                                *
 *                                                                       *
 * ===================================================================== *
 *
 * EXAMPLE CODE:

// *********************************************************************
void your_function_name(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // setup
    // is called only if it is started

    // starts a trigger event for the loop function every 100 milliseconds
    LCDML.FUNC_setLoopInterval(100);
  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    // loop
    // is called when it is triggered
    // - with LCDML_DISP_triggerMenu( milliseconds )
    // - with every button status change

    // check if any button is pressed (enter, up, down, left, right)
    if(LCDML.BT_checkAny()) {
      LCDML.FUNC_goBackToMenu();
    }
  }

  if(LCDML.FUNC_close())      // ****** STABLE END *********
  {
    // loop end
    // you can here reset some global vars or delete it
  }
}


 * ===================================================================== *
 */

// *********************************************************************
void mFunc_screensaver(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // setup function
    u8g.setFont(_LCDML_DISP_font);
    u8g.firstPage();
    do {
      u8g.drawStr( 0, (_LCDML_DISP_font_h * 1), F("screensaver"));
      u8g.drawStr( 0, (_LCDML_DISP_font_h * 2), F("press any key"));
      u8g.drawStr( 0, (_LCDML_DISP_font_h * 3), F("to leave it"));
    } while( u8g.nextPage() );

    LCDML.FUNC_setLoopInterval(100);  // starts a trigger event for the loop function every 100 milliseconds
  }

  if(LCDML.FUNC_loop())              // ****** LOOP *********
  {
    if (LCDML.BT_checkAny()) // check if any button is pressed (enter, up, down, left, right)
    {
      LCDML.FUNC_goBackToMenu();  // leave this function
    }
  }

  if(LCDML.FUNC_close())            // ****** STABLE END *********
  {
    // The screensaver go to the root menu
    LCDML.MENU_goRoot();
  }
}



// *********************************************************************
void mFunc_back(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // end function and go an layer back
    LCDML.FUNC_goBackToMenu(1);      // leave this function and go a layer back
  }
}


// *********************************************************************
void mFunc_goToRootMenu(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // go to root and display menu
    LCDML.MENU_goRoot();
  }
}


// *********************************************************************
void mFunc_tem(uint8_t param)
// *********************************************************************
{


  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // setup function
    port=0;
    u8g.setFont(_LCDML_DISP_font);
    u8g.firstPage();
 
    while( u8g.nextPage() );
    LCDML.FUNC_setLoopInterval(100);
    Ville=""; 
    Tempera="";
    Humidi="";
    VentVi="";
    VentDir="";
  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
   
  char c;
  int Read=0;
  boolean first_char = true;
  String s = "";
  
  while(ard.available()) {
    c = char(ard.read());
    
    if(Read!=0){
      
        if(c=='#'){
          break;
          
        }
        
        else{
          s=s+String(c);
        }
        
      }
      
      else{
        Read=0;
        first_char=true;
      }
    
    if (first_char){
      
      if(c=='!'){
        Read=1;
      }
      if(c=='$'){
        Read=2;
      }
      if(c=='*'){
        Read=3;
      }
      if(c=='='){
        Read=4;
      }
      if(c=='&'){
        Read=5;
      }
      first_char=false;
    }
    
  }

    switch(Read){
    case 1:Ville=s;     break;
    case 2:Tempera=s;   break;
    case 3:Humidi=s;    break;
    case 4:VentVi=s;    break;
    case 5:VentDir=s;   break;
  }
   
    char hum[20];
    char tem[20];
    char ven[20];
    char dir[20];
    String vent;
    String direct;
    
    
    sprintf (tem, "Temperature: %d *C", Tempera.toInt());
    
    vent="Vent: "+String(VentVi.toFloat()*3.6)+" Km/h";
    vent.toCharArray(ven,20);

    direct="Direction:"+VentDir;
    direct.toCharArray(dir,20);
    
    sprintf (hum, "Humidite: %d pcts", Humidi.toInt());
 
    u8g.firstPage();
    do {
     u8g.drawStr( _LCDML_DISP_box_x0+_LCDML_DISP_font_w , _LCDML_DISP_box_y0 + _LCDML_DISP_font_h * (1), tem);
     u8g.drawStr( _LCDML_DISP_box_x0+_LCDML_DISP_font_w ,  _LCDML_DISP_box_y0 + _LCDML_DISP_font_h * (2), hum);
     u8g.drawStr( _LCDML_DISP_box_x0+_LCDML_DISP_font_w , _LCDML_DISP_box_y0 + _LCDML_DISP_font_h * (3), ven);
     u8g.drawStr( _LCDML_DISP_box_x0+_LCDML_DISP_font_w , _LCDML_DISP_box_y0 + _LCDML_DISP_font_h * (4), dir);
     u8g.drawFrame(_LCDML_DISP_box_x0, _LCDML_DISP_box_y0, (_LCDML_DISP_box_x1-_LCDML_DISP_box_x0), (_LCDML_DISP_box_y1-_LCDML_DISP_box_y0));
    } while( u8g.nextPage() );


    
    if(LCDML.BT_checkEnter()) // check if any button is pressed (enter, up, down, left, right)
    {
      // LCDML_goToMenu stops a running menu function and goes to the menu
      LCDML.FUNC_goBackToMenu();
    }
  }

  if(LCDML.FUNC_close())      // ****** STABLE END *********
  {
    // you can here reset some global vars or do nothing
  }
}


// *********************************************************************
void mFunc_li(uint8_t param)
// *********************************************************************
{
  
  
  
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // setup function
    port=1;
    u8g.setFont(_LCDML_DISP_font);
    u8g.firstPage();

    while( u8g.nextPage() );
    LCDML.FUNC_setLoopInterval(100);
    Ville=""; 
    Tempera="";
    Humidi="";
    VentVi="";
    VentDir="";
    k=0;
    
  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
   
  char c;
  int Read=0;
  boolean first_char = true;
  String s = "";
  
  while(esp.available()) {
    c = char(esp.read());
    
    if(Read!=0){
      
        if(c=='#'){
          break;
          
        }
        
        else{
          s=s+String(c);
        }
        
      }
      
      else{
        Read=0;
        first_char=true;
      }
    
    if (first_char){
      
      if(c=='!'){
        Read=1;
      }
      if(c=='$'){
        Read=2;
      }
      if(c=='*'){
        Read=3;
      }
      if(c=='='){
        Read=4;
      }
      if(c=='&'){
        Read=5;
      }
      if(c=='+'){
        Read=6;
      }
      first_char=false;
    }
    
  }

    switch(Read){
    case 1:Ville=s;     break;
    case 2:Tempera=s;   break;
    case 3:Humidi=s;    break;
    case 4:VentVi=s;    break;
    case 5:VentDir=s;   break;
   
  }
   
    char hum[20];
    char tem[20];
    char ven[20];
    char dire[20];
    char vil[20];

    String ville;
    String vent;
    String direct;

    
    ville="Ville: "+Ville;
    ville.toCharArray(vil,20);
    
    sprintf (tem, "Temperature: %d *C", Tempera.toInt());
    
    vent="Vent: "+String(VentVi.toFloat()*3.6)+" Km/h";
    vent.toCharArray(ven,20);
    
    direct="Direction:"+VentDir;
    direct.toCharArray(dire,20);
    
    sprintf (hum, "Humidite: %d pcts", Humidi.toInt());
    
    u8g.firstPage();
    do {
     u8g.drawStr( _LCDML_DISP_box_x0+_LCDML_DISP_font_w , _LCDML_DISP_box_y0 + _LCDML_DISP_font_h * (1-(k*0.5)), vil);
     u8g.drawStr( _LCDML_DISP_box_x0+_LCDML_DISP_font_w , _LCDML_DISP_box_y0 + _LCDML_DISP_font_h * (2-(k*0.5)), tem);
     u8g.drawStr( _LCDML_DISP_box_x0+_LCDML_DISP_font_w , _LCDML_DISP_box_y0 + _LCDML_DISP_font_h * (3-(k*0.5)), hum);
     u8g.drawStr( _LCDML_DISP_box_x0+_LCDML_DISP_font_w , _LCDML_DISP_box_y0 + _LCDML_DISP_font_h * (4-(k*0.5)), ven);
     u8g.drawStr( _LCDML_DISP_box_x0+_LCDML_DISP_font_w , _LCDML_DISP_box_y0 + _LCDML_DISP_font_h * (5-(k*0.5)), dire);
   
     u8g.drawFrame(_LCDML_DISP_box_x0, _LCDML_DISP_box_y0, (_LCDML_DISP_box_x1-_LCDML_DISP_box_x0), (_LCDML_DISP_box_y1-_LCDML_DISP_box_y0));
    } while( u8g.nextPage() );

    if(LCDML.BT_checkUp()){
      if(k<4){
        k+=1;
      }
    }
    if(LCDML.BT_checkDown()){
      if(k>0){
        k-=1;
      }
    }
    
    if(LCDML.BT_checkEnter()) // check if any button is pressed (enter, up, down, left, right)
    {
      // LCDML_goToMenu stops a running menu function and goes to the menu
      LCDML.FUNC_goBackToMenu();
    }
  }

  if(LCDML.FUNC_close())      // ****** STABLE END *********
  {
    // you can here reset some global vars or do nothing
  }
}
