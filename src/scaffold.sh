#!/bin/bash

usage() {
    echo "Usage: "
    echo "  $0 <class-name>"
    echo "  E.g.: $0 ContextManager"
    echo ""
    echo "This will create ContextManager.cpp and ContextManager.hpp files."
    exit 0
}

[ "x$1" == "x" ] && usage

cat > "include/$1.hpp" <<EOF
#ifndef _$1_HPP_
#define _$1_HPP_

namespace Cycleshooter {
class $1 {

public:
  $1();
  virtual ~$1();
};

}

#endif
EOF

cat > "src/$1.cpp" << EOF
#include "$1.hpp"

namespace Cycleshooter {

$1::$1() {
}

virtual $1::~$1() {
}

}
EOF
