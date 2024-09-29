<html>

  <head>

      <title>Index</title>

      <style>

        .row
        {

          display:flex;
          height: 500px;
          
        }

        .column{

          width:50%;
          flex:1;
          margin: 10px;
        }
        
      </style>
    
  </head>

  
  <body>

    <div>

      <?php

        $shellNo = (int) $_GET["shellno"];
      
        echo('<div class="row">');
      
        for($i = 0; $i<$shellNo; $i++){

          echo('<div class="column">');
          $_GET["s"] = $i;
          include("InputInterface.php");
          echo('</div>');

          if(($i%2) != 0){

            echo('</div>');
            echo('<div class="row">');
            
          }
          
        }
      
        echo('</div>');
      
      ?>
      
    </div>
    
  </body>

</html>
