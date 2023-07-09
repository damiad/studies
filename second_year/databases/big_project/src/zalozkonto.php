<HTML>
<HEAD>
    <TITLE> Strona dodawania klienta</TITLE>
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
        $id_klienta = $_POST['id_klienta'];
        $haslo = $_POST['haslo'];
        $imie = $_POST['imie'];
        $nazwisko = $_POST['nazwisko'];
        $nr_telefonu = $_POST['nr_telefonu'];

        $stmt = oci_parse($conn, 'BEGIN dd439954.zaloz_konto_klientowi(:input_param1, :input_param2, :input_param3, :input_param4, :input_param5); END;');
        oci_bind_by_name($stmt, ':input_param1', $id_klienta);
        oci_bind_by_name($stmt, ':input_param2', $haslo);
        oci_bind_by_name($stmt, ':input_param3', $imie);
        oci_bind_by_name($stmt, ':input_param4', $nazwisko);
        oci_bind_by_name($stmt, ':input_param5', $nr_telefonu);

        oci_execute($stmt);
        header('Location: ' . $_SERVER['HTTP_REFERER']);
        exit();
    }
?>
    <H2> Utwórz konto klienta </H2>
    <FORM METHOD="POST">  
      Identyfikator klienta: <INPUT TYPE="number" NAME="id_klienta" required="yes" min="1000" max="9999999"><BR><BR>
      Hasło: <INPUT TYPE="TEXT" NAME="haslo" required="yes" VALUE=""><BR><BR>
      Imie: <INPUT TYPE="TEXT" NAME="imie" required="yes" VALUE=""><BR><BR>
      Nazwisko: <INPUT TYPE="TEXT" NAME="nazwisko" required="yes" VALUE=""><BR><BR>
      Numer telefonu: <INPUT TYPE="number" NAME="nr_telefonu" required="yes" min="100000000" max="999999999"><BR><BR>
      <INPUT TYPE="SUBMIT" NAME="button1" VALUE="Utwórz konto">
    </FORM>

</BODY>
</HTML>
