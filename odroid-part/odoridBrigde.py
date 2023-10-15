import googleSheetAPI, weatherAPI
import odroid_wiringpi as wpi
import time
from datetime import datetime
serial = wpi.serialOpen('/dev/ttyS1', 115200)

weather = weatherAPI.get_weather()

last_time = f'{datetime.now().hour}:{datetime.now().minute}:{datetime.now().second}'

def send_data_to_esp32(value):
    wpi.serialPuts(serial, value)
    time.sleep(0.3)

def receive_data_from_esp32():
    output_str = ''
    send_data_to_esp32(' ');
    while wpi.serialDataAvail(serial):
        output_str += chr(wpi.serialGetchar(serial))
    return output_str

while True:
    data = ''
    try:
        data = receive_data_from_esp32()
        if(data != ''):
            print(data)
            if(data[0] == 'U'):
                print("Update Sheet")
                data = receive_data_from_esp32()
                sheet_data = googleSheetAPI.read_sheet()
                s = f'{sheet_data[0][1]}'
                for row in sheet_data[1:]:
                    s += f',{row[1]}'
                send_data_to_esp32(s)
                values = list(map(int, data[1:].split(',')))
                print(values)
                googleSheetAPI.update_batches_value(values)
            elif(data[0] == 'W'):
                print("Get weather")
                weather_dict = {"Thunderstorm" : '0' , "Drizzle" : '1', "Rain" : '2', "Snow" : '3', "Atmosphere" : '4', "Clears" : '5', "Clouds" : '6'}
                time_diff = datetime.strptime(f"{datetime.now().hour}:{datetime.now().minute}:{datetime.now().second}", "%H:%M:%S") - datetime.strptime(last_time, "%H:%M:%S")
                hour = time_diff.total_seconds() / (60 * 60)
                print(hour)
                if(hour > 1):
                    weather = weatherAPI.get_weather()
                    last_time = f'{datetime.now().hour}:{datetime.now().minute}:{datetime.now().second}'
                print(weather)
                send_data_to_esp32('W' + weather_dict[weather])
    except:
        pass
wpi.serialClose(serial)
