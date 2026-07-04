# UART2 Stream Logging Script

import serial
import csv
from datetime import datetime

# Setup serial port read
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)

# Format csv file
csv_filename = f"logs/UART2_log_{datetime.now().strftime('%Y%m%d_%H%M%S')}.csv"
with open(csv_filename, 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow([
        'Date', 'Time', 'Temp (C)', 'Pressure (Pa)',
        'Accel ChipID', 'Gyro ChipID',
        'Accel X', 'Accel Y', 'Accel Z',
        'Gyro X', 'Gyro Y', 'Gyro Z',
        'GPS'
    ])

# Log incoming UART stream data
    print(f"Begin logging to {csv_filename}")
    try:
        temp = pressure = gps = None
        accel_chipid = gyro_chipid = None
        accel_x = accel_y = accel_z = None
        gyro_x = gyro_y = gyro_z = None

        while True:
            line = ser.readline().decode('utf-8', errors='replace').strip()
            if line:
                now = datetime.now()
                print(line)

                try:
                    if line.startswith("Temp"):
                        temp = line.split(":")[1].strip()
                    elif line.startswith("Pressure"):
                        pressure = line.split(":")[1].strip()
                    elif line.startswith("Accel ChipID"):
                        accel_chipid = line.split(":")[1].strip()
                    elif line.startswith("Gyro ChipID"):
                        gyro_chipid = line.split(":")[1].strip()
                    elif line.startswith("Accel"):
                        accel_x, accel_y, accel_z = [
                            v.strip() for v in line.split(":")[1].split(",")
                        ]
                    elif line.startswith("Gyro"):
                        gyro_x, gyro_y, gyro_z = [
                            v.strip() for v in line.split(":")[1].split(",")
                        ]
                except (IndexError, ValueError):
                    # Torn/interleaved UART line (Accel and Gyro prints can
                    # interrupt each other mid-write) - drop and continue.
                    continue

                if line.startswith("$GPRMC"):
                    gps = line
                    writer.writerow([
                        now.date(),
                        now.time().strftime('%H:%M:%S'),
                        temp,
                        pressure,
                        accel_chipid, gyro_chipid,
                        accel_x, accel_y, accel_z,
                        gyro_x, gyro_y, gyro_z,
                        gps
                    ])
                    csvfile.flush()
    except KeyboardInterrupt:
        print("Data logging stopped.")
