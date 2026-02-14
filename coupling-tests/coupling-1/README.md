<!--
SPDX-License-Identifier: MIT
-->

# Test Case 1 for Coupling Coverage

## Components

- main
- ss1
- ss2

## Data Coupling

`global_1`

- read
    - `consume_global1` in ss2
    - `consume_global1_next` in ss2
- write
    - `set_global1` in ss1
    - `set_global1_next` in ss1

`global_2`

- read
    - `consume_global2` in ss1
    - `consume_global2_next` in ss1
- write
    - `set_global2` in ss1

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
