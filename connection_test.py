# Script to automatically run speed & ping tests to be logged on a Domoticz server
# This script uses a (slighlty) customized version of Sivel's speedtest-cli https://github.com/sivel/speedtest-cli
# More information in the header of speedtest_cli.py

from client import *
from speedtest_cli import * 
import os

domoticz_ip = 'http://x.x.x.x:8080'
server_url = domoticz_ip +  '/json.htm?type=command&param=udevice&idx='

# server's id used by speedtest
paris_orange = '5559'

# id's of corresponding virtual devices in Domoticz
id_dl = 2
id_up = 3
id_ping = 4

# my computer's local ip
pc_ip = 'x.x.x.x'

server = Server(server_url)

def send_values(id_sensor, value):
	server.query(str(id_sensor) + '&nvalue=0&svalue=' + str(value))


def ping_test():
	# launch system ping command for 10 pings
	ping = os.popen('ping -c 10 google.fr')

	# catch the stats recap line
	line = ping.readlines()[-1]

	# recover wanted value, average time in ms
	avg = line.split('/')[4]

	return avg

# ping my computer to check if it's on
is_up = os.system('ping -c 1 ' + pc_ip)

# if the pc is on, then no tests because the bandwith is probably used
# if is_up != 0 : 
dl, up = main(paris_orange) # speedtest api to retrieve upload & download values (in MBytes/s)
ping = ping_test()

#send values to Domoticz' server
send_values(id_dl, dl)
send_values(id_up, up)
send_values(id_ping, ping)
