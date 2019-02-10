import urllib
import json

name = input('Enter username: ')
key = 'AIzaSyCP-GCr_uTdA10OYzTMPoNvL4LUB81LAYI'

url_data = urllib.request.urlopen('https://www.googleapis.com/youtube/v3/channels?part=statistics&forUsername='+name+'&key='+key).read()
data = json.loads(url_data)

