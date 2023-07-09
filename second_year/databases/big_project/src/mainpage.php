<HTML>
<HEAD>
    <TITLE> Bank - strona główna </TITLE>
    <META HTTP-EQUIV="content-type" CONTENT="text/html" CHARSET="utf-8">
</HEAD>
<BODY>
<?PHP 
    session_start();
    $_SESSION['LOGIN'] = $_POST['LOGN'];
    $_SESSION['PASS'] = $_POST['PASW'];
    $conn = oci_connect($_SESSION['LOGIN'],$_SESSION['PASS'],"//labora.mimuw.edu.pl/LABS");

    if (!$conn) {
	echo "oci_connect failed\n";
	$e = oci_error();
	echo $e['message'];
    } else{
?>
    <H2> Logowanie Pracownika </H2>
    <FORM ACTION="pracownik_login.php" METHOD="POST">  
      Użytkownik: <INPUT TYPE="TEXT" NAME="LOGNP" required="yes" VALUE=""><BR><BR>
      Hasło: <INPUT TYPE="PASSWORD" NAME="PASWP" VALUE=""><BR><BR>
      <INPUT TYPE="SUBMIT" VALUE="Zaloguj">
    </FORM>

    <H2> Logowanie Klienta </H2>
    <FORM ACTION="klient_login.php" METHOD="POST">  
      Użytkownik: <INPUT TYPE="TEXT" NAME="LOGNK" required="yes" VALUE=""><BR><BR>
      Hasło: <INPUT TYPE="PASSWORD" NAME="PASWK" required="yes" VALUE=""><BR><BR>
      <INPUT TYPE="SUBMIT" VALUE="Zaloguj">
    </FORM>

    <BR><BR>
    <BR><BR>
    <BR><BR>
    <BR><BR>
    <BR><BR>
    <BR><BR>
    <BR><BR>
    <BR><BR>
    <BR><BR>
    <BR><BR>
    <BR><BR>
    <BR><BR>
    <BR><BR>
    <h10>
        <FORM ACTION="zmien_date.php" METHOD="POST">  
        <INPUT TYPE="SUBMIT" VALUE="Zmien Date">
        </FORM>
    </h10>
<?PHP
    }
?>
</BODY>
</HTML>
