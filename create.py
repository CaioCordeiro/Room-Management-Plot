from __future__ import print_function
import datetime
from googleapiclient.discovery import build
from httplib2 import Http
from oauth2client import file, client, tools
import sys
from datetime import timedelta
import serial
import time
import struct


ser = serial.Serial('/dev/ttyACM1',9600)


# If modifying these scopes, delete the file token.json.
SCOPES = 'https://www.googleapis.com/auth/calendar'
def credentials():
    store = file.Storage('token.json')
    creds = store.get()
    if not creds or creds.invalid:
        flow = client.flow_from_clientsecrets('credentials.json', SCOPES)
        creds = tools.run_flow(flow, store)
    service = build('calendar', 'v3', http=creds.authorize(Http()))
    return service

def getmonth(date):
  m = date[5]+ date[6]
  return int(m)

def getday(date):
  d = date[8]+date[9]
  return int(d)

def gettime(date):
  h = date[11]+date[12]
  m = date[14]+date[15]
  s = date[17]+date[18]
  time = float(h)+ (float(m)/60) + (float(s)/3600)
  return time
def gettime2(date):
  h = date[11]+date[12]
  m = date[14]+date[15]
  s = ':'
  seq = (h,m)
  seq = s.join(seq)
  return seq
def check_status():
    now = datetime.datetime.utcnow().isoformat() + 'Z'
    service = credentials()
    events_result = service.events().list(calendarId='primary', timeMin=now,
                                        maxResults=10, singleEvents=True,
                                        orderBy='startTime').execute()
    x = events_result.get('items', [])
    if len(x) == 0:
      file= open("temp.txt", "w")
      file.write("livre")
      file.close()
      return 'livre'
    else:
      events = x[0]

    now2 = datetime.datetime.now().isoformat()
    start = events['start'].get('dateTime', events['start'].get('date'))
    m_now = getmonth(now2)
    m_start = getmonth(start)
    d_now = getday(now2)
    d_start = getday(start)
    h_now = gettime(now2)
    h_start = gettime(start)
    if m_now==m_start and d_now == d_start and (h_start - h_now)<1 and (h_start - h_now)>0:
        file= open("temp.txt", "w")
        file.write("livre")
        file.close()
        return "livre"
    elif m_now==m_start and d_now == d_start and (h_start - h_now)>1:
        file= open("temp.txt", "w")
        file.write("livre")
        file.close()
        return "livre"
    elif m_now==m_start and d_now == d_start and (h_start - h_now)<0:
        if (h_start - h_now)> -0.25:
          file= open("temp.txt", "w")
          file.write("timer")
          file.close()
          return "timer"
        else:
          file= open("temp.txt", "w")
          file.write("ocupado")
          file.close()
          return "ocupado"
def get_time_next():
    now = datetime.datetime.utcnow().isoformat() + 'Z'
    service = credentials()
    events_result = service.events().list(calendarId='primary', timeMin=now,
                                        maxResults=10, singleEvents=True,
                                        orderBy='startTime').execute()
    x = events_result.get('items', [])
    if len(x) == 0:
      file= open("temp.txt", "w")
      file.write("Livre")
      file.close()
      return 0
    else:
      events = x[0]

    start = events['start'].get('dateTime', events['start'].get('date'))
    m_start = getmonth(start)
    d_start = getday(start)
    h_start = gettime2(start)
    file= open("temp.txt", "w")
    file.write("{}/{}/->{}".format(d_start,m_start,h_start))
    file.close()

def get_email():
    service = credentials()
    now = datetime.datetime.utcnow().isoformat() + 'Z'
    events_result = service.events().list(calendarId='primary', timeMin=now,
                                        maxResults=10, singleEvents=True,
                                        orderBy='startTime').execute()
    x = events_result.get('items', [])
    if len(x) == 0:
      print("Não há eventos")
      return 0
    else:
      events = x[0]
    email = events['summary']
    email = email
    file= open("temp.txt", "w")
    file.write(email)
    file.close()
def check_time_diff():
    now = datetime.datetime.utcnow().isoformat() + 'Z'
    service = credentials()
    events_result = service.events().list(calendarId='primary', timeMin=now,
                                        maxResults=10, singleEvents=True,
                                        orderBy='startTime').execute()
    x = events_result.get('items', [])
    if len(x) == 0:
      return 1
    else:
      events = x[0]

    now2 = datetime.datetime.now().isoformat()
    start = events['start'].get('dateTime', events['start'].get('date'))
    m_now = getmonth(now2)
    m_start = getmonth(start)
    d_now = getday(now2)
    d_start = getday(start)
    h_now = gettime(now2)
    h_start = gettime(start)
    if m_now==m_start and d_now == d_start and (h_start - h_now)<1 and (h_start - h_now)>0:
      return h_start - h_now
    elif m_now==m_start and d_now == d_start and (h_start - h_now)>1:
      return 1
    else:
      return False
  
def agendar(email):
    service = credentials()
    start =datetime.datetime.now() # 'Z' indicates UTC time
    x = check_time_diff()
    if x == False:
      return 0
    else:
      end = start + datetime.timedelta(hours=x)  
    end = end.isoformat()
    start = start.isoformat()
    event_body = {
      'summary': email,
	    'location': '800 Howard St., San Francisco, CA 94103',
	    'description': 'A chance to hear more about Google\'s developer products.',
	    'start': {
	      'dateTime': start,
	      'timeZone': 'America/Bahia',
	    },
	    'end': {
	      'dateTime': end,
	      'timeZone': 'America/Bahia',
	    },
	    'recurrence': [
	      'RRULE:FREQ=DAILY;COUNT=1'
	    ],
	    'attendees': [
	      {'email': email},
	    ],
	    'reminders': {
	      'useDefault': False,
	      'overrides': [
	        {'method': 'email', 'minutes': 24 * 60},
	        {'method': 'popup', 'minutes': 10},
	      ],
	    },
	}

    event = service.events().insert(calendarId='primary', body=event_body).execute()
    print ('Event created')
    return 1
def deletar(service):
  now = datetime.datetime.utcnow().isoformat() + 'Z'
  events_result = service.events().list(calendarId='primary', timeMin=now,
                                      maxResults=10, singleEvents=True,
                                      orderBy='startTime').execute()
  events = events_result.get('items', [])[0]
  event0 = events['id']
  service.events().delete(calendarId='primary', eventId=event0).execute()
  print("CANCELANDO EVENTO...")


def led_on(x):
  if x == '1':
    time.sleep(2)
    ser.write(b'1')
  if x == '2':
    time.sleep(2)
    ser.write(b'2')
  if x== '3':
    time.sleep(2)
    ser.write(b'3')
  ser.close()


def read_serial():
  line = str(ser.readline()) 
  for i in line:
    if i == 'b' or i ==  "'" or i == "\\" or i == "n" or i == "r":
      line = line.replace(i,"")
  file= open("temp.txt", "w")
  file.write(line)
  file.close()
  ser.close()
  

def main():
  syslen = len(sys.argv)
  if syslen<2:
    mode  = sys.argv[1]
  else:
    mode = sys.argv[1]
    email = sys.argv[2]
  if mode == '1':
    agendar(email)
    ser.close()
  if mode == '2':
    service = credentials()
    deletar(service)
    ser.close()
  if mode == '3':
    get_email()
    ser.close()
  if mode == '4':
    status = check_status()
    ser.close()
    pass
  if mode == '5':
    get_time_next()
    ser.close()
  if mode == '6':
    led_on(email)
    ser.close()
  if mode == '7':
    read_serial()
    ser.close()





if __name__ == '__main__':
	main()