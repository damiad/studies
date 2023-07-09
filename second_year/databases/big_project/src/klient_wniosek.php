<HTML>
<HEAD>
    <TITLE> Strona Składania Wniosku</TITLE>
</HEAD>
<BODY>
<?PHP 
    session_start(); //Już zalogowani
    $conn = oci_connect($_SESSION['LOGIN'],$_SESSION['PASS'],"//labora.mimuw.edu.pl/LABS");
    
    if (!$conn) {
	echo "oci_connect failed";
	$e = oci_error();
	echo $e['message'];
    }


    if(isset($_POST['button1'])) {
        $id_klienta = $_SESSION['LOGINK'];
        $czy_zmienne = $_POST['czy_zmienne'];
        $oprocentowanie = $_POST['oprocentowanie'];
        $wielkosc = $_POST['wielkosc'];
        $dlugosc = $_POST['dlugosc'];
        $czy_kwartalne = $_POST['czy_kwartalne'];
        if($czy_kwartalne == 1){
            if($dlugosc < 4){
                header('Location: ' . $_SERVER['HTTP_REFERER']);
                exit();
            }
        }
        $stmt = oci_parse($conn, 'BEGIN dd439954.wnioskuj_o_kredyt(:input_param1, :input_param2, :input_param3, :input_param4, :input_param5, :input_param6); END;');
        oci_bind_by_name($stmt, ':input_param1', $czy_zmienne);
        oci_bind_by_name($stmt, ':input_param2', $oprocentowanie);
        oci_bind_by_name($stmt, ':input_param3', $wielkosc);
        oci_bind_by_name($stmt, ':input_param4', $dlugosc);
        oci_bind_by_name($stmt, ':input_param5', $czy_kwartalne);
        oci_bind_by_name($stmt, ':input_param6', $id_klienta);

        oci_execute($stmt);
        header('Location: ' . $_SERVER['HTTP_REFERER']);
        exit();
    }
?>
    <H2> Wniosek o Kredyt </H2>
    <FORM METHOD="POST">
        Rodzaj oprocentowania:  
        <select name="czy_zmienne" required="yes">
            <option value="0">Stałe</option>
            <option value="1">Zmienne</option>
        </select> <BR><BR>
        Wartość oprocentowania: <INPUT TYPE="number" NAME="oprocentowanie" required="yes" min="0.01" max="99.99" step="0.01"><BR><BR>
        Wielkość początkowa: <INPUT TYPE="number" NAME="wielkosc" required="yes" min="1000" max="9999999999"><BR><BR>
        Długość trwania w kwartałach: <INPUT TYPE="number" NAME="dlugosc" required="yes" min="1" max="40"><BR><BR>
        Rodzaj rat:  
        <select name="czy_kwartalne" required="yes">
            <option value="0">miesięczne</option>
            <option value="1">kwartalne</option>
        </select> <BR><BR>
      <INPUT TYPE="SUBMIT" NAME="button1" VALUE="Złóż wniosek">
    </FORM>
</BODY>
</HTML>
