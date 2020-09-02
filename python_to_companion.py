from pythonosc.udp_client import SimpleUDPClient
import mido

ip = "127.0.0.1"
port = 12321

midiport =  mido.open_input('Adafruit Trellis M4')
client = SimpleUDPClient(ip, port)  # Create client

def send_press(page, button):
    osc_message = "/press/bank/" + str(page) + "/" + str(button)
    client.send_message(osc_message, 1)
    client.send_message(osc_message, 0)

for msg in midiport:
    send_press(1, msg.note)
