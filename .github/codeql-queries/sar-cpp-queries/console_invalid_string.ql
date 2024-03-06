/**
 * @name Wrong type of arguments to formatting function
 * @description Calling a format string function with the wrong type of arguments causes unpredictable behavior.
 * @kind problem
 * @problem.severity error
 * @security-severity 7.5
 * @precision high
 * @id sar-cpp-queries/console-invalid-string
 * @tags reliability
 *       correctness
 *       security
 *       external/cwe/cwe-686
 */

import cpp

from VariableCall call, PointerFieldAccess access, int arg
where
  access = call.getExpr() and
  access.getQualifier().toString() = "console" and
  call.getArgument(0).toString().matches("%\\%s%") and
  exists(int n |
    call.getArgument(n)
        .getFullyConverted()
        .getUnspecifiedType()
        .toString()
        .matches("%basic_string%") and
    arg = n
  )
select call.getArgument(arg),
  "Passed invalid std::string to console function. Please use .c_str() to convert it to a C-string."
// from FunctionCall call, int arg
// where
//   call.getQualifier().toString() = "console" and
//   call.getArgument(0).toString().matches("%\\%s%") and
//   exists(int n |
//     call.getArgument(n).getFullyConverted().getType().toString().matches("%basic_string%") and
//     arg = n
//   )
// select call.getArgument(arg),
//   "Passed invalid std::string to console function. Please use .c_str() to convert it to a C-string."
