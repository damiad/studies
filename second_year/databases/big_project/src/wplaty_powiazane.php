<HTML>
<HEAD>
    <TITLE> Strona Wpłat na rzec kredytu</TITLE>
</HEAD>
<BODY>
<h2> Wpłaty na rzecz kredytu</h2>
<H4> Filtruj okres </H4>
<FORM METHOD="POST">
    Od okresu: <INPUT TYPE="date" NAME="pocz" value="2010-01-01" min="2010-01-01" max="2030-01-01"><BR><BR>
    Do okresu: <INPUT TYPE="date" NAME="koniec" value="2030-12-31" min="2010-01-01" max="2030-12-31"><BR><BR>
    <INPUT TYPE="SUBMIT" NAME="button1" VALUE="Filtruj">
</FORM>
<?PHP 
    session_start(); //Już zalogowani
    $conn = oci_connect($_SESSION['LOGIN'],$_SESSION['PASS'],"//labora.mimuw.edu.pl/LABS");
    
    if (!$conn) {
	echo "oci_connect failed";
	$e = oci_error();
	echo $e['message'];
    }

    if(isset($_POST['button1'])) {
        $pocz = $_POST['pocz'];
        $pocz[4]= $pocz[7] = '/';
        $koniec = $_POST['koniec'];
        $koniec[4]= $koniec[7] = '/';
        $stmt = oci_parse($conn, "SELECT * FROM dd439954.Wplata WHERE data BETWEEN TO_DATE('" . $pocz . "', 'yyyy/mm/dd') AND TO_DATE('" . $koniec . "', 'yyyy/mm/dd') AND id_kredytu = " . $_GET['id'] . " ORDER BY data DESC");

        oci_execute($stmt, OCI_NO_AUTO_COMMIT);
        while (($row = oci_fetch_array($stmt, OCI_BOTH))) {
            $status = null;
            if($row[1] == 0)
                $status = " - niezaksięgowana";
            else
                $status = " - zaksięgowana";
            echo "<BR>Identyfikator wplaty: <A HREF=\"wplata.php?id=" . $row[0] . "\">" . $row[0]  . "</A> Kwota: " . $row[2] . " PLN " . $row[3]  . " " . $status . "<BR>\n";
        }
    }
?>
</BODY>
</HTML>
