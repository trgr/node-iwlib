node-iwlib
##########
So far only iw_scan is implemented, and the interface is not by any means locked.

```
var iwlib = require("iwlib").iwlib
var iw = new iwlib()
console.log(iw.scan("wlan0")
```