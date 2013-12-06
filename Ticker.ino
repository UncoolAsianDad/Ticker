#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <Readline.h>
#include <SoftwareSerial.h>  
#include <Time.h>
#include <stdio.h>

// Read a line in from stream
ReadLine r;
char tmp[64] = "";

// Bluetooth Stream
//int bluetoothTx = 2;  // TX-O pin of bluetooth mate, Arduino D2
//int bluetoothRx = 3;  // RX-I pin of bluetooth mate, Arduino D3
//SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);


// LED Matrix stuff
int pinCS = 10; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 3;
int numberOfVerticalDisplays = 1;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

String tape = "Booting...";


int wait = 50; // In milliseconds

int spacer = 1;
int width = 5 + spacer; // The font width is 5 pixels

void writeChars() {
  for ( int i = 0 ; i < width * tape.length() + matrix.width() - 1 - spacer; i++ ) {
    if (Serial.available() ) return;

    matrix.fillScreen(LOW);

    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically

    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < tape.length() ) {
        matrix.drawChar(x, y, tape[letter], HIGH, LOW, 1);
      }

      letter--;
      x -= width;
    }

    matrix.write(); // Send bitmap to display

    delay(wait);
  }
}

void startBT() {

  Serial.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  Serial.print("$");
  Serial.print("$");
  Serial.print("$");
  delay(100);  // Short delay, wait for the Mate to send back CMD
  Serial.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  delay(500);  // Short delay, wait for the Mate finish switching back to Data
  Serial.end();
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  Serial.begin(9600);  // Start bluetooth serial at 9600

}

void stopBT() {

  Serial.print("$");
  Serial.print("$");
  Serial.print("$");
  delay(100);  // Short delay, wait for the Mate to send back CMD
  Serial.println("R,1");  // Temporarily Change the baudrate to 9600, no parity
  delay(500);  // Short delay, wait for the Mate finish switching back to Data
  Serial.end();
}

void setup() {
  matrix.setIntensity(7); // Use a value between 0 and 15 for brightness

  //  setTime(1386311613);
}

void loop() {

  if (Serial.available() > 0 ) {
    tape = r.feed(&Serial);
    Serial.println(tape);
  }

  //  writeChars();
  //  sprintf(tmp,"%02d:%02d:%02d", hour(),minute(),second());
  //  tape = String(tmp);

  for (int i=0;i<matrix.width();i++) {
    matrix.fillScreen(LOW);
    matrix.drawLine(0,0,i,i,1);
    matrix.drawPixel(matrix.width()/2, matrix.height()/2, 1);

    delay(1000);
  }
}

