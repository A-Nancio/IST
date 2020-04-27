// Bakeoff #2 - Seleção de Alvos e Fatores Humanos //<>// //<>//
// IPM 2019-20, Semestre 2
// Bake-off: durante a aula de lab da semana de 20 de Abril
// Submissão via Twitter: exclusivamente no dia 24 de Abril, até às 23h59

// Processing reference: https://processing.org/reference/

import java.util.Collections;
import java.util.Iterator;
import processing.sound.*;
SoundFile correct;
SoundFile wrong;

Target follow;

// Target properties
float PPI, PPCM;
float SCALE_FACTOR;
float TARGET_SIZE;
float TARGET_PADDING, MARGIN, LEFT_PADDING, TOP_PADDING;

// Study properties
ArrayList<Float> data = new ArrayList<Float>();
ArrayList<Integer> trials  = new ArrayList<Integer>();    // contains the order of targets that activate in the test
int trialNum               = 0;                           // the current trial number (indexes into trials array above)
final int NUM_REPEATS      = 3;                           // sets the number of times each target repeats in the test - FOR THE BAKEOFF NEEDS TO BE 3!
boolean ended              = false;

// Performance variables
int startTime              = 0;      // time starts when the first click is captured
int finishTime             = 0;      // records the time of the final click
int hits                   = 0;      // number of successful clicks
int misses                 = 0;      // number of missed clicks
float cursorRadius;

// Class used to store properties of a target
class Target
{
  float x, y;
  float w;

  Target(float posx, float posy, float twidth) 
  {
    x = posx;
    y = posy;
    w = twidth;
  }
}


// Setup window and vars - runs once
void setup()
{
  //size(900, 900);              // window size in px (use for debugging)
  fullScreen();                // USE THIS DURING THE BAKEOFF!
  
  SCALE_FACTOR    = 1.0 / displayDensity();            // scale factor for high-density displays
  String[] ppi_string = loadStrings("ppi.txt");        // The text from the file is loaded into an array.
  PPI            = float(ppi_string[1]);               // set PPI, we assume the ppi value is in the second line of the .txt
  PPCM           = PPI / 2.54 * SCALE_FACTOR;          // do not change this!
  TARGET_SIZE    = 1.5 * PPCM;                         // set the target size in cm; do not change this!
  TARGET_PADDING = 1.5 * PPCM;                         // set the padding around the targets in cm; do not change this!
  MARGIN         = 1.5 * PPCM;                         // set the margin around the targets in cm; do not change this!
  LEFT_PADDING   = width/2 - TARGET_SIZE - 1.5*TARGET_PADDING - 1.5*MARGIN;        // set the margin of the grid of targets to the left of the canvas; do not change this!
  TOP_PADDING    = height/2 - TARGET_SIZE - 1.5*TARGET_PADDING - 1.5*MARGIN;       // set the margin of the grid of targets to the top of the canvas; do not change this!
  noStroke();        // draw shapes without outlines
  frameRate(60);     // set frame rate

  // Text and font setup
  textFont(createFont("Arial", 16));    // sets the font to Arial size 16
  textAlign(CENTER);                    // align text
  
  correct = new SoundFile(this, "correct.wav");
  wrong = new SoundFile(this, "wrong.wav");
  correct.amp(0.3);
  wrong.amp(0.3);
  randomizeTrials();    // randomize the trial order for each participant
  follow = getTargetBounds(trialNum);
}

// Updates UI - this method is constantly being called and drawing targets
void draw()
{
  if (hasEnded()) return; // nothing else to do; study is over

  background(0);       // set background to black

  // Print trial count
  fill(255);          // set text fill color to white
  text("Trial " + (trialNum + 1) + " of " + trials.size(), 50, 20);    // display what trial the participant is on (the top-left corner)
  
  
  // Draw targets
  for (int i = 0; i < 16; i++) drawTarget(i);
  // Draw mouse
  
  Target firstClosest = getTargetBounds(0);
  Target secondClosest = getTargetBounds(1);
  float first = 0;
  fill(255);
  for (int i = 0; i < 16; i++) 
  {
    Target target = getTargetBounds(i);
    if (dist(target.x, target.y, mouseX, mouseY) < dist(firstClosest.x, firstClosest.y, mouseX, mouseY))
    {
      first = i;
      secondClosest = firstClosest;
      firstClosest = target;
    }

    else if (dist(target.x,target.y,mouseX,mouseY) < dist(secondClosest.x, secondClosest.y, mouseX, mouseY) && (first != i))
    {
      secondClosest = target;
    }
  }
  
  float conDi = TARGET_SIZE/2 + dist(firstClosest.x, firstClosest.y, mouseX, mouseY);
  
  float intDj = dist(secondClosest.x, secondClosest.y, mouseX, mouseY) - TARGET_SIZE/2;
  cursorRadius = min(conDi, intDj); 
  
  fill(0,0,255,125);
  blendMode(EXCLUSION);
  circle(firstClosest.x, firstClosest.y, firstClosest.w * 1.2);
  circle(mouseX, mouseY, cursorRadius*2);
}

boolean hasEnded() {
  if (ended) return true;    // returns if test has ended before

  // Check if the study is over
  if (trialNum >= trials.size())
  {
    float timeTaken = (finishTime-startTime) / 1000f;     // convert to seconds - DO NOT CHANGE!
    float penalty = constrain(((95f-((float)hits*100f/(float)(hits+misses)))*.2f), 0, 100);    // calculate penalty - DO NOT CHANGE!

    printResults(timeTaken, penalty);    // prints study results on-screen
    ended = true;
  }

  return ended;
}

// Randomize the order in the targets to be selected
// DO NOT CHANGE THIS METHOD!
void randomizeTrials()
{
  for (int i = 0; i < 16; i++)             // 4 rows times 4 columns = 16 target
    for (int k = 0; k < NUM_REPEATS; k++)  // each target will repeat 'NUM_REPEATS' times
      trials.add(i);
  Collections.shuffle(trials);             // randomize the trial order

  System.out.println("trial order: " + trials);    // prints trial order - for debug purposes
}

// Print results at the end of the study
void printResults(float timeTaken, float penalty)
{
  background(0);       // clears screen

  fill(255);    //set text fill color to white
  text(day() + "/" + month() + "/" + year() + "  " + hour() + ":" + minute() + ":" + second(), 100, 20);   // display time on screen

  text("Finished!", width / 2, height / 4); 
  text("Hits: " + hits, width / 2, height / 4 + 20);
  text("Misses: " + misses, width / 2, height / 4 + 40);
  text("Accuracy: " + (float)hits*100f/(float)(hits+misses) +"%", width / 2, height / 4 + 60);
  text("Total time taken: " + timeTaken + " sec", width / 2, height / 4 + 80);
  text("Average time for each target: " + nf((timeTaken)/(float)(hits+misses), 0, 3) + " sec", width / 2, height / 4 + 100);
  text("Average time for each target + penalty: " + nf(((timeTaken)/(float)(hits+misses) + penalty), 0, 3) + " sec", width / 2, height / 4 + 140);
  Iterator<Float> it = data.iterator();
  text("Fitts Index of Performance", width / 2, height / 4 + 180); 
  text("Target 1: ---", width/2 - 200, height / 4 + 200);
  int i = 2;
  while (it.hasNext())
  {
    if (i > 24)
      break;
    float val = it.next();
    if (val == 0.00) 
      text("Target " + i + ": MISSED", width / 2 - 200, height / 4 + 180 + 20 * i);
    else
      text("Target " + i + ": " + val, width / 2 - 200, height / 4 + 180 + 20 * i);
    i++;
  }
  
  while (it.hasNext())
  {
    float val = it.next();
    if (val == 0.00) 
      text("Target " + i + ": MISSED", width / 2 + 200, height / 4 + 200 + 20 * (i % 25));
    else
      text("Target " + i + ": " + val, width / 2 + 200, height / 4 + 200 + 20 * (i % 25));
    i++;
  }

  saveFrame("results-######.png");    // saves screenshot in current folder
}

// Mouse button was released - lets test to see if hit was in the correct target
void mouseReleased() 
{
  if (trialNum >= trials.size()) return;      // if study is over, just return
  if (trialNum == 0) startTime = millis();    // check if first click, if so, start timer
  if (trialNum == trials.size() - 1)          // check if final click
  {
    finishTime = millis();    // save final timestamp
    println("We're done!");
  }

  Target target = getTargetBounds(trials.get(trialNum));    // get the location and size for the target in the current trial

  // Check to see if mouse cursor is inside the target bounds
  if (dist(target.x, target.y, mouseX, mouseY) < target.w/2 + cursorRadius)
  {
    System.out.println("HIT! " + trialNum + " " + (millis() - startTime));     // success - hit!
    // Adicionar som
    hits++; // increases hits counter
    correct.play();
    if (trialNum != 0 && trialNum != trials.size())
    {
      Target aux = getTargetBounds(trialNum + 1);
      float prfm = dist(mouseX,mouseY,aux.x,aux.y);
      data.add(log(prfm/aux.w + 1) / log(2));
    }
  } 
  else
  {
    System.out.println("MISSED! " + trialNum + " " + (millis() - startTime));  // fail
    // Adicionar som
    wrong.play();
    misses++;   // increases misses counter
    data.add(0.0);
  }

  trialNum++;   // move on to the next trial; UI will be updated on the next draw() cycle
}  

// For a given target ID, returns its location and size
Target getTargetBounds(int i)
{
  int x = (int)LEFT_PADDING + (int)((i % 4) * (TARGET_SIZE + TARGET_PADDING) + MARGIN);
  int y = (int)TOP_PADDING + (int)((i / 4) * (TARGET_SIZE + TARGET_PADDING) + MARGIN);

  return new Target(x, y, TARGET_SIZE);
}

// Draw target on-screen
// This method is called in every draw cycle; you can update the target's UI here
void drawTarget(int i)
{
  Target target = getTargetBounds(i);   // get the location and size for the circle with ID:i

  // check whether current circle is the intended target
  if (trials.get(trialNum) == i) 
  { 
    // if so ...
    stroke((220));     // stroke light gray
    strokeWeight(4);   // stroke weight 2
    fill(#66FF66);         // fill red
    
    circle(target.x, target.y, target.w);   // draw target
    follow.x = lerp(follow.x, target.x, 0.25);
    follow.y = lerp(follow.y, target.y, 0.25);
    
    fill(255, 2, 2, 0);
    circle(follow.x, follow.y, follow.w);
  } 
  else if (trialNum == trials.size()) 
  {
    fill(#E80E0E);         // fill dark gray
    circle(target.x, target.y, target.w);   // draw target
  } 
  else if (trialNum + 1 < trials.size()) 
  {
    if (trials.get(trialNum + 1) == i) 
    {
      fill(#E80E0E);
      stroke(#ffff00);
      strokeWeight(8);
      circle(target.x, target.y, target.w);
    } 
    else 
    {
    fill(#E80E0E);         // fill dark gray
    circle(target.x, target.y, target.w);   // draw target
    } 
  } 
  else 
  {
    fill(#E80E0E);         // fill dark gray
    circle(target.x, target.y, target.w);   // draw target
  }
  noStroke();    // next targets won't have stroke (unless it is the intended target)
}
