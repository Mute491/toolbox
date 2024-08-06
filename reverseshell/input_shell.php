<?php
// Nome del file in cui salvare i dati
$filename = 'shell.txt';

// Verifica se il parametro POST 'output' è stato inviato
if (isset($_POST['input'])) {
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
            echo "Errore: non è stato possibile ottenere il lock sul file $filename.";
        }

        // Chiudi il file
        fclose($file);

        // Risposta di successo
        echo "Dati scritti correttamente in $filename.";
    } else {
        // Risposta di errore se il file non può essere aperto
        echo "Errore: non è stato possibile aprire il file $filename.";
    }
} else {
    // Risposta di errore se il parametro 'output' non è stato inviato
    echo "Errore: parametro 'input' mancante.";
}

?>
<html>
    <body>
    
        <form action="input_shell.php" method="post">

            <input type="text" name="input">
            <input type="submit">

        </form>

    </body>

</html>