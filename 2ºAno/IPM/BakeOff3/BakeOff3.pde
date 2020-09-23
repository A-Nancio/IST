// Bakeoff #3 - Escrita de Texto em Smartwatches
// IPM 2019-20, Semestre 2
// Entrega: exclusivamente no dia 22 de Maio, até às 23h59, via Twitter

// Processing reference: https://processing.org/reference/

import java.util.Arrays;
import java.util.Collections;
import java.util.Random;

// Screen resolution vars;
float PPI, PPCM;
float SCALE_FACTOR;

// Finger parameters
PImage fingerOcclusion;
int FINGER_SIZE;
int FINGER_OFFSET;

// Arm/watch parameters
PImage arm, backspace, keyboard_img;
int ARM_LENGTH;
int ARM_HEIGHT;

// Arrow parameters
PImage leftArrow, rightArrow;
int ARROW_SIZE;

// Study properties
String[] phrases;                   // contains all the phrases that can be tested
int NUM_REPEATS            = 2;     // the total number of phrases to be tested
int currTrialNum           = 0;     // the current trial number (indexes into phrases array above)
String currentPhrase       = "";    // the current target phrase
String currentTyped        = "";    // what the user has typed so far

// coisas adicionadas
float RATIO;
int NUMBEROFKEYS = 28;
int FIRSTLINEKEYS = 10;
int SECONDLINEKEYS = 9;
int THIRDLINEKEYS = 7;
ArrayList<Key> keyList  = new ArrayList<Key>();
char[] chars = {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'z', 'x', 'c', 'v', 'b', 'n', 'm'}; 
float SPACEBETWEENKEYS;
float FIRSTKEYX;
float FIRSTKEYY;
float KEYW;
float KEYH;
int[][] keyboardPixels;
boolean zoomIn = false;
int zoomX, zoomY;
int SCALE;
int auxX, auxY;
String words[];
String sketch = "";
ArrayList<String> predictedWords = new ArrayList<String>();

// Performance variables
float startTime            = 0;     // time starts when the user clicks for the first time
float finishTime           = 0;     // records the time of when the final trial ends
float lastTime             = 0;     // the timestamp of when the last trial was completed
float lettersEnteredTotal  = 0;     // a running total of the number of letters the user has entered (need this for final WPM computation)
float lettersExpectedTotal = 0;     // a running total of the number of letters expected (correct phrases)
float errorsTotal          = 0;     // a running total of the number of errors (when hitting next)

// Class used to store properties of a target
class Key
{
  float x, y, w, h;
  char keyChar;

  Key(float posx, float posy, float KeyWidth, float KeyHeight, char auxChar) 
  {
    x = posx;
    y = posy;
    w = KeyWidth;
    h = KeyHeight;
    keyChar = auxChar;
  }
}

void printKey(Key auxKey){
  if(auxKey.keyChar == '`'){
    image(backspace, auxKey.x + 0.25*PPCM, auxKey.y + 0.2 * PPCM, auxKey.w, auxKey.h);
  }
  else{
    fill(100);
    noStroke();
    rect(auxKey.x, auxKey.y, auxKey.w, auxKey.h);
    textAlign(CENTER);
    fill(230);
    text(""+ auxKey.keyChar, auxKey.x + 0.15*PPCM, auxKey.y + 0.34*PPCM);
  }
}

void printScaleKey(Key auxKey, float x, int scale){
  if(auxKey.keyChar == '`'){
    image(backspace, auxKey.x + 0.25*PPCM, auxKey.y + 0.2 * PPCM, auxKey.w, auxKey.h);
  }
  else{
    fill(100);
    noStroke();
    rect(x , auxKey.y + scale, auxKey.w + scale, auxKey.h + scale);
    textAlign(CENTER);
    fill(230);
    text(""+ auxKey.keyChar, x + 0.15*PPCM , auxKey.y + 0.34*PPCM + scale);
  }
}

//Setup window and vars - runs once
void setup()
{
  //size(900, 900);
  fullScreen();
  textFont(createFont("Arial", 24));  // set the font to arial 24
  noCursor();                         // hides the cursor to emulate a watch environment
  
  // Load images
  backspace = loadImage("backspace.png");
  arm = loadImage("arm_watch.png");
  fingerOcclusion = loadImage("finger.png");
  keyboard_img = loadImage("keyboard.png");
  
  // Load phrases
  phrases = loadStrings("phrases.txt");                       // load the phrase set into memory
  Collections.shuffle(Arrays.asList(phrases), new Random());  // randomize the order of the phrases with no seed
  
  // Load words
  words = loadStrings("count_1w.txt");                        // load most used words
  for (int i = 0; i < 3; i++){
    predictedWords.add(words[i]);
  }
  // Scale targets and imagens to match screen resolution
  SCALE_FACTOR = 1.0 / displayDensity();          // scale factor for high-density displays
  String[] ppi_string = loadStrings("ppi.txt");   // the text from the file is loaded into an array.
  PPI = float(ppi_string[1]);                     // set PPI, we assume the ppi value is in the second line of the .txt
  PPCM = PPI / 2.54 * SCALE_FACTOR;// do not change this!
  
  // sera???
  SPACEBETWEENKEYS = 0.07 * PPCM;
  FIRSTKEYX = width/2 - 1.92 * PPCM;
  FIRSTKEYY = height/2 ;
  KEYW = 0.325 * PPCM;
  KEYH = 0.45 * PPCM;
  SCALE = int(0.8 * PPCM);
  
  FINGER_SIZE = (int)(11 * PPCM);
  FINGER_OFFSET = (int)(0.8 * PPCM);
  ARM_LENGTH = (int)(19 * PPCM);
  ARM_HEIGHT = (int)(11.2 * PPCM);
  float x = FIRSTKEYX, y = FIRSTKEYY;
  for(int i = 0; i < FIRSTLINEKEYS; i++){
    Key keyAux = new Key(x, y, KEYW, KEYH, chars[i]);
    x += KEYW + SPACEBETWEENKEYS;
    keyList.add(keyAux);
  }
  x = FIRSTKEYX + (KEYW + SPACEBETWEENKEYS)/2;
  y = FIRSTKEYY + KEYH + SPACEBETWEENKEYS;
  for(int i = 0; i < SECONDLINEKEYS; i++){
    Key keyAux = new Key(x, y, KEYW, KEYH, chars[FIRSTLINEKEYS + i]);
    x += KEYW + SPACEBETWEENKEYS;
    keyList.add(keyAux);
  }
  x = FIRSTKEYX + KEYW + SPACEBETWEENKEYS +(KEYW + SPACEBETWEENKEYS)/2;
  y = FIRSTKEYY + 2*(KEYH + SPACEBETWEENKEYS);
  for(int i = 0; i < THIRDLINEKEYS; i++){
    Key keyAux = new Key(x, y, KEYW, KEYH, chars[FIRSTLINEKEYS + SECONDLINEKEYS + i]);
    x += KEYW + SPACEBETWEENKEYS;
    keyList.add(keyAux);
  }
  x = FIRSTKEYX + KEYW + SPACEBETWEENKEYS +(KEYW + SPACEBETWEENKEYS)/2;
  y += KEYH + SPACEBETWEENKEYS;
  keyList.add(new Key(x, y, 2.7*PPCM, 0.4*PPCM,' '));
  
  x += 7 * KEYW + 7 * SPACEBETWEENKEYS;
  y = FIRSTKEYY + 2*(KEYH + SPACEBETWEENKEYS);
  keyList.add(new Key(x, y, 0.55*PPCM, KEYH, '`'));
  

  keyboardPixels = new int[keyboard_img.width][keyboard_img.height];
  for (int i = 0; i < keyboard_img.height; i++) {
    for (int j = 0; j < keyboard_img.width; j++) {
      keyboardPixels[j][i] = keyboard_img.get(j, i);
    }
  }
}


void drawKeyboard(){
  PImage auxKeyboard = keyboard_img.get();
  PImage auxImg;
  int xCenter = int(width/2 - 2.0*PPCM + 4.0*PPCM/2);
  int yCenter = int(height/2 - 1.0*PPCM + 4.0*PPCM/2);

  auxKeyboard.resize(int(4.0*PPCM),int(4.0*PPCM));
  if (zoomIn){
    
    auxX = int(Math.abs(width/2 - 2.0*PPCM - zoomX)) - SCALE;
    auxY = int(Math.abs(height/2 - 1.0*PPCM - zoomY)) - SCALE;
    
    if (auxX < 0)
       auxX = 0;
    else if (auxX + 2*SCALE > int(4.0*PPCM))
       auxX = int(4.0*PPCM) - 2*SCALE;    
        
    if (auxY < 0)
       auxY = 0;
    else if (auxY + 2*SCALE > int(4.0*PPCM))
       auxY = int(4.0*PPCM) - 2*SCALE;       
   
    auxImg = auxKeyboard.get(auxX, auxY, SCALE*2, SCALE*2);
    auxImg.resize(int(4.0*PPCM), int(2.0*PPCM));  
    image(auxImg,xCenter,yCenter);
  }else{
    auxKeyboard.resize(int(4.0*PPCM), int(2.0*PPCM)); 
    image(auxKeyboard,xCenter, yCenter);  
  }
  stroke(0);
  noFill();
  rect(width/2 - 2.0*PPCM, height/2, 4 * PPCM, 2.0 *PPCM);
}
void draw()
{ 
  // Check if we have reached the end of the study
  if (finishTime != 0)  return;
 
  background(255);                                                         // clear background
  
  // Draw arm and watch background
  imageMode(CENTER);
  image(arm, width/2, height/2, ARM_LENGTH, ARM_HEIGHT);
  
  // Check if we just started the application
  if (startTime == 0 && !mousePressed)
  {
    fill(0);
    textAlign(CENTER);
    text("Tap to start time!", width/2, height/2);
  }
  else if (startTime == 0 && mousePressed) nextTrial();                    // show next sentence
  
  // Check if we are in the middle of a trial
  else if (startTime != 0){
    textAlign(LEFT);
    fill(100);
    text("Phrase " + (currTrialNum + 1) + " of " + NUM_REPEATS, width/2 - 4.0*PPCM, 50);   // write the trial count
    text("Target:    " + currentPhrase, width/2 - 4.0*PPCM, 100);                           // draw the target string
    fill(0);
    text("Entered:  " + currentTyped + "|", width/2 - 4.0*PPCM, 140);                      // draw what the user has entered thus far 
    
    // Draw very basic ACCEPT button - do not change this!
    textAlign(CENTER);
    noStroke();
    fill(0, 250, 0);
    rect(width/2 - 2*PPCM, 170, 4.0*PPCM, 2.0*PPCM);
    fill(0);
    text("ACCEPT >", width/2, 220);
    
    // Draw screen areas
    // simulates text box - not interactive
    stroke(0);
    fill(255);
    rect(width/2 - 2.0*PPCM, height/2 - 2.0*PPCM, 4.0*PPCM, 1.0*PPCM);
    
    // THIS IS THE ONLY INTERACTIVE AREA (4cm x 4cm); do not change size
    stroke(0, 255, 0);
    noFill();
    rect(width/2 - 2.0*PPCM, height/2 - 1.0*PPCM, 4.0*PPCM, 3.0*PPCM);
    
    noStroke();
    fill(0);
    //draw predicted words
    stroke(0);
    noFill();
    for (int i = 0; i < predictedWords.size(); i++){
      rect(width/2 +PPCM * (-2.0 + 1.333333 * i), height/2 -PPCM, PPCM * 1.333333333, 0.5* PPCM);
      text(predictedWords.get(i).split("\t")[0], width/2 +PPCM * (-2.0 + 1.333333 * i), height/2 -PPCM, PPCM * 1.333333333, 0.5* PPCM/*(width / 2) + PPCM * (1.2*i - 1.2), height/2 - 0.3*PPCM*/);
    }
    Key storedKey = null;
    if (!zoomIn){
      if(checkMousePosition(width/2 - 2.0*PPCM, height/2 - 1.0*PPCM, 4.0*PPCM, 3.0*PPCM)){
        for( int i = 0; i < NUMBEROFKEYS; i++){ 
          Key keyAux = keyList.get(i);
          if(checkMousePosition(keyAux.x, keyAux.y, keyAux.w, keyAux.h)){
            storedKey = keyAux;
          }
        }
      }
    }
    else{
      float auxOverlap = 0;
      float aux = 0;
      for( int i = 0; i < NUMBEROFKEYS; i++){ 
        Key keyAux = keyList.get(i);
        int aux1 = int( (width/2 - 2.0*PPCM + auxX) + int(4.0*PPCM)/2 - (int(width/2 - 2.0*PPCM + 4.0*PPCM/2)));
        int t1 = int( ( (keyAux.x - (width/2 - 2.0*PPCM + auxX)) * ((4.0*PPCM)/(3.5*SCALE)) + keyAux.x) );
        int t2 = int( ( (keyAux.y - (height/2 - 1.0*PPCM + auxY)) * ((2.0*PPCM)/(2*SCALE)) + keyAux.y) );        
        
        if(keyAux.keyChar == ' '){
          aux = checkMouseClosestTarget(t1 - aux1 , t2 - 1.0*PPCM, SCALE, SCALE);
        }else if(keyAux.keyChar == '`'){
          aux = checkMouseClosestTarget( t1 - aux1 , t2 - 1.0*PPCM, SCALE, SCALE);
        }      
        else{
          aux = checkMouseClosestTarget( t1 - aux1, t2 - 1.0*PPCM,SCALE, SCALE);
        }  
        if ((aux > 0) && (aux > auxOverlap)){ 
          auxOverlap = aux;
          storedKey = keyAux;
        }
      }
    }
    textFont(createFont("Arial", 50));
    if (storedKey != null){
      if(storedKey.keyChar == ' '){ 
      fill(0);
      text("_", width/2, height/2 - 1.3*PPCM);  
      }
      else if(storedKey.keyChar == '`'){
      image(backspace, width/2, height/2 - 1.5*PPCM, KEYW + 0.5*PPCM, KEYH + 0.2*PPCM);
      }
      else{
      fill(0);
      text("" + storedKey.keyChar, width/2, height/2 - 1.3*PPCM);
      }
    }
    textFont(createFont("Arial", 24));  
    drawKeyboard();           
  }
  
  
  // Draw the user finger to illustrate the issues with occlusion (the fat finger problem)
  imageMode(CORNER);
  image(fingerOcclusion, mouseX - FINGER_OFFSET, mouseY - FINGER_OFFSET, FINGER_SIZE, FINGER_SIZE);
}

// Check if mouse click was within certain bounds
boolean checkMousePosition(float x, float y, float w, float h)
{
  return (mouseX > x && mouseX < x + w && mouseY > y && mouseY < y + h);
    //return (Math.pow((x+w/2) - mouseX, 2) + Math.pow((y+h/2) - mouseY,2)) < Math.pow(radius,2);
}

float checkMouseClosestTarget(float x, float y, float w, float h){
  //click as a square (easier)
  int radius = int(0.1*PPCM);
  float x_overlap = Math.max(0, Math.min(2*mouseX, 2*x+w ) - Math.max(mouseX - radius, x));
  float y_overlap = Math.max(0, Math.min(2*mouseY, 2*y + h) - Math.max(mouseX - radius, y));

  return x_overlap * y_overlap;
}


void mousePressed()
{

  if (checkMousePosition(width/2 - 2*PPCM, 170, 4.0*PPCM, 2.0*PPCM)){ 
    nextTrial();
  }
 
  else if(checkMousePosition(width/2 - 2.0*PPCM, height/2 - 1.5 * PPCM, 4.0*PPCM, PPCM)){
    for (int i = 0; i < 3; i++){
      if(checkMousePosition(width/2 +PPCM * (-2.0 + 1.333333 * i), height/2 - 1.0 * PPCM, PPCM * 1.33333, 0.5 * PPCM)){
        sketch = "";
        if (currentTyped.length() == 0)
          currentTyped += predictedWords.get(i).split("\t")[0] + ' ';
        else{
          if (splitTokens(currentTyped).length == 1)
            currentTyped = predictedWords.get(i).split("\t")[0] + ' ';
          else{
            while (currentTyped.charAt(currentTyped.length() - 1) != ' ')
              currentTyped = currentTyped.substring(0, currentTyped.length()-1);
            currentTyped += predictedWords.get(i).split("\t")[0] + ' ';
          }
          sketch = "";
          break;
        }
      }
    }
  }   

  else if(checkMousePosition(width/2 - 2.0*PPCM, height/2 - 1.0*PPCM, 4.0*PPCM, 3.0*PPCM) && startTime != 0){
    if (!zoomIn){
      System.out.println("debug: Zoom is now true!");
      zoomIn = true; 
      zoomX = mouseX;
      zoomY = mouseY;
    }else{
      Key storedKey = null;
      float auxOverlap = 0;
      float aux = 0;
      for( int i = 0; i < NUMBEROFKEYS; i++){ 
        Key keyAux = keyList.get(i);
        int aux1 = int( (width/2 - 2.0*PPCM + auxX) + int(4.0*PPCM)/2 - (int(width/2 - 2.0*PPCM + 4.0*PPCM/2)));
        int t1 = int( ( (keyAux.x - (width/2 - 2.0*PPCM + auxX)) * ((4.0*PPCM)/(3.5*SCALE)) + keyAux.x) );
        int t2 = int( ( (keyAux.y - (height/2 - 1.0*PPCM + auxY)) * ((2.0*PPCM)/(2*SCALE)) + keyAux.y) );        
        
        if(keyAux.keyChar == ' '){
          aux = checkMouseClosestTarget(t1 - aux1 , t2 - 1.0*PPCM, SCALE, SCALE);
          System.out.println(aux);
        }else if(keyAux.keyChar == '`'){
          aux = checkMouseClosestTarget( t1 - aux1 , t2 - 1.0*PPCM, SCALE, SCALE);
        }      
        else{
          aux = checkMouseClosestTarget( t1 - aux1, t2 - 1.0*PPCM,SCALE, SCALE);
          if(keyAux.keyChar == 't') System.out.println(" t: " + aux);
        }  
        if ((aux > 0) && (aux > auxOverlap)){ 
          auxOverlap = aux;
          storedKey = keyAux;
          System.out.println("Stored is now: " + storedKey.keyChar);
          
        }
        
      }
      
      if (storedKey !=null){
        if(storedKey.keyChar == ' '){
          currentTyped += " ";
          sketch = "";
        }else if(storedKey.keyChar == '`'){
          if (currentTyped.length() != 0)
            currentTyped = currentTyped.substring(0, currentTyped.length() - 1);
          if (sketch.length() != 0)
            sketch = sketch.substring(0, sketch.length() - 1);
        }      
        else{
          currentTyped += storedKey.keyChar;
          sketch += storedKey.keyChar;
        }
      }
     zoomIn = false;
    }    
  }
  else System.out.println("debug: CLICK NOT ACCEPTED");
  //get new predicted words
  predictedWords.clear();
  int counter = 0;
  for (int i = 0; i < words.length; i++){
    if (counter == 3) break;
    if (words[i].startsWith(sketch)){
      predictedWords.add(words[i]);
      counter++;
    }
  } 
}



void nextTrial()
{
  if (currTrialNum >= NUM_REPEATS) return;                                            // check to see if experiment is done
  
  // Check if we're in the middle of the tests
  else if (startTime != 0 && finishTime == 0)                                         
  {
    System.out.println("==================");
    System.out.println("Phrase " + (currTrialNum+1) + " of " + NUM_REPEATS);
    System.out.println("Target phrase: " + currentPhrase);
    System.out.println("Phrase length: " + currentPhrase.length());
    System.out.println("User typed: " + currentTyped);
    System.out.println("User typed length: " + currentTyped.length());
    System.out.println("Number of errors: " + computeLevenshteinDistance(currentTyped.trim(), currentPhrase.trim()));
    System.out.println("Time taken on this trial: " + (millis() - lastTime));
    System.out.println("Time taken since beginning: " + (millis() - startTime));
    System.out.println("==================");
    lettersExpectedTotal += currentPhrase.trim().length();
    lettersEnteredTotal += currentTyped.trim().length();
    errorsTotal += computeLevenshteinDistance(currentTyped.trim(), currentPhrase.trim());
  }
  
  // Check to see if experiment just finished
  if (currTrialNum == NUM_REPEATS - 1)                                           
  {
    finishTime = millis();
    System.out.println("==================");
    System.out.println("Trials complete!"); //output
    System.out.println("Total time taken: " + (finishTime - startTime));
    System.out.println("Total letters entered: " + lettersEnteredTotal);
    System.out.println("Total letters expected: " + lettersExpectedTotal);
    System.out.println("Total errors entered: " + errorsTotal);

    float wpm = (lettersEnteredTotal / 5.0f) / ((finishTime - startTime) / 60000f);   // FYI - 60K is number of milliseconds in minute
    float freebieErrors = lettersExpectedTotal * .05;                                 // no penalty if errors are under 5% of chars
    float penalty = max(errorsTotal - freebieErrors, 0) * .5f;
    
    System.out.println("Raw WPM: " + wpm);
    System.out.println("Freebie errors: " + freebieErrors);
    System.out.println("Penalty: " + penalty);
    System.out.println("WPM w/ penalty: " + (wpm - penalty));                         // yes, minus, because higher WPM is better
    System.out.println("==================");
    
    printResults(wpm, freebieErrors, penalty);
    
    currTrialNum++;                                                                   // increment by one so this mesage only appears once when all trials are done
    return;
  }

  else if (startTime == 0)                                                            // first trial starting now
  {
    System.out.println("Trials beginning! Starting timer...");
    startTime = millis();                                                             // start the timer!
  } 
  else currTrialNum++;                                                                // increment trial number

  lastTime = millis();                                                                // record the time of when this trial ended
  currentTyped = "";                                                                  // clear what is currently typed preparing for next trial
  currentPhrase = phrases[currTrialNum];                                              // load the next phrase!
}

// Print results at the end of the study
void printResults(float wpm, float freebieErrors, float penalty)
{
  background(0);       // clears screen
  
  textFont(createFont("Arial", 16));    // sets the font to Arial size 16
  fill(255);    //set text fill color to white
  text(day() + "/" + month() + "/" + year() + "  " + hour() + ":" + minute() + ":" + second(), 100, 20);   // display time on screen
  
  text("Finished!", width / 2, height / 2); 
  text("Raw WPM: " + wpm, width / 2, height / 2 + 20);
  text("Freebie errors: " + freebieErrors, width / 2, height / 2 + 40);
  text("Penalty: " + penalty, width / 2, height / 2 + 60);
  text("WPM with penalty: " + (wpm - penalty), width / 2, height / 2 + 80);

  saveFrame("results-######.png");    // saves screenshot in current folder    
}

// This computes the error between two strings (i.e., original phrase and user input)
int computeLevenshteinDistance(String phrase1, String phrase2)
{
  int[][] distance = new int[phrase1.length() + 1][phrase2.length() + 1];

  for (int i = 0; i <= phrase1.length(); i++) distance[i][0] = i;
  for (int j = 1; j <= phrase2.length(); j++) distance[0][j] = j;

  for (int i = 1; i <= phrase1.length(); i++)
    for (int j = 1; j <= phrase2.length(); j++)
      distance[i][j] = min(min(distance[i - 1][j] + 1, distance[i][j - 1] + 1), distance[i - 1][j - 1] + ((phrase1.charAt(i - 1) == phrase2.charAt(j - 1)) ? 0 : 1));

  return distance[phrase1.length()][phrase2.length()];
}
