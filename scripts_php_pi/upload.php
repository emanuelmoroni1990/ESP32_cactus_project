<?php

$servername = "localhost";

$dbname = "esp_data";
$username = "root";
$password = "password";

// Crear conexión
$conn = new mysqli($servername, $username, $password, $dbname);
// Chequear conexión
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

// https://www.php.net/manual/es/mysqli-result.fetch-array.php

/*
$sql = "SELECT MAX(id_medicion) AS max_id_medicion FROM Medicion_Maceta";
$result = $conn->query($sql);
$row = $result->fetch_array(MYSQLI_NUM);
printf ("El valor máximo es: %s\n", $row[0]);
*/

$target_dir = "uploads/";
$datum = mktime(date('H')+0, date('i'), date('s'), date('m'), date('d'), date('y'));
$file_name = date('Y.m.d_H:i:s_', $datum) . basename($_FILES["imageFile"]["name"]);
$target_file = $target_dir . $file_name; //date('Y.m.d_H:i:s_', $datum) . basename($_FILES["imageFile"]["name"]);
$uploadOk = 1;
$imageFileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));

/*
$sql = "INSERT INTO Imagen (nombreImagen, id_medicion) VALUES('" . $file_name . "', '" . $row[0] . "')";
$result = $conn->query($sql);
printf ("El resultado de la query es: %s\n", $result); 
*/

if ($_SERVER["REQUEST_METHOD"] == "POST") {
          
      // Crear conexión
      $conn = new mysqli($servername, $username, $password, $dbname);
      // Chequear conexión
      if ($conn->connect_error) {
          die("Connection failed: " . $conn->connect_error);
      } 
      
      $sql = "SELECT MAX(id_medicion) AS max_id_medicion FROM Medicion_Maceta";
      $result = $conn->query($sql);
      $row = $result->fetch_array(MYSQLI_NUM);
      //printf ("El valor máximo es: %s\n", $row[0]);
            
      $sql = "INSERT INTO Imagen (nombreImagen, id_medicion) VALUES('$file_name', '$row[0]')"; //('$file_name')";
      
      if ($conn->query($sql) === TRUE) {
          echo "New record created successfully";
      } 
      else {
          echo "Error: " . $sql . "<br>" . $conn->error;
      }
  
      $conn->close();
    
}
else {
    echo "No hay información para publicar en la base de datos con la solicitud POST.";
}

// Check if image file is a actual image or fake image
if(isset($_POST["submit"])) {
  
  $check = getimagesize($_FILES["imageFile"]["tmp_name"]);
  if($check !== false) {
    echo "File is an image - " . $check["mime"] . ".";
    $uploadOk = 1;
  }
  else {
    echo "File is not an image.";
    $uploadOk = 0;
  }
}

// Check if file already exists
if (file_exists($target_file)) {
  echo "Sorry, file already exists.";
  $uploadOk = 0;
}

// Check file size
if ($_FILES["imageFile"]["size"] > 500000) {
  echo "Sorry, your file is too large.";
  $uploadOk = 0;
}

// Allow certain file formats
if($imageFileType != "jpg" && $imageFileType != "png" && $imageFileType != "jpeg"
&& $imageFileType != "gif" ) {
  echo "Sorry, only JPG, JPEG, PNG & GIF files are allowed.";
  $uploadOk = 0;
}

// Check if $uploadOk is set to 0 by an error
if ($uploadOk == 0) {
  echo "Sorry, your file was not uploaded.";
// if everything is ok, try to upload file
}
else {
  if (move_uploaded_file($_FILES["imageFile"]["tmp_name"], $target_file)) {
    echo "The file ". basename( $_FILES["imageFile"]["name"]). " has been uploaded.";
  }
  else {
    echo "Sorry, there was an error uploading your file.";
  }
}
?>
