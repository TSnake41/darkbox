; TCS Output - A tiny graphical server-based command for Windows batch.
; Copyright (c) 2017-2018 Teddy ASTIE (TSnake41)
;
; Permission to use, copy, modify, and/or distribute this software for any
; purpose with or without fee is hereby granted, provided that the above
; copyright notice and this permission notice appear in all copies.
;
; THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
; WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
; MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
; ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
; WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
; ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
; OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

format PE CONSOLE 4.0

include 'win32a.inc'

struc db [data]
{
  common
  . db data
  .size = $ - .
}

struct COORD
  X dw ?
  Y dw ?
ends

NULL = 0
EOF = -1

STR_BUF_SIZE = 1024

; Entry point
; Get hOut
invoke GetStdHandle, STD_OUTPUT_HANDLE
mov [hOut],eax

; Allocate str_buf on stack
sub esp, STR_BUF_SIZE
mov [str_buf], esp

loop_:
call near read_char_

cmp eax,'-'
  jne near loop_

; Get command char
call near read_char_

cmp eax, 'a'
  je near ascii_
cmp eax, 'c'
  je near color_
cmp eax, 'd'
  je near disp_
cmp eax, 'n'
  je near new_line_
cmp eax, 's'
  je near clear_
cmp eax, 'g'
  je near goto_
cmp eax, 'o'
  je near origin_
cmp eax, 'w'
  je near wait_
cmp eax, 'q'
  je near quit_
jmp loop_

ascii_:
  call near read_number_
  invoke putchar,eax
  jmp near loop_

color_:
  call near read_number_
  invoke SetConsoleTextAttribute,[hOut],eax
  jmp near loop_

disp_:
  call near read_str_
  invoke write,1,str_buf,ebx
  jmp near loop_

new_line_:
  invoke putchar,0xA
  jmp near loop_

clear_:
  ; Well
  invoke system,clear_cmd
  jmp near loop_

goto_:
  call near read_number_
  add ax,[origin_x]
  mov [p.X],ax

  call near read_number_
  add ax,[origin_y]
  mov [p.Y],ax

  invoke SetConsoleCursorPosition,[hOut],dword [p]
  jmp near loop_

origin_:
  call near read_number_
  mov [origin_x],ax

  call near read_number_
  mov [origin_y],ax

  jmp near loop_

wait_:
  call near read_number_
  invoke Sleep,eax
  jmp near loop_

quit_:
  invoke exit,0

read_str_:
  ; Skip all spaces
  call read_char_

  call is_space_
    jz near read_str_

  cmp eax,'"'
    je near read_str_quote_

  mov ebx,1 ; Offset in str_buf
  mov byte [str_mode],byte 1 ; Reading mode: space

  mov byte [str_buf],byte al

  read_str_loop_:
    cmp ebx,STR_BUF_SIZE
      je near read_str_end_

    call near read_char_

    cmp al,'\'
      je near read_str_escape_

    call read_str_cmp_
      je near read_str_end_

    read_str_escape_end_:
    cmp ebx,STR_BUF_SIZE
      je near read_str_end_

    mov byte [str_buf + ebx],byte al
    inc ebx
    jmp near read_str_loop_

  read_str_cmp_:
    cmp byte [str_mode],byte 1
      je near is_space_

    ; Quote mode
    cmp al,'"'
    ret

    is_space_:
      cmp byte al,' ' ; Space
        je near is_space_end_

      cmp byte al,0xA ; LF
        je near is_space_end_

      cmp byte al,0xD ; CR
        je near is_space_end_

      cmp byte al,0x9 ; TAB
        ; Useless jump
        ; je near is_space_end_

      is_space_end_:
        ret

  read_str_escape_:
    ; Read next char (skip "\)"
    call near read_char_
    jmp near read_str_escape_end_

  read_str_quote_:
    mov ebx,0 ; Offset is 0
    mov byte [str_mode],byte 2 ; Reading mode: quote
    jmp near read_str_loop_

  read_str_end_:
    ret

read_number_:
  call near read_str_
  ; Add '\0' to get a Null-terminated string.
  mov byte [str_buf + ebx], 0
  cinvoke strtol, str_buf, NULL, 0
  ret

read_char_:
  cinvoke getchar

  cmp eax,EOF
    jz near quit_

  ret

clear_cmd db 'cls', 0

; Variables
hOut dd ?
str_mode db ?
str_buf dd ?

origin_x dw 0
origin_y dw 0

p COORD ?

; Imports
data import

library \
  msvcrt, 'MSVCRT.DLL', \
  kernel32, 'KERNEL32.DLL'

import msvcrt, \
  exit, 'exit', \
  putchar, 'putchar', \
  getchar, 'getchar', \
  write, '_write', \
  strtol, 'strtol', \
  system, 'system', \
  puts, 'puts'

import kernel32, \
  SetConsoleTextAttribute,'SetConsoleTextAttribute', \
  SetConsoleCursorPosition, 'SetConsoleCursorPosition', \
  GetStdHandle, 'GetStdHandle', \
  Sleep, 'Sleep'

end data
