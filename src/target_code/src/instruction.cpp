#include "../include/instruction.h"

namespace target_code 
{
std::ostream& 
operator<<(std::ostream &os, const Instruction* instr) {
    PRECONDITION(instr != nullptr);
    return instr->LogInstruction(os);
}

const unsigned int
Instruction::get_label() const {
    POSTCONDITION(label >= 0);
    return label;
}

Vmarg*
Instruction::get_result() const{
    POSTCONDITION(result != nullptr);
    return result;
}

const Vmarg*
Instruction::get_arg2() const {
    return arg2;
}

const Vmarg* 
Instruction::get_arg1() const {
    return arg1;
}

const unsigned int
Instruction::get_src_line() const {
    POSTCONDITION(src_line >= 0);
    return src_line;
}

const Vmarg*
Instruction::VerifyArg(const Vmarg* arg) const {
    PRECONDITION(arg != nullptr);
    POSTCONDITION(arg != nullptr);
    return arg;
}

const unsigned int
Instruction::VerifyLabel(const unsigned int label) const {
    PRECONDITION(label >= 0);
    return label;
}

Vmarg*
Instruction::VerifyResult(Vmarg* result) const {
    PRECONDITION(result != nullptr);
    return result;
}

const unsigned int
Instruction::VerifySrcLine(const unsigned int src_line) const {
    PRECONDITION(src_line >= 0);
    return src_line;
}

uint8_t
Assign::get_opcode() const {
    return ASSIGN_VM;    
}

void            
Assign::Accept(const InstructionVisitor* visitor) {
    PRECONDITION(visitor != nullptr);    
    visitor->VisitAssign(this);
}

std::ostream&
Assign::LogInstruction(std::ostream &os) const {
    INVARIANT(IsStateValid());
    os  << get_label() << ": " << "assign";
    os << " " << get_result();
    if (get_arg1() != nullptr)
        os << " " << get_arg1();
    os << " [line " << get_src_line() << "]" << std::endl;
    INVARIANT(IsStateValid());
    return os;
}

bool
Assign::IsStateValid() const {
    return get_label() >= 0 && get_result() != nullptr && get_src_line() >= 0;
}

uint8_t
Add::get_opcode() const {
    return ADD_VM;    
}

void            
Add::Accept(const InstructionVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitAdd(this);    
}

std::ostream&
Add::LogInstruction(std::ostream &os) const {
    INVARIANT(IsStateValid());
    os  << get_label() << ": " << "add";
    os << " " << get_result();
    os << " " << get_arg1();
    os << " " << get_arg2();
    os << " [line " << get_src_line() << "]" << std::endl;
    INVARIANT(IsStateValid());
    return os;
}

bool
Add::IsStateValid() const {
    return get_label() >= 0 && get_result() != nullptr 
    && get_arg1() != nullptr && get_arg2() != nullptr
    && get_src_line() >= 0;
}

uint8_t        
Sub::get_opcode() const {
    return SUB_VM;    
}

void            
Sub::Accept(const InstructionVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitSub(this);    
}

std::ostream&
Sub::LogInstruction(std::ostream &os) const {
    INVARIANT(IsStateValid());
    os  << get_label() << ": " << "sub";
    os << " " << get_result();
    os << " " << get_arg1();
    os << " " << get_arg2();
    os << " [line " << get_src_line() << "]" << std::endl;
    INVARIANT(IsStateValid());
    return os;
}

bool
Sub::IsStateValid() const {
    return get_label() >= 0 && get_result() != nullptr 
    && get_arg1() != nullptr && get_arg2() != nullptr
    && get_src_line() >= 0;
}

uint8_t        
Mul::get_opcode() const {
    return MUL_VM;    
}

void            
Mul::Accept(const InstructionVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitMul(this);    
}

std::ostream&
Mul::LogInstruction(std::ostream &os) const {
    INVARIANT(IsStateValid());
    os  << get_label() << ": " << "mul";
    os << " " << get_result();
    os << " " << get_arg1();
    os << " " << get_arg2();
    os << " [line " << get_src_line() << "]" << std::endl;
    INVARIANT(IsStateValid());
    return os;
}

bool
Mul::IsStateValid() const {
    return get_label() >= 0 && get_result() != nullptr 
    && get_arg1() != nullptr && get_arg2() != nullptr
    && get_src_line() >= 0;
}

uint8_t        
Div::get_opcode() const {
    return DIV_VM;    
}

void            
Div::Accept(const InstructionVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitDiv(this);    
}

std::ostream&
Div::LogInstruction(std::ostream &os) const {
    INVARIANT(IsStateValid());
    os  << get_label() << ": " << "div";
    os << " " << get_result();
    os << " " << get_arg1();
    os << " " << get_arg2();
    os << " [line " << get_src_line() << "]" << std::endl;
    INVARIANT(IsStateValid());
    return os;
}

bool
Div::IsStateValid() const {
    return get_label() >= 0 && get_result() != nullptr 
    && get_arg1() != nullptr && get_arg2() != nullptr
    && get_src_line() >= 0;
}

uint8_t        
Mod::get_opcode() const {
    return MOD_VM;    
}

void            
Mod::Accept(const InstructionVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitMod(this);    
}

std::ostream&
Mod::LogInstruction(std::ostream &os) const {
    INVARIANT(IsStateValid());
    os  << get_label() << ": " << "mod";
    os << " " << get_result();
    os << " " << get_arg1();
    os << " " << get_arg2();
    os << " [line " << get_src_line() << "]" << std::endl;
    INVARIANT(IsStateValid());
    return os;
}

bool
Mod::IsStateValid() const {
    return get_label() >= 0 && get_result() != nullptr 
    && get_arg1() != nullptr && get_arg2() != nullptr
    && get_src_line() >= 0;
}

uint8_t        
Jeq::get_opcode() const {
    return JEQ_VM;    
}

void            
Jeq::Accept(const InstructionVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitJeq(this);    
}

std::ostream&
Jeq::LogInstruction(std::ostream &os) const {
    INVARIANT(IsStateValid());
    os  << get_label() << ": " << "jeq";
    os << " " << get_result();
    os << " " << get_arg1();
    os << " " << get_arg2();
    os << " [line " << get_src_line() << "]" << std::endl;
    INVARIANT(IsStateValid());
    return os;
}

bool
Jeq::IsStateValid() const {
    return get_label() >= 0 && get_result() != nullptr 
    && get_arg1() != nullptr && get_arg2() != nullptr
    && get_src_line() >= 0;
}

uint8_t        
Jne::get_opcode() const {
    return JNE_VM;    
}

void            
Jne::Accept(const InstructionVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitJne(this);    
}

std::ostream&
Jne::LogInstruction(std::ostream &os) const {
    INVARIANT(IsStateValid());
    os  << get_label() << ": " << "jne";
    os << " " << get_result();
    os << " " << get_arg1();
    os << " " << get_arg2();
    os << " [line " << get_src_line() << "]" << std::endl;
    INVARIANT(IsStateValid());
    return os;
}

bool
Jne::IsStateValid() const {
    return get_label() >= 0 && get_result() != nullptr 
    && get_arg1() != nullptr && get_arg2() != nullptr
    && get_src_line() >= 0;
}

uint8_t        
Jgt::get_opcode() const {
    return JGT_VM;    
}

void            
Jgt::Accept(const InstructionVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitJgt(this);    
}

std::ostream&
Jgt::LogInstruction(std::ostream &os) const {
    INVARIANT(IsStateValid());
    os  << get_label() << ": " << "jgt";
    os << " " << get_result();
    os << " " << get_arg1();
    os << " " << get_arg2();
    os << " [line " << get_src_line() << "]" << std::endl;
    INVARIANT(IsStateValid());
    return os;
}

bool
Jgt::IsStateValid() const {
    return get_label() >= 0 && get_result() != nullptr 
    && get_arg1() != nullptr && get_arg2() != nullptr
    && get_src_line() >= 0;
}

uint8_t        
Jlt::get_opcode() const {
    return JLT_VM;    
}

void            
Jlt::Accept(const InstructionVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitJlt(this);    
}

std::ostream&
Jlt::LogInstruction(std::ostream &os) const {
    INVARIANT(IsStateValid());
    os  << get_label() << ": " << "jlt";
    os << " " << get_result();
    os << " " << get_arg1();
    os << " " << get_arg2();
    os << " [line " << get_src_line() << "]" << std::endl;
    INVARIANT(IsStateValid());
    return os;
}

bool
Jlt::IsStateValid() const {
    return get_label() >= 0 && get_result() != nullptr 
    && get_arg1() != nullptr && get_arg2() != nullptr
    && get_src_line() >= 0;
}

uint8_t        
Jge::get_opcode() const {
    return JGE_VM;    
}

void            
Jge::Accept(const InstructionVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitJge(this);    
}

std::ostream&
Jge::LogInstruction(std::ostream &os) const {
    INVARIANT(IsStateValid());
    os  << get_label() << ": " << "jge";
    os << " " << get_result();
    os << " " << get_arg1();
    os << " " << get_arg2();
    os << " [line " << get_src_line() << "]" << std::endl;
    INVARIANT(IsStateValid());
    return os;
}

bool
Jge::IsStateValid() const {
    return get_label() >= 0 && get_result() != nullptr 
    && get_arg1() != nullptr && get_arg2() != nullptr
    && get_src_line() >= 0;
}

uint8_t        
Jle::get_opcode() const {
    return JLE_VM;    
}

void            
Jle::Accept(const InstructionVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitJle(this);    
}

std::ostream&
Jle::LogInstruction(std::ostream &os) const {
    INVARIANT(IsStateValid());
    os  << get_label() << ": " << "jle";
    os << " " << get_result();
    os << " " << get_arg1();
    os << " " << get_arg2();
    os << " [line " << get_src_line() << "]" << std::endl;
    INVARIANT(IsStateValid());
    return os;
}

bool
Jle::IsStateValid() const {
    return get_label() >= 0 && get_result() != nullptr 
    && get_arg1() != nullptr && get_arg2() != nullptr
    && get_src_line() >= 0;
}

uint8_t        
Jump::get_opcode() const {
    return JUMP_VM;    
}

void            
Jump::Accept(const InstructionVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitJump(this);    
}

std::ostream&
Jump::LogInstruction(std::ostream &os) const {
    INVARIANT(IsStateValid());
    os  << get_label() << ": " << "jump";
    os << " " << get_result();
    os << " [line " << get_src_line() << "]" << std::endl;
    INVARIANT(IsStateValid());
    return os;
}

bool
Jump::IsStateValid() const {
    return get_label() >= 0 && get_result() != nullptr 
    && get_src_line() >= 0;
}

uint8_t        
CallFunc::get_opcode() const {
    return CALLFUNC_VM;    
}

void            
CallFunc::Accept(const InstructionVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitCallFunc(this);    
}

std::ostream&
CallFunc::LogInstruction(std::ostream &os) const {
    INVARIANT(IsStateValid());
    os  << get_label() << ": " << "callfunc";
    os << " " << get_result();
    os << " [line " << get_src_line() << "]" << std::endl;
    INVARIANT(IsStateValid());
    return os;
}

bool
CallFunc::IsStateValid() const {
    return get_label() >= 0 && get_result() != nullptr 
    && get_src_line() >= 0;
}

uint8_t        
PushArg::get_opcode() const {
    return PUSHARG_VM;    
}

void            
PushArg::Accept(const InstructionVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitPushArg(this);    
}

std::ostream&
PushArg::LogInstruction(std::ostream &os) const {
    INVARIANT(IsStateValid());
    os  << get_label() << ": " << "pusharg";
    os << " " << get_result();
    os << " [line " << get_src_line() << "]" << std::endl;
    INVARIANT(IsStateValid());
    return os;
}

bool
PushArg::IsStateValid() const {
    return get_label() >= 0 && get_result() != nullptr 
    && get_src_line() >= 0;
}

uint8_t        
EnterFunc::get_opcode() const {
    return ENTERFUNC_VM;    
}

void            
EnterFunc::Accept(const InstructionVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitEnterFunc(this);    
}

std::ostream&
EnterFunc::LogInstruction(std::ostream &os) const {
    INVARIANT(IsStateValid());
    INVARIANT(IsStateValid());
    os  << get_label() << ": " << "enterfunc";
    os << " " << get_result();
    os << " [line " << get_src_line() << "]" << std::endl;
    INVARIANT(IsStateValid());
    INVARIANT(IsStateValid());
    return os;
}

bool
EnterFunc::IsStateValid() const {
    return get_label() >= 0 && get_result() != nullptr 
    && get_src_line() >= 0;
}

uint8_t        
ExitFunc::get_opcode() const {
    return EXITFUNC_VM;    
}

void            
ExitFunc::Accept(const InstructionVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitExitFunc(this);    
}

std::ostream&
ExitFunc::LogInstruction(std::ostream &os) const {
    INVARIANT(IsStateValid());
    os  << get_label() << ": " << "exitfunc";
    os << " " << get_result();
    os << " [line " << get_src_line() << "]" << std::endl;
    INVARIANT(IsStateValid());
    return os;
}

bool
ExitFunc::IsStateValid() const {
    return get_label() >= 0 && get_result() != nullptr 
    && get_src_line() >= 0;
}

uint8_t        
NewTable::get_opcode() const {
    return NEWTABLE_VM;    
}

void            
NewTable::Accept(const InstructionVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitNewTable(this);    
}

std::ostream&   
NewTable::LogInstruction(std::ostream &os) const {
    INVARIANT(IsStateValid());
    os  << get_label() << ": " << "newtable";
    os << " " << get_result();
    os << " [line " << get_src_line() << "]" << std::endl;
    INVARIANT(IsStateValid());
    return os;
}

bool
NewTable::IsStateValid() const {
    return get_label() >= 0 && get_result() != nullptr 
    && get_src_line() >= 0;
}

uint8_t        
TableGetElem::get_opcode() const {
    return TABLEGETELEM_VM;    
}

void            
TableGetElem::Accept(const InstructionVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitTableGetElem(this);    
}

std::ostream&   
TableGetElem::LogInstruction(std::ostream &os) const {
    INVARIANT(IsStateValid());
    os  << get_label() << ": " << "tablegetelem";
    os << " " << get_result();
    os << " " << get_arg1();
    os << " " << get_arg2();
    os << " [line " << get_src_line() << "]" << std::endl;
    INVARIANT(IsStateValid());
    return os;
}

bool
TableGetElem::IsStateValid() const {
    return get_label() >= 0 && get_result() != nullptr 
    && get_arg1() != nullptr && get_arg2() != nullptr
    && get_src_line() >= 0;
}

uint8_t        
TableSetElem::get_opcode() const {
    return TABLESETELEM_VM;    
}

void            
TableSetElem::Accept(const InstructionVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitTableSetElem(this);    
}

std::ostream&   
TableSetElem::LogInstruction(std::ostream &os) const {
    INVARIANT(IsStateValid());
    os  << get_label() << ": " << "tablesetelem";
    os << " " << get_result();
    os << " " << get_arg1();
    os << " " << get_arg2();
    os << " [line " << get_src_line() << "]" << std::endl;
    INVARIANT(IsStateValid());
    return os;
}

bool
TableSetElem::IsStateValid() const {
    return get_label() >= 0 && get_result() != nullptr 
    && get_arg1() != nullptr && get_arg2() != nullptr
    && get_src_line() >= 0;
}
}