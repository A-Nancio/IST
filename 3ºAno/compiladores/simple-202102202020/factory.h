#ifndef __SIMPLE_FACTORY_H__
#define __SIMPLE_FACTORY_H__

#include <memory>
#include <cdk/yy_factory.h>
#include "simple_scanner.h"

namespace simple {

  /**
   * This class implements the compiler factory for the Simple compiler.
   */
  class factory: public cdk::yy_factory<simple_scanner> {
    /**
     * This object is automatically registered by the constructor in the
     * superclass' language registry.
     */
    static factory _self;

  protected:
    /**
     * @param language name of the language handled by this factory (see .cpp file)
     */
    factory(const std::string &language = "simple") :
        cdk::yy_factory<simple_scanner>(language) {
    }

  };

} // simple

#endif
