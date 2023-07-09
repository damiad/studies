<HTML>
<HEAD>
    <TITLE> Strona Wpłaty</TITLE>
</HEAD>
<BODY>
<h2> Dane Wpłaty </h2>
<?PHP 
    session_start(); //Już zalogowani
    $conn = oci_connect($_SESSION['LOGIN'],$_SESSION['PASS'],"//labora.mimuw.edu.pl/LABS");
    
    if (!$conn) {
    echo "oci_connect failed";
    $e = oci_error();
    echo $e['message'];
    }

    $stmt = oci_parse($conn, "SELECT * FROM dd439954.Wplata WHERE id_wplaty = " . $_GET['id']);
    oci_execute($stmt, OCI_NO_AUTO_COMMIT);
    $row = oci_fetch_row($stmt);

    echo "<BR>Identyfikator wplaty: $row[0]<BR>\n";
    if($row[1] == 0)
        echo "<BR>Status: niezaksięgowana<BR>\n";
    else
        echo "<BR>Status: zaksięgowana<BR>\n";
    echo "<BR>Wysokość wpłaty: $row[2] PLN<BR>\n";
    echo "<BR>Data wpłaty: $row[3]<BR>\n";
    echo "<BR><A HREF=\"kredyt.php?id=" . $row[4] . "\">" . "Identyfikator kredytu ". $row[4]."</A><BR>\n";

    if(isset($_POST['button1'])) {
        $stmt3 = oci_parse($conn, 'BEGIN dd439954.wprowadz_wplate(:input_param3); END;');
        $input_param3 = $_GET['id'];
        oci_bind_by_name($stmt3, ':input_param3', $input_param3);
        oci_execute($stmt3);
        header('Location: ' . $_SERVER['HTTP_REFERER']);
        exit();
    }

    if($row[1] == 0){
        if(isset($_SESSION['LOGINP'])){  // tylko pracownik obsługi ma możliwość zaksięgowania
            $input_param1 = $_SESSION['LOGINP'];
            $stmt2 = oci_parse($conn, "SELECT czy_obslugi FROM dd439954.Pracownik WHERE id_pracownika= " . $input_param1);
            oci_execute($stmt2, OCI_NO_AUTO_COMMIT);
            $rowq = oci_fetch_row($stmt2);
            if($rowq[0] == 1){ // Dzial obsługi
                ?>
                <form method="post">
                    <input type="submit" name="button1"
                    value="WPROWADŹ WPŁATĘ"/>
                </form>
                <?PHP 
            }
        }
    }     
?>
</BODY>
</HTML>
