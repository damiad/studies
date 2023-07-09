<HTML>
<HEAD>
    <TITLE> Strona Wniosku o Kredyt </TITLE>
</HEAD>
<BODY>
<h2> Rozpatrz wniosek o kredyt </h2>
<?PHP 
    session_start(); //Już zalogowani
    $conn = oci_connect($_SESSION['LOGIN'],$_SESSION['PASS'],"//labora.mimuw.edu.pl/LABS");
    
    if (!$conn) {
	echo "oci_connect failed";
	$e = oci_error();
	echo $e['message'];
    }

    $stmt = oci_parse($conn, "SELECT * FROM dd439954.Wniosek WHERE id_wniosku = " . $_GET['id']);
    oci_execute($stmt, OCI_NO_AUTO_COMMIT);
    $row = oci_fetch_row($stmt);

    echo "<BR>Wniosek nr: $row[0]<BR>\n";
    if($row[1] == 0)
        echo "<BR>Oprocentowanie stałe ";
    else
        echo "<BR>Oprocentowanie zmienne ";
    echo "o wartości: $row[2] %<BR>\n";
    echo "<BR>Wysokość kreydtu: $row[3] PLN<BR>\n";
    $row[4]*=3;
    echo "<BR>Czas trwania w miesiącach: $row[4]<BR>\n";
    if($row[5] == 0)
        echo "<BR>Raty miesięczne<BR>\n";
    else
        echo "<BR>Raty kwartalne<BR>\n";
    echo "<BR><A HREF=\"klient.php?id=" . $row[6] . "\">" . "Identyfikator Klienta: ". $row[6]."</A><BR>\n";

    if(isset($_POST['button1'])) {
        $stmt2 = oci_parse($conn, 'BEGIN zatwierdz_wniosek(:input_param1); END;');
        $input_param1 = $_GET['id'];
        oci_bind_by_name($stmt2, ':input_param1', $input_param1);
        oci_execute($stmt2);
        header("Location: wnioski.php");
        exit();
    }
    if(isset($_POST['button2'])) {
        $stmt2 = oci_parse($conn, 'BEGIN odrzuc_wniosek(:input_param1); END;');
        $input_param1 = $_GET['id'];
        oci_bind_by_name($stmt2, ':input_param1', $input_param1);
        oci_execute($stmt2);
        header("Location: wnioski.php");
        exit();
    }
?>
     
    <form method="post">
        <input type="submit" name="button1"
                value="AKCEPTUJ"/>
         
        <input type="submit" name="button2"
                value="ODRZUĆ"/>
    </form>
</BODY>
</HTML>
