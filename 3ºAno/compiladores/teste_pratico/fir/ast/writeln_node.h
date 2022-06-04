#ifndef __FIR_AST_WRITElnLN_NODE_H__
#define __FIR_AST_WRITELN_NODE_H__

#include <cdk/ast/expression_node.h>

namespace fir {

  /**
   * Class for describing writeln nodes.
   */
  class writeln_node: public cdk::basic_node {
    cdk::expression_node *_argument;

  public:
    inline writeln_node(int lineno, cdk::expression_node *argument) :
        cdk::basic_node(lineno), _argument(argument) {
    }

  public:
    inline cdk::expression_node *argument() {
      return _argument;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_writeln_node(this, level);
    }

  };

} // fir

#endif
