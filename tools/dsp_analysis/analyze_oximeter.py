import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import butter, lfilter, find_peaks

def butter_bandpass(lowcut, highcut, fs, order=4):
    nyq = 0.5 * fs
    low = lowcut / nyq
    high = highcut / nyq
    b, a = butter(order, [low, high], btype='band')
    return b, a

def main():
    # Load data
    try:
        df = pd.read_csv('tools/dsp_analysis/data/sensor_data.csv')
    except:
        df = pd.read_csv('data/sensor_data.csv') # local run
        
    fs = 100.0  # 100 Hz
    
    # 1. DC Removal (Subtract mean)
    red_raw = df['RED'].values.astype(float)
    ir_raw = df['IR'].values.astype(float)
    
    red_dc = np.mean(red_raw)
    ir_dc = np.mean(ir_raw)
    
    red_ac_only = red_raw - red_dc
    ir_ac_only = ir_raw - ir_dc
    
    # 2. Bandpass Filter (0.5Hz to 5.0Hz) - Typical PPG range
    lowcut = 0.5
    highcut = 4.0 # max 240 bpm
    b, a = butter_bandpass(lowcut, highcut, fs, order=2)
    
    red_filtered = lfilter(b, a, red_ac_only)
    ir_filtered = lfilter(b, a, ir_ac_only)
    
    # 3. Peak Detection for HR
    # We use IR for peaks as it usually has better SNR
    distance = fs / 3  # Min distance between peaks (max 180 bpm)
    peaks, _ = find_peaks(ir_filtered, distance=distance, height=0)
    
    # Calculate BPM
    if len(peaks) > 1:
        intervals = np.diff(peaks) / fs
        avg_interval = np.mean(intervals)
        bpm = 60 / avg_interval
    else:
        bpm = 0
        
    # 4. SpO2 (Ratio of Ratios)
    # R = (RMS_red / DC_red) / (RMS_ir / DC_ir)
    red_rms = np.sqrt(np.mean(red_filtered**2))
    ir_rms = np.sqrt(np.mean(ir_filtered**2))
    
    R = (red_rms / red_dc) / (ir_rms / ir_dc)
    spo2 = 110 - 25 * R # Linear approximation
    
    print(f"Calculated BPM: {bpm:.1f}")
    print(f"Calculated SpO2: {spo2:.1f}%")
    print(f"Filter Coefficients (b): {b}")
    print(f"Filter Coefficients (a): {a}")

    # Plotting
    fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(10, 8), sharex=True)
    
    t = np.arange(len(red_raw)) / fs
    
    ax1.plot(t, red_raw, 'r', label='RED Raw')
    ax1.plot(t, ir_raw, 'b', label='IR Raw')
    ax1.set_title('Raw Signal (Red & IR)')
    ax1.legend()
    
    ax2.plot(t, red_ac_only, 'r', alpha=0.5, label='RED DC Removed')
    ax2.plot(t, ir_ac_only, 'b', alpha=0.5, label='IR DC Removed')
    ax2.set_title('AC Component (Mean Subtracted)')
    ax2.legend()
    
    ax3.plot(t, red_filtered, 'r', label='RED Filtered')
    ax3.plot(t, ir_filtered, 'b', label='IR Filtered')
    ax3.plot(t[peaks], ir_filtered[peaks], "x", label='Peaks')
    ax3.set_title('Bandpass Filtered (0.5 - 4.0 Hz)')
    ax3.set_xlabel('Time (s)')
    ax3.legend()
    
    plt.tight_layout()
    plt.savefig('tools/dsp_analysis/ppg_analysis.png')
    # plt.show()

if __name__ == "__main__":
    main()
