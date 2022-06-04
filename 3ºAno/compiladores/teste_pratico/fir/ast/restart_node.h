#ifndef __FIR_AST_NEXT_H__
#define __FIR_AST_NEXT_H__

#include <cdk/ast/basic_node.h>

namespace fir {

  class restart_node: public cdk::basic_node {
    int _level;

  public:
    restart_node(int lineno, int level = 1) :
        cdk::basic_node(lineno), _level(level) {
    }

  public:
    int level() const {
      return _level;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_restart_node(this, level);
    }

  };

} // fir

#endif
