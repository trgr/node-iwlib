node-iwlib
##########
implements:
iw_scan
get_kernel_we_version
get_basic_config

```
var iwlib = require("iwlib").iwlib
var iw = new iwlib()
console.log(iw.scan("wlan0"))
console.log(iw.get_kernel_we_version())
console.log(iw.get_basic_config("wlan0"))
```