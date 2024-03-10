/**
 * @name Missing newline in description
 * @description The description of a command or variable requires a newline at the end of the string.
 * @kind problem
 * @problem.severity recommendation
 * @precision high
 * @id sar-cpp-queries/description-missing-newline
 * @tags maintainability
 *       readability
 */

import cpp

class MissingNewlineInCommand extends MacroInvocation {
  MissingNewlineInCommand() {
    this.getMacroName() in [
        "CON_COMMAND", "CON_COMMAND_F", "CON_COMMAND_F_COMPLETION", "CON_COMMAND_COMPLETION"
      ] and
    not this.getUnexpandedArgument(1).matches("%\\n\"")
  }
}

class MissingNewlineInVariable extends GlobalVariable {
  MissingNewlineInVariable() {
    this.getType().getName() = "Variable" and
    exists(int arg, ConstructorCall ctor |
      ctor = this.getInitializer().getExpr().(ConstructorCall) and
      (
        ctor.getArgument(2).getType().toString().matches("const char%") and
        arg = 2
        or
        ctor.getArgument(3).getType().toString().matches("const char%") and
        arg = 3
        or
        ctor.getArgument(4).getType().toString().matches("const char%") and
        arg = 4
      ) and
      not ctor.getArgument(arg).getValue().matches("%\n")
    )
  }
}

from Locatable loc
where loc instanceof MissingNewlineInCommand or loc instanceof MissingNewlineInVariable
select loc, "Missing newline in description."
