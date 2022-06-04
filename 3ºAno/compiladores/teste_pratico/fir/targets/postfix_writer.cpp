#include <string>
#include <sstream>
#include <cdk/types/types.h>

#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "fir_parser.tab.h"
#include "ast/all.h"  // all.h is automatically generated

//---------------------------------------------------------------------------

void fir::postfix_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void fir::postfix_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}
void fir::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  // EMPTY
}
void fir::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) {
  // EMPTY
}
void fir::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl = ++_lbl;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JZ(mklbl(lbl));
  node->right()->accept(this, lvl + 2);
  _pf.AND();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}
void fir::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl = ++_lbl;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JNZ(mklbl(lbl));
  node->right()->accept(this, lvl + 2);
  _pf.OR();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    std::cout << i << std::endl;
    std::cout << node->node(i) << std::endl;
    std::cout << i << std::endl;
    node->node(i)->accept(this, lvl);
    std::cout << i << std::endl;
  }
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if(_inFunctionBody)
    _pf.INT(node->value());
  else
    _pf.SINT(node->value());
}

void fir::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int lbl1;

  _pf.RODATA(); 
  _pf.ALIGN(); 
  _pf.LABEL(mklbl(lbl1 = ++_lbl));
  _pf.SSTRING(node->value());

  _pf.ALIGN();
  if (_inFunctionBody) {
    _pf.TEXT();
    _pf.ADDR(mklbl(lbl1));
  }
  else
  {
    _pf.DATA();
    _pf.SADDR(mklbl(lbl1));
  }
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  _pf.NEG(); // 2-complement
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.ADD();
}
void fir::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.SUB();
}
void fir::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MUL();
}
void fir::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.DIV();
}
void fir::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}
void fir::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LT();
}
void fir::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LE();
}
void fir::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GE();
}
void fir::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GT();
}
void fir::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.NE();
}
void fir::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.EQ();
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  const std::string &id = node->name();
  auto symbol = _symtab.find(id);
  if (symbol->global()) {
    _pf.ADDR(symbol->name());
  } else
    _pf.LOCAL(symbol->offset());
}

void fir::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);
  _pf.LDINT(); // depends on type size
}

void fir::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->rvalue()->accept(this, lvl); // determine the new value
  _pf.DUP32();
  if (new_symbol() == nullptr) {
    node->lvalue()->accept(this, lvl); // where to store the value
  } else {
    _pf.DATA(); // variables are all global and live in DATA
    _pf.ALIGN(); // make sure we are aligned
    _pf.LABEL(new_symbol()->name()); // name variable location
    reset_new_symbol();
    _pf.SINT(0); // initialize it to 0 (zero)
    _pf.TEXT(); // return to the TEXT segment
    node->lvalue()->accept(this, lvl);  //DAVID: bah!
  }
  _pf.STINT(); // store the value at address
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_program_node(fir::program_node * const node, int lvl) {
  // Note that Simple doesn't have functions. Thus, it doesn't need
  // a function node. However, it must start in the main function.
  // The ProgramNode (representing the whole program) doubles as a
  // main function node.


  node->statements()->accept(this, lvl);


  // these are just a few library function imports

}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_evaluation_node(fir::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  if (node->argument()->is_typed(cdk::TYPE_INT)) {
    _pf.TRASH(4); // delete the evaluated value
  } else if (node->argument()->is_typed(cdk::TYPE_STRING)) {
    _pf.TRASH(4); // delete the evaluated value's address
  } else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

void fir::postfix_writer::do_print_node(fir::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->is_typed(cdk::TYPE_INT)) {
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  } else if (node->argument()->is_typed(cdk::TYPE_STRING)) {
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  } else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
  if (node->newline()) {
    _functions_to_declare.insert("println");
    _pf.CALL("println");
  }
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_read_node(fir::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _pf.CALL("readi");
  _pf.LDFVAL32();
  node->argument()->accept(this, lvl);
  _pf.STINT();
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_while_node(fir::while_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int while_condition = ++_lbl;
  int while_end = ++_lbl;

  _whileIni.push_back(++_lbl); // after init, before body
  _whileCondition.push_back(while_condition);
  _whileEnd.push_back(while_end);

  _pf.LABEL(mklbl(while_condition));
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(while_end));

  node->block()->accept(this, lvl + 2);

  _pf.JMP(mklbl(while_condition));
  _pf.LABEL(mklbl(while_end));
  _whileEnd.pop_back();
  _whileCondition.pop_back();
  
  
  if (node->has_finally_node()) {
    node->finally_node()->accept(this, lvl + 2);
  }
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_if_node(fir::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_if_else_node(fir::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

//---------------------------------------------------------------------------

void fir::postfix_writer::do_leave_node(fir::leave_node * const node, int lvl) {
  if(_whileIni.empty()) {
    throw std::string("leave instruction not in a cycle");
  }  
  else {
    int level = node->level();
    _pf.JMP(mklbl(_whileEnd[level-1]));

  }
}

void fir::postfix_writer::do_block_node(fir::block_node * const node, int lvl) {
  _symtab.push(); // for block-local vars
  if (node->declarations()) node->declarations()->accept(this, lvl + 2);
  if (node->instructions()) node->instructions()->accept(this, lvl + 2);
  _symtab.pop();
}

void fir::postfix_writer::do_return_node(fir::return_node * const node, int lvl) {}

void fir::postfix_writer::do_restart_node(fir::restart_node * const node, int lvl) {
  if (_whileIni.empty()) {
    throw std::string("restart instruction not in a cycle");
  }
  else {
    int level = node->level();
    _pf.JMP(mklbl(_whileCondition[level-1]));
  }
}

void fir::postfix_writer::do_nullptr_node(fir::nullptr_node * const node, int lvl) {}

void fir::postfix_writer::do_function_definition_node(fir::function_definition_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  if (_inFunctionBody || _inFunctionArgs) {
    throw std::string("cannot define function in body or in arguments");
    return;
  }
  
  // remember symbol so that args and body know
  _function = new_symbol();
  _functions_to_declare.erase(_function->name());  // just in case
  reset_new_symbol();
  
  _currentBodyRetLabel = mklbl(++_lbl);

  _offset = 8; // prepare for arguments (4: remember to account for return address)
  _symtab.push(); // scope of args
  
  
  _pf.TEXT();
  _pf.ALIGN();
  if (node->qualifier() == tPUBLIC) _pf.GLOBAL(_function->name(), _pf.FUNC());
  _pf.LABEL(_function->name());
  
  // compute stack size to be reserved for local variables
  frame_size_calculator lsc(_compiler, _symtab, _function);
  
  node->accept(&lsc, lvl);
  _pf.ENTER(lsc.localsize()); // total stack size reserved for local variables

  _offset = 0; // prepare for local variable

  // the following flag is a slight hack: it won't work with nested functions
  _inFunctionBody = true;
  os() << "        ;; before body " << std::endl;
  
  if(node->prologue())
    node->prologue()->accept(this, lvl + 4);
  if(node->block())
    node->block()->accept(this, lvl + 4);
  if(node->epilogue())
    node->epilogue()->accept(this, lvl + 4);
  os() << "        ;; after body " << std::endl;
  _inFunctionBody = false;
  _returnSeen = false;

  _pf.LABEL(_currentBodyRetLabel);
  _pf.LEAVE();
  _pf.RET();
  
  _symtab.pop(); // scope of arguments
  /*
  if (node->identifier() == "fir") {
    // declare external functions
    for (std::string s : _functions_to_declare)
      _pf.EXTERN(s);
  }
  */
}

void fir::postfix_writer::do_function_call_node(fir::function_call_node * const node, int lvl) {}

void fir::postfix_writer::do_function_declaration_node(fir::function_declaration_node * const node, int lvl) {}

void fir::postfix_writer::do_variable_declaration_node(fir::variable_declaration_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  auto id = node->identifier();
  
  std::cout << "INITIAL OFFSET: " << _offset << std::endl;

  // type size?
  int offset = 0, typesize = node->type()->size(); // in bytes
  std::cout << "ARG: " << id << ", " << typesize << std::endl;
  if (_inFunctionBody) {
    std::cout << "IN BODY" << std::endl;
    _offset -= typesize;
    offset = _offset;
  } else if (_inFunctionArgs) {
    std::cout << "IN ARGS" << std::endl;
    offset = _offset;
    _offset += typesize;
  } else {
    std::cout << "GLOBAL!" << std::endl;
    offset = 0; // global variable
  }
  std::cout << "OFFSET: " << id << ", " << offset << std::endl;

  auto symbol = new_symbol();
  if (symbol) {
    symbol->set_offset(offset);
    reset_new_symbol();
  }

  if (_inFunctionBody) {
    // if we are dealing with local variables, then no action is needed
    // unless an initializer exists
    if (node->initializer()) {
      node->initializer()->accept(this, lvl);
      if (node->is_typed(cdk::TYPE_INT)) {
        _pf.LOCAL(symbol->offset());
        _pf.STINT();
      } else {
        std::cerr << "cannot initialize" << std::endl;
      }
    }
  } else {
    if (!_function) {
      if (node->initializer() == nullptr) {
        _pf.BSS();
        _pf.ALIGN();
        _pf.LABEL(id);
        _pf.SALLOC(typesize);
      } else {

        if (node->is_typed(cdk::TYPE_INT)) {
          _pf.DATA();
          _pf.ALIGN();
          _pf.LABEL(id);
          node->initializer()->accept(this, lvl);

        } else if (node->is_typed(cdk::TYPE_STRING)) {
          _pf.DATA();
          _pf.ALIGN();
          _pf.LABEL(id);
          node->initializer()->accept(this, lvl);
        } else {
          std::cerr << node->lineno() << ": '" << id << "' has unexpected initializer\n";
          _errors = true;
        }

      }

    }
  }
}

void fir::postfix_writer::do_writeln_node(fir::writeln_node * const node, int lvl) {}

void fir::postfix_writer::do_sizeof_node(fir::sizeof_node * const node, int lvl) {}

void fir::postfix_writer::do_index_node(fir::index_node * const node, int lvl) {}

void fir::postfix_writer::do_address_of_node(fir::address_of_node * const node, int lvl) {}

void fir::postfix_writer::do_apply_node(fir::apply_node * const node, int lvl) {

  node->low()->accept(this, lvl);
  node->high()->accept(this, lvl);
  auto symbol = _symtab.find(node->function());
  if (symbol->is_typed(cdk::TYPE_DOUBLE))
    _pf.LDFVAL64();
  else if (symbol->is_typed(cdk::TYPE_INT) || symbol->is_typed(cdk::TYPE_POINTER) || symbol->is_typed(cdk::TYPE_STRING))
    _pf.LDFVAL32();
  else throw std::string("No correct type");
  if (!symbol) throw std::string("Function not found");

  //call the function various times
  for (int i = node->low(); i < node->high(); i++) {
    _pf.CALL(node->function());
    _pf.TRASH(4); // ONE ARGUMENT
  }
}



