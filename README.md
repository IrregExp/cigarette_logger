# About
This was built to log the number of cigarettes I smoked throughout the day. I connected an endstop to an ESP32, such that when the pack of cigarettes was opened, the endstop was triggered, causing the ESP32 to send a text to my phone. 
When I received the text message, I would reply with the number of cigarettes I had taken from the pack which would then be sent to an email account I had set up for this purpose. With `main.py` running in the background on my home PC,
it would check for new emails from my phone number and write the time and amount to `data/log.csv`.

# Motivation 
I wanted to quit smoking (and actually did thanks to this project :smile:). Being curious as to how many cigarettes I smoked throught the day, I looked up many tutorials on using an ESP32 to send emails. This project is a culmination of my drive to quit smoking and my desire to learn new things.

# Note
This program is NOT secure. In order to use it, you must enable 'less secure apps' on your respective email client. For this reason, it is recommended to
create a new email account for this project. If anyone has any questions, concerns or feedback for this project, I'd greatly appreciate it :smile:

# How to run this program
The `recipient` of the email should be formatted as an email address: xxxx@website.com. If the recipient is a phone number, the cell phone carrier will have a specific format to send an email as an SMS. For example, AT&T uses number@txt.att.net for SMS and number@mms.att.net for MMS.\
More information can be found at your respective carrier's website, or this [website](https://20somethingfinance.com/how-to-send-text-messages-sms-via-email-for-free/)

## Setting Up the ESP32
- Wire the ESP32 according to the schematic under `/doc/ESP_schematic.PDF`
- Install the latest version of arduino [here](https://www.arduino.cc/en/software)
- Open and edit `secrets.h` with your information
- Flash the sketch to the ESP32
- The ESP32 can be powered using the USB port, or using a LiPo battery to make the project portable.

## Setting Up the Email Checker
- Open `secrets.py` and uncomment `username`, `passw`, `sentFrom`, `imapServer` and `zone` with your information.
- Using Mac or Linux, nohup was used to allow the program to run in the background:
```
pip install pytz
chmod +x ./main.py
nohup python ./main.py > output.log &
```
- Using Windows:
```
pip install pytz
chmod +x ./main.py
start /b main.py run
```
Alternatively, `main.py` can be ran using `python ./main.py`

# Credits
- [Blog](https://janakiev.com/blog/python-background/) for running a python script in the background using `nohup`

- [Tutorial](https://randomnerdtutorials.com/esp32-send-email-smtp-server-arduino-ide/) for sending an email using an ESP32

- [Stackoverflow](https://stackoverflow.com/questions/13210737/get-only-new-emails-imaplib-and-python) question in regard to checking emails from python

- [Tutorial](https://www.tutorialspoint.com/python_network_programming/python_imap.htm) for checking Email using `imaplib`

- [Tutorial](https://lastminuteengineers.com/esp32-deep-sleep-wakeup-sources/) for using 'deep sleep' for the ESP32
