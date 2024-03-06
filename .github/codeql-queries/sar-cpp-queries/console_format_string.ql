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

abstract class FormatArgument extends Expr {
  abstract Expr getFormatArgument();
}

class ConsoleVariableCall extends VariableCall, FormatArgument {
  ConsoleVariableCall() {
    exists(PointerFieldAccess access |
      access = this.getExpr() and
      access.getQualifier().toString() = "console" and
      not access.toString().matches("LoggingSystem%") and
      (
        access.toString() != "ColorMsg" and
        this.getArgument(0).getType().getPointerIndirectionLevel() > 0
        or
        access.toString() = "ColorMsg" and
        this.getArgument(1).getType().getPointerIndirectionLevel() > 0
      )
    )
  }

  override Expr getFormatArgument() {
    this.getExpr().toString() != "ColorMsg" and
    result = this.getArgument(0)
    or
    this.getExpr().toString() = "ColorMsg" and
    result = this.getArgument(1)
  }
}

class ConsoleFunctionCall extends FunctionCall, FormatArgument {
  ConsoleFunctionCall() {
    exists(int argIndex |
      this.getQualifier().toString() = "console" and
      (
        this.toString() != "ColorMsg" and
        argIndex = 0
        or
        this.toString() = "ColorMsg" and
        argIndex = 1
      ) and
      this.getArgument(argIndex).getType().getPointerIndirectionLevel() > 0 and
      this.getArgument(argIndex).toString() != "m_pszHelpString"
    )
  }

  override Expr getFormatArgument() {
    this.toString() != "ColorMsg" and
    result = this.getArgument(0)
    or
    this.toString() = "ColorMsg" and
    result = this.getArgument(1)
  }
}

from Call call
where
  call instanceof ConsoleVariableCall or
  call instanceof ConsoleFunctionCall
select call.(FormatArgument).getFormatArgument(),
  "Call to console function uses an uncontrolled format string. Please use a controlled format string like \"%s\"."
