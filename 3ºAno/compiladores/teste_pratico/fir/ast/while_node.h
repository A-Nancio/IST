#ifndef __FIR_AST_WHILE_NODE_H__
#define __FIR_AST_WHILE_NODE_H__

#include <cdk/ast/expression_node.h>

namespace fir {

  /**
   * Class for describing while-cycle nodes.
   */
  class while_node: public cdk::basic_node {
    cdk::expression_node *_condition;
    cdk::basic_node *_block, *_finally;
    bool while_finally = false;

  public:
    inline while_node(int lineno, cdk::expression_node *condition, cdk::basic_node *block) :
        basic_node(lineno), _condition(condition), _block(block) {
    }

    inline while_node(int lineno, cdk::expression_node* condition, cdk::basic_node* block, cdk::basic_node* finally_node) :
        basic_node(lineno), _condition(condition), _block(block), _finally(finally_node) {
        while_finally = true;
    }

  public:
    inline cdk::expression_node *condition() {
      return _condition;
    }
    inline cdk::basic_node *block() {
      return _block;
    }
    inline cdk::basic_node * finally_node() {
      return _finally;
    }
    bool has_finally_node() {
      return while_finally;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_while_node(this, level);
    }

  };

} // fir

#endif