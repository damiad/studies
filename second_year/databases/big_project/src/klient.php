<HTML>
<HEAD>
    <TITLE> Dane Klienta</TITLE>
</HEAD>
<BODY>
<h2> Dane Klienta </h2>
<?PHP 
    session_start(); //Już zalogowani
    $conn = oci_connect($_SESSION['LOGIN'],$_SESSION['PASS'],"//labora.mimuw.edu.pl/LABS");
    
    if (!$conn) {
	echo "oci_connect failed";
	$e = oci_error();
	echo $e['message'];
    }

    $stmt = oci_parse($conn, "SELECT * FROM dd439954.Klient WHERE id_klienta = " . $_GET['id']);
    oci_execute($stmt, OCI_NO_AUTO_COMMIT);
    $row = oci_fetch_row($stmt);

    echo "<BR>Klient nr: $row[0]<BR>\n";
    echo "<BR>Imię: $row[2]<BR>\n";   
    echo "<BR>Nazwisko: $row[3]<BR>\n";
    echo "<BR>Numer telefonu: +48 $row[4]<BR>\n";
    echo "<BR>Najgorszy kredyt: $row[5]<BR>\n";

    if(isset($_SESSION['LOGINP'])){ // Dodana funkcjonalność
        echo "<br><br>\n";
        echo "<br><br>\n";
        echo"<h3> Kredyty Klienta: </h3>\n";
        $stmt2 = oci_parse($conn, "SELECT * FROM dd439954.Kredyt WHERE id_klienta=" . $_GET['id'] . "ORDER BY data_nadchodzacej_raty ASC");
        oci_execute($stmt2, OCI_NO_AUTO_COMMIT);
        while (($row2 = oci_fetch_array($stmt2, OCI_BOTH))) {
            echo "<BR><A HREF=\"kredyt.php?id=" . $row2[0] . "\">" . "Identyfikator kredyut: ". $row2[0]  . "</A><BR>\n";
        }
    }
?>
</BODY>
</HTML>
