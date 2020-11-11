/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   SCE212 Ajou University                                    */
/*   run.c                                                     */
/*   Adapted from CS311@KAIST                                  */
/*                                                             */
/***************************************************************/

#include <stdio.h>

#include "util.h"
#include "run.h"

/***************************************************************/
/*                                                             */
/* Procedure: get_inst_info                                    */
/*                                                             */
/* Purpose: Read insturction information                       */
/*                                                             */
/***************************************************************/
instruction* get_inst_info(uint32_t pc)
{
    return &INST_INFO[(pc - MEM_TEXT_START) >> 2];
}

/***************************************************************/
/*                                                             */
/* Procedure: process_instruction                              */
/*                                                             */
/* Purpose: Process one instrction                             */
/*                                                             */
/***************************************************************/
void process_instruction()
{
	/** Implement this function */
    if(INSTRUCTION_COUNT>=NUM_INST)
    {
        RUN_BIT=FALSE;
        return;
    }
    instruction* instr=get_inst_info(CURRENT_STATE.PC);
    int op=instr->opcode;
    
    if(!op) // R
    {
        //printf("%d %d %d %d %d %d\n",instr->opcode, instr->r_t.r_i.rs, instr->r_t.r_i.rt, instr->r_t.r_i.r_i.r.rd, instr->r_t.r_i.r_i.r.shamt, instr->func_code);
    
        uint32_t rs=instr->r_t.r_i.rs;
        uint32_t rt=instr->r_t.r_i.rt;
        uint32_t rd=instr->r_t.r_i.r_i.r.rd;
        uint32_t shm=instr->r_t.r_i.r_i.r.shamt;
        uint32_t fun=instr->func_code;
        

        switch (fun)
        {
        case 0b100001:      //addu
            CURRENT_STATE.REGS[rd]=CURRENT_STATE.REGS[rs]+CURRENT_STATE.REGS[rt];
            break;
        case 0b100100:      //and
            CURRENT_STATE.REGS[rd]=(CURRENT_STATE.REGS[rs]&CURRENT_STATE.REGS[rt]);
            break;
        case 0b001000:      //jr
            CURRENT_STATE.PC=CURRENT_STATE.REGS[rs];
            break;
        case 0b100111:      //nor
            CURRENT_STATE.REGS[rd]=~(CURRENT_STATE.REGS[rs]|CURRENT_STATE.REGS[rt]);
            break;
        case 0b100101:      //or
            CURRENT_STATE.REGS[rd]=(CURRENT_STATE.REGS[rs]|CURRENT_STATE.REGS[rt]);
            break;
        case 0b101011:      //sltu
            CURRENT_STATE.REGS[rd]=((CURRENT_STATE.REGS[rs]<CURRENT_STATE.REGS[rt]) ? 1 : 0);
            break;
        case 0b000000:      //sll
            CURRENT_STATE.REGS[rd]=(CURRENT_STATE.REGS[rt]<<shm);
            break;
        case 0b000010:      //srl
            CURRENT_STATE.REGS[rd]=(CURRENT_STATE.REGS[rt]>>shm);
            break;
        case 0b100011:      //subu
            CURRENT_STATE.REGS[rd]=CURRENT_STATE.REGS[rs]-CURRENT_STATE.REGS[rt];
            break;
        default:
            break;
        }
        if(fun!=0b001000)
        {
            CURRENT_STATE.PC+=4;
        }
        
    }
    else    //i,j
    {
        uint32_t rs=instr->r_t.r_i.rs;
        uint32_t rt=instr->r_t.r_i.rt;
        uint32_t imm=instr->r_t.r_i.r_i.imm;
        uint32_t adr=instr->r_t.target;

        switch (op)
        {
        case 0b001001:      //addiu
            CURRENT_STATE.REGS[rt]=CURRENT_STATE.REGS[rs]+imm;
            break;
        case 0b001100:      //andi
            CURRENT_STATE.REGS[rt]=(CURRENT_STATE.REGS[rs]&imm);
            break;
        case 0b000100:      //beq
            //printf("beq: %d %d\n",CURRENT_STATE.REGS[rs],CURRENT_STATE.REGS[rt]);
            if(CURRENT_STATE.REGS[rs]==CURRENT_STATE.REGS[rt])
            {
                CURRENT_STATE.PC+=(imm<<2);
            }
            break;
        case 0b000101:      //bne
            //printf("bne: %d %d\n",CURRENT_STATE.REGS[rs],CURRENT_STATE.REGS[rt]);
            if(CURRENT_STATE.REGS[rs]!=CURRENT_STATE.REGS[rt])
            {
                CURRENT_STATE.PC+=(imm<<2);
            }
            break;
        case 0b000010:      //j
            CURRENT_STATE.PC=((CURRENT_STATE.PC>>28)<<28)+(adr<<2);
            break;
        case 0b000011:      //jar
            CURRENT_STATE.REGS[31]=CURRENT_STATE.PC+8;
            CURRENT_STATE.PC=((CURRENT_STATE.PC>>28)<<28)+(adr<<2);
            break;
        case 0b001111:      //lui
            CURRENT_STATE.REGS[rt]=(imm<<16);
            //printf("lui: %d\n",CURRENT_STATE.REGS[rt]);
            break;
        case 0b100011:      //lw
            CURRENT_STATE.REGS[rt]=mem_read_32(CURRENT_STATE.REGS[rs]+imm);
            break;
        case 0b001101:      //ori
            CURRENT_STATE.REGS[rt]=(CURRENT_STATE.REGS[rs]|imm);
            //printf("ori: %d %d %d\n",CURRENT_STATE.REGS[rt],CURRENT_STATE.REGS[rs],imm);
            break;
        case 0b001011:      //sltiu
            CURRENT_STATE.REGS[rt]=(CURRENT_STATE.REGS[rs]<imm ? 1:0);
            break;
        case 0b101011:      //sw
            mem_write_32(CURRENT_STATE.REGS[rs]+imm,CURRENT_STATE.REGS[rt]);
            break;
        default:
            break;
        }
        if(op!=0b000010 && op!=0b000011)
        {
            CURRENT_STATE.PC+=4;
        }
    }
    
}
