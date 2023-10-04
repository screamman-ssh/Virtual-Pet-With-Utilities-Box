import googleSheetAPI, weatherAPI
import odroid_wiringpi as wpi
import time
serial = wpi.serialOpen('/dev/ttyS1', 115200)

# weather = weatherAPI.get_waether()


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
    data = receive_data_from_esp32()
    if(data != ''):
        print(data)
        if(data[0] == 'U'):
            print("Update Sheet")
            data = receive_data_from_esp32()
            values = list(map(int, data[1:].split(',')))
            if(values[0] >= 100 or values[1] >= 100):
                values.append(0)
                values.append(0)
            print(values)
            googleSheetAPI.update_batches_value(values)
            sheet_data = googleSheetAPI.read_sheet()
            s = f'{sheet_data[0][1]}'
            for row in sheet_data[1:]:
                s += f',{row[1]}'
            send_data_to_esp32(s)
        elif(data == 'W'):
            print("get weather")
            send_data_to_esp32(weather)
        elif(data == 'S'):
            print("get weather")
            send_data_to_esp32(weather)
wpi.serialClose(serial)
