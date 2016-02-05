/*
 Name:		FirstProject.ino
 Created:	2/1/2016 2:37:54 PM
 Author:	earosner
*/

/*****************************************************************
XBee_Serial_Passthrough.ino

Set up a software serial port to pass data between an XBee Shield
and the serial monitor.

Hardware Hookup:
The XBee Shield makes all of the connections you'll need
between Arduino and XBee. If you have the shield make
sure the SWITCH IS IN THE "DLINE" POSITION. That will connect
the XBee's DOUT and DIN pins to Arduino pins 2 and 3.

*****************************************************************/
// We'll use SoftwareSerial to communicate with the XBee:
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
// Choose two Arduino pins to use for software serial
/** The GPS Shield uses D2 and D3 by default when in DLINE mode
  Changes made: Using the hardware UART to use pins 0 and 1
  MAKE SURE TO UPLOAD CODE WITH SWITCH SET TO DLINE! AFTER UPLOAD,
  SWITCH TO UART!!!!
  */
int RXPin = 0;
int TXPin = 1;
int beginGPSsearch = 0;
// The Skytaq EM-506 GPS module included in the GPS Shield Kit
// uses 4800 baud by default
int GPSBaud = 4800;
int XBeeBaud = 9600;

// Create a TinyGPS++ object called "gps"
TinyGPSPlus gps;


// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
SoftwareSerial XBee(2, 3); // RX, TX

void setup()
{
	// Start the Arduino hardware serial port at 9600 baud
	Serial.begin(GPSBaud);

	// Start the software serial port at the XBEE's default baud
	XBee.begin(XBeeBaud);

	XBee.println(F("FirstProject.ino"));
	XBee.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
	XBee.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
	XBee.println(F("by Mikal Hart"));
	XBee.println();
}

void loop()
{
	if (beginGPSsearch == 0)
	{
		while (XBee.available() > 0)
		{
			beginGPSsearch = XBee.read();
		}
	}
	else
	{


		// This sketch displays information every time a new sentence is correctly encoded.
		while (Serial.available() > 0)
			if (gps.encode(Serial.read()))
				displayInfo();

		// If 5000 milliseconds pass and there are no characters coming in
		// over the software serial port, show a "No GPS detected" error
		if (millis() > 5000 && gps.charsProcessed() < 10)
		{
			XBee.println(F("No GPS detected"));
			while (true);
		}
	}
}

	void displayInfo()
	{
		XBee.print(F("Location: "));
		if (gps.location.isValid())
		{
			XBee.print(gps.location.lat(), 6);
			XBee.print(F(","));
			XBee.print(gps.location.lng(), 6);
		}
		else
		{
			XBee.print(F("INVALID"));
		}

		XBee.print(F("  Date/Time: "));
		if (gps.date.isValid())
		{
			XBee.print(gps.date.month());
			XBee.print(F("/"));
			XBee.print(gps.date.day());
			XBee.print(F("/"));
			XBee.print(gps.date.year());
		}
		else
		{
			XBee.print(F("INVALID"));
		}

		XBee.print(F(" "));
		if (gps.time.isValid())
		{
			if (gps.time.hour() < 10) XBee.print(F("0"));
			XBee.print(gps.time.hour());
			XBee.print(F(":"));
			if (gps.time.minute() < 10) XBee.print(F("0"));
			XBee.print(gps.time.minute());
			XBee.print(F(":"));
			if (gps.time.second() < 10) XBee.print(F("0"));
			XBee.print(gps.time.second());
			XBee.print(F("."));
			if (gps.time.centisecond() < 10) XBee.print(F("0"));
			XBee.print(gps.time.centisecond());
		}
		else
		{
			XBee.print(F("INVALID"));
		}

		XBee.println();
	
}
