import serial
import time

puerto_proteus = 'COM2'  # COMPIM de Proteus
puerto_esp32 = 'COM3'    # ESP32 física

try:
    ser_proteus = serial.Serial(puerto_proteus, 9600, timeout=1)
    ser_esp32 = serial.Serial(puerto_esp32, 9600, timeout=1)
    
    print("=== PUENTE SERIAL ACTIVO ===")
    print(f"PROTEUS (COM4) -> ESP32 (COM3)")
    print("Transmitiendo datos...")
    print("=============================")
    
    while True:
        # Proteus -> ESP32
        if ser_proteus.in_waiting > 0:
            data = ser_proteus.readline().decode().strip()
            if data:
                ser_esp32.write((data + '\n').encode())
                print(f"TENSION: {data} V")
        
        time.sleep(0.1)
        
except Exception as e:
    print(f"Error: {e}")