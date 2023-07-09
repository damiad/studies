<HTML>
<HEAD>
    <TITLE> Strona Klienta</TITLE>
</HEAD>
<BODY>
<h2> Panel klienta </h2>
<?PHP 
    session_start(); //Już zalogowani
    $conn = oci_connect($_SESSION['LOGIN'],$_SESSION['PASS'],"//labora.mimuw.edu.pl/LABS");
    
    if (!$conn) {
	echo "oci_connect failed";
	$e = oci_error();
	echo $e['message'];
    }

    $_SESSION['LOGINK'] = $_POST['LOGNK'];
    $_SESSION['PASSK'] = $_POST['PASWK'];
    
    // Prepare the statement
    $stmt = oci_parse($conn, 'BEGIN dd439954.login_klient(:input_param1, :input_param2, :output_param); END;');
    // Bind the input parameters
    $input_param1 = $_SESSION['LOGINK'];
    oci_bind_by_name($stmt, ':input_param1', $input_param1, 10, SQLT_CHR);
    $input_param2 = $_SESSION['PASSK'];
    oci_bind_by_name($stmt, ':input_param2', $input_param2, 26, SQLT_CHR);
    // Register the output parameter
    $result = null;
    oci_bind_by_name($stmt, ':output_param', $result);
    // Execute the statement
    oci_execute($stmt);
    
    if ($result == 0) {
        echo "Zły login lub hasło.\n";
    }else{
        echo "Witamy w serwisie.\n";
?>
        <BR><a href="klient_wniosek.php">Wnioskuj o Kredyt</a><BR>
        <BR><a href="klient_kredyty.php">Przejrzyj Kredyty</a><BR>
        <BR><a href="klient_wplaty.php">Zobacz Wplaty</a><BR>
        <BR><a href="panel_powiadomien.php">Zobacz Panel Powiadomien</a><BR>
<?PHP 
    }
?>
</BODY>
</HTML>
