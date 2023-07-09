<HTML>
<HEAD>
    <TITLE> Symulacja spłaty</TITLE>
</HEAD>
<BODY>
<h2> Symulacja spłaty </h2>
<?PHP 
    session_start(); //Już zalogowani
    $conn = oci_connect($_SESSION['LOGIN'],$_SESSION['PASS'],"//labora.mimuw.edu.pl/LABS");
    
    if (!$conn) {
	echo "oci_connect failed";
	$e = oci_error();
	echo $e['message'];
    }else{
        echo "<BR><b>Kredyt numer: " . $_GET['id']. "</b><BR>\n";
        $stmt = oci_parse($conn, "SELECT * FROM TABLE(dd439954.symulacja_splaty(" . $_GET['id']. "))");
        oci_execute($stmt, OCI_NO_AUTO_COMMIT);
        while (($row = oci_fetch_array($stmt, OCI_BOTH))) {
            echo "<BR>Data raty: $row[0],  kwota raty: $row[1] PLN,  do spłaty pozostanie: $row[2] PLN<BR>\n";
        }
    }

?>
</BODY>
</HTML>
