
import processing.serial.*;
Serial port;



int roomScale = 3;
int drawStart = 75;

int doorwayWidth = 15 * roomScale;

int[] roomWidth = { 
  130 * roomScale, 60 * roomScale, 25 * roomScale, 85 * roomScale, 130 * roomScale
};
int[] roomHeight = { 
  100 * roomScale, 90 * roomScale, 90 * roomScale, 85 * roomScale, 75 * roomScale
};


String OLDDATA = "nothing";

Integer[] persons = {
  0, 0, 0, 0
};

int[] doorwayState = {
  0, 0, 0
};

void setup()
{
  size(800, 700);
  port = new Serial(this, "COM14", 9600);
  port.bufferUntil('\n');  
  textAlign(CENTER, CENTER);
  textSize(20);
}

void draw()
{
  background(0);

  drawRooms();
}

void drawRooms()
{
  stroke(255);

  strokeWeight(10);

  fill(150);

  rect(drawStart, drawStart, roomWidth[0], roomHeight[0]);

  rect(drawStart + roomWidth[0], drawStart, roomWidth[1], roomHeight[1]);

  rect(drawStart + roomWidth[0] + roomWidth[1], drawStart, roomWidth[2], roomHeight[2]);

  rect(drawStart + roomWidth[0], drawStart + roomHeight[1], roomWidth[3], roomHeight[3]);

  rect(drawStart, drawStart + roomHeight[0], roomWidth[4], roomHeight[4]);



  if (checkDoorway(0)) {
    stroke(255, 0, 0);
  } else {
    stroke(50);
  }


  line(drawStart + roomWidth[0], drawStart + roomHeight[0] * 0.6, drawStart + roomWidth[0], doorwayWidth + drawStart + roomHeight[0] * 0.6);

  if (checkDoorway(1)) {
    stroke(255, 0, 0);
  } else {
    stroke(50);
  }


  line(drawStart + roomWidth[0] + roomWidth[1], drawStart + roomHeight[0] * 0.15, drawStart + roomWidth[0] + roomWidth[1], doorwayWidth + drawStart + roomHeight[0] * 0.15);

  if (checkDoorway(2)) {
    stroke(255, 0, 0);
  } else {
    stroke(50);
  }


  line(drawStart + roomWidth[0] + roomWidth[1] + roomWidth[2], drawStart + roomHeight[0] * 0.1, drawStart + roomWidth[0] + roomWidth[1] + roomWidth[2], 2 * doorwayWidth + drawStart + roomHeight[0] * 0.1);

  stroke(50);

  line(drawStart + roomWidth[0] + roomWidth[1] + (roomWidth[2] - doorwayWidth)/2, drawStart + roomHeight[2], drawStart + roomWidth[0] + roomWidth[1] + doorwayWidth + (roomWidth[2] - doorwayWidth)/2, drawStart + roomHeight[2]);

  line(drawStart + roomWidth[0], drawStart + roomHeight[0] + roomHeight[4] * 0.4, drawStart + roomWidth[0], doorwayWidth + drawStart + roomHeight[0] + roomHeight[4] * 0.4);


  fill(255);

  for (int i = 0; i < 4; i++) {
    text(persons[i].toString(), 20, 20+20*i);
  }
  
  text(OLDDATA, 200, 30);
}

boolean checkDoorway(int currentDoorway)
{
  boolean result = false;
/*  
  if (doorwayState[currentDoorway] > 0) {
    result = true;
  }
  
  return result;
*/
  switch(doorwayState[currentDoorway]) {
  case 1:
    result = true;
    if (persons[currentDoorway] > 0) {
      persons[currentDoorway]--;
    }
    persons[currentDoorway+1]++;
    doorwayState[currentDoorway] = 0;
    break;

  case 2:
    result = true;
    if (persons[currentDoorway+1] > 0) {
      persons[currentDoorway+1]--;
    }
    persons[currentDoorway]++;
    doorwayState[currentDoorway] = 0;
    break;
  }  
  return result;
  
}

void serialEvent (Serial port)
{
  try {
    String data = port.readStringUntil('\r');
 /*   
    while (port.available() > 0) {
    int inByte = port.read();
    println(inByte);
    }
*/
    OLDDATA = data;
    
    data.trim();
    
    String[] list = split(data, ':');
    //int[] doorway = int(split(data, ':'));
    
    Integer a = int(list[0]);
    Integer b = int(list[1].substring(0, 1));
    

//    doorwayState[doorway[0]]=doorway[1];
    doorwayState[a]=b;
    
    println(data);
    println(list[0]);
    println(a);
    
    println(list[1]);
    println(b);
    
  
    
//    println("ggg");
    
//    println(doorway[0]);
//    println(doorway[1]);
    
    
    
  } 
  catch (Exception e) {
    println("Connection...");
  }
}

void stop() {
  port.clear();
  port.stop();
} 

