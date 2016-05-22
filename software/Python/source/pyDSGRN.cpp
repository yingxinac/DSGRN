
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "DSGRN.h"

#include <vector>

// Network wrapping incidentals

// IntList 
typedef std::vector<uint64_t> IntList;
std::string
printIntList ( IntList const* self ) {
  std::stringstream ss;
  bool first = true;
  ss << "[";
  for ( auto const& item : *self ) {
    if ( first ) first = false; else ss << ", ";
    ss << item;
  }
  ss << "]";
  return ss.str();
}

// IntListList
typedef std::vector<std::vector<uint64_t>> IntListList;
std::string
printIntListList ( IntListList const* self ) {
  std::stringstream ss;
  bool first = true;
  ss << "[";
  for ( auto const& item : *self ) {
    if ( first ) first = false; else ss << ", ";
    ss << printIntList(&item);
  }
  ss << "]";
  return ss.str();
}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(network_overloads, graphviz, 0, 1)

// LogicParameter wrapping incidentals

//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(LogicParameter_overloads, operator(), 0, 2)

typedef std::vector<LogicParameter> LogicParameterList;
std::string
printLogicParameterList( LogicParameterList const* self ) {
  std::stringstream ss;
  bool first = true;
  ss << "[";
  for ( auto const& item : *self ) {
    if ( first ) first = false; else ss << ", ";
    ss << item.stringify();
  }
  ss << "]";
  return ss.str();
}

// OrderParameter wrapping incidentals
typedef std::vector<OrderParameter> OrderParameterList;
std::string
printOrderParameterList( OrderParameterList const* self ) {
  std::stringstream ss;
  bool first = true;
  ss << "[";
  for ( auto const& item : *self ) {
    if ( first ) first = false; else ss << ", ";
    ss << item.stringify();
  }
  ss << "]";
  return ss.str();
}

BOOST_PYTHON_MODULE(libpyDSGRN)
{
    using namespace boost::python;

    // Network

    class_<IntList>("IntList")
      .def(vector_indexing_suite<IntList>() )
      .def("__str__", &printIntList)
      .def("__repr__", &printIntList)
    ;

    class_<IntListList>("IntListList")
      .def(vector_indexing_suite<IntListList>() )
      .def("__str__", &printIntListList)
      .def("__repr__", &printIntListList)
    ;

    class_<Network>("Network", init<std::string>())
      .def(init<>())
      .def("load", &Network::load)  
      .def("assign", &Network::assign)  
      .def("size", &Network::size)  
      .def("index", &Network::index)  
      .def("name", &Network::name, return_value_policy<copy_const_reference>()) 
      .def("inputs", &Network::inputs, return_value_policy<copy_const_reference>()) 
      .def("outputs", &Network::inputs, return_value_policy<copy_const_reference>()) 
      .def("logic", &Network::logic, return_value_policy<copy_const_reference>())
      .def("essential", &Network::essential) 
      .def("interaction", &Network::interaction) 
      .def("order", &Network::order) 
      .def("domains", &Network::domains) 
      .def("specification", &Network::specification)
      .def("graphviz", &Network::graphviz, network_overloads()) 
      .def("__str__", &Network::specification)
    ;

    // LogicParameter

    class_<LogicParameterList>("LogicParameterList")
      .def(vector_indexing_suite<LogicParameterList>() )
      .def("__str__", &printLogicParameterList)
      .def("__repr__", &printLogicParameterList)
    ; // Note: to get this to work I needed to make LogicParameter == comparable in C++ code

    class_<LogicParameter>("LogicParameter", init<uint64_t, uint64_t, std::string>())
      .def(init<>())
      .def("assign", &LogicParameter::assign)  
      .def("__call__",static_cast<bool (LogicParameter::*)( std::vector<bool> const&, uint64_t ) const>
         (&LogicParameter::operator()) ) // How is python user going to get hands on vector<bool> ? TODO
      .def("__call__",static_cast<bool (LogicParameter::*)( uint64_t ) const>
         (&LogicParameter::operator()) )
      .def("bin", &LogicParameter::bin)
      .def("stringify", &LogicParameter::stringify)
      .def("parse", &LogicParameter::parse)
      .def("hex", &LogicParameter::hex, return_value_policy<copy_const_reference>())
      .def("adjacencies", &LogicParameter::adjacencies)
      .def("__str__", &LogicParameter::stringify)
    ;

    // OrderParameter

    class_<OrderParameterList>("OrderParameterList")
      .def(vector_indexing_suite<OrderParameterList>() )
      .def("__str__", &printOrderParameterList)
      .def("__repr__", &printOrderParameterList)
    ; // Note: to get this to work I needed to make OrderParameter == comparable in C++ code

    class_<OrderParameter>("OrderParameter", init<>())
      .def(init<uint64_t,uint64_t>())
      .def(init<std::vector<uint64_t>const&>()) // TODO
      .def("assign", static_cast<void (OrderParameter::*)( uint64_t, uint64_t )>(&OrderParameter::assign) )
      .def("assign", static_cast<void (OrderParameter::*)( std::vector<uint64_t> const&)>(&OrderParameter::assign) )
      .def("__call__", &OrderParameter::operator() )
      .def("inverse", &OrderParameter::inverse )
      .def("permutation", &OrderParameter::permutation, return_value_policy<copy_const_reference>())
      .def("index", &OrderParameter::index)
      .def("size", &OrderParameter::size)
      .def("stringify", &OrderParameter::stringify)
      .def("parse", &OrderParameter::parse)
      .def("adjacencies", &OrderParameter::adjacencies)
      .def("__str__", &OrderParameter::stringify)
    ;

    // Parameter

    class_<Parameter>("Parameter", init<>())
      .def(init<std::vector<LogicParameter> const&, std::vector<OrderParameter> const&, Network const& >())
      .def(init<Network const&>())
      .def("assign", static_cast<void (Parameter::*)(std::vector<LogicParameter> const&, std::vector<OrderParameter> const&, Network const& network)>(&Parameter::assign) )
      .def("assign", static_cast<void (Parameter::*)(Network const& )>(&Parameter::assign) )
      .def("attracting", &Parameter::attracting)
      .def("asborbing", &Parameter::absorbing)
      .def("regulator", &Parameter::regulator)
      .def("labelling", &Parameter::labelling)
      .def("network", &Parameter::network)
      .def("stringify", &Parameter::stringify)
      .def("parse", &Parameter::parse)
      .def("inequalities", &Parameter::parse)
      .def("logic", &Parameter::logic, return_value_policy<copy_const_reference>())
      .def("order", &Parameter::order, return_value_policy<copy_const_reference>())
      .def("__str__", &Parameter::stringify)
    ;

    // ParameterGraph

    class_<ParameterGraph>("ParameterGraph", init<>())
      .def(init<Network const&>())
      .def("assign", &ParameterGraph::assign)
      .def("size", &ParameterGraph::size)
      .def("parameter", &ParameterGraph::parameter)
      .def("index", &ParameterGraph::index)
      .def("adjacencies", &ParameterGraph::adjacencies)
      .def("network", &ParameterGraph::network)
      .def("fixedordersize", &ParameterGraph::fixedordersize)
      .def("reorderings", &ParameterGraph::reorderings)
    ;

}
