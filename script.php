<?php

$hostname = "endereço"; // Endereço do servidor de banco de dados
$username = "usuarios"; // Nome de usuário do banco de dados
$password = "senha"; // Senha do banco de dados
$database = "nome do banco"; // Nome do banco de dados

$conn = mysqli_connect($hostname, $username, $password, $database); // Conexão com o banco de dados

if (!$conn) {
    die("Conexão falhou: " . mysqli_connect_error()); // Verifica se a conexão com o banco de dados foi bem-sucedida
}

echo "Conectado com sucesso"; // Exibe uma mensagem de sucesso se a conexão foi estabelecida corretamente

if(isset($_POST["temperatura"]) && isset($_POST["umidade"]) && isset($_POST["device"])) {

    $t = $_POST["temperatura"]; // Obtém o valor da temperatura enviado pelo dispositivo
    $h = $_POST["umidade"]; // Obtém o valor da umidade enviado pelo dispositivo
    $device = $_POST["device"]; // Obtém o identificador do dispositivo enviado pelo dispositivo

    $sql = "INSERT INTO leitura_sensor (device, temperatura, umidade) VALUES (".$device.", ".$t.", ".$h.")"; // Cria a query SQL para inserir os dados na tabela "leitura_sensor"

    if (mysqli_query($conn, $sql)) { 
        echo "\nNew record created successfully"; // Exibe uma mensagem de sucesso se o registro foi inserido corretamente no banco de dados
    } else { 
        echo "Error: " . $sql . "<br>" . mysqli_error($conn); // Exibe uma mensagem de erro se houve algum problema na inserção do registro
    }
}
?>
