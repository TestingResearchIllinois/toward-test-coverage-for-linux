<!--
SPDX-License-Identifier: MIT
-->

# Test Case 2 for Coupling Coverage

## Components

- main
- ss1
- ss2
- ss3

## Data Coupling

`global_1`

- read
    - `consume_global1` in ss2
    - `consume_global1_next` in ss2
    - `read_global1` in ss1 (?)
    - `consume_global1_in_ss3` in ss3
        - This function reads `global_1` four times
- write
    - `set_global1` in ss1
    - `set_global1_next` in ss1

`global_2`

- read
    - `consume_global2` in ss1
    - `consume_global2_next` in ss1
- write
    - `set_global2` in ss2
    - `set_global2_in_ss3` in ss3 (?)

`global_3`

- read
    - `consume_global1_and_global3` in ss2
- write
    - `set_global3` in ss3

## Invocation in Main

```
set_global1_next();
consume_global1();
call_global1();
consume_global1_next();

set_global2();
consume_global2();
call_global2();
```

<!-- WIP(wt) -->
