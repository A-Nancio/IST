#ifndef __FIR_AST_APPLY_NODE_H__
#define __FIR_AST_APPLY_NODE__H__

#include <cdk/ast/basic_node.h>

namespace fir {

  class apply_node: public cdk::basic_node {
    std::shared_ptr<cdk::basic_type> _vector;
    cdk::expression_node *_low;
    cdk::expression_node *_high;
    std::string _function;

  public:
    apply_node(int lineno, std::shared_ptr<cdk::basic_type> vector, cdk::expression_node *low, cdk::expression_node *high, std::string function) :
        cdk::basic_node(lineno), _vector(vector), _low(low), _high(high), _function(function) {
    }

  public:
    std::shared_ptr<cdk::basic_type> vector {
      return _vector;
    }
    cdk::expression_node* low() {
      return _low;
    }
    cdk::expression_node* high() {
      return _high;
    }
    std::string& function() {
      return _function;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_apply_node(this, level);
    }

  };

} // fir

#endif