#include <stdio.h>
#include <stdint.h>
uint8_t PC = 0;
uint8_t R[4];
uint8_t M[16] = {
    0b10001001, 
    0b10010001, 
    0b10100001, 
    0b10110010, 
    0b00010111, 
    0b00100110, 
    0b11010001,
    0b01011110,
};

uint8_t inst_fetch() {
    return M[PC++];
}

uint8_t inst_decode(uint8_t inst) {
    uint8_t opcode = (inst >> 6) & 0x03;   
    return opcode;
}

void inst_execute(uint8_t opcode, uint8_t inst) {
    uint8_t rd = (inst >> 4) & 0x03;
    uint8_t rs1 = (inst >> 2) & 0x03;  
    uint8_t rs2 = inst& 0x03;
    uint8_t imm = inst & 0x0F;
    uint8_t addr = (inst >> 2) & 0x0F;
    switch (opcode) {
        case 0: 
            R[rd] = R[rs1] + R[rs2];
            break;
        case 2: 
            R[rd] = imm;
            break;
        case 3: 
            if(R[0] != R[rs2]) {
                PC = addr;
            }
            break;
        default:
            break;
    }
}

uint8_t inst_cycle() {
    uint8_t inst = inst_fetch();
    uint8_t opcode = inst_decode(inst);
    inst_execute(opcode, inst);
    return inst;
}

void main(void){
    while (1){
        if (inst_cycle() == 0){
        goto end;
        }
    }
    end:
        printf("result: r0=%d r1=%d r2=%d r3=%d\n", R[0], R[1], R[2], R[3]);
    return;
}
