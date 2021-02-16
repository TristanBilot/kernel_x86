%macro ISR_NO_ERROR 1  ; define a macro, taking one parameter
    [GLOBAL isr%1]        ; %1 accesses the first parameter.
    isr%1:
        push byte 0
        push byte %1
        jmp isr_common_stub
%endmacro

%macro ISR_ERROR 1
    [GLOBAL isr%1]
    isr%1:
        push byte %1
        jmp isr_common_stub
%endmacro



ISR_NO_ERROR  0
ISR_NO_ERROR  1
ISR_NO_ERROR  2
ISR_NO_ERROR  3
ISR_NO_ERROR  4
ISR_NO_ERROR  5
ISR_NO_ERROR  6
ISR_NO_ERROR  7
ISR_ERROR    8
ISR_NO_ERROR  9
ISR_ERROR   10
ISR_ERROR   11
ISR_ERROR   12
ISR_ERROR   13
ISR_ERROR   14
ISR_NO_ERROR 15
ISR_NO_ERROR 16
ISR_ERROR 17
ISR_NO_ERROR 18
ISR_NO_ERROR 19
ISR_NO_ERROR 20
ISR_NO_ERROR 21
ISR_NO_ERROR 22
ISR_NO_ERROR 23
ISR_NO_ERROR 24
ISR_NO_ERROR 25
ISR_NO_ERROR 26
ISR_NO_ERROR 27
ISR_NO_ERROR 28
ISR_NO_ERROR 29
ISR_NO_ERROR 30
ISR_NO_ERROR 31
ISR_NO_ERROR 32
ISR_NO_ERROR 33
ISR_NO_ERROR 34
ISR_NO_ERROR 35
ISR_NO_ERROR 36
ISR_NO_ERROR 37
ISR_NO_ERROR 38
ISR_NO_ERROR 39
ISR_NO_ERROR 40
ISR_NO_ERROR 41
ISR_NO_ERROR 42
ISR_NO_ERROR 43
ISR_NO_ERROR 44
ISR_NO_ERROR 45
ISR_NO_ERROR 46
ISR_NO_ERROR 47
ISR_NO_ERROR 48
ISR_NO_ERROR 49
ISR_NO_ERROR 50
ISR_NO_ERROR 51
ISR_NO_ERROR 52
ISR_NO_ERROR 53
ISR_NO_ERROR 54
ISR_NO_ERROR 55
ISR_NO_ERROR 56
ISR_NO_ERROR 57
ISR_NO_ERROR 58
ISR_NO_ERROR 59
ISR_NO_ERROR 60
ISR_NO_ERROR 61
ISR_NO_ERROR 62
ISR_NO_ERROR 63
ISR_NO_ERROR 64
ISR_NO_ERROR 65
ISR_NO_ERROR 66
ISR_NO_ERROR 67
ISR_NO_ERROR 68
ISR_NO_ERROR 69
ISR_NO_ERROR 70
ISR_NO_ERROR 71
ISR_NO_ERROR 72
ISR_NO_ERROR 73
ISR_NO_ERROR 74
ISR_NO_ERROR 75
ISR_NO_ERROR 76
ISR_NO_ERROR 77
ISR_NO_ERROR 78
ISR_NO_ERROR 79
ISR_NO_ERROR 80
ISR_NO_ERROR 81
ISR_NO_ERROR 82
ISR_NO_ERROR 83
ISR_NO_ERROR 84
ISR_NO_ERROR 85
ISR_NO_ERROR 86
ISR_NO_ERROR 87
ISR_NO_ERROR 88
ISR_NO_ERROR 89
ISR_NO_ERROR 90
ISR_NO_ERROR 91
ISR_NO_ERROR 92
ISR_NO_ERROR 93
ISR_NO_ERROR 94
ISR_NO_ERROR 95
ISR_NO_ERROR 96
ISR_NO_ERROR 97
ISR_NO_ERROR 98
ISR_NO_ERROR 99
ISR_NO_ERROR 100
ISR_NO_ERROR 101
ISR_NO_ERROR 102
ISR_NO_ERROR 103
ISR_NO_ERROR 104
ISR_NO_ERROR 105
ISR_NO_ERROR 106
ISR_NO_ERROR 107
ISR_NO_ERROR 108
ISR_NO_ERROR 109
ISR_NO_ERROR 110
ISR_NO_ERROR 111
ISR_NO_ERROR 112
ISR_NO_ERROR 113
ISR_NO_ERROR 114
ISR_NO_ERROR 115
ISR_NO_ERROR 116
ISR_NO_ERROR 117
ISR_NO_ERROR 118
ISR_NO_ERROR 119
ISR_NO_ERROR 120
ISR_NO_ERROR 121
ISR_NO_ERROR 122
ISR_NO_ERROR 123
ISR_NO_ERROR 124
ISR_NO_ERROR 125
ISR_NO_ERROR 126
ISR_NO_ERROR 127
ISR_NO_ERROR 128
ISR_NO_ERROR 129
ISR_NO_ERROR 130
ISR_NO_ERROR 131
ISR_NO_ERROR 132
ISR_NO_ERROR 133
ISR_NO_ERROR 134
ISR_NO_ERROR 135
ISR_NO_ERROR 136
ISR_NO_ERROR 137
ISR_NO_ERROR 138
ISR_NO_ERROR 139
ISR_NO_ERROR 140
ISR_NO_ERROR 141
ISR_NO_ERROR 142
ISR_NO_ERROR 143
ISR_NO_ERROR 144
ISR_NO_ERROR 145
ISR_NO_ERROR 146
ISR_NO_ERROR 147
ISR_NO_ERROR 148
ISR_NO_ERROR 149
ISR_NO_ERROR 150
ISR_NO_ERROR 151
ISR_NO_ERROR 152
ISR_NO_ERROR 153
ISR_NO_ERROR 154
ISR_NO_ERROR 155
ISR_NO_ERROR 156
ISR_NO_ERROR 157
ISR_NO_ERROR 158
ISR_NO_ERROR 159
ISR_NO_ERROR 160
ISR_NO_ERROR 161
ISR_NO_ERROR 162
ISR_NO_ERROR 163
ISR_NO_ERROR 164
ISR_NO_ERROR 165
ISR_NO_ERROR 166
ISR_NO_ERROR 167
ISR_NO_ERROR 168
ISR_NO_ERROR 169
ISR_NO_ERROR 170
ISR_NO_ERROR 171
ISR_NO_ERROR 172
ISR_NO_ERROR 173
ISR_NO_ERROR 174
ISR_NO_ERROR 175
ISR_NO_ERROR 176
ISR_NO_ERROR 177
ISR_NO_ERROR 178
ISR_NO_ERROR 179
ISR_NO_ERROR 180
ISR_NO_ERROR 181
ISR_NO_ERROR 182
ISR_NO_ERROR 183
ISR_NO_ERROR 184
ISR_NO_ERROR 185
ISR_NO_ERROR 186
ISR_NO_ERROR 187
ISR_NO_ERROR 188
ISR_NO_ERROR 189
ISR_NO_ERROR 190
ISR_NO_ERROR 191
ISR_NO_ERROR 192
ISR_NO_ERROR 193
ISR_NO_ERROR 194
ISR_NO_ERROR 195
ISR_NO_ERROR 196
ISR_NO_ERROR 197
ISR_NO_ERROR 198
ISR_NO_ERROR 199
ISR_NO_ERROR 200
ISR_NO_ERROR 201
ISR_NO_ERROR 202
ISR_NO_ERROR 203
ISR_NO_ERROR 204
ISR_NO_ERROR 205
ISR_NO_ERROR 206
ISR_NO_ERROR 207
ISR_NO_ERROR 208
ISR_NO_ERROR 209
ISR_NO_ERROR 210
ISR_NO_ERROR 211
ISR_NO_ERROR 212
ISR_NO_ERROR 213
ISR_NO_ERROR 214
ISR_NO_ERROR 215
ISR_NO_ERROR 216
ISR_NO_ERROR 217
ISR_NO_ERROR 218
ISR_NO_ERROR 219
ISR_NO_ERROR 220
ISR_NO_ERROR 221
ISR_NO_ERROR 222
ISR_NO_ERROR 223
ISR_NO_ERROR 224
ISR_NO_ERROR 225
ISR_NO_ERROR 226
ISR_NO_ERROR 227
ISR_NO_ERROR 228
ISR_NO_ERROR 229
ISR_NO_ERROR 230
ISR_NO_ERROR 231
ISR_NO_ERROR 232
ISR_NO_ERROR 233
ISR_NO_ERROR 234
ISR_NO_ERROR 235
ISR_NO_ERROR 236
ISR_NO_ERROR 237
ISR_NO_ERROR 238
ISR_NO_ERROR 239
ISR_NO_ERROR 240
ISR_NO_ERROR 241
ISR_NO_ERROR 242
ISR_NO_ERROR 243
ISR_NO_ERROR 244
ISR_NO_ERROR 245
ISR_NO_ERROR 246
ISR_NO_ERROR 247
ISR_NO_ERROR 248
ISR_NO_ERROR 249
ISR_NO_ERROR 250
ISR_NO_ERROR 251
ISR_NO_ERROR 252
ISR_NO_ERROR 253
ISR_NO_ERROR 254
ISR_NO_ERROR 255





[EXTERN generic_handler]

isr_common_stub:
    pusha                    ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov ax, ds               ; Lower 16-bits of eax = ds.
    push eax                 ; save the data segment descriptor

    mov ax, 0x10  ; load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp

    call generic_handler
    pop ebx

    pop ebx        ; reload the original data segment descriptor
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa                     ; Pops edi,esi,ebp...
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP


; This macro creates a stub for an IRQ - the first parameter is
; the IRQ number, the second is the ISR number it is remapped to.
%macro IRQ 2
[GLOBAL irq%1]
    irq%1:
        cli
        push byte 0
        push byte %2
        jmp irq_common_stub
%endmacro

IRQ  0, 32
IRQ  1, 33
IRQ  2, 34
IRQ  3, 35
IRQ  4, 36
IRQ  5, 37
IRQ  6, 38
IRQ  7, 39
IRQ  8, 40
IRQ  9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47


; In isr.c
[EXTERN generic_handler]
; This is our common IRQ stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame. 
irq_common_stub:
   pusha                    ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

   mov ax, ds               ; Lower 16-bits of eax = ds.
   push eax                 ; save the data segment descriptor

   mov ax, 0x10  ; load the kernel data segment descriptor
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax
   push esp

   call generic_handler
   pop ebx

   pop ebx        ; reload the original data segment descriptor
   mov ds, bx
   mov es, bx
   mov fs, bx
   mov gs, bx

   popa                     ; Pops edi,esi,ebp...
   add esp, 8     ; Cleans up the pushed error code and pushed ISR number
   iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP