#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define MEM_SIZE 296512
#define REG_SIZE 32

uint32_t PC = 0;
int32_t R[REG_SIZE];
uint32_t ROM[MEM_SIZE]={
    // ===== 第一阶段：初始化 =====
    // 1. ADDI - 设置数据
    0x00500093, // ADDI x1, x0, 5     ; x1 = 5
    
    // 2. ADDI  
    0x00600113, // ADDI x2, x0, 6     ; x2 = 6
    
    // 3. ADD
    0x002081B3, // ADD  x3, x1, x2    ; x3 = 5+6=11
    
    // 4. LUI - 设置跳转目标地址
    0x000042B7, // LUI  x5, 4         ; x5 = 0x4000
    
    // 5. ADDI - 调整跳转地址为指令边界
    0x02828293, // ADDI x5, x5, 40    ; x5 = 0x4000 + 40 = 0x4028
    
    // ===== 第二阶段：内存操作 =====
    // 6. SW - 存储数据
    0x0032A023, // SW   x3, 0(x5)     ; Mem[0x4028] = 11
    
    // 7. LW - 加载数据
    0x0002A283, // LW   x5, 0(x5)     ; x5 = Mem[0x4028] = 11
    
    // 8. SB - 字节存储
    0x00A00113, // ADDI x2, x0, 0xA   ; x2 = 0xA
    0x00208023, // SB   x2, 0(x1)     ; Mem[5] = 0xA
    
    // 9. LBU - 字节加载
    0x0000C183, // LBU  x3, 0(x1)     ; x3 = Mem[5] = 0x0000000A
    
    // ===== 第三阶段：JALR测试 =====
    // 10. 先设置跳转目标地址（跳过EBREAK）
    0x03400413, // ADDI x8, x0, 52    ; x8 = 52 (0x34)
    
    // 11. JALR - 跳转到0x34并链接
    0x008400E7, // JALR x1, 8(x8)     ; x1 = PC+4, PC = x8 + 8 = 60(0x3C)
    // PC当前=0x28, 所以x1=0x2C
    
    // ===== 这里应该被跳过 =====
    0x00100113, // ADDI x2, x0, 1     ; 这行不应该执行（如果JALR工作正常）
    
    // ===== 跳转目标地址0x3C =====
    // 12. 跳转到这里执行
    0x00200793, // ADDI x15, x0, 2    ; x15 = 2
    
    // 13. 跳转回主流程
    0x02C00067, // JALR x0, 44(x0)    ; PC = 0 + 44 = 0x2C
    // 跳回到地址0x2C
    
    // ===== 继续执行 =====
    // 14. 验证是否返回
    0x00300113, // ADDI x2, x0, 3     ; x2 = 3
    
    // 15. EBREAK
    0x00100073, // EBREAK             ; 停止
};
uint32_t RAM[MEM_SIZE];
typedef enum {
    ITYPE,
    RTYPE,
    STYPE,
    UTYPE
}InstructionType;

struct Itype{
    int32_t opcode;
    int32_t rd;
    int32_t funct3;
    int32_t rs1;
    int32_t imm;
};
struct Rtype{
    int32_t opcode;
    int32_t rd;
    int32_t funct3;
    int32_t rs1;
    int32_t rs2;
    int32_t funct7;
};
struct Stype{
    int32_t opcode;
    int32_t imm4_0;
    int32_t funct3;
    int32_t rs1;
    int32_t rs2;
    int32_t imm11_5;
};
struct Utype{
    int32_t opcode;
    int32_t rd;
    int32_t imm31_12;
};
typedef struct Itype Itype;
typedef struct Rtype Rtype;
typedef struct Stype Stype;
typedef struct Utype Utype;

struct Type{
    Itype itype;
    Rtype rtype;
    Stype stype;
    Utype utype;
    uint8_t type;
};
typedef struct Type Type;

uint32_t fetch() {
    printf("PC 0x%08X : 0x%08X\n", PC, ROM[PC / 4]);
    return ROM[PC / 4];
}

Type decode_itype(uint32_t instruction, Type type) {
    type.itype.opcode = instruction & 0x7F;
    type.itype.rd = (instruction >> 7) & 0x1F;
    type.itype.funct3 = (instruction >> 12) & 0x7;
    type.itype.rs1 = (instruction >> 15) & 0x1F;
    type.itype.imm = (instruction >> 20) & 0xFFF;
    return type; // Simplified for demonstration
}
Type decode_rtype(uint32_t instruction, Type type) {
    type.rtype.opcode = instruction & 0x7F;
    type.rtype.rd = (instruction >> 7) & 0x1F;
    type.rtype.funct3 = (instruction >> 12) & 0x7;
    type.rtype.rs1 = (instruction >> 15) & 0x1F;
    type.rtype.rs2 = (instruction >> 20) & 0x1F;
    type.rtype.funct7 = (instruction >> 25) & 0x7F;
    return type; // Simplified for demonstration
}
Type decode_stype(uint32_t instruction, Type type) {
    type.stype.opcode = instruction & 0x7F;
    type.stype.imm4_0 = (instruction >> 7) & 0x1F;
    type.stype.funct3 = (instruction >> 12) & 0x7;
    type.stype.rs1 = (instruction >> 15) & 0x1F;
    type.stype.rs2 = (instruction >> 20) & 0x1F;
    type.stype.imm11_5 = (instruction >> 25) & 0x7F;
    return type; // Simplified for demonstration
}
Type decode_utype(uint32_t instruction, Type type) {
    type.utype.opcode = instruction & 0x7F;
    type.utype.rd = (instruction >> 7) & 0x1F;
    type.utype.imm31_12 = (instruction >> 12) & 0xFFFFF;
    return type; // Simplified for demonstration
}

Type decode(uint32_t instruction, Type type) {
    int32_t opcode = instruction & 0x7F;
    switch (opcode) {
        case 0x13: // ADDI
        case 0x67: // JALR
        case 0x03: // LW or LBU
        case 0x73: // EBREAK
            type = decode_itype(instruction, type);
            type.type = ITYPE;
            return type;
        case 0x33: // ADD
            type = decode_rtype(instruction, type);
            type.type = RTYPE;
            return type;
        case 0x23: // SW or SB
            type = decode_stype(instruction, type);
            type.type = STYPE;  
            return type;
        case 0x37: // LUI
            type = decode_utype(instruction, type);
            type.type = UTYPE;
            return type;
        default:
            printf("Unknown opcode: 0x%02X\n", opcode);
            exit(1);
    }
} 

int32_t signextend(int32_t imm) {
    if (imm & 0x800) { // Check if the sign bit is set
        return imm | 0xFFFFF000; // Extend the sign
    }
    return imm;
}

void execute(uint32_t instruction, Type type) {
    uint32_t target;
    // printf("Executing instruction: 0x%08X\n", instruction);
    switch (type.type) {
        case ITYPE: // ADDI
            if (type.itype.opcode == 0x13) { // ADDI
                int32_t imm = signextend(type.itype.imm);
                if(type.itype.rd == 0) {
                    PC += 4;
                    return; // R0 is always 0
                }
                R[type.itype.rd] = R[type.itype.rs1] + imm;
                // printf("ADDI executed: R[%d] = 0x%08X\n", type.itype.rd, R[type.itype.rd]);
                PC += 4; 
            } else if (type.itype.opcode == 0x67) { // JALR
                int32_t imm = signextend(type.itype.imm);
                target = (R[type.itype.rs1] + imm) & ~1;
                if (type.itype.rd == 0) {
                    PC = target;
                    return; // R0 is always 0
                }
                R[type.itype.rd] = PC + 4;
                PC = target;
                // printf("JALR executed: R[%d] = 0x%08X, PC = 0x%08X\n", type.itype.rd, R[type.itype.rd], target);
            } else if (type.itype.opcode == 0x03) { // LW or LBU
                if (type.itype.funct3 == 0x2){ // LW
                    int32_t imm = signextend(type.itype.imm);
                    uint32_t addr = R[type.itype.rs1] + imm;
                    if (type.itype.rd == 0){
                        PC += 4;
                        return; // R0 is always 0
                    }
                    R[type.itype.rd] = RAM[addr / 4];
                    PC += 4;
                    // printf("LW executed: R[%d] = 0x%08X\n", type.itype.rd, R[type.itype.rd]);
                } else if (type.itype.funct3 == 0x4) { // LBU
                    int32_t imm = signextend(type.itype.imm);
                    uint32_t addr = R[type.itype.rs1] + imm;
                    uint32_t byte_offset = addr & 3;
                    uint32_t word_addr = addr / 4;
                    uint32_t byte_value = (RAM[word_addr] >> (byte_offset * 8)) & 0xFF;
                    if (type.itype.rd == 0){
                        PC += 4;
                        return; // R0 is always 0
                    }
                    R[type.itype.rd] = byte_value; // Zero-extend
                    PC += 4;
                    // printf("LBU executed: R[%d] = 0x%08X\n", type.itype.rd, R[type.itype.rd]);
                } else {
                    printf("Unknown funct3 for ITYPE load: 0x%X\n", type.itype.funct3);
                    exit(1);
                }
            } else if (type.itype.opcode == 0x73) { // EBREAK
                printf("EBREAK encountered. Halting execution.\n");
                printf("Final PC: 0x%08X\n", PC);
                for (size_t i = 0; i < REG_SIZE; i++)
                {
                    printf("R[%02zu] = 0x%08X\n", i, R[i]);
                }
                exit(0);    
            }
            break;
        case RTYPE: // ADD
            if (type.rtype.opcode == 0x33) { // ADD
                if (type.rtype.rd == 0){
                    PC += 4;
                    return; // R0 is always 0
                }
                R[type.rtype.rd] = R[type.rtype.rs1] + R[type.rtype.rs2];
                PC += 4;
                // printf("ADD executed: R[%d] = R[%d] + R[%d] = 0x%08X + 0x%08x\n", type.rtype.rd, type.rtype.rs1, type.rtype.rs2 ,R[type.rtype.rs1], R[type.rtype.rs2]);
            }
            break;
        case STYPE: // SW
            if (type.stype.opcode == 0x23) { //SW or SB
                if (type.stype.funct3 == 0x2){ // SW
                    int32_t imm = (type.stype.imm11_5 << 5) | type.stype.imm4_0;
                    imm = signextend(imm);
                    uint32_t addr = R[type.stype.rs1] + imm;
                    RAM[addr / 4] = R[type.stype.rs2];
                    PC += 4;
                    // printf("SW executed: Mem[0x%08X] = 0x%08X\n", addr, R[type.stype.rs2]);
                } else if (type.stype.funct3 == 0x0){ // SB
                    int32_t imm = (type.stype.imm11_5 << 5) | type.stype.imm4_0;
                    imm = signextend(imm);
                    uint32_t addr = R[type.stype.rs1] + imm;
                    uint32_t byte_offset = addr % 4;
                    uint32_t word_addr = addr / 4;
                    uint32_t existing_word = RAM[word_addr];
                    existing_word &= ~(0xFF << (byte_offset * 8)); // Clear the target byte
                    existing_word |= (R[type.stype.rs2] & 0xFF) << (byte_offset * 8); // Set the target byte
                    RAM[word_addr] = existing_word;
                    PC += 4;
                    // printf("SB executed: Mem[0x%08X] = 0x%02X\n", addr, R[type.stype.rs2] & 0xFF);
                } else {
                    printf("Unknown funct3 for STYPE store: 0x%X\n", type.stype.funct3);
                    exit(1);
                }        
            }
            break;
        case UTYPE: // LUI
            if (type.utype.opcode == 0x37) { // LUI
                if (type.utype.rd == 0){
                    PC += 4;
                    return; // R0 is always 0
                }
                R[type.utype.rd] = type.utype.imm31_12 << 12;
                // printf("LUI executed: R[%d] = 0x%08X\n", type.utype.rd, R[type.utype.rd]);
                PC += 4;
            }
            break;
        default:
            break;
    }
}

void load_program(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open program file");
        exit(1);
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (file_size > MEM_SIZE * sizeof(uint32_t)) {
        fprintf(stderr, "Program size exceeds memory size\n");
        fprintf(stderr, "Program size: %ld bytes, Memory size: %ld bytes\n", file_size, MEM_SIZE * sizeof(uint32_t));
        fclose(file);
        exit(1);
    }
    size_t rom_read = fread(ROM, sizeof(uint32_t), file_size, file);
    if (rom_read != file_size / sizeof(uint32_t)){
        fprintf(stderr, "Failed to read the entire program file\n");
        fclose(file);
        exit(1);
    }
    fclose(file);
    file = fopen(filename, "rb");
    size_t ram_read = fread(RAM, sizeof(uint32_t), file_size, file);
    if (ram_read != file_size / sizeof(uint32_t)){
        fprintf(stderr, "Failed to read the entire program file into RAM\n");
        fclose(file);
        exit(1);
    }   
    fclose(file);
    printf("Program loaded: %ld bytes\n", file_size);
}

int main(int argc, char* argv[]) {
    // Load a simple program into memory (for demonstration)
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <program_file>\n", argv[0]);
        return 1;
    }
    load_program(argv[1]);
    // Main execution loop
    while (1) {
        uint32_t instruction = fetch();
        Type type;
        type = decode(instruction, type);
        execute(instruction, type);
        if (PC / 4 >= MEM_SIZE) {
            printf("Program counter out of bounds. Halting execution.\n");
            printf("Final PC: 0x%08X\n", PC);
            break; // Exit if PC goes out of memory bounds
        }
    }       
    return 0;
}