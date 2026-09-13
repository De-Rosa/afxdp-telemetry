# mutex vs spsc

## initial tests
-- mutex queue --
[saturated run] order valid?: yes
p50: 84782.917 us
p99: 90358.167 us
p99.9: 90737.375 us
max: 90755.292 us
[paced run] order valid?: yes
p50: 12.292 us
p99: 51.959 us
p99.9: 505.209 us
max: 8156.500 us
-- spsc queue --
[saturated run] order valid?: yes
p50: 8103.417 us
p99: 13865.333 us
p99.9: 14468.708 us
max: 14531.333 us
[paced run] order valid?: yes
p50: 0.167 us
p99: 1.167 us
p99.9: 25.791 us
max: 952.500 us

