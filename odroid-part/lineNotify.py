import requests
import os

# ของ LINE Notify Access Token
LINE_NOTIFY_ACCESS_TOKEN = '0iDnKcDgo9LuiioiOJZMcf9CHeEc0h5KxXbGBIBzMjF'

# ตัวแปรที่เก็บไฟล์รูปภาพ
image_path = 'happiness_icon.png'

# ตัวแปรสำหรับสร้างไฟล์สำหรับการส่ง
files = {'imageFile': open(image_path, 'rb')}

# ส่งรูปไปที่ LINE Notify
try:
    response = requests.post('https://notify-api.line.me/api/notify',
                             headers={'Authorization': f'Bearer {LINE_NOTIFY_ACCESS_TOKEN}'},
                             files=files, data = {'message': 'Yoo'})
    
    # ตรวจสอบว่าการส่งสำเร็จหรือไม่
    if response.status_code == 200:
        print('รูปถูกส่งไปที่ LINE Notify สำเร็จแล้ว!')
    else:
        print('เกิดข้อผิดพลาดในการส่งรูปไปที่ LINE Notify:', response.status_code, response.text)
except Exception as e:
    print('เกิดข้อผิดพลาด:', str(e))