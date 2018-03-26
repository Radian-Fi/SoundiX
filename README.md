# SoundiX
Utility which is designed to analyze sound samples (from various formats) and convert then to MIDI.

External library named 'libsndfile' is needed.
For instalation use: sudo apt-get install libsndfile1 libsndfile1-dev

For compilation of newest release use:
make all

For this moment is MIDI support still unavaible, only "Pseudo-MIDI" format output notes.out 
with its structure:

t	//as timestamp

v	//velocity or volume (also MIDI compatible)

f	//frequency in MIDI compatible formate (dec -> byte)
