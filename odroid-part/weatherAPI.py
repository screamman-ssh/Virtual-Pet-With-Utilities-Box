import requests

API_KEY = 'd68e5322268edcce664382a80ef2e746'
CITY_NAME = 'Songkhla'
URL = f'https://api.openweathermap.org/data/2.5/weather?q={CITY_NAME}&appid={API_KEY}'

def get_waether():
    r = requests.get(URL)
    data = r.json()
    # print(data['weather'][0]['main'])
    return data['weather'][0]['main']

if __name__ == "__main__":
    pass