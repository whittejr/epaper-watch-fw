import serial
import csv
import time
import sys

# Configurações da Serial
SERIAL_PORT = '/dev/ttyACM0'  # Altere para a porta do seu ST-Link/UART
BAUD_RATE = 115200
OUTPUT_FILE = 'sensor_data.csv'
DURATION = 30  # Tempo de captura em segundos

def main():
    print(f"Iniciando captura na porta {SERIAL_PORT}...")
    
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    except Exception as e:
        print(f"Erro ao abrir porta serial: {e}")
        return

    # Garante que o arquivo de saída esteja na pasta correta
    filename = OUTPUT_FILE
    if len(sys.argv) > 1:
        filename = sys.argv[1]

    print(f"Gravando dados em {filename} por {DURATION} segundos. Coloque o dedo no sensor!")

    with open(filename, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(['RED', 'IR']) # Header
        
        start_time = time.time()
        count = 0
        
        while time.time() - start_time < DURATION:
            line = ser.readline().decode('utf-8').strip()
            if line:
                try:
                    # Esperamos o formato: RED,IR
                    parts = line.split(',')
                    if len(parts) == 2:
                        red = int(parts[0])
                        ir = int(parts[1])
                        writer.writerow([red, ir])
                        count += 1
                        if count % 100 == 0:
                            print(f"Amostras capturadas: {count}...")
                except ValueError:
                    continue

    ser.close()
    print(f"Captura finalizada! {count} amostras salvas.")

if __name__ == "__main__":
    main()
