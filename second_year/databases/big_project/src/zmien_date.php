<HTML>
<HEAD>
    <TITLE> Zmiana Daty </TITLE>
</HEAD>
<BODY>
<h2> Zmień date na potrzeby testów</h2>
<?PHP 
    session_start(); //Już zalogowani
    $conn = oci_connect($_SESSION['LOGIN'],$_SESSION['PASS'],"//labora.mimuw.edu.pl/LABS");
    
    if (!$conn) {
	echo "oci_connect failed";
	$e = oci_error();
	echo $e['message'];
    }

    if(isset($_POST['button1'])) {
        $stmt = oci_parse($conn, 'BEGIN dd439954.zmien_date; END;');
        oci_execute($stmt);
        echo "<br> Data zmieniona pomyślnie<br>\n";
    }

?>
    <form method="post">
        <input type="submit" name="button1"
                value="Dodaj miesiąc"/>
    </form>
</BODY>
</HTML>
