#ifndef __SIMPLE_TARGETS_C_TARGET_H__
#define __SIMPLE_TARGETS_C_TARGET_H__

#include <cdk/targets/basic_target.h>
#include <cdk/ast/basic_node.h>
#include "targets/c_writer.h"

namespace simple {

  class c_target: public cdk::basic_target {
    static c_target _self;

  private:
    c_target() :
        cdk::basic_target("c") {
    }

  public:
    bool evaluate(std::shared_ptr<cdk::compiler> compiler) {
      // this symbol table will be used to check identifiers
      // an exception will be thrown if identifiers are used before declaration
      cdk::symbol_table<simple::symbol> symtab;

      // generate C code
      c_writer writer(compiler, symtab);
      compiler->ast()->accept(&writer, 0);

      return true;
    }

  };

} // simple

#endif
