/**
 * @name Wrong type of arguments to formatting function
 * @description Calling a format string function with the wrong type of arguments causes unpredictable behavior.
 * @kind problem
 * @problem.severity error
 * @security-severity 7.5
 * @precision high
 * @id sar-cpp-queries/console-invalid-string-2
 * @tags reliability
 *       correctness
 *       security
 *       external/cwe/cwe-686
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
      this.getArgument(0).toString().matches("%\\%s%") and
      exists(int argIndex |
        this.getArgument(argIndex)
            .getFullyConverted()
            .getUnspecifiedType()
            .toString()
            .matches("%basic_string%")
      )
    )
  }

  override Expr getFormatArgument() {
    exists(int argIndex |
      this.getArgument(argIndex)
          .getFullyConverted()
          .getUnspecifiedType()
          .toString()
          .matches("%basic_string%") and
      result = this.getArgument(argIndex)
    )
  }
}

class ConsoleFunctionCall extends FunctionCall, FormatArgument {
  ConsoleFunctionCall() {
    this.getQualifier().toString() = "console" and
    this.getArgument(0).toString().matches("%\\%s%") and
    exists(int argIndex |
      this.getArgument(argIndex).getFullyConverted().getType().toString().matches("%basic_string%")
    )
  }

  override Expr getFormatArgument() {
    exists(int argIndex |
      this.getArgument(argIndex).getFullyConverted().getType().toString().matches("%basic_string%") and
      result = this.getArgument(argIndex)
    )
  }
}

from Call call
where
  call instanceof ConsoleVariableCall or
  call instanceof ConsoleFunctionCall
select call.(FormatArgument).getFormatArgument(),
  "Passed invalid std::string to console function. Please use .c_str() to convert it to a C-string."
