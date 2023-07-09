<HTML>
<HEAD>
    <TITLE> Strona Kredytu</TITLE>
</HEAD>
<BODY>
<h2> Dane Kredytu </h2>
<?PHP 
    session_start(); //Już zalogowani
    $conn = oci_connect($_SESSION['LOGIN'],$_SESSION['PASS'],"//labora.mimuw.edu.pl/LABS");
    
    if (!$conn) {
	echo "oci_connect failed";
	$e = oci_error();
	echo $e['message'];
    }

    $stmt = oci_parse($conn, "SELECT * FROM dd439954.Kredyt WHERE id_kredytu = " . $_GET['id']);
    oci_execute($stmt, OCI_NO_AUTO_COMMIT);
    $row = oci_fetch_row($stmt);

    echo "<BR>Identyfikator kredytu: $row[0]<BR>\n";
    if($row[1] == 0)
        echo "<BR>Oprocentowanie stałe ";
    else
        echo "<BR>Oprocentowanie zmienne ";
    echo "o wartości: $row[2] %<BR>\n";
    echo "<BR>Wysokość początkowa kredytu: $row[3] PLN<BR>\n";
    echo "<BR>Wysokość pozostała kredytu: $row[4] PLN<BR>\n";

    $rzeczywista_kwota = round($row[11] + $row[8]*(1+$row[2]/100),2);
    echo "<BR>Rzeczywista kwota pozostała do całkowitej spłaty kredytu: $rzeczywista_kwota PLN<BR>\n";

    echo "<BR>Ilość rat początkowa: $row[5]<BR>\n";
    echo "<BR>Ilość rat pozostała: $row[6]<BR>\n";
    if($row[7] == 0)
        echo "<BR>Raty miesięczne<BR>\n";
    else
        echo "<BR>Raty kwartalne<BR>\n";
    echo "<BR>Po najbliższej racie pozostanie do spłaty: $row[8] PLN<BR>\n";
    echo "<BR>Niedopłaty historyczne skumulowane: $row[9] PLN<BR>\n";
    echo "<BR>Data nadchodzacej raty: $row[10]<BR>\n";
    echo "<BR>Wielkość nadchodzącej raty: $row[11] PLN<BR>\n";

    if(isset($_POST['button1'])) {
        $stmt4 = oci_parse($conn,'BEGIN dd439954.wezwij_do_splaty(:input_param1); END;');
        $input_param1 = $_GET['id'];
        oci_bind_by_name($stmt4, ':input_param1', $input_param1);
        oci_execute($stmt4);
    }

    if(isset($_POST['button2'])) { // Dodana funkcjonalność
        $stmt5 = oci_parse($conn,'BEGIN dd439954.zmien_rate(:input_param2, :input_param3); END;');
        $input_param2 = $row[0];
        oci_bind_by_name($stmt5, ':input_param2', $input_param2);
        $input_param3 = $_POST['oprocentowanie'];
        oci_bind_by_name($stmt5, ':input_param3', $input_param3);
        oci_execute($stmt5);
        header('Location: ' . $_SERVER['HTTP_REFERER']);
    }

    if(isset($_SESSION['LOGINP'])){
        echo "<BR><A HREF=\"klient.php?id=" . $row[12] . "\">" . "Identyfikator Klienta: ". $row[12]."</A><BR>\n";    

// Dodana funkcjonalność
?> 
            <form method="post"> 
                </select> <BR><BR>
                    Nowa wartość oprocentowania: <INPUT TYPE="number" NAME="oprocentowanie" required="yes" min="0.01" max="99.99" step="0.01">
                <input type="submit" name="button2" value="ZMIEŃ WARTOŚĆ OPROCENTOWANIA">
            </form>

<?PHP

        $stmt2 = oci_parse($conn, "SELECT data FROM dd439954.FakeData");
        oci_execute($stmt2, OCI_NO_AUTO_COMMIT);
        $row2 = oci_fetch_row($stmt2);

        if(strtotime($row[10]) < strtotime($row2[0])){
?>
            <form method="post">
                <input type="submit" name="button1"
                value="WEZWIJ DO SPŁATY"/>
            </form>
<?PHP
        }
    }
    echo "<BR><A HREF=\"wplaty_powiazane.php?id=" . $row[0] . "\">" . "Zobacz wpłaty powiązane ". "</A><BR>\n";

    if(isset($_SESSION['LOGINK']))
        echo "<BR><A HREF=\"symulacja_splaty.php?id=" . $row[0] . "\">" . "Zobacz symulację spłaty"."</A><BR>\n";

?>
</BODY>
</HTML>
