#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "../../util/contract/contract.h"
#include "vm_opcodes.h"
#include "vm_arg.h"
#include <ostream>

namespace target_code 
{
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
    Instruction(const unsigned int label, const Vmarg* result,
    const Vmarg* arg1, const Vmarg* arg2, const unsigned int src_line)
    : label(VerifyLabel(label)), result(VerifyResult(result)), arg1(arg1),
    arg2(arg2), src_line(VerifySrcLine(src_line)) {INVARIANT(IsStateValid());}
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
    const Vmarg*            get_result() const;
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
    const Vmarg*            result;
    const Vmarg*            arg1;
    const Vmarg*            arg2;
    const unsigned int      src_line;

    const unsigned int    VerifyLabel(const unsigned int label) const;
    const Vmarg*          VerifyResult(const Vmarg* result) const;
    const unsigned int    VerifySrcLine(const unsigned int src_line) const;
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
     * @param arg1 the arg1 of the new Assign instruction object, not null
     * @param src_line the source line, at which the quad, based on which the
     * new Assign instruction object is generated, was emitted,
     * greater than zero
     */
    Assign(const unsigned int label, const Vmarg* result, const Vmarg* arg1,
    const unsigned int src_line) : Instruction(label, result, VerifyArg(arg1),
    nullptr, src_line) {}
    /**
     * @brief Destroys this Assign object.
     * 
     */
    ~Assign() = default;
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
    Add(const unsigned int label, const Vmarg* result, const Vmarg* arg1,
    const Vmarg* arg2, const unsigned int src_line) : Instruction(label, result,
    VerifyArg(arg1), VerifyArg(arg2), src_line) {}    
    /**
     * @brief Destroys this Add object.
     * 
     */
    ~Add() = default;
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
    Sub(const unsigned int label, const Vmarg* result, const Vmarg* arg1,
    const Vmarg* arg2, const unsigned int src_line) : Instruction(label, result,
    VerifyArg(arg1), VerifyArg(arg2), src_line) {}
    /**
     * @brief Destroys this Sub object.
     * 
     */
    ~Sub() = default;
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
    Mul(const unsigned int label, const Vmarg* result, const Vmarg* arg1,
    const Vmarg* arg2, const unsigned int src_line) : Instruction(label, result,
    VerifyArg(arg1), VerifyArg(arg2), src_line) {}
    /**
     * @brief Destroys this Mul object.
     * 
     */
    ~Mul() = default;
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
    Div(const unsigned int label, const Vmarg* result, const Vmarg* arg1,
    const Vmarg* arg2, const unsigned int src_line)
    : Instruction(label, result, VerifyArg(arg1), VerifyArg(arg2), src_line) {}
    /**
     * @brief Destroys this Div object.
     * 
     */
    ~Div() = default;
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
    Mod(const unsigned int label, const Vmarg* result, const Vmarg* arg1,
    const Vmarg* arg2, const unsigned int src_line)
    : Instruction(label, result, arg1, arg2, src_line) {}
    /**
     * @brief Destroys this Mod object.
     * 
     */
    ~Mod() = default;
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
    Jeq(const unsigned int label, const Vmarg* result, const Vmarg* arg1,
    const Vmarg* arg2, const unsigned int src_line)
    : Instruction(label, result, VerifyArg(arg1), VerifyArg(arg2), src_line) {}
    /**
     * @brief Destroys this Jeq object.
     * 
     */
    ~Jeq() = default;
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
    Jne(const unsigned int label, const Vmarg* result, const Vmarg* arg1,
    const Vmarg* arg2, const unsigned int src_line)
    : Instruction(label, result, VerifyArg(arg1), VerifyArg(arg2), src_line) {}
    /**
     * @brief Destroys this Jne object.
     * 
     */
    ~Jne() = default;
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
    Jgt(const unsigned int label, const Vmarg* result, const Vmarg* arg1,
    const Vmarg* arg2, const unsigned int src_line)
    : Instruction(label, result, VerifyArg(arg1), VerifyArg(arg2), src_line) {}
    /**
     * @brief Destroys this Jgt object.
     * 
     */
    ~Jgt() = default;
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
    Jlt(const unsigned int label, const Vmarg* result, const Vmarg* arg1,
    const Vmarg* arg2, const unsigned int src_line)
    : Instruction(label, result, VerifyArg(arg1), VerifyArg(arg2), src_line) {}
    /**
     * @brief Destroys this Jlt object.
     * 
     */
    ~Jlt() = default;
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
    Jge(const unsigned int label, const Vmarg* result, const Vmarg* arg1,
    const Vmarg* arg2, const unsigned int src_line)
    : Instruction(label, result, VerifyArg(arg1), VerifyArg(arg2), src_line) {}
    /**
     * @brief Destroys this Jge object.
     * 
     */
    ~Jge() = default;
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
    Jle(const unsigned int label, const Vmarg* result, const Vmarg* arg1,
    const Vmarg* arg2, const unsigned int src_line)
    : Instruction(label, result, VerifyArg(arg1), VerifyArg(arg2), src_line) {}
    /**
     * @brief Destroys this Jle object.
     * 
     */
    ~Jle() = default;
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
    Jump(const unsigned int label, const Vmarg* result,
    const unsigned int src_line)
    : Instruction(label, result, nullptr, nullptr, src_line) {}
    /**
     * @brief Destroys this Jump object.
     * 
     */
    ~Jump() = default;
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
    CallFunc(const unsigned int label, const Vmarg* result,
    const unsigned int src_line)
    : Instruction(label, result, nullptr, nullptr, src_line) {}
    /**
     * @brief Destroys this Call Func object.
     * 
     */
    ~CallFunc() = default;
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
    PushArg(const unsigned int label, const Vmarg* result,
    const unsigned int src_line)
    : Instruction(label, result, nullptr, nullptr, src_line) {}
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
    EnterFunc(const unsigned int label, const Vmarg* result,
    const unsigned int src_line)
    : Instruction(label, result, nullptr, nullptr, src_line) {}
    /**
     * @brief Destroys this EnterFunc object.
     * 
     */
    ~EnterFunc() = default;
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
    ExitFunc(const unsigned int label, const Vmarg* result, 
    const unsigned int src_line)
    : Instruction(label, result, nullptr, nullptr, src_line) {}
    /**
     * @brief Destroys this ExitFunc object.
     * 
     */
    ~ExitFunc() = default;
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
    NewTable(const unsigned int label, const Vmarg* result,
    const unsigned int src_line)
    : Instruction(label, result, nullptr, nullptr, src_line) {}
    /**
     * @brief Destroys this NewTable object.
     * 
     */
    ~NewTable() = default;
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
    TableGetElem(const unsigned int label, const Vmarg* result,
    const Vmarg* arg1, const Vmarg* arg2, const unsigned int src_line)
    : Instruction(label, result, VerifyArg(arg1), VerifyArg(arg2), src_line) {}
    /**
     * @brief Destroys this TableGetElem object.
     * 
     */
    ~TableGetElem() = default;
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
    TableSetElem(const unsigned int label, const Vmarg* result,
    const Vmarg* arg1, const Vmarg* arg2, const unsigned int src_line)
    : Instruction(label, result, VerifyArg(arg1), VerifyArg(arg2), src_line) {}
    /**
     * @brief Destroys this TableSetElem object.
     * 
     */
    ~TableSetElem() = default;
private:
    std::ostream&   LogInstruction(std::ostream &os) const override;    
    bool            IsStateValid() const override;
};
}

#endif 