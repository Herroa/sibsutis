# signal_app.py
import signal
import time

def signal_handler(sig, frame):
    print(f'Received signal: {sig}')
    if sig == signal.SIGTERM:
        print("Exiting gracefully...")
        exit(0)

signal.signal(signal.SIGTERM, signal_handler)

while True:
    print("Running... Press Ctrl+C to stop.")
    time.sleep(2)  