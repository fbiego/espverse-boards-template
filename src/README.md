# Source Layout

`main.cpp` is the application entry point. Supporting implementations are
grouped by responsibility:

- `board/` owns the selected board profile instance.
- `display/` connects LVGL to the selected display and touch hardware.
- `network/` provides asynchronous HTTP request helpers.
- `os/` implements LVGL's custom FreeRTOS integration.

Public declarations mirror this layout under `include/`. Include them by their
module path, for example:

```cpp
#include "display/lvgl_port.hpp"
#include "network/requests.hpp"
```
