import numpy as np
import scipy.signal as signal
import matplotlib.pyplot as plt

# PARAMETERS
fs = 1600 # Hz
passband_freq = 300
stopband_freq = 350
passband_ripple = 0.5 # dB
stopband_atten = 65

# DERIVED VALUES
numtaps = int(input("Number of Taps (odd):"))
nyquist = fs / 2
passband = passband_freq / nyquist
stopband = stopband_freq / nyquist
delta_p = (10**(passband_ripple / 20) - 1) / ((10**(passband_ripple / 20) + 1))
delta_s = 10**(-stopband_atten / 20)
delta = min(delta_p, delta_s)
atten = -20*np.log10(delta)
transistion_width = stopband - passband

# Using installed scipy package 
beta = signal.kaiser_beta(atten)


coefficients = signal.firwin(numtaps, cutoff=(passband_freq + stopband_freq)/2, fs=fs, window=('kaiser', beta), pass_zero='lowpass')
print(f'Filter coefficients: {coefficients}')

w, h = signal.freqz(coefficients, worN=800, fs=fs)
plt.figure()
plt.plot(w, 20*np.log10(abs(h)))
plt.title("FIR Filter Frequency Response")
plt.xlabel("Frequency (Hz)")
plt.ylabel("Gain (dB)")
plt.grid()
plt.show()

data = np.random.randn(1000)
filtered_data = signal.lfilter(coefficients, 1.0, data)

plt.figure(figsize=(12, 6))
plt.plot(data, label="Original Data")
plt.plot(filtered_data, label="Filtered Data", linewidth=2)
plt.legend()
plt.title("Random Data Before and After Filtering")
plt.xlabel("Sample Index")
plt.ylabel("Amplitude")
plt.grid()
plt.show()

