@Echo off
(
    echo -s

    echo -nc 0xa
    darkbox -? | darkbox_t -e

    echo -nc 0xb
    darkbox_i -? | darkbox_t -e
	
    echo -nc 0xc
    call darkbox_s.bat | darkbox_t -e

    echo -nc 0xd
    darkbox_t | darkbox_t -e

    echo -rq
) | darkbox
pause>nul
