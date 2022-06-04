#include <string>
#include "targets/interpreter.h"
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

//---------------------------------------------------------------------------

void simple::interpreter::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void simple::interpreter::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}
void simple::interpreter::do_double_node(cdk::double_node * const node, int lvl) {
  // EMPTY
}
void simple::interpreter::do_not_node(cdk::not_node * const node, int lvl) {
  // EMPTY
}
void simple::interpreter::do_and_node(cdk::and_node * const node, int lvl) {
  // EMPTY
}
void simple::interpreter::do_or_node(cdk::or_node * const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void simple::interpreter::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void simple::interpreter::do_integer_node(cdk::integer_node * const node, int lvl) {
  _stack.push(node->value());
}

void simple::interpreter::do_string_node(cdk::string_node * const node, int lvl) {
  // dirty trick: store string address
  _stack.push((long)node->value().c_str());
}

//---------------------------------------------------------------------------

void simple::interpreter::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
  _stack.push(-_stack.pop());
}

//---------------------------------------------------------------------------

void simple::interpreter::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->right()->accept(this, lvl);
  node->left()->accept(this, lvl);
  _stack.push(_stack.pop() + _stack.pop());
}
void simple::interpreter::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->right()->accept(this, lvl);
  node->left()->accept(this, lvl);
  _stack.push(_stack.pop() - _stack.pop());
}
void simple::interpreter::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->right()->accept(this, lvl);
  node->left()->accept(this, lvl);
  _stack.push(_stack.pop() * _stack.pop());
}
void simple::interpreter::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->right()->accept(this, lvl);
  node->left()->accept(this, lvl);
  _stack.push(_stack.pop() / _stack.pop());
}
void simple::interpreter::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->right()->accept(this, lvl);
  node->left()->accept(this, lvl);
  _stack.push(_stack.pop() % _stack.pop());
}
void simple::interpreter::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->right()->accept(this, lvl);
  node->left()->accept(this, lvl);
  _stack.push(_stack.pop() < _stack.pop());
}
void simple::interpreter::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->right()->accept(this, lvl);
  node->left()->accept(this, lvl);
  _stack.push(_stack.pop() <= _stack.pop());
}
void simple::interpreter::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->right()->accept(this, lvl);
  node->left()->accept(this, lvl);
  _stack.push(_stack.pop() >= _stack.pop());
}
void simple::interpreter::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->right()->accept(this, lvl);
  node->left()->accept(this, lvl);
  _stack.push(_stack.pop() > _stack.pop());
}
void simple::interpreter::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->right()->accept(this, lvl);
  node->left()->accept(this, lvl);
  _stack.push(_stack.pop() != _stack.pop());
}
void simple::interpreter::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->right()->accept(this, lvl);
  node->left()->accept(this, lvl);
  _stack.push(_stack.pop() == _stack.pop());
}

//---------------------------------------------------------------------------

void simple::interpreter::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _symbol = _symtab.find(node->name());
}

void simple::interpreter::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);
  _stack.push(_symbol->value());
}

void simple::interpreter::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->rvalue()->accept(this, lvl);
  if (new_symbol() == nullptr) {
    node->lvalue()->accept(this, lvl);
  } else {
    _symbol = new_symbol();
    reset_new_symbol();
  }
  _symbol->value(_stack.pop());
}

//---------------------------------------------------------------------------

void simple::interpreter::do_program_node(simple::program_node * const node, int lvl) {
  node->statements()->accept(this, lvl);
}

//---------------------------------------------------------------------------

void simple::interpreter::do_evaluation_node(simple::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
}

void simple::interpreter::do_print_node(simple::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
  if (node->argument()->is_typed(cdk::TYPE_INT))
    os() << _stack.pop() << std::endl;
  else if (node->argument()->is_typed(cdk::TYPE_STRING))
    os() << (const char *)_stack.pop() << std::endl;
  else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

//---------------------------------------------------------------------------

void simple::interpreter::do_read_node(simple::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl + 2);
  int val;
  std::cin >> val;
  _symbol->value(val);
}

//---------------------------------------------------------------------------

void simple::interpreter::do_while_node(simple::while_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->condition()->accept(this, lvl);
  while (_stack.pop()) {
    node->block()->accept(this, lvl);
    node->condition()->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void simple::interpreter::do_if_node(simple::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->condition()->accept(this, lvl);
  if (_stack.pop())
    node->block()->accept(this, lvl);
}

void simple::interpreter::do_if_else_node(simple::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->condition()->accept(this, lvl);
  if (_stack.pop())
    node->thenblock()->accept(this, lvl);
  else
    node->elseblock()->accept(this, lvl);
}
