# UART2 Stream Logging Script

import serial
import csv
from datetime import datetime

# Setup serial port read
ser = serial.Serial('/dev/ttyACM1', 9600, timeout=1)

# Format csv file 
csv_filename = f"logs/UART2_log_{datetime.now().strftime('%Y%m%d_%H%M%S')}.csv"
with open(csv_filename, 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(['Date', 'Time', 'Temp (C)', 'Pressure (Pa)', 'GPS'])

# Log incoming UART stream data 
    print(f"Begin logging to {csv_filename}")
    try:
        temp = pressure = gps = None

        while True:
            line = ser.readline().decode('utf-8').strip()
            if line:
                now = datetime.now()
                print(line)

                if line.startswith("Temp"):
                    temp = line.split(":")[1].strip()
                elif line.startswith("Pressure"):
                    pressure = line.split(":")[1].strip()
                elif line.startswith("$GPRMC"):
                    gps = line
                    writer.writerow([
                        now.date(),
                        now.time().strftime('%H:%M:%S'),
                        temp,
                        pressure,
                        gps
                    ])
                    csvfile.flush()
    except KeyboardInterrupt:
        print("Data logging stopped.")
