#ifndef __SIMPLE_TARGETS_INTERPRETER_TARGET_H__
#define __SIMPLE_TARGETS_INTERPRETER_TARGET_H__

#include <cdk/targets/basic_target.h>
#include <cdk/ast/basic_node.h>
#include "targets/interpreter.h"

namespace simple {

  class interpreter_target: public cdk::basic_target {
    static interpreter_target _self;

  private:
    interpreter_target() :
        cdk::basic_target("@@INTERPRET@@") {
    }

  public:
    bool evaluate(std::shared_ptr<cdk::compiler> compiler) {
      // this symbol table will be used to check identifiers
      cdk::symbol_table<simple::symbol> symtab;

      // - the interpreter will use the syntactic tree to directly run the program
      //   an exception will be thrown if identifiers are used before declaration
      // - the interpreter always uses the standard output
      interpreter interpreter(compiler, symtab);
      compiler->ast()->accept(&interpreter, 0);

      return true;
    }

  };

} // simple

#endif
