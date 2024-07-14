verify: verify.sav
        spld --static --exechome=/opt/sicstus/sicstus4.8.0/bin/ verify.sav -o verify
verify.sav: verify.pl
        echo "compile('verify.pl'). save_program('verify.sav')." | sicstus
