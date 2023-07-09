<HTML>
<HEAD>
    <TITLE> Przegląd Klientów</TITLE>
</HEAD>
<BODY>
<h2> Przeglądaj Klientów </h2>
<?PHP 
    session_start(); //Już zalogowani
    $conn = oci_connect($_SESSION['LOGIN'],$_SESSION['PASS'],"//labora.mimuw.edu.pl/LABS");
    
    if (!$conn) {
	echo "oci_connect failed";
	$e = oci_error();
	echo $e['message'];
    }

    $stmt = oci_parse($conn, "SELECT * FROM dd439954.Klient");
    oci_execute($stmt, OCI_NO_AUTO_COMMIT);
    while (($row = oci_fetch_array($stmt, OCI_BOTH))) {
        echo "<BR><A HREF=\"klient.php?id=" . $row[0] . "\">" . "Klient nr: ". $row[0]  . "   Imię - " . $row[2] . ",   Nazwisko - " . $row[3] ."</A><BR>\n";
    }

?>
</BODY>
</HTML>
