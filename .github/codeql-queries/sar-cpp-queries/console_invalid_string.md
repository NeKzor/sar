# Wrong type of arguments to formatting function
The logging methods of the `console` object do not support `std::string` directly.


## Recommendation
Pass in a C-string `char*` to the method, rather than passing in a `std::string` variable.


## Example
The following code is vulnerable:


```cpp
console->Warning(
    "Failed to load \"%s\"\n"
    "Make sure both the path and the filename are correct.\n",
    filename);
```

## Example
Use `.c_str()` to convert the variable to a C-string:


```cpp
console->Warning(
    "Failed to load \"%s\"\n"
    "Make sure both the path and the filename are correct.\n",
    filename.c_str());
```

## References
* Common Weakness Enumeration: [CWE-686](https://cwe.mitre.org/data/definitions/686.html).
