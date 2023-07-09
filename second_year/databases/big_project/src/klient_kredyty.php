<HTML>
<HEAD>
    <TITLE> Strona Kredytów Klienta</TITLE>
</HEAD>
<BODY>
<h2> Przegladaj Kredyty</h2>
<?PHP 
    session_start(); //Już zalogowani
    $conn = oci_connect($_SESSION['LOGIN'],$_SESSION['PASS'],"//labora.mimuw.edu.pl/LABS");
    
    if (!$conn) {
	echo "oci_connect failed";
	$e = oci_error();
	echo $e['message'];
    }

    $stmt = oci_parse($conn, "SELECT * FROM dd439954.Kredyt WHERE id_klienta=" . $_SESSION['LOGINK'] . "ORDER BY data_nadchodzacej_raty ASC");
    oci_execute($stmt, OCI_NO_AUTO_COMMIT);
    while (($row = oci_fetch_array($stmt, OCI_BOTH))) {
        echo "<BR><A HREF=\"kredyt.php?id=" . $row[0] . "\">" . "Identyfikator kredyut: ". $row[0]  . "</A><BR>\n";
    }

?>
</BODY>
</HTML>
