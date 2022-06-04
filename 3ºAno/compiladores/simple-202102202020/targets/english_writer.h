#ifndef __SIMPLE_TARGETS_ENGLISH_WRITER_H__
#define __SIMPLE_TARGETS_ENGLISH_WRITER_H__

#include "targets/basic_ast_visitor.h"

namespace simple {

  /**
   * Print nodes as English language to the output stream.
   */
  class english_writer: public basic_ast_visitor {
    cdk::symbol_table<simple::symbol> &_symtab;

  public:
    english_writer(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<simple::symbol> &symtab) :
        basic_ast_visitor(compiler), _symtab(symtab) {
    }

  public:
    ~english_writer() {
      os().flush();
    }

  protected:
    void processUnaryExpression(cdk::unary_operation_node * const node, int lvl);
    void processBinaryExpression(cdk::binary_operation_node * const node, int lvl, const std::string &op);

  public:
  // do not edit these lines
#define __IN_VISITOR_HEADER__
#include "ast/visitor_decls.h"       // automatically generated
#undef __IN_VISITOR_HEADER__
  // do not edit these lines: end

  };

} // simple

#endif
