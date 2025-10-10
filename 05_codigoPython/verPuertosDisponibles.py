import serial.tools.list_ports

# Listar todos los puertos COM disponibles
puertos = serial.tools.list_ports.comports()
print("=== PUERTOS COM DISPONIBLES ===")
for puerto in puertos:
    print(f"Puerto: {puerto.device} - Descripción: {puerto.description}")
print("================================")

if not puertos:
    print("No se encontraron puertos COM disponibles")