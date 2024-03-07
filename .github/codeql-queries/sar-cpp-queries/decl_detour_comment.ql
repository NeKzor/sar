/**
 * @name Missing comment for function hook
 * @description Every function hook should contain a comment above which documents the name of the original function.
 * @kind problem
 * @problem.severity recommendation
 * @precision high
 * @id sar-cpp-queries/decl-detour-comment
 * @tags maintainability
 *       readability
 */

import cpp

class DeclDetourMacro extends MacroInvocation {
  DeclDetourMacro() {
    this.getMacroName().matches("DECL_DETOUR%") and
    this.getMacroName() != "DECL_DETOUR_COMMAND"
  }
}

pragma[inline]
predicate isOneLineBefore(Location before, Location after) {
  before.getFile() = after.getFile() and
  before.getEndLine() = after.getStartLine() - 1
}

from DeclDetourMacro missing
where
  not exists(DeclDetourMacro macro, Comment comment |
    isOneLineBefore(comment.getLocation(), macro.getLocation()) and
    macro = missing
  ) and
  not exists(DeclDetourMacro macro, PreprocessorElse pelse |
    isOneLineBefore(pelse.getLocation(), macro.getLocation()) and
    macro = missing
  )
select missing, "Missing comment for function hook."
