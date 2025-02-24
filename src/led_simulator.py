import time
import os
from colorama import init, Fore, Style
import sysv_ipc
import numpy as np

# Initialize colorama for Windows compatibility
init()

class LEDSimulation:
    def __init__(self):
        # Create shared memory using System V IPC
        try:
            # Try to create new shared memory
            self.shm = sysv_ipc.SharedMemory(0x1010, sysv_ipc.IPC_CREX, 0o777, 3)
            # Initialize LED states
            self.shm.write(b'\x00\x00\x00')
        except sysv_ipc.ExistentialError:
            # If shared memory already exists, connect to it
            self.shm = sysv_ipc.SharedMemory(0x1010)
        
        # Create numpy array mapped to shared memory
        self.led_states = np.ndarray((3,), dtype=np.uint8, buffer=self.shm.read()).copy()

        # LED colors when ON
        self.led_colors = [Fore.GREEN, Fore.YELLOW, Fore.RED]

    def clear_screen(self):
        # Clear screen (works on both Windows and Unix-based systems)
        os.system('cls' if os.name == 'nt' else 'clear')

    def draw_leds(self):
        # LED representation
        led_off = f"{Fore.WHITE}○{Style.RESET_ALL}"  # White circle for OFF state
        
        # Update led_states from shared memory
        self.led_states[:] = np.frombuffer(self.shm.read(), dtype=np.uint8)
        
        led1 = f"{self.led_colors[0]}●{Style.RESET_ALL}" if self.led_states[0] else led_off
        led2 = f"{self.led_colors[1]}●{Style.RESET_ALL}" if self.led_states[1] else led_off
        led3 = f"{self.led_colors[2]}●{Style.RESET_ALL}" if self.led_states[2] else led_off

        # Display LEDs
        print("\nLED Simulation (Press Ctrl+C to exit)")
        print("\nLED1   LED2   LED3")
        print(f" {led1}      {led2}      {led3}")
        print("\n")

    def run(self):
        print("Starting LED simulation...")
        print("LED states can be updated by other processes")
        
        try:
            while True:
                self.clear_screen()
                self.draw_leds()
                time.sleep(0.5)  # Update display every 0.5 seconds
                
        except KeyboardInterrupt:
            self.clear_screen()
            print("\nLED simulation stopped")
            print("Goodbye!")
        finally:
            # Cleanup
            try:
                self.shm.detach()
                self.shm.remove()
            except:
                pass

    def __del__(self):
        # Ensure shared memory is cleaned up
        try:
            self.shm.detach()
        except:
            pass

if __name__ == "__main__":
    sim = LEDSimulation()
    sim.run()
