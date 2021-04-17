import csv
import imaplib
from time import sleep 
from email import message_from_bytes
from email.utils import parsedate_to_datetime as pd2dt

import pytz

from secrets import *

def main(user, password, sender, checkTime, imapHost):
    '''Access `user` email account, which is set in secrets.py. 
    Checks inbox for email from `sender`, every `checkTime` minutes. 
    Outputs email date and subject to ./data/log.csv. Loops forever

    - `user` -> email account username, `username` in secrets.py
    - `password` -> password for email account, `passw` in secrets.py
    - `sender` -> check inbox for messages from `sender`, `sentFrom` in secrets.py
    - `imapHost` -> imap server which request to check email is sent to, `imapServer` in secrets.py
        usually in form: imap.company.com. Each email provider will have a different imap server name.
    '''

    checkTime *= 60

    while True:
        mail = imaplib.IMAP4_SSL(imapHost)  #Sets imap host
        mail.login(user, password)          #and login to email account

        #Selects mail from inbox, readonly=False allows mail to be marked as read or deleted:
        mail.select('inbox', readonly=False)

        #Searches inbox for unread messages from `sender`, `unreadEmails` formatted as two element tuple.
        #therefore, access the second element and create a list of bytes:
        unreadEmails = mail.search(None, '(UNSEEN)', f'(FROM {sender})')[1][0].split()

        try:                                                                       
            if not len(unreadEmails):   #If there are no new emails,
                print('No new emails')  
                mail.logout()           #logout of email account and sleep.
                sleep(checkTime)

            else:                       #If there are unread emails:
                data = []
                for i in unreadEmails:
                        mailContent = mail.fetch(i, "(RFC822)")[1][0][1]    #Fetches the body of the email. Various
                        msg = message_from_bytes(mailContent)               #parts of the email can now be accessed
                                                                            #such as `Subject`, using msg['Subject'].
                        try:
                            amount = eval(msg['Subject'])       #Evaluates string, the str 'Subject'
                                                                #to an `int`, `float` or fraction.

                            #Format `date` as YYYY-mm-dd HH:mm:ss, accounting for local timezone.
                            date = pd2dt(msg["Date"]).astimezone(tz).strftime("%Y-%m-%d %X") 

                        #If eval evaluates a str and is unable to do so, it will either throw a
                        #SyntaxError or NameError. In either case, the `subject` is not a number
                        #so `date` and `amount` is assigned the value, 'None'.
                        except (SyntaxError, NameError):     
                            date = None                     
                            amount = None

                        finally:
                            data.append([date, amount])   #Finally, append `date` and `amount` to `data`
                #Writes items in `data` to ./data/log.csv, 
                with open('./data/log.csv', 'a') as dataFile:           
                    for entry in data:                                  #For each sublist in `data`,
                        writer = csv.writer(dataFile)               
                        writer.writerow(entry)                          #append to to the end of ./data/log.csv
                                                                        #with each sublist on its own line
                        print(f'Wrote {entry} to cigLog.csv')
                    dataFile.close()
                    mail.logout()
                    sleep(checkTime)
                    
        except mail.abort:                          #If you get logged out of 
            mail.login(user, password)              #email acct., log back in 

if __name__ == '__main__':
    main(username, passw, sentFrom, interval, imapServer)