from doctest import master
from multiprocessing.dummy import Namespace
from quopri import encodestring
from tkinter.messagebox import NO
from traceback import print_tb
from flask_socketio import SocketIO
from flask import Flask, render_template
from random import random
from threading import Thread, Event
import serial
import serial.tools.list_ports as pt
import time
import csv



__author__ = 'rafay'

app = Flask(__name__)
app.config['SECRET_KEY'] = '*rafay*'
app.config['DEBUG'] = True

#turn the flask app into a socketio app
socketio = SocketIO(app, async_mode=None, logger=True, engineio_logger=True)
# status = ["Connected"]

#random number Generator Thread
thread = Thread()
thread_stop_event = Event()

batteries_data = {
    'battery_1': {'voltage_1': 0.00, 'current_1': 0.00, 'soc_1': 0.00, 'max_temp_1': 0.00, 'min_temp_1': 0.00, 'energy_1': 0.00},
    'battery_2': {'voltage_2': 0.00, 'current_2': 0.00, 'soc_2': 0.00, 'max_temp_2': 0.00, 'min_temp_2': 0.00, 'energy_2': 0.00},
    'battery_3': {'voltage_3': 0.00, 'current_3': 0.00, 'soc_3': 0.00, 'max_temp_3': 0.00, 'min_temp_3': 0.00, 'energy_3': 0.00},
    'battery_4': {'voltage_4': 0.00, 'current_4': 0.00, 'soc_4': 0.00, 'max_temp_4': 0.00, 'min_temp_4': 0.00, 'energy_4': 0.00},

}

header_fields = ['voltage_1','current_1', 'soc_1', 'max_temp_1', 'min_temp_1', 'energy_1']
def readstm():
    print("READING")
    ports = pt.comports()
    print(len(ports))
    master_device = None
    while(master_device == None):
        for devices in range(len(ports)):
            if 'FT232R' in ports[devices].product:
                ser = serial.Serial(ports[devices].device, 115200, timeout=5)
                data_serial = ser.readline().decode().strip().split(",")
                if data_serial[0] == 'MASTER' or data_serial[0].isupper():
                    master_device = devices
    print("Master device is: ", master_device)
    while not thread_stop_event.isSet():
        try:
            battery_time_1 = time.time()
            battery_time_2 = time.time()
            battery_time_3 = time.time()
            battery_time_4 = time.time()

         
            if 'FT232R' in ports[master_device].product:
                print("YESSSSSSSS")
                ser = serial.Serial(ports[master_device].device, 115200,timeout=10)
            # ser_2 = serial.Serial(ports[1].device, 115200)
                data_serial = ser.readline().decode().strip().split(",")
                # Will use startswith and endswith to check if the data is valid and send to appropriate display
                if data_serial[0].startswith('A') and data_serial[0].endswith('1'):
                    battery_time_1 = time.time()
                    print(battery_time_1)
                    byte0 = bytes(data_serial[2],'UTF-8')
                    byte1 = bytes(data_serial[3], 'UTF-8')
                    # voltage_1 = (int(byte0+byte1,16))/10.0
                    batteries_data['battery_1']['voltage_1'] = (int(byte0+byte1,16))/10.0
                    byte4 = bytes(data_serial[6],'UTF-8')
                    byte5 = bytes(data_serial[7], 'UTF-8')
                    # current_1 = (int(byte4+byte5,16)-30000)/10.0
                    batteries_data['battery_1']['current_1'] = (int(byte4+byte5,16)-30000)/10.0



                    byte6 = bytes(data_serial[8],'UTF-8')
                    byte7 = bytes(data_serial[9], 'UTF-8')
                    # soc_1 = (int(byte6+byte7,16))/10.0
                    batteries_data['battery_1']['soc_1'] = (int(byte6+byte7,16))/10.0


                if data_serial[0].startswith('A') and data_serial[0].endswith('2'):
                    byte_max_t = bytes(data_serial[2],'UTF-8')
                    byte_min_t = bytes(data_serial[4], 'UTF-8')
                    # max_temp = (int(byte_max_t,16))-40.0
                    batteries_data['battery_1']['max_temp_1'] = (int(byte_max_t,16))-40.0
                    # min_temp = (int(byte_min_t,16))-40.0
                    batteries_data['battery_1']['min_temp_1'] = (int(byte_min_t,16))-40.0
                    byte_e_1 = bytes(data_serial[6],'UTF-8')
                    byte_e_2 = bytes(data_serial[7], 'UTF-8')
                  
                    # energy_1 = int((byte_e_1+byte_e_2),16)/1000.0
                    batteries_data['battery_1']['energy_1'] = int((byte_e_1+byte_e_2),16)/1000.0
                if data_serial[0].startswith('B') and data_serial[0].endswith('1'):
                    battery_time_2 = time.time()
                    print(battery_time_2)
                    byte0 = bytes(data_serial[2],'UTF-8')
                    byte1 = bytes(data_serial[3], 'UTF-8')
                    # voltage_2 = (int(byte0+byte1,16))/10.0
                    batteries_data['battery_2']['voltage_2'] = (int(byte0+byte1,16))/10.0
                    byte4 = bytes(data_serial[6],'UTF-8')
                    byte5 = bytes(data_serial[7], 'UTF-8')
                    # current_2 = (int(byte4+byte5,16)-30000)/10.0
                    batteries_data['battery_2']['current_2'] = (int(byte4+byte5,16)-30000)/10.0



                    byte6 = bytes(data_serial[8],'UTF-8')
                    byte7 = bytes(data_serial[9], 'UTF-8')
                    # soc_2 = (int(byte6+byte7,16))/10.0
                    batteries_data['battery_2']['soc_2'] = (int(byte6+byte7,16))/10.0

                if data_serial[0].startswith('B') and data_serial[0].endswith('2'):
                    byte_max_t = bytes(data_serial[2],'UTF-8')
                    byte_min_t = bytes(data_serial[4], 'UTF-8')
                    # max_temp = (int(byte_max_t,16))-40.0
                    batteries_data['battery_2']['max_temp_2'] = (int(byte_max_t,16))-40.0
                    # min_temp = (int(byte_min_t,16))-40.0
                    batteries_data['battery_2']['min_temp_2'] = (int(byte_min_t,16))-40.0
                    byte_e_1 = bytes(data_serial[6],'UTF-8')
                    byte_e_2 = bytes(data_serial[7], 'UTF-8')

                    # energy_2 = int((byte_e_1+byte_e_2),16)/1000.0
                    batteries_data['battery_2']['energy_2'] = int((byte_e_1+byte_e_2),16)/1000.0
                if data_serial[0].startswith('C') and data_serial[0].endswith('1'):
                    battery_time_3 = time.time()
                    print(battery_time_3)
                    byte0 = bytes(data_serial[2],'UTF-8')
                    byte1 = bytes(data_serial[3], 'UTF-8')
                    # voltage_3 = (int(byte0+byte1,16))/10.0
                    batteries_data['battery_3']['voltage_3'] = (int(byte0+byte1,16))/10.0
                    byte4 = bytes(data_serial[6],'UTF-8')
                    byte5 = bytes(data_serial[7], 'UTF-8')
                    # current_3 = (int(byte4+byte5,16)-30000)/10.0
                    batteries_data['battery_3']['current_3'] = (int(byte4+byte5,16)-30000)/10.0



                    byte6 = bytes(data_serial[8],'UTF-8')
                    byte7 = bytes(data_serial[9], 'UTF-8')
                    # soc_3 = (int(byte6+byte7,16))/10.0
                    batteries_data['battery_3']['soc_3'] = (int(byte6+byte7,16))/10.0

                if data_serial[0].startswith('C') and data_serial[0].endswith('2'):
                    byte_max_t = bytes(data_serial[2],'UTF-8')
                    byte_min_t = bytes(data_serial[4], 'UTF-8')
                    # max_temp = (int(byte_max_t,16))-40.0
                    batteries_data['battery_3']['max_temp_3'] = (int(byte_max_t,16))-40.0
                    # min_temp = (int(byte_min_t,16))-40.0
                    batteries_data['battery_3']['min_temp_3'] = (int(byte_min_t,16))-40.0
                    byte_e_1 = bytes(data_serial[6],'UTF-8')
                    byte_e_2 = bytes(data_serial[7], 'UTF-8')
                    # energy_3 = int((byte_e_1+byte_e_2),16)/1000.0
                    batteries_data['battery_3']['energy_3'] = int((byte_e_1+byte_e_2),16)/1000.0
                if data_serial[0].startswith('D') and data_serial[0].endswith('1'):
                    battery_time_4 = time.time()
                    print(battery_time_4)
                    byte0 = bytes(data_serial[2],'UTF-8')
                    byte1 = bytes(data_serial[3], 'UTF-8')
                    # voltage_4 = (int(byte0+byte1,16))/10.0
                    batteries_data['battery_4']['voltage_4'] = (int(byte0+byte1,16))/10.0
                    byte4 = bytes(data_serial[6],'UTF-8')
                    byte5 = bytes(data_serial[7], 'UTF-8')
                    # current_4 = (int(byte4+byte5,16)-30000)/10.0
                    batteries_data['battery_4']['current_4'] = (int(byte4+byte5,16)-30000)/10.0



                    byte6 = bytes(data_serial[8],'UTF-8')
                    byte7 = bytes(data_serial[9], 'UTF-8')
                    # soc_4 = (int(byte6+byte7,16))/10.0
                    batteries_data['battery_4']['soc_4'] = (int(byte6+byte7,16))/10.0

                if data_serial[0].startswith('D') and data_serial[0].endswith('2'):
                    byte_max_t = bytes(data_serial[2],'UTF-8')
                    byte_min_t = bytes(data_serial[4], 'UTF-8')
                    # max_temp = (int(byte_max_t,16))-40.0
                    batteries_data['battery_4']['max_temp_4'] = (int(byte_max_t,16))-40.0
                    # min_temp = (int(byte_min_t,16))-40.0
                    batteries_data['battery_4']['min_temp_4'] = (int(byte_min_t,16))-40.0
                    byte_e_1 = bytes(data_serial[6],'UTF-8')
                    byte_e_2 = bytes(data_serial[7], 'UTF-8')
                    # energy_4 = int((byte_e_1+byte_e_2),16)/1000.0

                    batteries_data['battery_4']['energy_4'] = int((byte_e_1+byte_e_2),16)/1000.0

                    
                                     
                    
                   
                 

            # data_1 = ser1.readline().decode().strip().split(",")    
            # data_2 = ser_2.readline().decode().strip().split(",")  
            print(batteries_data['battery_1'])
            if batteries_data['battery_1']['voltage_1'] != 0.00 and batteries_data['battery_1']['max_temp_1'] > 0.00:
                print("++++++++++++++++++++")
                print("Data: ", batteries_data['battery_1'])
                socketio.emit('data_1', {'data_1': batteries_data['battery_1']}, namespace='/test')
                with open('battery1_data.csv', 'w', encoding='UTF8', newline='') as f:
                    writer = csv.DictWriter(f, fieldnames=header_fields)
                    writer.writeheader()
                    writer.writerow(batteries_data['battery_1'])
            
            if time.time() - battery_time_1 > 5:
                print("offline")
                batteries_data['battery_1'] = {'voltage_1': 0.00, 'current_1': 0.00, 'soc_1': 0.00, 'max_temp_1': 0.00, 'min_temp_1': 0.00, 'energy_1': 0.00}
                socketio.emit('data_1', {'data_1': batteries_data['battery_1']}, namespace='/test')
                with open('battery1_data.csv', 'w', encoding='UTF8', newline='') as f:
                    writer = csv.DictWriter(f, fieldnames=header_fields)
                    writer.writeheader()
                    writer.writerow(batteries_data['battery_1'])
            
            if batteries_data['battery_2']['voltage_2'] != 0.00 and batteries_data['battery_2']['max_temp_2'] > 0.00:
                print("++++++++++++++++++++")
                print("Data: ", batteries_data['battery_2'])
                socketio.emit('data_2', {'data_2': batteries_data['battery_2']}, namespace='/test')
                with open('battery2_data.csv', 'w', encoding='UTF8', newline='') as f:
                    writer = csv.DictWriter(f, fieldnames=header_fields)
                    writer.writeheader()
                    writer.writerow(batteries_data['battery_2'])
            if time.time() - battery_time_2 > 5:
                batteries_data['battery_2'] = {'voltage_2': 0.00, 'current_2': 0.00, 'soc_2': 0.00, 'max_temp_2': 0.00, 'min_temp_2': 0.00, 'energy_2': 0.00}
                socketio.emit('data_2', {'data_2': batteries_data['battery_2']}, namespace='/test')
                with open('battery2_data.csv', 'w', encoding='UTF8', newline='') as f:
                    writer = csv.DictWriter(f, fieldnames=header_fields)
                    writer.writeheader()
                    writer.writerow(batteries_data['battery_2'])
            if batteries_data['battery_3']['voltage_3'] != 0.00 and batteries_data['battery_3']['max_temp_3'] > 0.00:
                print("++++++++++++++++++++")
                print("Data: ", batteries_data['battery_3'])
                socketio.emit('data_3', {'data_3': batteries_data['battery_3']}, namespace='/test')
                with open('battery3_data.csv', 'w', encoding='UTF8', newline='') as f:
                    writer = csv.DictWriter(f, fieldnames=header_fields)
                    writer.writeheader()
                    writer.writerow(batteries_data['battery_3'])
            if time.time() - battery_time_3 > 5:
                batteries_data['battery_3'] = {'voltage_3': 0.00, 'current_3': 0.00, 'soc_3': 0.00, 'max_temp_3': 0.00, 'min_temp_3': 0.00, 'energy_3': 0.00}
                socketio.emit('data_3', {'data_3': batteries_data['battery_3']}, namespace='/test')
                with open('battery3_data.csv', 'w', encoding='UTF8', newline='') as f:
                    writer = csv.DictWriter(f, fieldnames=header_fields)
                    writer.writeheader()
                    writer.writerow(batteries_data['battery_3'])
            if batteries_data['battery_4']['voltage_4'] != 0.00 and batteries_data['battery_4']['max_temp_4'] > 0.00:
                print("++++++++++++++++++++")
                print("Data: ", batteries_data['battery_4'])
                socketio.emit('data_4', {'data_4': batteries_data['battery_4']}, namespace='/test')
                with open('battery4_data.csv', 'w', encoding='UTF8', newline='') as f:
                    writer = csv.DictWriter(f, fieldnames=header_fields)
                    writer.writeheader()
                    writer.writerow(batteries_data['battery_4'])
            if time.time() - battery_time_4 > 5:
                batteries_data['battery_4'] = {'voltage_4': 0.00, 'current_4': 0.00, 'soc_4': 0.00, 'max_temp_4': 0.00, 'min_temp_4': 0.00, 'energy_4': 0.00}
                socketio.emit('data_4', {'data_4': batteries_data['battery_4']}, namespace='/test')
                with open('battery4_data.csv', 'w', encoding='UTF8', newline='') as f:
                    writer = csv.DictWriter(f, fieldnames=header_fields)
                    writer.writeheader()
                    writer.writerow(batteries_data['battery_4'])
            
            # if data_2:
            #     print("Data: ", data_2)
            #     socketio.emit('data_2', {'data_2': data_2}, namespace='/test')    
    
         
            # stm32_1 = serial.Serial(port=ports[0][0], baudrate=115200, timeout=3)
           
            # data = stm32_1.readline().decode().strip().split(",")
           


            if len(data_serial) < 2:
                status = ["Board Not Connected"]
            else: 
                status = ["Board Connected"]
                # socketio.emit('data_1', {'data': data}, namespace='/test')

            # socketio.emit('status', {'data': status}, namespace='/test')
            # socketio.sleep(2)
        except Exception as e:
            print("Error: ", e)
            status = ["No Serial Port Connected"]
            socketio.emit('status', {'data': status}, namespace='/test')
            # socketio.sleep(2)

            # print("Serial USB is not connected")
       


@app.route('/')
def index():
    print("Rendering page")
    #only by sending this page first will the client be connected to the socketio instance
    return render_template('index.html')

@socketio.on('connect', namespace='/test')
def test_connect():
    # need visibility of the global thread object
    global thread
    print('Client connected')

    #Start the random number generator thread only if the thread has not been started before.
    if not thread.is_alive():
        print("Starting Thread")
        thread = socketio.start_background_task(readstm)

@socketio.on('disconnect', namespace='/test')
def test_disconnect():
    print('Client disconnected')


if __name__ == '__main__':
    socketio.run(app)
