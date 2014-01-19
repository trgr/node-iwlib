node-iwlib
##########
So far only iw_scan/get_kernel_we_version is implemented, and the interface is not by any means locked (pondering if async or sync is the way to go with this). Pull requests are welcomed, hopefully this weill one day be a complete interface to iwlib.

```
var iwlib = require("iwlib").iwlib
var iw = new iwlib()
console.log(iw.scan("wlan0")
console.log(iw.get_kernel_we_version())
```