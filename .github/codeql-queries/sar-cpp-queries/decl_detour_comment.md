# Missing comment for function hook
All declared function hooks should have a comment that document the full name of the function.


## Recommendation
Add a comment like `// <classname>::<function>` above the macro.


## Example
Here is an example of what is could look like:


```cpp
// CInput::CreateMove
DECL_DETOUR(CInput_CreateMove, int sequence_number, float input_sample_frametime, bool active);
```
