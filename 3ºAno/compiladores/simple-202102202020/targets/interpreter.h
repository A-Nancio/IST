#ifndef __SIMPLE_TARGETS_INTERPRETER_H__
#define __SIMPLE_TARGETS_INTERPRETER_H__

#include "targets/basic_ast_visitor.h"

#include <vector>

namespace simple {

  //!
  //! Runs the program by interpreting the syntactic tree directly
  //!
  class interpreter: public basic_ast_visitor {
    // last seen identifier and corresponding value
    std::shared_ptr<simple::symbol> _symbol;

    // this is a rare anonymous class in C++
    class {
      std::vector<long> _stack;
    public:
      void push(long i) {
        _stack.push_back(i);
      }
      long pop() {
        long i = _stack.back();
        _stack.pop_back();
        return i;
      }
    } _stack;

    cdk::symbol_table<simple::symbol> &_symtab;

  public:
    interpreter(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<simple::symbol> &symtab) :
        basic_ast_visitor(compiler), _symtab(symtab) {
    }

  public:
    ~interpreter() {
      os().flush();
    }

  public:
  // do not edit these lines
#define __IN_VISITOR_HEADER__
#include "ast/visitor_decls.h"       // automatically generated
#undef __IN_VISITOR_HEADER__
  // do not edit these lines: end

  };

} // simple

#endif
