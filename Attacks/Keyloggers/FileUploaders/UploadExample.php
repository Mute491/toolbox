<?php
    if ($_SERVER['REQUEST_METHOD'] == 'POST' && isset($_FILES['uploaded_file'])) {
        $uploadDirectory = __DIR__ . '/uploads/';
        $uploadFile = $uploadDirectory . basename($_FILES['uploaded_file']['name']);

        if (!file_exists($uploadDirectory)) {
            mkdir($uploadDirectory, 0777, true);
        }

        if (move_uploaded_file($_FILES['uploaded_file']['tmp_name'], $uploadFile)) {
            echo "File uploaded successfully.";
        } else {
            echo "Failed to upload file.";
        }
    } else {
        echo "No file uploaded.";
    }
?>
