@Echo off
(
    echo -s

    echo -nc 0xa
    darkbox -? | darkbox -e

    echo -nc 0xb
    darkbox_i -? | darkbox -e

    echo -nc 0xc
    ptnio -? | darkbox -e

    echo -rq
) | darkbox
pause>nul
