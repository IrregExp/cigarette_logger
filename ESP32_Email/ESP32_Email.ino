/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-send-email-smtp-server-arduino-ide/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include "ESP32_MailClient.h"
#include "secrets.h"

byte redPin = 19;   //Can be changed depending on which pins you decide to use
byte greenPin = 18;
byte bluePin = 5;
byte switchPin = 12;

//These are accessed from secrets.h file:
const char ssid[] = SECRET_SSID;
const char password[] = WIFI_PASS;
const char emailSenderAccount[] = SENDER_ACCT;
const char emailSenderPassword[] = SENDER_PASS;
const char emailRecipient[] = RECIPIENT;
const char smtpServer[] = SMTPSERVER;
int smtpServerPort = SMTPPORT;
//

RTC_DATA_ATTR int bootCount = 0;  //Counts number of times ESP32 has rebooted, stored in RTC memory
SMTPData smtpData;

void sendCallback(SendStatus info);

void setup() {
  Serial.begin(115200);
  pinMode(switchPin, INPUT_PULLUP);     //Initialize endstop and RGB LED
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword); //Login to smtp server,
  //Using credentials defined in secrets.h
  smtpData.setSender("ESP32", emailSenderAccount);                                        //Account that is sending the email
  smtpData.setPriority("High");                                                           //set priority
  smtpData.setMessage("Enter # Cigarettes Smoked:", true);                                //Message to be sent to recipient
  smtpData.addRecipient(emailRecipient);
  smtpData.setSendCallback(sendCallback);

  Serial.println();
  Serial.print("Connecting");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { //Event: Connecting to WiFi -> Rainbow until connected
    RGB(0, 1, 1);
    delay(150);
    RGB(0, 0, 1);
    delay(150);
    RGB(1, 0, 1);
    delay(150);
    RGB(1, 0, 0);
    delay(150);
    RGB(1, 1, 0);
    delay(150);
    RGB(0, 1, 0);
    delay(150);
  }
  RGB(1, 0, 1);       //RGB LED green for 1 s once connected to WiFi.
  delay(1000);
  Serial.println();
  Serial.println("WiFi connected.");
  Serial.println();
  Serial.println("Preparing to send email");
  Serial.println();
  
  ++bootCount;            //Increment bootCount by 1
  if (bootCount > 1) {    //Sends email only when endstop is clicked, not when ESP32 is first started.
    sendEmail();
  }
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_12, 0);   //Enables deep sleep mode, with endstop as external wakeup trigger.
  esp_deep_sleep_start();                         //Initializes deep sleep
}

void loop() {}                        //loop() isn't necessary, since deep sleep is enabled after setup()

void sendCallback(SendStatus msg) {   //callback after email is sent
  Serial.println(msg.info());
  if (msg.success()) {                //msg sent, success -> rgb led = blue for 1.5 secs.
    RGB(1, 1, 0);
    Serial.println("Success!");
    delay(1500);
  }
}

void RGB(bool redVal, bool greenVal, bool blueVal) { //Creates a color for the RGB LED, 0 = on, 1 = off
  digitalWrite(redPin, redVal);
  digitalWrite(greenPin, greenVal);
  digitalWrite(bluePin, blueVal);
}

void sendEmail() {                      //Called after ESP32 wakes up, when endstop is clicked. Sends an email to `RECIPIENT`
  RGB(0, 1, 0);                         //RGB LED -> magenta for 1 s, indicates message is being prepared to send
  delay(1000);
  if (!MailClient.sendMail(smtpData)) { //msg not sent (failure) RGB LED -> red for 5 secs.
    RGB(0, 1, 1);
    delay(5000);
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
  }
}
