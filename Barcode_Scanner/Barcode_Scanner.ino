//Library for PS2 port connection
#include <PS2Keyboard.h>
//Library for lcd
#include <LiquidCrystal.h>
const int DataPin=4;
const int ClockPin=3;
//object for the library
char string[14];
PS2Keyboard scanner;
//Structure for a student object
struct Student{
  String id;
  float attendance_B;
  float attendance_C;
  float attendance_D;
  bool checked;
};
//Declared students
Student S1={"2652AT-0326-2",0,0,0,false};       //add 13 digit barcode of student in 1st value
Student S2={"3536100183151",0,0,0,false};
Student S3={"SA2652A-6082",0,0,0,false};
float noOfClasses=5;
LiquidCrystal lcd(12, 11, 36, 38, 40, 42);

//function to read the data from the barcode scanner returns true if new data arrived and false otherwise
bool getString(){
  //variable for the indexes of the string
  int i=0;
  char c=0;
  while(1){
    //checks to see if new values arrived from the scanner
    if(scanner.available()){
      //gets a single charachter from the scanner
      c=scanner.read();
      //store the charachter in a string at the current index
      string[i]=c;
      //increment the index if the current charachter is not a line end
      if(c!=PS2_ENTER){
        //string[i]=c;
        i++;
      }
      //if the index of the string goes greater than 12 end the string and return true
      if(i>12){
        string[i]='\0';
        i++;
        string[i]='\0';
        Serial.println(string);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(string);
        return true;
      }
       delay(10);
    }
    //return false if there was no data available form the scanner
    else{
      return false; 
    }
  }
}

//Function for starting a class and checking the students entering
//cl is the charachter with the class name B,C or D
void startClass(char cl){
  Serial.println();
  Serial.print("Class ");
  Serial.print(cl);
  Serial.print(" started!\n");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Class ");
  lcd.print(cl);
  lcd.setCursor(0,1);
  lcd.print("started!");
  //variable to store current time when the class is starting
  unsigned long startTime=millis();
  //the total interval of the class no. of secs * 1000
  unsigned long interval=5*1000;
  //runs the loop for the currently set interval
  while(millis()-startTime<=interval){
    if(getString()){              //if new data arrives from scanner
      String a=string;            //stores the charachter array in a String object for comparison
      if(a==S1.id && !S1.checked){               //Compare string from scanner with the id of Student-1 and see if the student isnt already checked in
        Serial.println("S1 checked");
        lcd.setCursor(0,1);
        lcd.print("S1 checked");
        S1.checked=true;          //The student is now checked in for class
      }
      //Same step repeted for S2 and S3
      else if(a==S2.id && !S2.checked){
        Serial.println("S2 checked");
        lcd.setCursor(0,1);
        lcd.print("S2 checked");
        S2.checked=true;
      }
      else if(a==S3.id && !S3.checked){
        Serial.println("S3 checked");
        lcd.setCursor(0,1);
        lcd.print("S3 checked");
        S3.checked=true;
      }
      //if the id from the scanner does not match any id from the students the student is not registered in the class
      else{
        lcd.clear();
        lcd.print("Unregistered");
        lcd.setCursor(0,1);
        lcd.print("Student");
        Serial.println("The student is not registered in this class");
      }
    }
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Class Ended!");
  Serial.println("Class Ended!");
  delay(2000);
}

//Function for starting a final and checking the attendance of the students
void startFinal(char cl){
  Serial.println();
  Serial.print("Final ");
  Serial.print(cl);
  Serial.print(" Started!");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Final ");
  lcd.print(cl);
  lcd.setCursor(0,1);
  lcd.print("Started!");
  //variable to store current time when starting the final
  unsigned long startTime=millis();
  //to store the total interval value of the final no. of secs * 1000
  unsigned long interval=10*1000;
  //variable to store the calculated attendance
  float attendance=0;
  while(millis()-startTime<=interval){            //runs the loop for the time interval set above
    if(getString()){                              //check to see if new value arrived from the scanner
      String a=string;                            //store the value in the String object
      if(a==S1.id){                               //check to see if the string is equal to the id of 1st student
        if(cl=='B'){                              //check to see the current class for the final
          attendance=S1.attendance_B/noOfClasses;          //calculate attendance by dividing classes taken by total classes
        }
        else if(cl=='C'){                         //repeat for class C and D
          attendance=S1.attendance_C/noOfClasses;
        }
        else if(cl=='D'){
          attendance=S1.attendance_D/noOfClasses;
        }
      }
      else if(a==S2.id){                         //repeat the steps for Student 2
        if(cl=='B'){
          attendance=S2.attendance_B/noOfClasses;
        }
        else if(cl=='C'){
          attendance=S2.attendance_C/noOfClasses;
        }
        else if(cl=='D'){
          attendance=S2.attendance_D/noOfClasses;
        }
      }
      else if(a==S3.id){                        //repeat the steps for Student 3
        if(cl=='B'){
          attendance=S3.attendance_B/noOfClasses;
        }
        else if(cl=='C'){
          attendance=S3.attendance_C/noOfClasses;
        }
        else if(cl=='D'){
          attendance=S3.attendance_D/noOfClasses;
        }
      }
      else{              //if the string from the scanner does not match any students id the student is not registered
        lcd.clear();
        lcd.print("Unregistered");
        lcd.setCursor(0,1);
        lcd.print("Student");
        Serial.println("The student is not registered in this class");
      }
      attendance=attendance*100;            //convert attendance to percentage
      if(attendance<75){                    //if the attendace is less than .75 or 75% the student cannot enter
        Serial.print("Attendance is ");
        Serial.print(attendance);
        Serial.println(" You cannot enter");
        lcd.clear();
        lcd.print("Attendance ");
        lcd.print(attendance);
        lcd.print('%');
        lcd.setCursor(0,1);
        lcd.print("You cannot enter");
      }
      else{                                   //else the student can enter
        Serial.print("Attendance is ");
        Serial.print(attendance);
        Serial.println("You can enter the hall");
        lcd.clear();
        lcd.print("Attendance ");
        lcd.print(attendance);
        lcd.print('%');
        lcd.setCursor(0,1);
        lcd.print("You can enter");
      }
    }
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Final Ended!");
  Serial.println("Final Ended!");
  delay(2000);
}

void setup() {
  //makes the connection with the scanner
  scanner.begin(DataPin,ClockPin);
  delay(1000);
  lcd.begin(16, 2);
  Serial.begin(9600);
  Serial.println("Test:");
}

void loop() {
//  while(1){
//  if(getString()){
//    //String a=S1.id;
//    String b=string;
//    Serial.println(b==S1.id);
//  }
//  }
  //counters for the no of classes that have been marked
  int counter_B=0;
  int counter_C=0;
  int counter_D=0;
  while(counter_B<noOfClasses && counter_C<noOfClasses && counter_D<noOfClasses){
    startClass('B');        //start class B
    if(S1.checked){         //if student 1 is checked after class
      S1.attendance_B++;    //increase the attendance of the student
      S1.checked=false;     //uncheck the student for the next class
    }
    if(S2.checked){         //repeat steps for S2
      S2.attendance_B++;
      S2.checked=false;
    }
    if(S3.checked){         //repeat steps for S3
      S3.attendance_B++;
      S3.checked=false;
    }    
    counter_B++;            //incement counter for class B
    startClass('C');        //repeat all steps for class C
    if(S1.checked){
      S1.attendance_C++;
      S1.checked=false;
    }
    if(S2.checked){
      S2.attendance_C++;
      S2.checked=false;
    }
    if(S3.checked){
      S3.attendance_C++;
      S3.checked=false;
    }
    counter_C++;        
    startClass('D');        //repeat all steps for class D
    if(S1.checked){
      S1.attendance_D++;
      S1.checked=false;
    }
    if(S2.checked){
      S2.attendance_D++;
      S2.checked=false;
    }
    if(S3.checked){
      S3.attendance_D++;
      S3.checked=false;
    }
    counter_D++;
  }
  Serial.println();
  Serial.println();
  delay(1000);
  startFinal('B');        //start finals B,C and D
  startFinal('C');
  startFinal('D');
}
