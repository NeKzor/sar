/**
 * @name Uncontrolled format string
 * @description Using externally-controlled format strings functions can lead to buffer overflows or data representation
 *              problems.
 * @kind problem
 * @problem.severity recommendation
 * @security-severity 9.3
 * @precision high
 * @id sar-cpp-queries/console-format-string
 * @tags maintainability
 *       correctness
 *       security
 *       external/cwe/cwe-134
 */

import cpp

from VariableCall call, PointerFieldAccess access, string fn, int arg
where
  access = call.getExpr() and
  fn = access.toString() and
  access.getQualifier().toString() = "console" and
  not fn.matches("LoggingSystem%") and
  (
    not fn = "ColorMsg" and
    call.getArgument(0).getType().getPointerIndirectionLevel() > 0 and
    arg = 0
    or
    fn = "ColorMsg" and
    call.getArgument(1).getType().getPointerIndirectionLevel() > 0 and
    arg = 1
  )
select call.getArgument(arg),
  "Call to console function uses an uncontrolled format string. Please use a controlled format string like \"%s\"."
// from FunctionCall call, string fn, int arg
// where
//   fn = call.toString() and
//   call.getQualifier().toString() = "console" and
//   (
//     not fn = "ColorMsg" and
//     call.getArgument(0).getType().getPointerIndirectionLevel() > 0 and
//     arg = 0
//     or
//     fn = "ColorMsg" and
//     call.getArgument(1).getType().getPointerIndirectionLevel() > 0 and
//     arg = 1
//   ) and
//   not call.getArgument(arg).toString() = "m_pszHelpString"
// select call.getArgument(arg),
//   "Call to console function uses an uncontrolled format string. Please use a controlled format string like \"%s\"."
