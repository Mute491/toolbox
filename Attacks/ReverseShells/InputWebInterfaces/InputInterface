<?php

    $shellNumber = $_GET['s'];

    $shellQuantity = $_GET['shellno'];
?>

<html> 

    <head>

        <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0-beta3/css/all.min.css">
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.7.1/jquery.min.js"></script>
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
                margin-top: 6px;
                margin-bottom: 6px;
                margin-left: 8px;
                white-space: nowrap;
            }

            .hacker-input {
                background: #000000;
                border: 1px solid #00ff00;
                color: #00ff00;
                font-family: 'Courier New', Courier, monospace;
                font-size: 16px;
                padding: 10px;
                margin: 10px 0;
                width: 100%;
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

            .shellOutputDiv{

                border: 1px solid #00ff00;
                width: 100%;
                height: 300px;
                overflow-y: auto;
                
            }

        </style>

    </head>

    <script>

        <?php echo("const shellOutputUrl".$shellNumber." = 'shell" . $shellNumber . ".txt';"); ?>

        <?php echo('var lastLenght'.$shellNumber.' = 0;'); ?>
        
        <?php echo("function displayOutput".$shellNumber."(startIndex, shellOutput){"); ?>
            
            <?php echo('const outputDiv = "shellOutputDiv'.$shellNumber.'";'); ?>

            <?php echo('const OutDiv'.$shellNumber.' = document.getElementById(outputDiv);'); ?>
        
            const fileLinesCount = shellOutput.length;
            
            console.log(fileLinesCount);
            console.log(startIndex);

            for(let i=startIndex; i<fileLinesCount; i++){
                
                const iconUser = document.createElement("i");
                const iconPc = document.createElement("i");

                const outputLine = document.createElement("p");
                let line = shellOutput[i];

                iconPc.classList.add("fas");
                iconPc.classList.add("fa-desktop");
                iconUser.classList.add("fas");
                iconUser.classList.add("fa-user-secret");
                
                if(line.substring(0, 2) === ";-"){

                    outputLine.appendChild(iconPc);
                    outputLine.innerHTML += " :: ";
                    line = line.substring(2, line.length);
                
                }
                else if(line.substring(0, 1) === ";"){
                    
                    outputLine.appendChild(iconUser);
                    outputLine.innerHTML += " :: ";
                    line = line.substring(1, line.length);
                
                }

                line = line.replace("<", "&lt;").replace(">", "&gt;");
                
                outputLine.innerHTML += line;

                <?php echo('OutDiv'.$shellNumber.'.appendChild(outputLine);'); ?>

                <?php echo('lastLenght'.$shellNumber.' = fileLinesCount;'); ?>
            
            }

        }
        

    
        setInterval(() => {

            <?php echo("fetch(shellOutputUrl".$shellNumber.").then(response => {"); ?>

                console.log("fetching");
                if (response.ok) {

                    return response.text();
                    
                } else {
                    
                    return "Error";

                }
            
            }).then(data => {
                
                if(data !== "Error"){

                    let temp = data.split("\n");
                    temp.pop();
                    const outputFileContent = temp;
                    const contentLenght = outputFileContent.length;

                    <?php echo('if(contentLenght >= lastLenght'.$shellNumber.'){'); ?>
                    
                        <?php echo("displayOutput".$shellNumber."(lastLenght".$shellNumber.", outputFileContent);"); ?>
                        console.log(outputFileContent);
                    }
                    <?php echo('else if(contentLenght < lastLenght'.$shellNumber.'){'); ?>
                    
                    <?php echo("displayOutput".$shellNumber."(0, outputFileContent);"); ?>

                  }
                    
                }
                
            });
        
        }, 5000); // Check every 5 seconds
      
        
        //jquery

        $(document).ready(function (){


            <?php echo('$("#sendCmdButton'.$shellNumber.'").click(function(){'); ?>

                let postData = {

                    input: <?php echo('$("#textInputShell'.$shellNumber.'").val(),'); ?>
                    shellNo: <?php echo($shellNumber); ?>

                };
                
                console.log(postData);

                $.post("shellExecutor.php", postData, function(data, status){

                    console.log(data + " " + status);

                });

            });

            <?php echo('$("#clearButton'.$shellNumber.'").click(function(){'); ?>

                let postData = {

                    clear: "clearShell",
                    shellNo: <?php echo($shellNumber); ?>

                };

                $.post("shellExecutor.php", postData, function(data, status){

                    if(data.substring(0,1) === "2"){

                        <?php echo('$("#shellOutputDiv' .$shellNumber. '").empty();'); ?>

                    }

                });

            });

        });


    </script>

  
    <body> 
        <div class="icons">
            <i class="fas fa-terminal"></i>
            <i class="fas fa-keyboard"></i>
        </div>
        <div>
            <h3>shell <?php echo($_GET["s"]); ?></h3>
        </div>
        <div>

          <?php echo("<div id='shellOutputDiv" . $shellNumber . "' class='shellOutputDiv'>") ?>
            
          </div>

        </div>
        <div>

            <?php echo('<input type="text" id="textInputShell'.$shellNumber.'" name="input" class="hacker-input" />'); ?>
            <?php echo('<input type="button" id="sendCmdButton'.$shellNumber.'" class="buttons" value="Send"/>'); ?>

        </div> 
        <div>

            <?php echo('<input type="button" id="clearButton'.$shellNumber.'" class="buttons" value="Clear"/>'); ?>

        </div>

    </body>

</html>
