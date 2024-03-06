# Uncontrolled format string
The logging methods of the `console` object require a controlled format string.


## Recommendation
Pass in a string like "%s" as the format, rather than passing in a variable directly.


## Example
The following code is vulnerable:


```cpp
console->Warning(lodepng_error_text(error));
```

## Example
Use `"%s"` as the first argument to fix this:


```cpp
console->Warning("%s", lodepng_error_text(error));
```

## References
* Common Weakness Enumeration: [CWE-134](https://cwe.mitre.org/data/definitions/134.html).
