#ifndef __SIMPLE_TARGETS_ENGLISH_TARGET_H__
#define __SIMPLE_TARGETS_ENGLISH_TARGET_H__

#include <cdk/targets/basic_target.h>
#include <cdk/ast/basic_node.h>
#include "targets/english_writer.h"

namespace simple {

  class english_target: public cdk::basic_target {
    static english_target _self;

  private:
    english_target() :
        cdk::basic_target("english") {
    }

  public:
    bool evaluate(std::shared_ptr<cdk::compiler> compiler) {
      // this symbol table will be used to check identifiers
      // an exception will be thrown if identifiers are used before declaration
      cdk::symbol_table<simple::symbol> symtab;

      english_writer writer(compiler, symtab);
      compiler->ast()->accept(&writer, 0);
      return true;
    }

  };

} // simple

#endif
