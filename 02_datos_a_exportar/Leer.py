# -*- coding: utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt
import os

try:
    # Intentar cargar el archivo con el nombre correcto
    datos1 = np.loadtxt("datosVoltaje.DAT", usecols=(0,1), skiprows=1, delimiter=',')
    
    time1 = datos1[:,0]
    Vout1 = datos1[:,1]
    
    # Crear la gráfica
    fig, axes = plt.subplots(figsize=(14, 6))
    axes.plot(time1, Vout1, color="r", linewidth=3)
    axes.set_ylabel('$V_{outINA219}$ (V)', fontsize=22)
    axes.set_xlabel('Tiempo ($s$)', fontsize=22)
    axes.grid(True)
    plt.savefig('Vout.jpg', dpi=500, bbox_inches='tight')
    plt.show()
    
    print("Gráfica generada exitosamente!")
    
except FileNotFoundError:
    print("Error: No se pudo encontrar el archivo 'datosVoltaje.DAT'")
    print("Archivos disponibles en el directorio:")
    for archivo in os.listdir():
        print(f"  - {archivo}")
except Exception as e:
    print(f"Error al procesar los datos: {e}")