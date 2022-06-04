#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated
#include <cdk/types/primitive_type.h>
#include "fir_parser.tab.h"

#define ASSERT_UNSPEC { if (node->type() != nullptr && !node->is_typed(cdk::TYPE_UNSPEC)) return; }

//---------------------------------------------------------------------------

void fir::type_checker::do_sequence_node(cdk::sequence_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void fir::type_checker::do_nil_node(cdk::nil_node *const node, int lvl) {
  // EMPTY
}
void fir::type_checker::do_data_node(cdk::data_node *const node, int lvl) {
  // EMPTY
}
void fir::type_checker::do_double_node(cdk::double_node *const node, int lvl) {
  // EMPTY
}
void fir::type_checker::do_not_node(cdk::not_node *const node, int lvl) {
  // EMPTY
}
void fir::type_checker::do_and_node(cdk::and_node *const node, int lvl) {
  do_BooleanLogicalExpression(node, lvl);
}
void fir::type_checker::do_or_node(cdk::or_node *const node, int lvl) {
  do_BooleanLogicalExpression(node, lvl);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_integer_node(cdk::integer_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void fir::type_checker::do_string_node(cdk::string_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
}

//---------------------------------------------------------------------------

void fir::type_checker::processUnaryExpression(cdk::unary_operation_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (!node->argument()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in argument of unary expression");

  // in Simple, expressions are always int
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void fir::type_checker::do_neg_node(cdk::neg_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl);
  if (node->argument()->is_typed(cdk::TYPE_INT)) {
    node->type(node->argument()->type());
  } else {
    throw std::string("integer or vector expressions expected");
  }
}

//---------------------------------------------------------------------------

void fir::type_checker::processBinaryExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (!node->left()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in left argument of binary expression");

  node->right()->accept(this, lvl + 2);
  if (!node->right()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in right argument of binary expression");

  // in Simple, expressions are always int
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

//---------------------------------------------------------------------------

//protected:
void fir::type_checker::do_BooleanLogicalExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (!node->left()->is_typed(cdk::TYPE_INT)) {
    throw std::string("integer expression expected in binary expression");
  }

  node->right()->accept(this, lvl + 2);
  if (!node->right()->is_typed(cdk::TYPE_INT)) {
    throw std::string("integer expression expected in binary expression");
  }

  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
} 

void fir::type_checker::do_add_node(cdk::add_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_sub_node(cdk::sub_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_mul_node(cdk::mul_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_div_node(cdk::div_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_mod_node(cdk::mod_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_lt_node(cdk::lt_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_le_node(cdk::le_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_ge_node(cdk::ge_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_gt_node(cdk::gt_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_ne_node(cdk::ne_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void fir::type_checker::do_eq_node(cdk::eq_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_variable_node(cdk::variable_node *const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->name();
  std::shared_ptr<fir::symbol> symbol = _symtab.find(id);

  if (symbol != nullptr) {
    node->type(symbol->type());
  } else {
    throw id;
  }
}

void fir::type_checker::do_rvalue_node(cdk::rvalue_node *const node, int lvl) {
  ASSERT_UNSPEC;
  try {
    node->lvalue()->accept(this, lvl);
    node->type(node->lvalue()->type());
  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }
}

void fir::type_checker::do_assignment_node(cdk::assignment_node *const node, int lvl) {
  ASSERT_UNSPEC;

  try {
    node->lvalue()->accept(this, lvl);
  } catch (const std::string &id) {
    auto symbol = std::make_shared<fir::symbol>(cdk::primitive_type::create(4, cdk::TYPE_INT), id, 0, true, false);
    _symtab.insert(id, symbol);
    _parent->set_new_symbol(symbol);  // advise parent that a symbol has been inserted
    node->lvalue()->accept(this, lvl);  //DAVID: bah!
  }

  if (!node->lvalue()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in left argument of assignment expression");

  node->rvalue()->accept(this, lvl + 2);
  if (!node->rvalue()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in right argument of assignment expression");

  // in Simple, expressions are always int
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

//---------------------------------------------------------------------------

void fir::type_checker::do_program_node(fir::program_node *const node, int lvl) {
  // EMPTY
}

void fir::type_checker::do_evaluation_node(fir::evaluation_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

void fir::type_checker::do_print_node(fir::print_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_read_node(fir::read_node *const node, int lvl) {
  try {
    node->argument()->accept(this, lvl);
  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }
}

//---------------------------------------------------------------------------

void fir::type_checker::do_while_node(fir::while_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_if_node(fir::if_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

void fir::type_checker::do_if_else_node(fir::if_else_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_leave_node(fir::leave_node * const node, int lvl) {
  // EMPTY
}

void fir::type_checker::do_block_node(fir::block_node * const node, int lvl) {}

void fir::type_checker::do_return_node(fir::return_node * const node, int lvl) {}

void fir::type_checker::do_restart_node(fir::restart_node * const node, int lvl) {
  // EMPTY
}

void fir::type_checker::do_nullptr_node(fir::nullptr_node * const node, int lvl) {}

void fir::type_checker::do_function_definition_node(fir::function_definition_node * const node, int lvl) {
  std::string id;
  
  // "fix" naming issues...
  if (node->identifier() == "fir")
    id = "_main";
  else if (node->identifier() == "_main")
    id = "._main";
  else
    id = node->identifier();

  _inBlockReturnType = nullptr;
  
  // remember symbol so that args know
  auto function = fir::make_symbol(node->type(), id, node->qualifier(), false,  false, true);
  
  std::shared_ptr<fir::symbol> previous = _symtab.find(function->name());
  if (previous) {
    if (previous->forward()
        && ((previous->qualifier() == tPUBLIC && node->qualifier() == tPUBLIC)
            || (previous->qualifier() == tPRIVATE && node->qualifier() == tPRIVATE))) {
      _symtab.replace(function->name(), function);
      _parent->set_new_symbol(function);
    } else {
      throw std::string("conflicting definition for '" + function->name() + "'");
    }
  } else {
    _symtab.insert(function->name(), function);
    _parent->set_new_symbol(function);
  }
  
}

void fir::type_checker::do_function_call_node(fir::function_call_node * const node, int lvl) {}

void fir::type_checker::do_function_declaration_node(fir::function_declaration_node * const node, int lvl) {}

void fir::type_checker::do_variable_declaration_node(fir::variable_declaration_node * const node, int lvl) {
  if (node->initializer() != nullptr) {
    node->initializer()->accept(this, lvl + 2);

    if (node->is_typed(cdk::TYPE_INT)) {
      if (!node->initializer()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type for initializer (integer expected).");
    } else if (node->is_typed(cdk::TYPE_STRING)) {
      if (!node->initializer()->is_typed(cdk::TYPE_STRING))
        throw std::string("wrong type for initializer (string expected).");
    } else {
      throw std::string("unknown type for initializer.");
    }
     /*else if (node->is_typed(cdk::TYPE_DOUBLE)) {
      if (!node->initializer()->is_typed(cdk::TYPE_INT) && !node->initializer()->is_typed(cdk::TYPE_DOUBLE)) {
        throw std::string("wrong type for initializer (integer or double expected).");
      }
    } */
    /*else if (node->is_typed(cdk::TYPE_POINTER)) {
      //DAVID: FIXME: trouble!!!
      if (!node->initializer()->is_typed(cdk::TYPE_POINTER)) {
        auto in = (cdk::literal_node<int>*)node->initializer();
        if (in == nullptr || in->value() != 0) throw std::string("wrong type for initializer (pointer expected).");
      }*/
  }

  const std::string &id = node->identifier();
  auto symbol = fir::make_symbol(node->type(), id, node->qualifier(), (bool)node->initializer(), false, false);
  if (_symtab.insert(id, symbol)) {
    _parent->set_new_symbol(symbol);  // advise parent that a symbol has been inserted
  } else {
    throw std::string("variable '" + id + "' redeclared");
  }
}

void fir::type_checker::do_writeln_node(fir::writeln_node * const node, int lvl) {}

void fir::type_checker::do_sizeof_node(fir::sizeof_node * const node, int lvl) {}

void fir::type_checker::do_index_node(fir::index_node * const node, int lvl) {}

void fir::type_checker::do_address_of_node(fir::address_of_node * const node, int lvl) {}

void fir::type_checker::do_apply_node(fir::apply_node * const node, int lvl) {
  //verify low value
  node->low()->accept();
  if (node->low()->isTyped(cdk::TYPE_INT))
    throw std::string("Expected value of type integer");

  //verify high value
  node->high()->accept();
  if (node->high()->isTyped(cdk::TYPE_INT))
    throw std::string("Expected value of type integer");

  const std::string &id = node->function();
  auto symbol = _symtab.find(id);

  if (symbol == nullptr)
    throw std::string("function '" + id + "' is undeclared.");
  if (!symbol->function())
    throw std::string("symbol '" + id + "' is not a function.");

  //verify if there is an vector matches with function argument
  /*
  for (arguments names in function, variable from symbol table)
    if match is found {match = true};
  
  if match was not found
    throw std:string("no matching arguments in function")
  */
 
}
