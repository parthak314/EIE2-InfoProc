from scipy.signal import firwin

num_taps = 5
fs = 1600 # Sampling Rate
cutoff = 300 # Passband frequency

coefficients = firwin(num_taps, cutoff, fs=fs)
print(coefficients)