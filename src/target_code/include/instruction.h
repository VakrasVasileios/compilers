#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "../../../util/contract/include/contract.h"
#include "vm_arg.h"
#include <ostream>

namespace target_code 
{
/**
 * @brief A virtual machine opcode.
 * 
 */
enum Vmopcode : uint8_t {
    ASSIGN_VM,

    ADD_VM,
    SUB_VM,
    MUL_VM,
    DIV_VM,
    MOD_VM,

    JEQ_VM,
    JNE_VM,
    JGT_VM,
    JLT_VM,
    JGE_VM,
    JLE_VM,

    JUMP_VM,

    CALLFUNC_VM,
    PUSHARG_VM,
    ENTERFUNC_VM,
    EXITFUNC_VM,

    NEWTABLE_VM,
    TABLEGETELEM_VM,
    TABLESETELEM_VM
};    

class InstructionVisitor; // fwd declare

/**
 * @brief A virtual machine instruction.
 * 
 */
class Instruction {
public:    
    /**
     * @brief Constructs a new instruction object.
     * 
     * @param label the label of the new instruction object,
     * greater or equal to zero 
     * @param result the result of the new instruction object, not null
     * @param arg1 the arg1 of the new instruction object
     * @param arg2 the arg2 of the new instruction object
     * @param src_line  the source line, at which the quad, based on which the
     * new instruction object is generated, was emitted, greater than zero
     */
    Instruction(const unsigned int label, Vmarg* result,
    const Vmarg* arg1, const Vmarg* arg2, const unsigned int src_line)
    : label(VerifyLabel(label)), result(VerifyResult(result)), arg1(arg1),
    arg2(arg2), src_line(VerifySrcLine(src_line)) {}
    /**
     * @brief Destroys this Instruction object.
     * 
     */
    virtual ~Instruction() = default;

    friend std::ostream &operator<<(std::ostream &os, const Instruction* instr);
    /**
     * @brief Returns a read access to this Instruction label.
     * 
     * @return a read access to this Instruction label,
     * greater or equal to zero
     */
    const unsigned int      get_label() const;
    /**
     * @brief Returns a read access to this Instruction result.
     * 
     * @return a read access to this Instruction result,
     * not null 
     */
    Vmarg*                  get_result() const;
    /**
     * @brief Returns a read access to this Instruction arg1.
     * 
     * @return a read access to this Instruction arg1,
     * can be null 
     */
    const Vmarg*            get_arg1() const;
    /**
     * @brief Returns a read access to this Instruction arg2.
     * 
     * @return a read access to this Instruction arg2,
     * can be null 
     */
    const Vmarg*            get_arg2() const;
    /**
     * @brief Returns a read access to this Instruction
     * source line.
     * 
     * @return a read access to this Instruction
     * source line, greater or equal to zero 
     */
    const unsigned int      get_src_line() const;
    /**
     * @brief Returns a read access to this Instruction vm
     * opcode.
     * 
     * @return a read access to this Instruction vm
     * opcode
     */
    virtual uint8_t         get_opcode() const = 0;
    /**
     * @brief Accepts an instruction visitor at this Instruction.
     * 
     * @param visitor the instruction visitor to be accepted at this Instruction
     * , not null
     */
    virtual void            Accept(const InstructionVisitor* visitor) = 0;
protected:
    /**
     * @brief Checks the state of this Instruction.
     * 
     * @return  wether the state of this Instruction is valid.
     */
    virtual bool            IsStateValid() const = 0;
    /**
     * @brief Verifies wether a vm argument is not null
     * 
     * @param arg the vm argument to be verified,
     * not null
     * @return the vm argument to be verified,
     * not null
     */
    const Vmarg*            VerifyArg(const Vmarg* arg) const;
    /** 
     * @brief Logs this Instruction to an output.
     * 
     * @param os the output to log this instruction
     * @return the output to log this instruction
     */
    virtual std::ostream&   LogInstruction(std::ostream &os) const = 0;
private:    
    const unsigned int      label;
    Vmarg*                  result;
    const Vmarg*            arg1;
    const Vmarg*            arg2;
    const unsigned int      src_line;

    const unsigned int      VerifyLabel(const unsigned int label) const;
    Vmarg*                  VerifyResult(Vmarg* result) const;
    const unsigned int      VerifySrcLine(const unsigned int src_line) const;
};

/**
 * @brief An assignment instruction.
 * 
 */
class Assign final : public Instruction {
public:
    /**
     * @brief Constructs a new Assign instruction object.
     * 
     * @param label the label of the new Assign instruction object,
     * greater or equal to zero 
     * @param result the result of the new Assign instruction object, not null
     * @param arg1 the arg1 of the new Assign instruction object, can be null
     * @param src_line the source line, at which the quad, based on which the
     * new Assign instruction object is generated, was emitted,
     * greater than zero
     */
    Assign(const unsigned int label, Vmarg* result, const Vmarg* arg1, 
    const unsigned int src_line)
    : Instruction(label, result, arg1, nullptr, src_line) 
    {INVARIANT(IsStateValid());}
    /**
     * @brief Destroys this Assign object.
     * 
     */
    ~Assign() = default;
    uint8_t         get_opcode() const override;
    void            Accept(const InstructionVisitor* visitor) override;
private:
    std::ostream&   LogInstruction(std::ostream &os) const override;  
    bool            IsStateValid() const override;  
};

/**
 * @brief An addition instruction.
 * 
 */
class Add final : public Instruction {
public:
    /**
     * @brief Constructs a new Add instruction object.
     * 
     * @param label the label of the new Add instruction object,
     * greater or equal to zero 
     * @param result the result of the new Add instruction object, not null
     * @param arg1 the arg1 of the new Add instruction object, not null
     * @param arg2 the arg2 of the new Add instruction object, not null
     * @param src_line  the source line, at which the quad, based on which the
     * new Add instruction object is generated, was emitted, greater than zero
     */
    Add(const unsigned int label, Vmarg* result, const Vmarg* arg1,
    const Vmarg* arg2, const unsigned int src_line) : Instruction(label, result,
    VerifyArg(arg1), VerifyArg(arg2), src_line) {INVARIANT(IsStateValid());}    
    /**
     * @brief Destroys this Add object.
     * 
     */
    ~Add() = default;
    uint8_t         get_opcode() const override;
    void            Accept(const InstructionVisitor* visitor) override;
private:
    std::ostream&   LogInstruction(std::ostream &os) const override;    
    bool            IsStateValid() const override;
};

/**
 * @brief A subtraction instruction.
 * 
 */
class Sub final : public Instruction {
public:
    /**
     * @brief Constructs a new Sub instruction object.
     * 
     * @param label the label of the new Sub instruction object,
     * greater or equal to zero 
     * @param result the result of the new Sub instruction object, not null
     * @param arg1 the arg1 of the new Sub instruction object, not null
     * @param arg2 the arg2 of the new Sub instruction object, not null
     * @param src_line the source line, at which the quad, based on which the
     * new Sub instruction object is generated, was emitted, greater than zero
     */
    Sub(const unsigned int label, Vmarg* result, const Vmarg* arg1,
    const Vmarg* arg2, const unsigned int src_line) : Instruction(label, result,
    VerifyArg(arg1), VerifyArg(arg2), src_line) {INVARIANT(IsStateValid());}
    /**
     * @brief Destroys this Sub object.
     * 
     */
    ~Sub() = default;
    uint8_t         get_opcode() const override;
    void            Accept(const InstructionVisitor* visitor) override;
private:
    std::ostream&   LogInstruction(std::ostream &os) const override;    
    bool            IsStateValid() const override;
};

/**
 * @brief A multiplication instruction.
 * 
 */
class Mul final : public Instruction {
public:
    /**
     * @brief Constructs a new Mul instruction object.
     * 
     * @param label the label of the new Mul instruction object,
     * greater or equal to zero 
     * @param result the result of the new Mul instruction object, not null
     * @param arg1 the arg1 of the new Mul instruction object, not null
     * @param arg2 the arg2 of the new Mul instruction object, not null
     * @param src_line the source line, at which the quad, based on which the
     * new Mul instruction object is generated, was emitted, greater than zero
     */
    Mul(const unsigned int label, Vmarg* result, const Vmarg* arg1,
    const Vmarg* arg2, const unsigned int src_line) : Instruction(label, result,
    VerifyArg(arg1), VerifyArg(arg2), src_line) {INVARIANT(IsStateValid());}
    /**
     * @brief Destroys this Mul object.
     * 
     */
    ~Mul() = default;
    uint8_t         get_opcode() const override;
    void            Accept(const InstructionVisitor* visitor) override;
private:
    std::ostream&   LogInstruction(std::ostream &os) const override;  
    bool            IsStateValid() const override;  
};

/**
 * @brief A division instruction.
 * 
 */
class Div final : public Instruction {
public:
    /**
     * @brief Constructs a new Div instruction object.
     * 
     * @param label the label of the new Div instruction object,
     * greater or equal to zero 
     * @param result the result of the new Div instruction object, not null
     * @param arg1 the arg1 of the new Div instruction object, not null
     * @param arg2 the arg2 of the new Div instruction object, not null
     * @param src_line the source line, at which the quad, based on which the
     * new Div instruction object is generated, was emitted, greater than zero
     */
    Div(const unsigned int label, Vmarg* result, const Vmarg* arg1,
    const Vmarg* arg2, const unsigned int src_line)
    : Instruction(label, result, VerifyArg(arg1), VerifyArg(arg2), src_line)
    {INVARIANT(IsStateValid());}
    /**
     * @brief Destroys this Div object.
     * 
     */
    ~Div() = default;
    uint8_t         get_opcode() const override;
    void            Accept(const InstructionVisitor* visitor) override;
private:
    std::ostream&   LogInstruction(std::ostream &os) const override;   
    bool            IsStateValid() const override; 
};

/**
 * @brief A modulation instruction.
 * 
 */
class Mod final : public Instruction {
public:
    /**
     * @brief Constructs a new Mod instruction object.
     * 
     * @param label the label of the new Mod instruction object,
     * greater or equal to zero 
     * @param result the result of the new Mod instruction object, not null
     * @param arg1 the arg1 of the new Mod instruction object, not null
     * @param arg2 the arg2 of the new Mod instruction object, not null
     * @param src_line the source line, at which the quad, based on which the
     * new Mod instruction object is generated, was emitted, greater than zero
     */
    Mod(const unsigned int label, Vmarg* result, const Vmarg* arg1,
    const Vmarg* arg2, const unsigned int src_line)
    : Instruction(label, result, arg1, arg2, src_line) 
    {INVARIANT(IsStateValid());}
    /**
     * @brief Destroys this Mod object.
     * 
     */
    ~Mod() = default;
    uint8_t         get_opcode() const override;
    void            Accept(const InstructionVisitor* visitor) override;
private:
    std::ostream&   LogInstruction(std::ostream &os) const override;    
    bool            IsStateValid() const override;
};

/**
 * @brief A jump if equal instruction.
 * 
 */
class Jeq final : public Instruction {
public:
    /**
     * @brief Constructs a new Jeq instruction object.
     * 
     * @param label the label of the new Jeq instruction object,
     * greater or equal to zero 
     * @param result the result of the new Jeq instruction object, not null
     * @param arg1 the arg1 of the new Jeq instruction object, not null
     * @param arg2 the arg2 of the new Jeq instruction object, not null
     * @param src_line the source line, at which the quad, based on which the
     * new Jeq instruction object is generated, was emitted, greater than zero
     */
    Jeq(const unsigned int label, Vmarg* result, const Vmarg* arg1,
    const Vmarg* arg2, const unsigned int src_line)
    : Instruction(label, result, VerifyArg(arg1), VerifyArg(arg2), src_line)
    {INVARIANT(IsStateValid());}
    /**
     * @brief Destroys this Jeq object.
     * 
     */
    ~Jeq() = default;
    uint8_t         get_opcode() const override;
    void            Accept(const InstructionVisitor* visitor) override;
private:
    std::ostream&   LogInstruction(std::ostream &os) const override;
    bool            IsStateValid() const override;
};

/**
 * @brief A jump if not equal instruction.
 * 
 */
class Jne final : public Instruction {
public:
    /**
     * @brief Constructs a new Jne instruction object.
     * 
     * @param label     the label of the new Jne instruction object,
     * greater or equal to zero  
     * @param result    the result of the new Jne instruction object, not null
     * @param arg1      the arg1 of the new Jne instruction object, not null
     * @param arg2      the arg2 of the new Jne instruction object, not null
     * @param src_line  the source line, at which the quad, based on which the
     * new Jne instruction object is generated, was emitted, greater than zero
     */
    Jne(const unsigned int label, Vmarg* result, const Vmarg* arg1,
    const Vmarg* arg2, const unsigned int src_line)
    : Instruction(label, result, VerifyArg(arg1), VerifyArg(arg2), src_line)
    {INVARIANT(IsStateValid());}
    /**
     * @brief Destroys this Jne object.
     * 
     */
    ~Jne() = default;
    uint8_t         get_opcode() const override;
    void            Accept(const InstructionVisitor* visitor) override;
private:
    std::ostream&   LogInstruction(std::ostream &os) const override;
    bool            IsStateValid() const override;
};

/**
 * @brief A jump if greater than instruction.
 * 
 */
class Jgt final : public Instruction {
public:
    /**
     * @brief Constructs a new Jgt instruction object.
     * 
     * @param label the label of the new Jgt instruction object,
     * greater or equal to zero 
     * @param result the result of the new Jgt instruction object, not null
     * @param arg1 the arg1 of the new Jgt instruction object, not null
     * @param arg2 the arg2 of the new Jgt instruction object, not null
     * @param src_line the source line, at which the quad, based on which the
     * new Jgt instruction object is generated, was emitted, greater than zero
     */
    Jgt(const unsigned int label, Vmarg* result, const Vmarg* arg1,
    const Vmarg* arg2, const unsigned int src_line)
    : Instruction(label, result, VerifyArg(arg1), VerifyArg(arg2), src_line)
    {INVARIANT(IsStateValid());}
    /**
     * @brief Destroys this Jgt object.
     * 
     */
    ~Jgt() = default;
    uint8_t         get_opcode() const override;
    void            Accept(const InstructionVisitor* visitor) override;
private:
    std::ostream&   LogInstruction(std::ostream &os) const override;  
    bool            IsStateValid() const override;  
};

/**
 * @brief A jump if less than instruction.
 * 
 */
class Jlt final : public Instruction {
public:
    /**
     * @brief Constructs a new Jlt instruction object.
     * 
     * @param label the label of the new Jlt instruction object,
     * greater or equal to zero 
     * @param result the result of the new Jlt instruction object, not null
     * @param arg1 the arg1 of the new Jlt instruction object, not null
     * @param arg2 the arg2 of the new Jlt instruction object, not null
     * @param src_line  the source line, at which the quad, based on which the
     * new Jlt instruction object is generated, was emitted, greater than zero
     */
    Jlt(const unsigned int label, Vmarg* result, const Vmarg* arg1,
    const Vmarg* arg2, const unsigned int src_line)
    : Instruction(label, result, VerifyArg(arg1), VerifyArg(arg2), src_line)
    {INVARIANT(IsStateValid());}
    /**
     * @brief Destroys this Jlt object.
     * 
     */
    ~Jlt() = default;
    uint8_t         get_opcode() const override;
    void            Accept(const InstructionVisitor* visitor) override;
private:
    std::ostream&   LogInstruction(std::ostream &os) const override; 
    bool            IsStateValid() const override;   
};

/**
 * @brief A jump if greater equal than instruction.
 * 
 */
class Jge final : public Instruction {
public:
    /**
     * @brief Constructs a new Jge instruction object.
     * 
     * @param label     the label of the new Jge instruction object,
     * greater or equal to zero 
     * @param result    the result of the new Jge instruction object, not null
     * @param arg1      the arg1 of the new Jge instruction object, not null
     * @param arg2      the arg2 of the new Jge instruction object, not null
     * @param src_line  the source line, at which the quad, based on which the
     * new Jge instruction object is generated, was emitted, greater than zero
     */
    Jge(const unsigned int label, Vmarg* result, const Vmarg* arg1,
    const Vmarg* arg2, const unsigned int src_line)
    : Instruction(label, result, VerifyArg(arg1), VerifyArg(arg2), src_line) 
    {INVARIANT(IsStateValid());}
    /**
     * @brief Destroys this Jge object.
     * 
     */
    ~Jge() = default;
    uint8_t         get_opcode() const override;
    void            Accept(const InstructionVisitor* visitor) override;
private:
    std::ostream&   LogInstruction(std::ostream &os) const override; 
    bool            IsStateValid() const override;   
};

/**
 * @brief A jump less equal than instruction.
 * 
 */
class Jle final : public Instruction {
public:
    /**
     * @brief Constructs a new Jle instruction object.
     * 
     * @param label     the label of the new Jle instruction object,
     * greater or equal to zero 
     * @param result    the result of the new Jle instruction object, not null
     * @param arg1      the arg1 of the new Jle instruction object, not null
     * @param arg2      the arg2 of the new Jle instruction object, not null
     * @param src_line  the source line, at which the quad, based on which the
     * new Jle instruction object is generated, was emitted, greater than zero
     */
    Jle(const unsigned int label, Vmarg* result, const Vmarg* arg1,
    const Vmarg* arg2, const unsigned int src_line)
    : Instruction(label, result, VerifyArg(arg1), VerifyArg(arg2), src_line)
    {INVARIANT(IsStateValid());}
    /**
     * @brief Destroys this Jle object.
     * 
     */
    ~Jle() = default;
    uint8_t         get_opcode() const override;
    void            Accept(const InstructionVisitor* visitor) override;
private:
    std::ostream&   LogInstruction(std::ostream &os) const override; 
    bool            IsStateValid() const override;   
};

/**
 * @brief A jump instruction.
 * 
 */
class Jump final : public Instruction {
public:
    /**
     * @brief Constructs a new Jump instruction object.
     * 
     * @param label     the label of the new Jump instruction object,
     * greater or equal to zero 
     * @param result    the result of the new Jump instruction object, not null
     * @param src_line  the source line, at which the quad, based on which the
     * new Jump instruction object is generated, was emitted, greater than zero
     */
    Jump(const unsigned int label, Vmarg* result,
    const unsigned int src_line)
    : Instruction(label, result, nullptr, nullptr, src_line) 
    {INVARIANT(IsStateValid());}
    /**
     * @brief Destroys this Jump object.
     * 
     */
    ~Jump() = default;
    uint8_t         get_opcode() const override;
    void            Accept(const InstructionVisitor* visitor) override;
private:
    std::ostream&   LogInstruction(std::ostream &os) const override;  
    bool            IsStateValid() const override;  
};

/**
 * @brief A function call instruction.
 * 
 */
class CallFunc final : public Instruction {
public:
    /**
     * @brief Constructs a new CallFunc instruction object.
     * 
     * @param label the label of the new CallFunc instruction object,
     * greater or equal to zero 
     * @param result the result of the new CallFunc instruction object, not null
     * @param src_line the source line, at which the quad, based on which the
     * new CallFunc instruction object is generated, was emitted,
     * greater than zero
     */
    CallFunc(const unsigned int label, Vmarg* result,
    const unsigned int src_line)
    : Instruction(label, result, nullptr, nullptr, src_line)
    {INVARIANT(IsStateValid());}
    /**
     * @brief Destroys this Call Func object.
     * 
     */
    ~CallFunc() = default;
    uint8_t         get_opcode() const override;
    void            Accept(const InstructionVisitor* visitor) override;
private:
    std::ostream&   LogInstruction(std::ostream &os) const override;    
    bool            IsStateValid() const override;
};

/**
 * @brief A push call argument instruction.
 * 
 */
class PushArg final : public Instruction {
public:
    /**
     * @brief Constructs a new PushArg instruction object.
     * 
     * @param label the label of the new PushArg instruction object,
     * greater or equal to zero 
     * @param result the result of the new PushArg instruction object, not null
     * @param src_line the source line, at which the quad, based on which the
     * new PushArg instruction object is generated, was emitted,
     * greater than zero
     */
    PushArg(const unsigned int label, Vmarg* result,
    const unsigned int src_line)
    : Instruction(label, result, nullptr, nullptr, src_line)
    {INVARIANT(IsStateValid());}
    uint8_t         get_opcode() const override;
    void            Accept(const InstructionVisitor* visitor) override;
private:
    std::ostream&   LogInstruction(std::ostream &os) const override;    
    bool            IsStateValid() const override;
};

/**
 * @brief A enter function instruction.
 * 
 */
class EnterFunc final : public Instruction {
public:
    /**
     * @brief Constructs a new EnterFunc instruction object.
     * 
     * @param label the label of the new EnterFunc instruction object,
     * greater or equal to zero 
     * @param result the result of the new EnterFunc instruction object,
     * not null
     * @param src_line the source line, at which the quad, based on which the
     * new EnterFunc instruction object is generated, was emitted,
     * greater than zero
     */
    EnterFunc(const unsigned int label, Vmarg* result,
    const unsigned int src_line)
    : Instruction(label, result, nullptr, nullptr, src_line) 
    {INVARIANT(IsStateValid());}
    /**
     * @brief Destroys this EnterFunc object.
     * 
     */
    ~EnterFunc() = default;
    uint8_t         get_opcode() const override;
    void            Accept(const InstructionVisitor* visitor) override;
private:
    std::ostream&   LogInstruction(std::ostream &os) const override;    
    bool            IsStateValid() const override;
};

/**
 * @brief An exit function instruction.
 * 
 */
class ExitFunc final : public Instruction {
public:
    /**
     * @brief Constructs a new ExitFunc instruction object.
     * 
     * @param label the label of the new ExitFunc instruction object,
     * greater or equal to zero 
     * @param result the result of the new ExitFunc instruction object,
     * not null
     * @param src_line the source line, at which the quad, based on which the
     * new ExitFunc instruction object is generated, was emitted,
     * greater than zero
     */
    ExitFunc(const unsigned int label, Vmarg* result, 
    const unsigned int src_line)
    : Instruction(label, result, nullptr, nullptr, src_line) 
    {INVARIANT(IsStateValid());}
    /**
     * @brief Destroys this ExitFunc object.
     * 
     */
    ~ExitFunc() = default;
    uint8_t         get_opcode() const override;
    void            Accept(const InstructionVisitor* visitor) override;
private:
    std::ostream&   LogInstruction(std::ostream &os) const override;    
    bool            IsStateValid() const override;
};

/**
 * @brief A table create instruction.
 * 
 */
class NewTable final : public Instruction {
public:
    /**
     * @brief Constructs a new NewTable instruction object.
     * 
     * @param label the label of the new NewTable instruction object,
     * greater or equal to zero 
     * @param result the result of the new NewTable instruction object, not null
     * @param src_line the source line, at which the quad, based on which the
     * new NewTable instruction object is generated, was emitted,
     * greater than zero
     */
    NewTable(const unsigned int label, Vmarg* result,
    const unsigned int src_line)
    : Instruction(label, result, nullptr, nullptr, src_line) 
    {INVARIANT(IsStateValid());}
    /**
     * @brief Destroys this NewTable object.
     * 
     */
    ~NewTable() = default;
    uint8_t         get_opcode() const override;
    void            Accept(const InstructionVisitor* visitor) override;
private:
    std::ostream&   LogInstruction(std::ostream &os) const override;    
    bool            IsStateValid() const override;
};

/**
 * @brief A table get element instruction.
 * 
 */
class TableGetElem final : public Instruction {
public:
    /**
     * @brief Constructs a new TableGetElem instruction object.
     * 
     * @param label the label of the new TableGetElem instruction object,
     * greater or equal to zero 
     * @param result the result of the new TableGetElem instruction object,
     * not null
     * @param arg1 the arg1 of the new TableGetElem instruction object, not null
     * @param arg2 the arg2 of the new TableGetElem instruction object, not null
     * @param src_line the source line, at which the quad, based on which the
     * new TableGetElem instruction object is generated, was emitted,
     * greater than zero
     */
    TableGetElem(const unsigned int label, Vmarg* result,
    const Vmarg* arg1, const Vmarg* arg2, const unsigned int src_line)
    : Instruction(label, result, VerifyArg(arg1), VerifyArg(arg2), src_line)
    {INVARIANT(IsStateValid());}
    /**
     * @brief Destroys this TableGetElem object.
     * 
     */
    ~TableGetElem() = default;
    uint8_t         get_opcode() const override;
    void            Accept(const InstructionVisitor* visitor) override;
private:
    std::ostream&   LogInstruction(std::ostream &os) const override;    
    bool            IsStateValid() const override;
};

/**
 * @brief A table set element instruction.
 * 
 */
class TableSetElem final : public Instruction {
public:
    /**
     * @brief Constructs a new TableSetElem instruction object.
     * 
     * @param label the label of the new TableSetElem instruction object,
     * greater or equal to zero 
     * @param result the result of the new TableSetElem instruction object,
     * not null
     * @param arg1 the arg1 of the new TableSetElem instruction object,
     * not null
     * @param arg2 the arg2 of the new TableSetElem instruction object,
     * not null
     * @param src_line the source line, at which the quad, based on which the
     * new Add instruction object is generated, was emitted, greater than zero
     */
    TableSetElem(const unsigned int label, Vmarg* result,
    const Vmarg* arg1, const Vmarg* arg2, const unsigned int src_line)
    : Instruction(label, result, VerifyArg(arg1), VerifyArg(arg2), src_line)
    {INVARIANT(IsStateValid());}
    /**
     * @brief Destroys this TableSetElem object.
     * 
     */
    ~TableSetElem() = default;
    uint8_t         get_opcode() const override;
    void            Accept(const InstructionVisitor* visitor) override;
private:
    std::ostream&   LogInstruction(std::ostream &os) const override;    
    bool            IsStateValid() const override;
};

/**
 * @brief Interface for visiting concrete instructions.
 * 
 */
class InstructionVisitor {
public:
    /**
     * @brief Visits an assign instruction.
     * 
     * @param inst the assign instruction to be visited, not null
     */
    virtual void VisitAssign(Assign* inst) const = 0;
    /**
     * @brief Visits an add instruction.
     * 
     * @param inst the add instruction to be visited, not null
     */
    virtual void VisitAdd(Add* inst) const = 0;
    /**
     * @brief Visits a sub instruction.
     * 
     * @param inst the sub instruction to be visited, not null
     */
    virtual void VisitSub(Sub* inst) const = 0;
    /**
     * @brief Visits a mul instruction.
     * 
     * @param inst the mul instruction to be visited, not null
     */
    virtual void VisitMul(Mul* inst) const = 0;
    /**
     * @brief Visits a div instruction.
     * 
     * @param inst the div instruction to be visited, not null
     */
    virtual void VisitDiv(Div* inst) const = 0;
    /**
     * @brief Visits a mod instruction.
     * 
     * @param inst the mod instruction to be visited, not null
     */
    virtual void VisitMod(Mod* inst) const = 0;
    /**
     * @brief Visits a jeq instruction.
     * 
     * @param inst the jeq instruction to be visited, not null
     */
    virtual void VisitJeq(Jeq* inst) const = 0;
    /**
     * @brief Visits a jne instruction.
     * 
     * @param inst the jne instruction to be visited, not null
     */
    virtual void VisitJne(Jne* inst) const = 0;
    /**
     * @brief Visits a jgt instruction.
     * 
     * @param inst the jgt instruction to be visited, not null
     */
    virtual void VisitJgt(Jgt* inst) const = 0;
    /**
     * @brief Visits a jlt instruction.
     * 
     * @param inst the jlt instruction to be visited, not null
     */
    virtual void VisitJlt(Jlt* inst) const = 0;
    /**
     * @brief Visits a jge instruction.
     * 
     * @param inst the jge instruction to be visited, not null
     */
    virtual void VisitJge(Jge* inst) const = 0;
    /**
     * @brief Visits a jle instruction.
     * 
     * @param inst the jle instruction to be visited, not null
     */
    virtual void VisitJle(Jle* inst) const = 0;
    /**
     * @brief Visits a jump instruction.
     * 
     * @param inst the jump instruction to be visited, not null
     */
    virtual void VisitJump(Jump* inst) const = 0;
    /**
     * @brief Visits a callfunc instruction.
     * 
     * @param inst the callfunc instruction to be visited, not null
     */
    virtual void VisitCallFunc(CallFunc* inst) const = 0;
    /**
     * @brief Visits a pusharg instruction.
     * 
     * @param inst the pusharg instruction to be visited, not null
     */
    virtual void VisitPushArg(PushArg* inst) const = 0;
    /**
     * @brief Visits an enterfunc instruction.
     * 
     * @param inst the enterfunc instruction to be visited, not null
     */
    virtual void VisitEnterFunc(EnterFunc* inst) const = 0;
    /**
     * @brief Visits an exitfunc instruction.
     * 
     * @param inst the exitfunc instruction to be visited, not null
     */
    virtual void VisitExitFunc(ExitFunc* inst) const = 0;
    /**
     * @brief Visits a newtable instruction.
     * 
     * @param inst the newtable instruction to be visited, not null
     */
    virtual void VisitNewTable(NewTable* inst) const = 0;
    /**
     * @brief Visits a tablegetelem instruction.
     * 
     * @param inst the tablegetelem instruction to be visited, not null
     */
    virtual void VisitTableGetElem(TableGetElem* inst) const = 0;
    /**
     * @brief Visits a tablesetelem instruction.
     * 
     * @param inst the tablesetelem instruction to be visited, not null
     */
    virtual void VisitTableSetElem(TableSetElem* inst) const = 0;
};

}

#endif 