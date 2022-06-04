#include <string>
#include "targets/c_writer.h"
#include "targets/type_checker.h"
#include "ast/all.h"  /* automatically generated */

//---------------------------------------------------------------------------

void simple::c_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void simple::c_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}
void simple::c_writer::do_double_node(cdk::double_node * const node, int lvl) {
  // EMPTY
}
void simple::c_writer::do_not_node(cdk::not_node * const node, int lvl) {
  // EMPTY
}
void simple::c_writer::do_and_node(cdk::and_node * const node, int lvl) {
  // EMPTY
}
void simple::c_writer::do_or_node(cdk::or_node * const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void simple::c_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl);
}

//---------------------------------------------------------------------------

void simple::c_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  os() << node->value();
}

void simple::c_writer::do_string_node(cdk::string_node * const node, int lvl) {
  os() << "\"" << node->value() << "\"";
}

//---------------------------------------------------------------------------

void simple::c_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  os() << " - ";
  node->argument()->accept(this, lvl);
}

//---------------------------------------------------------------------------

void simple::c_writer::do_binary_operation_node(cdk::binary_operation_node * const node, int lvl, const char *op) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  os() << " " << op << " ";
  node->right()->accept(this, lvl);
}

void simple::c_writer::do_add_node(cdk::add_node * const node, int lvl) {
  do_binary_operation_node(node, lvl, "+");
}
void simple::c_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  do_binary_operation_node(node, lvl, "-");
}
void simple::c_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  do_binary_operation_node(node, lvl, "*");
}
void simple::c_writer::do_div_node(cdk::div_node * const node, int lvl) {
  do_binary_operation_node(node, lvl, "/");
}
void simple::c_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  do_binary_operation_node(node, lvl, "%");
}
void simple::c_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  do_binary_operation_node(node, lvl, "<");
}
void simple::c_writer::do_le_node(cdk::le_node * const node, int lvl) {
  do_binary_operation_node(node, lvl, "<=");
}
void simple::c_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  do_binary_operation_node(node, lvl, ">=");
}
void simple::c_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  do_binary_operation_node(node, lvl, ">");
}
void simple::c_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  do_binary_operation_node(node, lvl, "!=");
}
void simple::c_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  do_binary_operation_node(node, lvl, "==");
}

//---------------------------------------------------------------------------

void simple::c_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  const std::string &id = node->name();
  os() << id;
}

void simple::c_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);
}

void simple::c_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if (new_symbol() == nullptr) {
    node->lvalue()->accept(this, lvl);
  } else {
    os() << std::string(lvl, ' ') << "int " << new_symbol()->name();
    reset_new_symbol();
  }

  os() << " = ";
  node->rvalue()->accept(this, lvl);
}

//---------------------------------------------------------------------------

void simple::c_writer::do_program_node(simple::program_node * const node, int lvl) {
  // Note that Simple doesn't have functions. Thus, it doesn't need
  // a function node. However, it must start in the main function.
  // The ProgramNode (representing the whole program) doubles as a
  // main function node.

  // open the main function
  os() << "#include <stdio.h>\n" << "int main() {\n";

  node->statements()->accept(this, lvl);

  // close the outer function
  os() << "  return 0;\n}\n";
}

void simple::c_writer::do_evaluation_node(simple::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl + 2, ' ');
  node->argument()->accept(this, lvl);
  os() << ";" << std::endl;
}

void simple::c_writer::do_print_node(simple::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (node->argument()->is_typed(cdk::TYPE_INT))
    os() << std::string(lvl + 2, ' ') << "printf(\"%d\\n\", ";
  else if (node->argument()->is_typed(cdk::TYPE_STRING))
    os() << std::string(lvl + 2, ' ') << "printf(\"%s\\n\", ";
  else {
    std::cerr << "CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
  node->argument()->accept(this, lvl);
  os() << ");\n";
}

//---------------------------------------------------------------------------

void simple::c_writer::do_read_node(simple::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl + 2, ' ') << "scanf(\"%d\", &";
  node->argument()->accept(this, lvl + 2);
  os() << ");\n";
}

//---------------------------------------------------------------------------

void simple::c_writer::do_while_node(simple::while_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl + 2, ' ') << "while(";
  node->condition()->accept(this, lvl);
  os() << ") {\n";
  node->block()->accept(this, lvl + 2);
  os() << std::string(lvl + 2, ' ') << "}\n";
}

//---------------------------------------------------------------------------

void simple::c_writer::do_if_node(simple::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl + 2, ' ') << "if (";
  node->condition()->accept(this, lvl);
  os() << ") {\n";
  node->block()->accept(this, lvl + 2);
  os() << std::string(lvl + 2, ' ') << "}\n";
}

void simple::c_writer::do_if_else_node(simple::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl + 2, ' ') << "if (";
  node->condition()->accept(this, lvl);
  os() << ") {\n";
  node->thenblock()->accept(this, lvl + 2);
  os() << std::string(lvl + 2, ' ') << "} else {\n";
  node->elseblock()->accept(this, lvl + 2);
  os() << std::string(lvl + 2, ' ') << "}\n";
}
