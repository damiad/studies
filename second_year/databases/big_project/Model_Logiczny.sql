-- DROP SEQUENCE UNIQUE_ID;
-- DROP TABLE FakeData;
-- DROP TABLE Pracownik;
-- DROP TABLE Wplata;
-- DROP TABLE Wiadomosc;
-- DROP TABLE Wniosek;
-- DROP TABLE Kredyt;
-- DROP TABLE Klient;
-- DROP TYPE symulacja_table;
-- DROP TYPE symulacja_record;
-- @Model_Logiczny.sql
-- ssh -C -L 1700:labora.mimuw.edu.pl:1521 dd439954@students.mimuw.edu.pl
-- rlwrap -ir sqlplus id/hasło@"(DESCRIPTION=(ADDRESS = (PROTOCOL = TCP)(HOST =localhost)(PORT = 1700))(CONNECT_DATA =(SID=LABS)))"
-- https://students.mimuw.edu.pl/~dd439954/html_Projekt/

-- Sekwencje: --------------------------------------------------------------------------------------------------------------------------------------------
CREATE SEQUENCE unique_id
START WITH 1000000001
INCREMENT BY 1;

-- Tabele: -----------------------------------------------------------------------------------------------------------------------------------------------
CREATE TABLE FakeData( --na potrzeby testow
	id NUMBER(1) PRIMARY KEY,
	data DATE
);

CREATE TABLE Klient(
	id_klienta NUMBER(7) PRIMARY KEY, --login
	haslo VARCHAR(20) NOT NULL,
	imie VARCHAR(15) NOT NULL,
	nazwisko VARCHAR(15) NOT NULL,
	nr_telefonu NUMBER(9) NOT NULL,
	najgorszy_kredyt NUMBER(12,6) NOT NULL --maksymalny stosunek niedoplaty_historyczne_skumulowane / wielkosc_poczatkowa 
);

CREATE TABLE Kredyt( 
	id_kredytu NUMBER(10) PRIMARY KEY,
	czy_zmienne NUMBER(1) NOT NULL CHECK (czy_zmienne BETWEEN 0 AND 1),-- Zamiast rodzaj_oprocentowania zmienne albo stałe
	wartosc_oprocentowania NUMBER(4,2) NOT NULL CHECK (wartosc_oprocentowania > 0),
	wielkosc_poczatkowa NUMBER(12,2) NOT NULL CHECK (wielkosc_poczatkowa > 0),
	wielkosc_pozostala NUMBER(12,2) NOT NULL,
	ilosc_rat_poczatkowa NUMBER(3,0) NOT NULL CHECK (ilosc_rat_poczatkowa BETWEEN 1 AND 120),
	ilosc_rat_pozostala NUMBER(3,0),
	czy_kwartalna NUMBER(1) NOT NULL CHECK (czy_kwartalna BETWEEN 0 AND 1), --zamiast rodzaj raty info czy kwartalna czy miesięczna
	po_racie_pozostanie_do_splaty NUMBER(12,2) NOT NULL, --pomocnicze w obliczeniach, czesto wywolywane
	niedoplaty_historyczne_skumulowane NUMBER(12,2) NOT NULL,
	data_nadchodzacej_raty DATE NOT NULL,
	wielkosc_nadchodzacej_raty NUMBER(12,2) NOT NULL,
	id_klienta NUMBER(7) NOT NULL,
	FOREIGN KEY (id_klienta) REFERENCES Klient(id_klienta)
);

CREATE TABLE Wplata(
	id_wplaty NUMBER(10) PRIMARY KEY,
	czy_zaksiegowana NUMBER(1) NOT NULL CHECK (czy_zaksiegowana BETWEEN 0 AND 1),
	kwota NUMBER(12,2) NOT NULL CHECK (kwota > 0),
	data DATE NOT NULL,
	id_kredytu NUMBER(10) NOT NULL,
	FOREIGN KEY (id_kredytu) REFERENCES Kredyt(id_kredytu)
);

CREATE TABLE Wiadomosc(
	id_wiadomosci NUMBER(10) PRIMARY KEY,
	typ VARCHAR2(20) CHECK(typ IN ('WNIOSEK ROZPATRZONY', 'KREDYT ZAKONCZONY', 'NIEUISZCZONA OPLATA')),
	tresc VARCHAR(240) NOT NULL,
	data DATE NOT NULL,
	id_klienta NUMBER(7) NOT NULL,
	FOREIGN KEY (id_klienta) REFERENCES Klient(id_klienta)
);

CREATE TABLE Wniosek(
	id_wniosku NUMBER(10) PRIMARY KEY,
	czy_zmienne NUMBER(1) NOT NULL CHECK (czy_zmienne BETWEEN 0 AND 1),-- Zamiast rodzaj_oprocentowania zmienne albo stałe
	wartosc_oprocentowania NUMBER(4,2) NOT NULL,
	wielkosc_poczatkowa NUMBER(12,2) NOT NULL,
	dlugosc_trwania NUMBER(2,0) NOT NULL CHECK (dlugosc_trwania BETWEEN 1 AND 40), --liczba od 1-40 w kwartałach
	czy_kwartalna NUMBER(1) NOT NULL CHECK (czy_kwartalna BETWEEN 0 AND 1), --zamiast rodzaj raty info czy kwartalna czy miesięczna
	id_klienta NUMBER(7) NOT NULL,
	FOREIGN KEY (id_klienta) REFERENCES Klient(id_klienta)
);

CREATE TABLE Pracownik(
	id_pracownika NUMBER(7) PRIMARY KEY, --login
	haslo VARCHAR(20) NOT NULL, 
	imie VARCHAR(15) NOT NULL,
	nazwisko VARCHAR(15) NOT NULL,
	czy_obslugi NUMBER(1) NOT NULL CHECK (czy_obslugi BETWEEN 0 AND 1) --albo pracownik dzialu obslugi albo kredytowy (zamiast osobnych klas)
);

-- Przykladowe dane: -------------------------------------------------------------------------------------------------------------------------------------
INSERT INTO FakeData (id, data) VALUES 
	(1,TO_DATE('2023/01/27','yyyy/mm/dd'));

INSERT INTO Pracownik VALUES
	(1000001,'tiger','John','Young',0);
INSERT INTO Pracownik VALUES
	(1000002,'tiger','Eric','Lang',0);
INSERT INTO Pracownik VALUES
	(1000003,'tiger','Tom','Fang',0);
INSERT INTO Pracownik VALUES
    (1000004,'tiger','Kasia','Poung',0);
INSERT INTO Pracownik VALUES
	(2000001,'wolf','Pati','Roung',1);
INSERT INTO Pracownik VALUES
	(2000002,'wolf','Alfred','Soung', 1);
INSERT INTO Pracownik VALUES
	(2000003,'wolf','Sandra','Ding', 1);
INSERT INTO Pracownik VALUES
    (2000004,'lion','Jessica','Doung', 1);

INSERT INTO Klient VALUES
	(3000001,'maslo','Chris','Hemsworth',782315422,0);
INSERT INTO Klient VALUES
	(3000002,'sloma','Scott','Lang',985281672,0.123);
INSERT INTO Klient VALUES
	(3000003,'lpg12','Bruce','Banner',629753981,0.0001);
INSERT INTO Klient VALUES
	(3000004,'bmw11','Jessica','Jones',927321235,3.14);
INSERT INTO Klient VALUES
	(3000005,'gusads','Albert','Einstain',986113765,0.0032);
INSERT INTO Klient VALUES
	(3000006,'qwerty','Nita','Orta',459257937,0.88);
INSERT INTO Klient VALUES
	(3000007,'olokolo','Lady','Gaga',147258369,12.92);
INSERT INTO Klient VALUES
	(3000008,'12345','Bruce','Wayne',709754281,0.00001);
INSERT INTO Klient VALUES
	(3000009,'tajemnica','Scarlett','Johanson',230706861,0);

INSERT INTO Wniosek VALUES 
	(unique_id.nextval,0,2.2,60000,20,0,3000001);
INSERT INTO Wniosek VALUES 
	(unique_id.nextval,0,1.8,160025,30,0,3000001);
INSERT INTO Wniosek VALUES 
	(unique_id.nextval,0,0.2,90400,10,1,3000004);
INSERT INTO Wniosek VALUES 
	(unique_id.nextval,1,1.8,9872,8,1,3000006);
INSERT INTO Wniosek VALUES 
	(unique_id.nextval,1,1.92,120900,18,0,3000008);
INSERT INTO Wniosek VALUES 
	(unique_id.nextval,1,12.25,800000,36,1,3000003);
INSERT INTO Wniosek VALUES 
	(unique_id.nextval,0,2.11,12345,6,1,3000002);
INSERT INTO Wniosek VALUES 
	(unique_id.nextval,1,40,98765,12,0,3000003);

INSERT INTO Kredyt VALUES 
	(1,0,4.5,90000,90000,60,60,0,88500,0,TO_DATE('2023/01/28','yyyy/mm/dd'),1567.5,3000004);
INSERT INTO Kredyt VALUES 
	(2,1,0.25,90000,90000,60,60,0,88500,0,TO_DATE('2023/01/30','yyyy/mm/dd'),1725,3000001);
INSERT INTO Kredyt VALUES 
	(3,0,2.5,180000,180000,30,30,1,174000,0,TO_DATE('2023/01/31','yyyy/mm/dd'),6150,3000003);
INSERT INTO Kredyt VALUES 
	(4,1,0.5,180000,180000,30,30,1,174000,0,TO_DATE('2023/01/29','yyyy/mm/dd'),6900,3000001);

INSERT INTO Wplata VALUES 
	(unique_id.nextval,0,6600,TO_DATE('2023/01/27','yyyy/mm/dd'),4);
INSERT INTO Wplata VALUES 
	(unique_id.nextval,0,1725,TO_DATE('2023/01/28','yyyy/mm/dd'),2);
INSERT INTO Wplata VALUES 
	(unique_id.nextval,0,1567.5,TO_DATE('2023/01/29','yyyy/mm/dd'),1);

INSERT INTO Wiadomosc VALUES 
	(unique_id.nextval,'WNIOSEK ROZPATRZONY','Szanowny Kliencie, Twoj wniosek o kredyt, numer: 999 zostal rozpatrzony pozytywnie.',TO_DATE('2023/01/01','yyyy/mm/dd'),3000001);

COMMIT;

-- Funkcje i procedury: ----------------------------------------------------------------------------------------------------------------------------------

-- SELECT * FROM TABLE(symulacja_splaty(3)); --wywołanie

CREATE OR REPLACE TYPE symulacja_record AS OBJECT (
	data_raty DATE,
	kwota_raty NUMBER(12,2),
	pozostanie_do_splaty NUMBER(12,2)
);
/

CREATE OR REPLACE TYPE symulacja_table AS TABLE OF symulacja_record;
/

CREATE OR REPLACE FUNCTION symulacja_splaty (id_kredyt NUMBER)
RETURN symulacja_table AS
	symulacja_return symulacja_table;
	i INT := 1;
	diff_months NUMBER(1,0) := 1;
	fetch_kredyt Kredyt%ROWTYPE;
	rata NUMBER(12,2);
BEGIN
	symulacja_return := symulacja_table();
	SELECT * INTO fetch_kredyt FROM Kredyt WHERE Kredyt.id_kredytu = id_kredyt;
	IF (fetch_kredyt.czy_kwartalna = 1) THEN
		diff_months := 3;
	END IF;
	symulacja_return.extend; symulacja_return(symulacja_return.count) := symulacja_record(fetch_kredyt.data_nadchodzacej_raty, fetch_kredyt.wielkosc_nadchodzacej_raty, fetch_kredyt.po_racie_pozostanie_do_splaty);
	--
	IF (fetch_kredyt.czy_zmienne = 1) THEN --obsłóż zmienne raty kredytu
		WHILE fetch_kredyt.ilosc_rat_pozostala > 0 LOOP
			fetch_kredyt.data_nadchodzacej_raty := ADD_MONTHS(fetch_kredyt.data_nadchodzacej_raty, diff_months);
			rata := fetch_kredyt.po_racie_pozostanie_do_splaty/ fetch_kredyt.ilosc_rat_pozostala;
			fetch_kredyt.po_racie_pozostanie_do_splaty := fetch_kredyt.po_racie_pozostanie_do_splaty - rata;
			symulacja_return.extend; symulacja_return(symulacja_return.count) := symulacja_record(fetch_kredyt.data_nadchodzacej_raty, rata + (fetch_kredyt.po_racie_pozostanie_do_splaty + rata) * fetch_kredyt.wartosc_oprocentowania/100, fetch_kredyt.po_racie_pozostanie_do_splaty);
			fetch_kredyt.ilosc_rat_pozostala := fetch_kredyt.ilosc_rat_pozostala - 1;
		END LOOP;
	ELSE --obsłuż stałe raty kredytu
		rata := (fetch_kredyt.wielkosc_poczatkowa / fetch_kredyt.ilosc_rat_poczatkowa)*(1+ fetch_kredyt.wartosc_oprocentowania/100);
		WHILE i < fetch_kredyt.ilosc_rat_pozostala LOOP
			fetch_kredyt.data_nadchodzacej_raty := ADD_MONTHS(fetch_kredyt.data_nadchodzacej_raty, diff_months);
			fetch_kredyt.po_racie_pozostanie_do_splaty := fetch_kredyt.po_racie_pozostanie_do_splaty - (fetch_kredyt.wielkosc_poczatkowa / fetch_kredyt.ilosc_rat_poczatkowa);
			symulacja_return.extend; symulacja_return(symulacja_return.count) := symulacja_record(fetch_kredyt.data_nadchodzacej_raty, rata, fetch_kredyt.po_racie_pozostanie_do_splaty);
			i:=i+1;
		END LOOP;
	END IF;
	RETURN symulacja_return;
END symulacja_splaty;
/


CREATE OR REPLACE PROCEDURE
wiadomosc_koniec_kredytu(id_kredyt NUMBER, id_klient NUMBER) IS
BEGIN
	INSERT INTO Wiadomosc VALUES(
		unique_id.nextval,
		'KREDYT ZAKONCZONY',
		'Gratulacje, pomyslnie udalo Ci sie splacic kredyt numer: ' || id_kredyt || '.',
		(SELECT data FROM FakeData),
		id_klient
	);
	COMMIT; 
END;
/

CREATE OR REPLACE PROCEDURE
zakoncz_kredyt(id_kredyt NUMBER) IS
	fetch_kredyt Kredyt%ROWTYPE;
	ratio NUMBER(12,6);
	kredyt_najgorszy NUMBER(12,6);
BEGIN
	SELECT * INTO fetch_kredyt FROM Kredyt WHERE Kredyt.id_kredytu = id_kredyt;
	ratio := fetch_kredyt.niedoplaty_historyczne_skumulowane/fetch_kredyt.wielkosc_poczatkowa;
	SELECT najgorszy_kredyt INTO kredyt_najgorszy FROM Klient WHERE Klient.id_klienta = fetch_kredyt.id_klienta;
	IF (ratio > kredyt_najgorszy) THEN
		UPDATE Klient SET najgorszy_kredyt  = ratio WHERE Klient.id_klienta = fetch_kredyt.id_klienta;
	END IF;
	wiadomosc_koniec_kredytu(id_kredyt, fetch_kredyt.id_klienta);
	DELETE FROM Wplata WHERE Wplata.id_kredytu = id_kredyt;
	DELETE FROM Kredyt WHERE Kredyt.id_kredytu = id_kredyt; 
	COMMIT;
END;
/

CREATE OR REPLACE PROCEDURE
wnioskuj_o_kredyt(czy_zmienne NUMBER, wartosc_oprocentowania NUMBER, wielkosc_poczatkowa NUMBER, 
	dlugosc_trwania NUMBER, czy_kwartalna NUMBER, id_klienta NUMBER) IS
BEGIN --ewentualnie jakieś check tutaj
	INSERT INTO Wniosek VALUES(
		unique_id.nextval,
		czy_zmienne,
		wartosc_oprocentowania,
		wielkosc_poczatkowa,
		dlugosc_trwania,
		czy_kwartalna,
		id_klienta
	);
	COMMIT;
END;
/

CREATE OR REPLACE PROCEDURE
wiadomosc_wniosek_rozpatrzony(id_wniosek NUMBER, id_klient NUMBER, decyzja VARCHAR) IS
BEGIN
	INSERT INTO Wiadomosc VALUES(
		unique_id.nextval,
		'WNIOSEK ROZPATRZONY',
		'Szanowny Kliencie, Twoj wniosek o kredyt, numer: ' || id_wniosek || ' zostal rozpatrzony ' || decyzja,
		(SELECT data FROM FakeData),
		id_klient
	);
	COMMIT;
END;
/


CREATE OR REPLACE PROCEDURE
zatwierdz_wniosek(id_wniosek NUMBER) IS
	fetch_wniosek Wniosek%ROWTYPE;
	diff_months NUMBER(1,0) := 1;
BEGIN
	SELECT * INTO fetch_wniosek FROM Wniosek WHERE Wniosek.id_wniosku = id_wniosek;
	IF (fetch_wniosek.czy_kwartalna = 0) THEN
		fetch_wniosek.dlugosc_trwania := fetch_wniosek.dlugosc_trwania * 3; 
	END IF;
	IF(fetch_wniosek.czy_kwartalna = 1) THEN diff_months := 3;
	END IF;
	INSERT INTO Kredyt VALUES(
		unique_id.nextval,
		fetch_wniosek.czy_zmienne,
		fetch_wniosek.wartosc_oprocentowania,
		fetch_wniosek.wielkosc_poczatkowa,
		fetch_wniosek.wielkosc_poczatkowa, --wielkość pozostała
		fetch_wniosek.dlugosc_trwania, --ilosc_rat_poczatkowa
		fetch_wniosek.dlugosc_trwania, --ilosc_rat_pozostala
		fetch_wniosek.czy_kwartalna,
		fetch_wniosek.wielkosc_poczatkowa - (fetch_wniosek.wielkosc_poczatkowa / fetch_wniosek.dlugosc_trwania), --po_racie_pozostanie_do_splaty
		0, --niedoplaty_historyczne_skumulowane
		ADD_MONTHS((SELECT data FROM FakeData),1),
		(fetch_wniosek.wielkosc_poczatkowa / fetch_wniosek.dlugosc_trwania) * (1 + fetch_wniosek.wartosc_oprocentowania/100), -- wielkosc_nadchodzacej_raty
		fetch_wniosek.id_klienta
	);
	wiadomosc_wniosek_rozpatrzony(id_wniosek, fetch_wniosek.id_klienta, 'pozytywnie.');
	DELETE FROM Wniosek WHERE Wniosek.id_wniosku = id_wniosek; 
	COMMIT;
END;
/

CREATE OR REPLACE PROCEDURE
odrzuc_wniosek(id_wniosek NUMBER) IS
	klient_id NUMBER;
BEGIN
	SELECT id_klienta INTO klient_id FROM Wniosek WHERE Wniosek.id_wniosku = id_wniosek;
	wiadomosc_wniosek_rozpatrzony(id_wniosek, klient_id, 'negatywnie.');
	DELETE FROM Wniosek WHERE id_wniosku = id_wniosek; 
	COMMIT;
END;
/

CREATE OR REPLACE PROCEDURE
wiadomosc_nieuiszcona_oplata(id_kredyt NUMBER, id_klient NUMBER, niedoplata NUMBER) IS
	f_niedoplata NUMBER(12,2);
BEGIN
	f_niedoplata := niedoplata * 1.03;
	INSERT INTO Wiadomosc VALUES(
		unique_id.nextval,
		'NIEUISZCZONA OPLATA',
		'Szanowny Kliencie, prosimy o jak najszybsze uregulowanie niedoplat i zrobienie przelewu w wysokosci: ' ||
			f_niedoplata || ' na rzecz kredytu o numerze: ' || id_kredyt || '.',
		(SELECT data FROM FakeData),
		id_klient
	);
	UPDATE Kredyt SET wielkosc_nadchodzacej_raty  = f_niedoplata  WHERE id_kredytu = id_kredyt;
	COMMIT;
END;
/

CREATE OR REPLACE PROCEDURE
wprowadz_wplate(id_wplata NUMBER) IS
	fetch_wplata Wplata%ROWTYPE;
	fetch_kredyt Kredyt%ROWTYPE;
	diff_months NUMBER(1,0) := 1;
	rata NUMBER(12,2);
	przesuniecie NUMBER(3,0);
	nadplata NUMBER(12,2);
	niedoplata NUMBER(12,2); 
BEGIN --aktualizuje fetch_kredyt, na koniec zapisuje go w normalnym kredycie (szybciej!)
	UPDATE Wplata SET czy_zaksiegowana = 1 WHERE Wplata.id_wplaty = id_wplata;
	SELECT * INTO fetch_wplata FROM Wplata WHERE Wplata.id_wplaty = id_wplata;
	SELECT * INTO fetch_kredyt FROM Kredyt WHERE Kredyt.id_kredytu = fetch_wplata.id_kredytu;
	IF (fetch_kredyt.czy_kwartalna = 1) THEN
		diff_months := 3;
	END IF;
	IF (fetch_wplata.kwota >= fetch_kredyt.wielkosc_nadchodzacej_raty) THEN
		fetch_wplata.kwota := fetch_wplata.kwota - fetch_kredyt.wielkosc_nadchodzacej_raty;
		IF(fetch_kredyt.czy_zmienne = 1) THEN
			fetch_kredyt.ilosc_rat_pozostala := fetch_kredyt.ilosc_rat_pozostala - 1;
			fetch_kredyt.wielkosc_pozostala := fetch_kredyt.po_racie_pozostanie_do_splaty - fetch_wplata.kwota; --jak ujemne lub 0 to kredyt zakonczony
			IF (fetch_kredyt.wielkosc_pozostala <= 0) THEN
				zakoncz_kredyt(fetch_kredyt.id_kredytu);
			ELSE 
				fetch_kredyt.po_racie_pozostanie_do_splaty := fetch_kredyt.wielkosc_pozostala - (fetch_kredyt.wielkosc_pozostala/fetch_kredyt.ilosc_rat_pozostala);
				fetch_kredyt.data_nadchodzacej_raty := ADD_MONTHS(fetch_kredyt.data_nadchodzacej_raty, diff_months);
				fetch_kredyt.wielkosc_nadchodzacej_raty := (fetch_kredyt.wielkosc_pozostala/fetch_kredyt.ilosc_rat_pozostala) + (fetch_kredyt.wielkosc_pozostala * fetch_kredyt.wartosc_oprocentowania/100);
			END IF;
		ELSE  --raty stałe
			rata := (fetch_kredyt.wielkosc_poczatkowa / fetch_kredyt.ilosc_rat_poczatkowa)*(1+ fetch_kredyt.wartosc_oprocentowania/100);
			przesuniecie := FLOOR (fetch_wplata.kwota / rata);
			nadplata := fetch_wplata.kwota - rata * przesuniecie;
			fetch_kredyt.wielkosc_pozostala := fetch_kredyt.po_racie_pozostanie_do_splaty - (fetch_kredyt.wielkosc_poczatkowa / fetch_kredyt.ilosc_rat_poczatkowa) * przesuniecie - nadplata;
			IF(fetch_kredyt.wielkosc_pozostala <=0) THEN
				zakoncz_kredyt(fetch_kredyt.id_kredytu);
			ELSE
				fetch_kredyt.ilosc_rat_pozostala := fetch_kredyt.ilosc_rat_pozostala - przesuniecie - 1;
				fetch_kredyt.po_racie_pozostanie_do_splaty := fetch_kredyt.wielkosc_poczatkowa / fetch_kredyt.ilosc_rat_poczatkowa * (fetch_kredyt.ilosc_rat_pozostala - 1);
				fetch_kredyt.data_nadchodzacej_raty := ADD_MONTHS(fetch_kredyt.data_nadchodzacej_raty, diff_months); 
				fetch_kredyt.wielkosc_nadchodzacej_raty := rata - nadplata; 
			END IF;
		END IF;
		IF(fetch_kredyt.wielkosc_pozostala>0) THEN
			UPDATE Kredyt SET
				ilosc_rat_pozostala = fetch_kredyt.ilosc_rat_pozostala,
				wielkosc_pozostala = fetch_kredyt.wielkosc_pozostala,
				po_racie_pozostanie_do_splaty = fetch_kredyt.po_racie_pozostanie_do_splaty,
				data_nadchodzacej_raty = fetch_kredyt.data_nadchodzacej_raty,
				wielkosc_nadchodzacej_raty = fetch_kredyt.wielkosc_nadchodzacej_raty
			WHERE Kredyt.id_kredytu = fetch_kredyt.id_kredytu; 
		END IF;
	ELSE -- fetch_wplata.kwota < fetch_kredyt.wielkosc_nadchodzacej_raty
		niedoplata := fetch_kredyt.wielkosc_nadchodzacej_raty - fetch_wplata.kwota;
		fetch_kredyt.wielkosc_pozostala := fetch_kredyt.wielkosc_pozostala - (fetch_kredyt.wielkosc_pozostala - fetch_kredyt.po_racie_pozostanie_do_splaty) * ( (fetch_kredyt.wielkosc_nadchodzacej_raty - niedoplata) / fetch_kredyt.wielkosc_nadchodzacej_raty);
		fetch_kredyt.niedoplaty_historyczne_skumulowane := fetch_kredyt.niedoplaty_historyczne_skumulowane + niedoplata; 
		fetch_kredyt.wielkosc_nadchodzacej_raty := niedoplata;
		UPDATE Kredyt SET
			wielkosc_pozostala = fetch_kredyt.wielkosc_pozostala,
			niedoplaty_historyczne_skumulowane = fetch_kredyt.niedoplaty_historyczne_skumulowane,
			wielkosc_nadchodzacej_raty = fetch_kredyt.wielkosc_nadchodzacej_raty
		WHERE Kredyt.id_kredytu = fetch_kredyt.id_kredytu;
		wiadomosc_nieuiszcona_oplata(fetch_kredyt.id_kredytu, fetch_kredyt.id_klienta, niedoplata); 
	END IF;
	COMMIT;
END;
/

CREATE OR REPLACE PROCEDURE
wezwij_do_splaty(id_kredyt NUMBER) IS
	fetch_kredyt Kredyt%ROWTYPE;
BEGIN
	SELECT * INTO fetch_kredyt FROM Kredyt WHERE Kredyt.id_kredytu = id_kredyt;
	wiadomosc_nieuiszcona_oplata(id_kredyt, fetch_kredyt.id_klienta, fetch_kredyt.wielkosc_nadchodzacej_raty);
END;
/

CREATE OR REPLACE PROCEDURE
dokonaj_wplaty(id_kredyt NUMBER, kwota NUMBER) IS
BEGIN
	INSERT INTO Wplata VALUES(
		unique_id.nextval,
		0, --czy_zaksiegowana
		kwota,
		(SELECT data FROM FakeData),
		id_kredyt
	);
	COMMIT;
END;
/


CREATE OR REPLACE PROCEDURE
zaloz_konto_klientowi(id_klienta NUMBER, haslo VARCHAR, imie VARCHAR, nazwisko VARCHAR, nr_telefonu NUMBER) IS
BEGIN
	INSERT INTO Klient VALUES(
		id_klienta,
		haslo,
		imie,
		nazwisko,
		nr_telefonu,
		0 -- najgorszy_kredyt
	);
	COMMIT;
END;
/


CREATE OR REPLACE PROCEDURE
login_pracownik(id_pracownik IN VARCHAR, haslo IN VARCHAR, sukces OUT NUMBER) IS
	fetch_haslo VARCHAR(20);
	pracownik_nr NUMBER;
	ile NUMBER;
BEGIN
	pracownik_nr:= TO_NUMBER(id_pracownik);
	SELECT COUNT(*) INTO ile FROM Pracownik WHERE Pracownik.id_pracownika = pracownik_nr;
	IF ile > 0 THEN
		SELECT haslo INTO fetch_haslo FROM Pracownik WHERE Pracownik.id_pracownika = pracownik_nr;
		IF (fetch_haslo = haslo) THEN
			sukces:=1;
		ELSE
			sukces:=0;
		END IF;
	ELSE
		sukces:=0;
	END IF;
END;
/

CREATE OR REPLACE PROCEDURE
login_klient(id_klient IN VARCHAR, haslo IN VARCHAR, sukces OUT NUMBER) IS
	fetch_haslo VARCHAR(20);
	klient_nr NUMBER;
	ile NUMBER;
BEGIN
	klient_nr:= TO_NUMBER(id_klient);
	SELECT COUNT(*) INTO ile FROM Klient WHERE Klient.id_klienta = klient_nr;
	IF ile > 0 THEN
		SELECT haslo INTO fetch_haslo FROM Klient WHERE Klient.id_klienta = klient_nr;
		IF (fetch_haslo = haslo) THEN
			sukces:=1;
		ELSE
			sukces:=0;
		END IF;
	ELSE
		sukces:=0;
	END IF;
END;
/

CREATE OR REPLACE PROCEDURE
zmien_date IS
BEGIN
	UPDATE FakeData SET data = ADD_MONTHS(data,1);
	COMMIT;
END;
/

CREATE OR REPLACE PROCEDURE -- Dodana funkcjonalność
zmien_rate(id_kredyt NUMBER, nowe_oprocentowanie NUMBER) IS
BEGIN
	UPDATE Kredyt SET wartosc_oprocentowania = nowe_oprocentowanie WHERE id_kredytu = id_kredyt;
	COMMIT;
END;
/