// Cactus Proyect - Base de datos 2020
// Emanuel Moroni

<?php
// En php para declarar variables debo anteponer el símbolo pesos al nombre de la variable en cuestión. emoroni.
$servername = "localhost";

$dbname = "esp_data";
$username = "root";
$password = "password";

$api_key_value = "emoroni_utn_frba_2020";

$api_key= $humedad_suelo = $int_luminica = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
        $humedad_suelo = test_input($_POST["humdsuelo"]);
        $int_luminica = test_input($_POST["intlum"]);
        $id_seccion = test_input($_POST["idsec"]);
        
        // Crear conexión
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Chequar conexión
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        } 
        
        // Debo modificar la forma en la que almaceno el id_maceta. emoroni 20/11/2020
        $sql = "INSERT INTO Medicion_Planta (humedadSuelo, iluminancia, id_seccion)
        VALUES ('" . $humedad_suelo . "', '" . $int_luminica . "', '" . $id_seccion . "')";
        
        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } 
        else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
    
        $conn->close();
    }
    else {
        echo "La apikey es incorrecta.";
    }

}
else {
    echo "No hay información para publicar en la base de datos con la solicitud POST.";
}

// emoroni. 25/10/2020.
function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
?>
