#ifndef __FIR_TARGETS_SYMBOL_H__
#define __FIR_TARGETS_SYMBOL_H__

#include <string>
#include <memory>
#include <cdk/types/basic_type.h>

namespace fir {

  class symbol {
    int _value; // hack!
    std::shared_ptr<cdk::basic_type> _type;
    std::string _name;
    int _qualifier; // qualifiers: public, private, forward
    bool _initialized; // initialized?
    int _offset = 0; // 0 (zero) means global variable/function
    bool _function; // false for variables
    std::vector<std::shared_ptr<cdk::basic_type>> _argument_types;
    bool _forward = false;

  public:
    symbol(std::shared_ptr<cdk::basic_type> type, const std::string &name, 
          int qualifier, bool initialized, bool function, bool forward = false) :
        _value(0), _type(type), _name(name), _qualifier(qualifier), _initialized(initialized), _function(function), 
        _forward(forward) {
    }

    virtual ~symbol() {
      // EMPTY
    }

    const std::string &name() const {
      return _name;
    }
    std::shared_ptr<cdk::basic_type> type() const {
      return _type;
    }
    void set_type(std::shared_ptr<cdk::basic_type> t) {
      _type = t;
    }
    bool is_typed(cdk::typename_type name) const {
      return _type->name() == name;
    }

    const std::string& identifier() const {
      return name();
    }
    int qualifier() const {
      return _qualifier;
    }
    bool initialized() const {
      return _initialized;
    }
    int offset() const {
      return _offset;
    }
    void set_offset(int offset) {
      _offset = offset;
    }
    bool isFunction() const {
      return _function;
    }
    
    bool global() const {
      return _offset == 0;
    }
    bool isVariable() const {
      return !_function;
    }

    bool forward() const {
      return _forward;
    }

    void set_argument_types(const std::vector<std::shared_ptr<cdk::basic_type>> &types) {
      _argument_types = types;
    }

    bool argument_is_typed(size_t ax, cdk::typename_type name) const {
      return _argument_types[ax]->name() == name;
    }
    std::shared_ptr<cdk::basic_type> argument_type(size_t ax) const {
      return _argument_types[ax];
    }

    size_t argument_size(size_t ax) const {
      return _argument_types[ax]->size();
    }

    size_t number_of_arguments() const {
      return _argument_types.size();
    }
    
    long value() const {
      return _value;
    }
    long value(long v) {
      return _value = v;
    }

  };
  inline auto make_symbol(std::shared_ptr<cdk::basic_type> type, const std::string &name, 
          int qualifier, bool initialized, bool function, bool forward = false) {
    return std::make_shared<symbol>(type, name, qualifier, initialized, function, forward);
  }

} // fir

#endif