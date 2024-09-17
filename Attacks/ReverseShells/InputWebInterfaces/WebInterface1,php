<html> 

    <head>

        <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0-beta3/css/all.min.css">
        <style>

            body {
                background-color: #000000;
                color: #00ff00;
                font-family: 'Courier New', Courier, monospace;
                margin: 0;
                padding: 20px;
                display: flex;
                justify-content: flex-start;
                align-items: flex-start;
                height: 100vh;
                flex-direction: column;
            }

            h3 {
                color: #00ff00;
                text-shadow: 0 0 10px #00ff00;
                margin: 0;
            }

            p {
                color: #00ff00;
                text-shadow: 0 0 10px #00ff00;
                margin: 0;
            }

            .hacker-input {
                background: #000000;
                border: 1px solid #00ff00;
                color: #00ff00;
                font-family: 'Courier New', Courier, monospace;
                font-size: 16px;
                padding: 10px;
                margin: 10px 0;
                width: calc(100% - 24px);
                outline: none;
            }

            .buttons {
                background: #000000;
                border: 1px solid #00ff00;
                color: #00ff00;
                font-family: 'Courier New', Courier, monospace;
                font-size: 16px;
                padding: 10px 20px;
                margin: 10px 0;
                cursor: pointer;
                outline: none;
                text-shadow: 0 0 10px #00ff00;
            }

            .buttons:hover {
                background: #00ff00;
                color: #000000;
            }

            form {
                display: flex;
                flex-direction: column;
                width: 100%;
            }

            div {
                width: 100%;
            }

        </style>

    </head>

    <body> 
        <div class="icons">
            <i class="fas fa-terminal"></i>
            <i class="fas fa-keyboard"></i>
        </div>
        <div>
            <h3>shell</h3>
        </div>
        <div>
            <pre>
<?php

//evita di far salvare la pagina nella cache del browser
header("Cache-Control: no-cache, must-revalidate"); // HTTP 1.1.
header("Expires: Sat, 26 Jul 1997 05:00:00 GMT"); // Data nel passato
header("Pragma: no-cache");
// Nome del file in cui salvare i dati
$filename = 'shell.txt';

// Verifica se il parametro POST 'output' è stato inviato
if (isset($_POST['input']) and $_POST["input"] != "") {
    // Ottieni il valore del parametro 'output'
    $command = ";" . $_POST['input'];

    // Aggiungi una nuova riga al contenuto
    $command .= PHP_EOL;

    // Apri il file in modalità append
    $file = fopen($filename, 'a');

    // Verifica se il file è stato aperto correttamente
    if ($file) {
        // Blocca il file
        if (flock($file, LOCK_EX)) {
            // Scrivi i dati nel file
            fwrite($file, $command);

            // Rilascia il lock
            flock($file, LOCK_UN);
        } else {
            // Risposta di errore se il file non può essere bloccato
            echo "<p>Errore: non è stato possibile ottenere il lock sul file $filename.</p>";
        }

        // Chiudi il file
        fclose($file);

        // Risposta di successo
        echo "<p>Dati scritti correttamente in $filename.</p>";
    } else {
        // Risposta di errore se il file non può essere aperto
        echo "<p>Errore: non è stato possibile aprire il file $filename.</p>";
    }
}
elseif (isset($_POST["clear"])) {

    $file = fopen($filename, 'w');
    fwrite($file, ";echo connesso" . PHP_EOL);
    fclose($file);
}
else {
    // Risposta di errore se il parametro 'output' non è stato inviato
    echo "<p>Errore: parametro 'input' mancante.</p>";
}

//stampa il contenuto di shell.txt
$file = fopen($filename, 'r');

if($file){

    while (($line = fgets($file)) !== false) {

        $line = rtrim($line, "\r\n");
        
        
        if(str_starts_with($line, ";-")){

            $line = str_replace(";-","", $line);
            echo("<br><p><div class='icons'><i class='fas fa-desktop'></i> :: " . htmlspecialchars($line) . "</div></p>");

        }
        elseif(str_starts_with($line, ";")){

            $line = str_replace(';',"", $line);
            echo("<br><p><div class='icons'><i class='fas fa-user-secret'></i> :: " . htmlspecialchars($line) . "</div></p>");

        }
        else{

            echo("<br><p>" . htmlspecialchars($line) . "</div></p>");


        }
    }

    fclose($file);

}


?>
            </pre>
        </div>
        <div>
            <form action="input_shell.php" method="post">

                <input type="text" name="input" class="hacker-input">
                <input type="submit" class="buttons">

            </form>
        </div> 
        <div>
            <form action="input_shell.php" method="post">

                <input type="submit" name="clear" value="clear" class="buttons">
                <input type="submit" name="refresh" value="refresh" class="buttons">

            </form>
        </div>

    </body>

</html>
