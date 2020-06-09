/* Instruction set simulator for Y64 Architecture */

#include <stdio.h>
#include <stdlib.h>

#include "y64sim.h"

#define err_print(_s, _a ...) \
    fprintf(stdout, _s"\n", _a);


typedef enum {STAT_AOK, STAT_HLT, STAT_ADR, STAT_INS} stat_t;

char *stat_names[] = { "AOK", "HLT", "ADR", "INS" };

char *stat_name(stat_t e)
{
    if (e < STAT_AOK || e > STAT_INS)
        return "Invalid Status";
    return stat_names[e];
}

char *cc_names[8] = {
    "Z=0 S=0 O=0",
    "Z=0 S=0 O=1",
    "Z=0 S=1 O=0",
    "Z=0 S=1 O=1",
    "Z=1 S=0 O=0",
    "Z=1 S=0 O=1",
    "Z=1 S=1 O=0",
    "Z=1 S=1 O=1" };

char *cc_name(cc_t c)
{
    int ci = c;
    if (ci < 0 || ci > 7)
        return "???????????";
    else
        return cc_names[c];
}

bool_t get_byte_val(mem_t *m, long_t addr, byte_t *dest)
{
    if (addr < 0 || addr >= m->len)
        return FALSE;
    *dest = m->data[addr];
    return TRUE;
}

bool_t get_long_val(mem_t *m, long_t addr, long_t *dest)
{
    int i;
    long_t val;
    if (addr < 0 || addr + 8 > m->len)
	    return FALSE;
    val = 0;
    for (i = 0; i < 8; i++)
	    val = val | ((long_t)m->data[addr+i])<<(8*i);
    *dest = val;
    return TRUE;
}

bool_t set_byte_val(mem_t *m, long_t addr, byte_t val)
{
    if (addr < 0 || addr >= m->len)
	    return FALSE;
    m->data[addr] = val;
    return TRUE;
}

bool_t set_long_val(mem_t *m, long_t addr, long_t val)
{
    int i;
    if (addr < 0 || addr + 8 > m->len)
	    return FALSE;
    for (i = 0; i < 8; i++) {
    	m->data[addr+i] = val & 0xFF;
    	val >>= 8;
    }
    return TRUE;
}

mem_t *init_mem(int len)
{
    mem_t *m = (mem_t *)malloc(sizeof(mem_t));
    len = ((len+BLK_SIZE-1)/BLK_SIZE)*BLK_SIZE;
    m->len = len;
    m->data = (byte_t *)calloc(len, 1);

  
    return m;
}

void free_mem(mem_t *m)
{
    free((void *) m->data);
    free((void *) m);
}

mem_t *dup_mem(mem_t *oldm)
{
    mem_t *newm = init_mem(oldm->len);
    memcpy(newm->data, oldm->data, oldm->len);
    return newm;
}

bool_t diff_mem(mem_t *oldm, mem_t *newm, FILE *outfile)
{
    long_t pos;
    int len = oldm->len;
    bool_t diff = FALSE;
    
    if (newm->len < len)
	    len = newm->len;
    
    for (pos = 0; (!diff || outfile) && pos < len; pos += 8) {
        long_t ov = 0;  long_t nv = 0;
        get_long_val(oldm, pos, &ov);
        get_long_val(newm, pos, &nv);
        if (nv != ov) {
            diff = TRUE;
            if (outfile)
                fprintf(outfile, "0x%.16lx:\t0x%.16lx\t0x%.16lx\n", pos, ov, nv);
        }
    }
    return diff;
}


reg_t reg_table[REG_NONE] = {
    {"%rax", REG_RAX},
    {"%rcx", REG_RCX},
    {"%rdx", REG_RDX},
    {"%rbx", REG_RBX},
    {"%rsp", REG_RSP},
    {"%rbp", REG_RBP},
    {"%rsi", REG_RSI},
    {"%rdi", REG_RDI},
    {"%r8",  REG_R8},
    {"%r9",  REG_R9},
    {"%r10", REG_R10},
    {"%r11", REG_R11},
    {"%r12", REG_R12},
    {"%r13", REG_R13},
    {"%r14", REG_R14}
};

long_t get_reg_val(mem_t *r, regid_t id)
{
    long_t val = 0;
    if (id >= REG_NONE)
        return 0;
    get_long_val(r, id*8, &val);
    return val;
}

void set_reg_val(mem_t *r, regid_t id, long_t val)
{
    if (id < REG_NONE)
        set_long_val(r, id*8, val);
}

mem_t *init_reg()
{
    return init_mem(REG_SIZE);
}

void free_reg(mem_t *r)
{
    free_mem(r);
}

mem_t *dup_reg(mem_t *oldr)
{
    return dup_mem(oldr);
}

bool_t diff_reg(mem_t *oldr, mem_t *newr, FILE *outfile)
{
    long_t pos;
    int len = oldr->len;
    bool_t diff = FALSE;
    
    if (newr->len < len)
	    len = newr->len;
    
    for (pos = 0; (!diff || outfile) && pos < len; pos += 8) {
        long_t ov = 0;
        long_t nv = 0;
        get_long_val(oldr, pos, &ov);
        get_long_val(newr, pos, &nv);
        if (nv != ov) {
            diff = TRUE;
            if (outfile)
                fprintf(outfile, "%s:\t0x%.16lx\t0x%.16lx\n",
                        reg_table[pos/8].name, ov, nv);
        }
    }
    return diff;
}

/* create an y64 image with registers and memory */
y64sim_t *new_y64sim(int slen)
{
    y64sim_t *sim = (y64sim_t*)malloc(sizeof(y64sim_t));
    sim->pc = 0;
    sim->r = init_reg();
    sim->m = init_mem(slen);
    sim->cc = DEFAULT_CC;
    return sim;
}

void free_y64sim(y64sim_t *sim)
{
    free_reg(sim->r);
    free_mem(sim->m);
    free((void *) sim);
}

/* load binary code and data from file to memory image */
int load_binfile(mem_t *m, FILE *f)
{
    int flen;

    clearerr(f);
    flen = fread(m->data, sizeof(byte_t), m->len, f);
    if (ferror(f)) {
        err_print("fread() failed (0x%x)", flen);
        return -1;
    }
    if (!feof(f)) {
        err_print("too large memory footprint (0x%x)", flen);
        return -1;
    }
    return 0;
}

/*
 * compute_alu: do ALU operations 
 * args
 *     op: operations (A_ADD, A_SUB, A_AND, A_XOR)
 *     argA: the first argument 
 *     argB: the second argument
 *
 * return
 *     val: the result of operation on argA and argB
 */
long_t compute_alu(alu_t op, long_t argA, long_t argB)
{
    long_t val = 0;
    switch(op)
	{
		case A_ADD:
			val = argA + argB;break;
		case A_SUB:
			val= argB - argA;break;
		case A_AND:
			val= argA & argB;break;
		case A_XOR:
			val = argA ^ argB;break;
		case A_NONE:
			val =0;
	} 
    return val;
}

/*
 * compute_cc: modify condition codes according to operations 
 * args
 *     op: operations (A_ADD, A_SUB, A_AND, A_XOR)
 *     argA: the first argument 
 *     argB: the second argument
 *     val: the result of operation on argA and argB
 *
 * return
 *     PACK_CC: the final condition codes
 */
cc_t compute_cc(alu_t op, long_t argA, long_t argB, long_t val)
{
    bool_t zero = (val == 0);
    bool_t sign = ((int)val < 0);  //why turn to int first??
    bool_t ovf = FALSE;

	switch(op)
	{
		case A_ADD:
			if((((int)argA<0) ==((int) argB<0)) && (((int)val <0) !=((int) argA<0))) 
            {
                ovf = TRUE;
            }
			break;
		case A_SUB:
			if((((int)argA<0) !=((int) argB<0)) && (((int)val <0) ==((int) argA<0))) 
            {
                ovf=TRUE;
            }
			break;
		case A_AND:
			ovf = FALSE;
			break;
		case A_XOR:
			ovf = FALSE;
			break;
		case A_NONE:
			ovf =FALSE;
			break;
	}
    return PACK_CC(zero,sign,ovf);
}

/*
 * cond_doit: whether do (mov or jmp) it?  
 * args
 *     PACK_CC: the current condition codes
 *     cond: conditions (C_YES, C_LE, C_L, C_E, C_NE, C_GE, C_G)
 *
 * return
 *     TRUE: do it
 *     FALSE: not do it
 */
bool_t cond_doit(cc_t cc, cond_t cond) 
{
    bool_t doit = FALSE;
	byte_t o= GET_OF(cc);
	byte_t s=GET_SF(cc);
	byte_t z=GET_ZF(cc);

	switch(cond)
	{
		case C_YES:
			doit = TRUE;
			break;
		case C_LE:
			if((s ^ o) | z) 
            doit =TRUE;
			break;
		case C_L:
			if(s^o)
            doit=TRUE;
			break;
		case C_E:
			if(z)
            doit=TRUE;
			break;
		case C_NE:
			if(!z)
            doit=TRUE;
			break;
		case C_GE:
			if(!(s^o))
            doit=TRUE;
			break;
		case C_G:
			if(!(s^o)&!z)
            doit=TRUE;
			break;
			
	}
    return doit;
}

/* 
 * nexti: execute single instruction and return status.
 * args
 *     sim: the y64 image with PC, register and memory
 *
 * return
 *     STAT_AOK: continue
 *     STAT_HLT: halt
 *     STAT_ADR: invalid instruction address
 *     STAT_INS: invalid instruction, register id, data address, stack address, ...
 */
stat_t nexti(y64sim_t *sim)
{
    byte_t codefun = 0; /* 1 byte */
    itype_t icode;
    alu_t ifun;
	cond_t con;
    long_t next_pc = sim->pc;
	byte_t reg=0;
	regid_t regA=0,regB=0;
	long_t imm=0;
    long_t memdata=0;
	long_t *memory = &memdata;
	long_t ans=0;
	long_t rsp;
	long_t valA=0;
	    
    /* get code and function （1 byte) */
    if (!get_byte_val(sim->m, next_pc, &codefun)) {
        err_print("PC = 0x%lx, Invalid instruction address", sim->pc);
        return STAT_ADR;
    }
    icode = GET_ICODE(codefun);
    ifun = GET_FUN(codefun);
	con = GET_FUN(codefun);
    next_pc++;

    /* get registers if needed (1 byte) */
    if((icode==I_RRMOVQ) || (icode==I_IRMOVQ) || (icode==I_RMMOVQ) || (icode==I_MRMOVQ) || (icode == I_ALU)||(icode==I_POPQ)||(icode==I_PUSHQ))
	{
		if(!get_byte_val(sim->m,next_pc,&reg))
		{
			err_print("PC = 0x%lx,Invalid instruction address",sim->pc);
			return STAT_ADR;
		}
		regA=GET_REGA(reg);
		regB=GET_REGB(reg);
		next_pc++;
	}

    /* get immediate if needed (8 bytes) */
    if((icode==I_IRMOVQ)||(icode==I_RMMOVQ)||(icode==I_MRMOVQ)||(icode==I_JMP)||(icode==I_CALL)){
		if(!get_long_val(sim->m,next_pc,&imm)){
			err_print("PC = 0x%lx,Invalid instruction address",sim->pc);
			return STAT_ADR;
		}
		next_pc+=8;
	}

    /* execute the instruction*/
    switch (icode) {
      case I_HALT: /* 0:0 */
	    return STAT_HLT;
	    break;

      case I_NOP: /* 1:0 */
    	sim->pc = next_pc;
    	break;

      case I_RRMOVQ:  /* 2:x regA:regB */
        if(con> C_G)
        {
            err_print("PC = 0x%lx,Invalid instruction ",sim->pc);
			return STAT_INS;
        }
        if(cond_doit(sim->cc,con))
		{
			set_reg_val(sim->r,regB,get_reg_val(sim->r,regA));
		
		}
		sim->pc =next_pc;
		break;

      case I_IRMOVQ: /* 3:0 F:regB imm */
		set_reg_val(sim->r,regB,imm);
		sim->pc=next_pc;
		break;

      case I_RMMOVQ: /* 4:0 regA:regB imm */
		if(!set_long_val(sim->m,imm+get_reg_val(sim->r,regB),get_reg_val(sim->r,regA))){
			err_print("PC = 0x%lx,Invalid data address",sim->pc);
			return STAT_ADR;
		}
		sim->pc = next_pc;
		break;

      case I_MRMOVQ: /* 5:0 regB:regA imm */
		
        if(!get_long_val(sim->m,imm + get_reg_val(sim->r,regB),memory))
        {
			err_print("PC = 0x%lx, Invalid data address 0x%lx",sim->pc ,imm+get_reg_val(sim->r,regB));
			return STAT_ADR;
		}
		set_reg_val(sim->r,regA,*memory);
		sim->pc = next_pc;
		break;

      case I_ALU: /* 6:x regA:regB */
		if(ifun>=A_NONE)
		{
			err_print("PC = 0x%lx,Invalid instruction ",sim->pc);
			return STAT_INS;
		}
		ans = compute_alu(ifun,get_reg_val(sim->r,regA),get_reg_val(sim->r,regB));
		set_reg_val(sim->r, regB,ans);
		sim->cc = compute_cc(ifun,get_reg_val(sim->r,regA),get_reg_val(sim->r,regB),ans);
		sim->pc = next_pc;
		break;

      case I_JMP: /* 7:x imm */
		if(con > C_G)
		{
			err_print("PC = 0x%lx,con = %u,Invalid instruction ",sim->pc,con);
			return STAT_INS;
		}
		if(cond_doit(sim->cc,con))
		{
			if(imm < 0 || imm  + 8 > sim->m->len)
			{
				err_print("PC = 0x%lx,addr = 0x%lx,Invalid jump address",sim->pc, imm);
				return STAT_ADR;
			}
			sim->pc = imm;
		}
		else
			sim->pc = next_pc;
	

		break;

      case I_CALL: /* 8:x imm */
	    if(imm < 0 || imm  + 8 > sim->m->len)
		{
			err_print("PC = 0x%lx,addr = 0x%lx,Invalid call address",sim->pc,imm);
			return STAT_ADR;
		}
		rsp=get_reg_val(sim->r,REG_RSP);
		if(!set_long_val(sim->m,rsp-8,next_pc))
		{
			err_print("PC = 0x%lx, Invalid stack address 0x%lx",sim->pc,rsp-8);
            set_reg_val(sim->r,REG_RSP,rsp-8);//?????????
			return STAT_ADR;
		}
		set_reg_val(sim->r,REG_RSP,rsp-8);
		sim->pc = imm;

		break;

      case I_RET: /* 9:0 */
		rsp=get_reg_val(sim->r,REG_RSP);
		set_reg_val(sim->r,REG_RSP,rsp+8);

		if(!get_long_val(sim->m,rsp,&memdata))
		{
			err_print("PC = 0x%lx,addr(rsp) = 0x%lx,Invalid stack address",sim->pc, rsp);
			return STAT_ADR;
		}

		sim->pc = memdata;
		break;


      case I_PUSHQ: /* A:0 regA:F */
		rsp=get_reg_val(sim->r,REG_RSP);
		
		valA = get_reg_val(sim->r,regA);
		if(!set_long_val(sim->m,rsp-8,valA))
		{
			err_print("PC = 0x%lx, Invalid stack address 0x%lx",sim->pc,rsp-8);
             set_reg_val(sim->r,REG_RSP,rsp-8);
			return STAT_ADR;
		}
        set_reg_val(sim->r,REG_RSP,rsp-8);
		sim->pc = next_pc;
		break;

			
		case I_POPQ: /* B:0 regA:F */
			valA = get_reg_val(sim->r,REG_RSP);
			set_reg_val(sim->r,REG_RSP,valA+8);
			if(!get_long_val(sim->m ,valA,&memdata))
			{
				err_print("PC = 0x%lx, addr(rsp) = 0x%lx ,Invalid stack address", sim->pc,valA);
				return STAT_ADR;
			}
			set_reg_val(sim->r , regA, memdata);
			sim->pc=next_pc;
			break;

      default:
    	err_print("PC = 0x%lx, Invalid instruction %.2x", sim->pc, codefun);
    	return STAT_INS;
    }
    
    return STAT_AOK;
}

void usage(char *pname)
{
    printf("Usage: %s file.bin [max_steps]\n", pname);
    exit(0);
}

int main(int argc, char *argv[])
{
    FILE *binfile;
    int max_steps = MAX_STEP;
    y64sim_t *sim;
    mem_t *saver, *savem;
    int step = 0;
    stat_t e = STAT_AOK;

    if (argc < 2 || argc > 3)
        usage(argv[0]);

    /* set max steps */
    if (argc > 2)
        max_steps = atoi(argv[2]);

    /* load binary file to memory */
    if (strcmp(argv[1]+(strlen(argv[1])-4), ".bin"))
        usage(argv[0]); /* only support *.bin file */
    
    binfile = fopen(argv[1], "rb");
    if (!binfile) {
        err_print("Can't open binary file '%s'", argv[1]);
        exit(1);
    }

    sim = new_y64sim(MEM_SIZE);
    if (load_binfile(sim->m, binfile) < 0) {
        err_print("Failed to load binary file '%s'", argv[1]);
        free_y64sim(sim);
        exit(1);
    }
    fclose(binfile);

    /* save initial register and memory stat */
    saver = dup_reg(sim->r);
    savem = dup_mem(sim->m);

    /* execute binary code step-by-step */
    for (step = 0; step < max_steps && e == STAT_AOK; step++)
        e = nexti(sim);

    /* print final stat of y64sim */
    printf("Stopped in %d steps at PC = 0x%lx.  Status '%s', CC %s\n",
            step, sim->pc, stat_name(e), cc_name(sim->cc));

    printf("Changes to registers:\n");
    diff_reg(saver, sim->r, stdout);

    printf("\nChanges to memory:\n");
    diff_mem(savem, sim->m, stdout);

    free_y64sim(sim);
    free_reg(saver);
    free_mem(savem);

    return 0;
}

;
