#include <Arduino.h>
#include <Preferences.h>
Preferences prefs;

#define LED 2
//קריאת נתונים חיוצנית ושמירה בתוכנה

// void setup() {
// Serial.begin(115200);

 
//     delay(1000);

//   Serial.println("Hi, what's your name?");

//   while (Serial.available() == 0) {
//     delay(10);
//   }

//   String name = Serial.readStringUntil('\n'); // קורא את הקלט עד שורה חדשה
//   name.trim(); // מסיר רווחים ותווי סוף שורה מיותרים

//   prefs.begin("user",false);
//   prefs.putString("name",name);
//   prefs.end();
// }

// void loop() {
// delay(1000);
// prefs.begin("user",true);
// String userName= prefs.getString("name","");
//  prefs.end();
// Serial.println("your name "+userName);
// }
// void setup() {
// Serial.begin(115200);
// }
// void loop() {
//   int variable_1 = analogRead(34);
//    int variable_2 = analogRead(35); 
//    int variable_3 = analogRead(32); 
//    int variable_4 = analogRead(33); 
//    Serial.print(variable_1);
//     Serial.print(" "); // a tab '\t' or space ' ' character is printed between the two values.
//      Serial.print(variable_2); Serial.print(" "); // a tab '\t' or space ' ' character is printed between the two values. 
//      Serial.print(variable_3); Serial.print(" "); // a tab '\t' or space ' ' character is printed between the two values.
//       Serial.println(variable_4); // the last value is terminated by a newline characters. 
//       delay(100); 
// }
#include <OneWire.h>
#include <DallasTemperature.h>

#define TRIG_PIN 23
#define ECHO_PIN 22
#define TEMP_PIN 4

#define RELAY_VALVE 18    // ברז חשמלי
#define RELAY_HEATER 19   // גוף חימום

#define MAX_DISTANCE_CM 1.0  // מרחק גדול מ־2 מטר = גובה מים נמוך
#define MAX_TEMP_C 40.0       // כיבוי חימום מעל 100°C

#define TIMEOUT_MICROS 30000   // טיים־אאוט למניעת תקיעות

OneWire oneWire(TEMP_PIN);
DallasTemperature sensors(&oneWire);

float readTemperature() {
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  if (temp == DEVICE_DISCONNECTED_C || temp < -30 || temp > 120)
    return -127.0;
  return temp;
}

float ultrasonicMeasure() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, TIMEOUT_MICROS);
  if (duration == 0) return -1;
  float distance = duration * 0.034 / 2;
  return distance;
}

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY_VALVE, OUTPUT);
  pinMode(RELAY_HEATER, OUTPUT);

  digitalWrite(RELAY_VALVE, LOW);   // ברז סגור
  digitalWrite(RELAY_HEATER, LOW);  // חימום כבוי

  sensors.begin();
}
void loop() {
  float temp = readTemperature();
  float distance = ultrasonicMeasure();

  bool tempValid = temp != -127.0;
  bool distanceValid = distance > 0 && distance < 400;

  Serial.print("טמפ': ");
  Serial.print(temp);
  Serial.print(" °C | מרחק: ");
  Serial.print(distance);
  Serial.println(" ס\"מ");

  // מצב ברירת מחדל
  bool turnOnValve = false;
  bool turnOnHeater = true;

  // ✅ אם גובה המים נמוך – מדליקים ברז
  if (distanceValid && distance > MAX_DISTANCE_CM) {
    turnOnValve = true;
  }

  // ❌ אם טמפ' לא תקינה או חורגת – מכבים חימום
  if (!tempValid || temp >= MAX_TEMP_C) {
    turnOnHeater = false;
  }

  // הפעלה בפועל
  digitalWrite(RELAY_VALVE, turnOnValve ? HIGH : LOW);
  digitalWrite(RELAY_HEATER, turnOnHeater ? HIGH : LOW);

  Serial.print("→ ברז ");
  Serial.print(turnOnValve ? "פתוח" : "סגור");
  Serial.print(", חימום ");
  Serial.println(turnOnHeater ? "פעיל" : "כבוי");

  delay(10000); // כל 10 שניות
}

