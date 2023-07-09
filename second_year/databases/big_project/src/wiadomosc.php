<HTML>
<HEAD>
    <TITLE> Strona Wiadomości</TITLE>
</HEAD>
<BODY>
<h2> Dane Wiadomości</h2>
<?PHP 
    session_start(); //Już zalogowani
    $conn = oci_connect($_SESSION['LOGIN'],$_SESSION['PASS'],"//labora.mimuw.edu.pl/LABS");
    
    if (!$conn) {
	echo "oci_connect failed";
	$e = oci_error();
	echo $e['message'];
    }

    $stmt = oci_parse($conn, "SELECT * FROM dd439954.Wiadomosc WHERE id_wiadomosci = " . $_GET['id']);
    oci_execute($stmt, OCI_NO_AUTO_COMMIT);
    $row = oci_fetch_row($stmt);

    echo "<BR>Identyfikator wiadomości: $row[0]<BR>\n";
    echo "<BR>Typ: $row[1]<BR>\n";
    echo "<BR>Data: $row[3]<BR>\n";
    echo "<h3><BR>Treść: $row[2]<BR><h3>\n";

    if(isset($_POST['button1'])) {
        $stmt2 = oci_parse($conn,"DELETE FROM dd439954.Wiadomosc WHERE id_wiadomosci = " . $_GET['id']);
        oci_execute($stmt2, OCI_COMMIT_ON_SUCCESS);
        header("Location: panel_powiadomien.php");
        exit();
    }
?>
    <form method="post">
        <input type="submit" name="button1"
                value="USUŃ WIADOMOŚĆ"/>
    </form>
</BODY>
</HTML>
