<HTML>
<HEAD>
    <TITLE> Strona Wpłat</TITLE>
</HEAD>
<BODY>
<h2> Przeglądaj Wpłaty </h2>
<?PHP 
    session_start(); //Już zalogowani
    $conn = oci_connect($_SESSION['LOGIN'],$_SESSION['PASS'],"//labora.mimuw.edu.pl/LABS");
    
    if (!$conn) {
	echo "oci_connect failed";
	$e = oci_error();
	echo $e['message'];
    }

    $stmt = oci_parse($conn, "SELECT * FROM dd439954.Wplata ORDER BY data DESC");
    oci_execute($stmt, OCI_NO_AUTO_COMMIT);
    while (($row = oci_fetch_array($stmt, OCI_BOTH))) {
        $status = null;
        if($row[1] == 0)
            $status = " - niezaksięgowana";
        else
            $status = " - zaksięgowana";
        echo "<BR>Identyfikator wplaty: <A HREF=\"wplata.php?id=" . $row[0] . "\">" . $row[0]  . "</A> Identyfikator kredytu: <A HREF=\"kredyt.php?id=" . $row[4] . "\">" . $row[4]  . "</A>" . $status . "<BR>\n";
    }
?>
</BODY>
</HTML>
