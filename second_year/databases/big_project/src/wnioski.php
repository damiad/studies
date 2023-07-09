<HTML>
<HEAD>
    <TITLE> Strona Wniosków</TITLE>
</HEAD>
<BODY>
<h2> Przeglądaj wnioski </h2>
<?PHP 
    session_start(); //Już zalogowani
    $conn = oci_connect($_SESSION['LOGIN'],$_SESSION['PASS'],"//labora.mimuw.edu.pl/LABS");
    
    if (!$conn) {
	echo "oci_connect failed";
	$e = oci_error();
	echo $e['message'];
    }

    $stmt = oci_parse($conn, "SELECT * FROM dd439954.Wniosek");
    oci_execute($stmt, OCI_NO_AUTO_COMMIT);
    while (($row = oci_fetch_array($stmt, OCI_BOTH))) {
        echo "<BR><A HREF=\"wniosek.php?id=" . $row[0] . "\">" . "Wniosek nr: ". $row[0]  . " o wysokości - " . $row[3] . " PLN" ."</A><BR>\n";
    }

?>
</BODY>
</HTML>
