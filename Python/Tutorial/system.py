#===================================time============================

import time

ticks = time.time()
localtime = time.localtime(ticks)
print(time.asctime(localtime))
print(time.tzname)