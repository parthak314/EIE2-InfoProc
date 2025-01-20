from scipy.signal import firwin

num_taps = 5
fs = 1600
cutoff = 300

coefficients = firwin(num_taps, cutoff, fs=fs)
print(coefficients)