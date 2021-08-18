import datetime
import json
import urllib.request
import sys



def time_converter(time):
    converted_time = datetime.datetime.fromtimestamp(
        int(time)
    ).strftime('%I:%M %p')
    return converted_time


def url_builder(city_id):
    user_api = '6ad1576301dad58f45d173da350f3d75'  # Obtain yours form: http://openweathermap.org/
    unit = 'metric'  # For Fahrenheit use imperial, for Celsius use metric, and the default is Kelvin.
    api = 'http://api.openweathermap.org/data/2.5/weather?q='     # Search for your city ID here: http://bulk.openweathermap.org/sample/city.list.json.gz

    full_api_url = api + city_id  + '&mode=json&units=' + unit + '&APPID=' + user_api
    return full_api_url


def data_fetch(full_api_url):
    url = urllib.request.urlopen(full_api_url)
    output = url.read().decode('utf-8')
    raw_api_dict = json.loads(output)
    url.close()
    return raw_api_dict


def data_organizer(raw_api_dict):
    data = dict(
        city=raw_api_dict.get('name'),
        country=raw_api_dict.get('sys').get('country'),
        temp=raw_api_dict.get('main').get('temp'),
        temp_max=raw_api_dict.get('main').get('temp_max'),
        temp_min=raw_api_dict.get('main').get('temp_min'),
        humidity=raw_api_dict.get('main').get('humidity'),
        pressure=raw_api_dict.get('main').get('pressure'),
        sky=raw_api_dict['weather'][0]['main'],
        sunrise=time_converter(raw_api_dict.get('sys').get('sunrise')),
        sunset=time_converter(raw_api_dict.get('sys').get('sunset')),
        wind=raw_api_dict.get('wind').get('speed'),
        wind_deg=raw_api_dict.get('deg'),
        dt=time_converter(raw_api_dict.get('dt')),
        cloudiness=raw_api_dict.get('clouds').get('all')
    )
    return data

def dayLenght(data):
    time1 = data['sunrise']
    time2 = data['sunset'] 		
    h1 = int(time1[:2])
    h2 = int(time2[:2])
    m1 = int(time1[3:5])
    m2 = int(time2[3:5])

    t1minutes = h1 * 60 + m1
    t2minutes = h2 * 60 + m2

    if (time1[6] == "P"): t1minutes += 12 * 60
    if (time2[6] == "P"): t2minutes += 12 * 60
 
    if t1minutes > t2minutes:
        t1 = (24*60 - t1minutes) + t2minutes
        return ("<b>Dlugosc dnia: " + str(int(t1 / 60)) + "h " + str(t1 - (int(t1 / 60) * 60)) + "m")

    if t1minutes < t2minutes:
        t1 = (12*60 - t1minutes) + (t2minutes - 12*60)
        return ("<b>Dlugosc dnia: " + str(int(t1 / 60)) + "h " + str(t1 - (int(t1 / 60) * 60)) + "m")

def data_output(data):
    m_symbol = "*C"
    print('<b>Pogoda: {}, {}:'.format(data['city'], data['country']))
    print("<b>Temperatura:",data['temp'], m_symbol, data['sky'])
    #print('')
    print('<b>Predkosc wiatru: {} '.format(data['wind'] ))
    print(('<b>Wilgotnosc powietrza: {}'.format(data['humidity']) +'%'))
    print(('<b>Zachmurzenie: {}'.format(data['cloudiness']))+'%')
    print('<b>Cisnienie: {}'.format(data['pressure']))
    print('<b>Wschod sloneczka: {}'.format(data['sunrise']))
    print('<b>Zachod sloneczka: {}'.format(data['sunset'])) 
    print(dayLenght(data))

if __name__ == '__main__':
    try:
        x = len(sys.argv)
        if x==2:
            city_name = sys.argv[1]
        elif x==3:
            city_name=sys.argv[1] + ' ' + sys.argv[2]
        else:
            city_name=sys.argv[1] + ' ' + sys.argv[2] + ' ' + sys.argv[3]

        data_output(data_organizer(data_fetch(url_builder(city_name))))
    except IOError:
        print('<b>nie znam takiej dziury jak ' + city_name)

