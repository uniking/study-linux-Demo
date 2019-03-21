.data
msg:
    .ascii    "Hello, ARM!\n"
len = . - msg


.text
.globl main
.thumb
main:
    #arm code
    #ldr pc,[pc]

    #bl r0
    ble error
    bls error
    blt error

    bx r0
    blx r0

    #b r0
    bne error
    beq error
    bcs error
    bcc error
    bmi error
    bpl error
    bvs error
    bvc error
    bhi error
    bge error
    bgt error

    add r0, r1, r3
    mov r0,r1
error:
    
    mov r0, #0     /* status -> 0 */
