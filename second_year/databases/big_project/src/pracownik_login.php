<HTML>
<HEAD>
    <TITLE> Strona Pracownika</TITLE>
</HEAD>
<BODY>
<h2> Panel pracownika </h2>
<?PHP 
    session_start(); //Już zalogowani
    $conn = oci_connect($_SESSION['LOGIN'],$_SESSION['PASS'],"//labora.mimuw.edu.pl/LABS");
    
    if (!$conn) {
	echo "oci_connect failed";
	$e = oci_error();
	echo $e['message'];
    }

    $_SESSION['LOGINP'] = $_POST['LOGNP'];
    $_SESSION['PASSP'] = $_POST['PASWP'];
    
    // Prepare the statement
    $stmt = oci_parse($conn, 'BEGIN dd439954.login_pracownik(:input_param1, :input_param2, :output_param); END;');
    // Bind the input parameters
    $input_param1 = $_SESSION['LOGINP'];
    oci_bind_by_name($stmt, ':input_param1', $input_param1, 10, SQLT_CHR);
    $input_param2 = $_SESSION['PASSP'];
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
        $stmt2 = oci_parse($conn, "SELECT czy_obslugi FROM Pracownik WHERE id_pracownika= " . $input_param1);
        oci_execute($stmt2, OCI_NO_AUTO_COMMIT);
        $row = oci_fetch_row($stmt2);
        if($row[0] == 0){ // Dzial kredytow
            echo '<BR><a href="wnioski.php">Zobacz Wnioski o Kredyt</a><BR>';
        }
?>
        <BR><a href="klienci.php">Zobacz Klientów</a><BR>
        <BR><a href="kredyty.php">Zobacz Kredyty</a><BR>
        <BR><a href="wplaty.php">Zobacz Wpłaty</a><BR>
        <BR><a href="zalozkonto.php">Załóż Konto Nowemu Klientowi</a><BR>
<?PHP 
    }
?>
</BODY>
</HTML>
